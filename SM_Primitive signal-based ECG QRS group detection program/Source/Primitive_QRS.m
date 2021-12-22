function DB = Primitive_QRS(DB)
DB.Sig_Length = length(DB.Sig);
DB.Pr_Length = length(DB.Pr);

%% Primitive Fitting
DB = Primitive_Fitting(DB);
%% 전위값 기반 후보점 검출
DB = Find_Peak_Candi_1(DB);
%% 전위 및 곡률 기반 1차 R-peak 검출
DB = Find_R_peak_Detect_1(DB);
%% 주기 기반 후보 재검출
DB = Find_Peak_Candi_2(DB);
%%  Q, R, S peak 검출
DB = Find_QRS_Peak_Candi(DB);
%% Primitive 생성
DB = Find_QRS_Poly_Candi(DB);
%% on off 검출
DB = Find_QRS_On_Off_Candi(DB);
% 첨도 기반 2차 R-peak 검출
DB = Find_R_peak_Detect_2(DB);
%% 검출율 측정
DB = Check_Detection_Result(DB);

%=======================================================================%
%=======================================================================%
function DB = Primitive_Fitting(DB)
if DB.Pr1_exist == 1
    if DB.Pr_Sign == 1
        [~,DB.Pr1_Max_Pos] = min(DB.Pr1);
    elseif DB.Pr_Sign == -1
        [~,DB.Pr1_Max_Pos] = max(DB.Pr1);
    end
end
if DB.Pr_Sign == 1
    [~,DB.Pr2_Max_Pos] = max(DB.Pr2);
elseif DB.Pr_Sign == -1
    [~,DB.Pr2_Max_Pos] = min(DB.Pr2);
end
if DB.Pr3_exist == 1
    if DB.Pr_Sign == 1
        [~,DB.Pr3_Max_Pos] = min(DB.Pr3);
    elseif DB.Pr_Sign == -1
        [~,DB.Pr3_Max_Pos] = max(DB.Pr3);
    end
end

if DB.Pr1_exist == 1
    Pr1_S = DB.Pr1_X(DB.Pr1_Max_Pos)-2;
    for j = DB.Pr1_X(DB.Pr1_Max_Pos)-3 : -1 : DB.Pr1_X(1)
        if DB.Sig_Cur2(j) <= 0
            Pr1_S = j+1;
            break
        end
    end
    Pr1_E = DB.Pr1_X(DB.Pr1_Max_Pos)+2;
    for j = DB.Pr1_X(DB.Pr1_Max_Pos)+3 : DB.Pr1_X(end)
        if DB.Sig_Cur2(j) <= 0
            Pr1_E = j-1;
            break
        end
    end
    DB.Pr1_Poly_X = Pr1_S : Pr1_E;
    DB.Pr1_Poly_Move = mean(DB.Pr1_Poly_X);
    DB.Pr1_Poly = polyfit(DB.Pr1_Poly_X-DB.Pr1_Poly_Move,DB.Sig(DB.Pr1_Poly_X),2);
    DB.Pr1_Poly_val = polyval(DB.Pr1_Poly, DB.Pr1_Poly_X);
    
    Pr2_S= floor((DB.Pr1_X(DB.Pr1_Max_Pos)+DB.Pr2_X(DB.Pr2_Max_Pos))/2);
    for j = DB.Pr2_X(DB.Pr2_Max_Pos)-3 : -1 : DB.Pr1_X(DB.Pr1_Max_Pos)+1
        if DB.Sig_Cur2(j) >= 0
            Pr2_S = j+1;
            break
        end
    end
else    
    Pr2_S= DB.Pr2_X(1);
    for j = DB.Pr2_X(DB.Pr2_Max_Pos)-3 : -1 : DB.Pr2_X(1)
        if DB.Sig_Cur2(j) >= 0
            Pr2_S = j+1;
            break
        end
    end
end


if DB.Pr3_exist == 1
    Pr3_S = DB.Pr3_X(DB.Pr3_Max_Pos)-2;
    for j = DB.Pr3_X(DB.Pr3_Max_Pos)-3 : -1 : DB.Pr3_X(1)
        if DB.Sig_Cur2(j) <= 0
            Pr3_S = j+1;
            break
        end
    end
    Pr3_E = DB.Pr3_X(DB.Pr3_Max_Pos)+2;
    for j = DB.Pr3_X(DB.Pr3_Max_Pos)+3 : DB.Pr3_X(end)
        if DB.Sig_Cur2(j) <= 0
            Pr3_E = j-1;
            break
        end
    end
    DB.Pr3_Poly_X = Pr3_S : Pr3_E;
    DB.Pr3_Poly_Move = mean(DB.Pr3_Poly_X);
    DB.Pr3_Poly = polyfit(DB.Pr3_Poly_X-DB.Pr3_Poly_Move,DB.Sig(DB.Pr3_Poly_X),2);
    DB.Pr3_Poly_val = polyval(DB.Pr3_Poly, DB.Pr3_Poly_X);
    
    Pr2_E= ceil((DB.Pr3_X(DB.Pr3_Max_Pos)+DB.Pr2_X(DB.Pr2_Max_Pos))/2);
    for j = DB.Pr2_X(DB.Pr2_Max_Pos)+3 : DB.Pr3_X(DB.Pr3_Max_Pos)-1
        if DB.Sig_Cur2(j) >= 0
            Pr2_E = j-1;
            break
        end
    end
else    
    Pr2_E= DB.Pr2_X(end);
    for j = DB.Pr2_X(DB.Pr2_Max_Pos)+3 : DB.Pr2_X(end)
        if DB.Sig_Cur2(j) >= 0
            Pr2_E = j-1;
            break
        end
    end
end

DB.Pr2_Poly_X = Pr2_S : Pr2_E;
DB.Pr2_Poly_Move = mean(DB.Pr2_Poly_X);
DB.Pr2_Poly = polyfit(DB.Pr2_Poly_X-DB.Pr2_Poly_Move,DB.Sig(DB.Pr2_Poly_X),2);
DB.Pr2_Poly_val = polyval(DB.Pr2_Poly, DB.Pr2_Poly_X);
%=======================================================================%
function DB = Find_Peak_Candi_1(DB)

DB.Peak_Candi1 = [];
Last_x = 1;
for i = 1 : DB.Sig_Length
    if  (max(abs(DB.Sig_Morp(max(1,i-DB.Cur_Max_Size) : min(DB.Sig_Length,i+DB.Cur_Max_Size)))) == abs(DB.Sig_Morp(i)) && abs(DB.Sig_Morp(i)) > abs(DB.Pr_Amplitude)*DB.Pr_Basic_Weight)
        if Last_x + DB.Cur_Max_Size < i || isempty(DB.Peak_Candi1)
            DB.Peak_Candi1(end+1) = i;
            Last_x = i;
        end
    end
end
%=======================================================================%
function DB = Find_R_peak_Detect_1(DB)
DB.Peak_Candi2 = [];
DB.R_peak_Detect1 = [];
for i = 1 : length(DB.Peak_Candi1)
    if abs(DB.Sig_Morp(DB.Peak_Candi1(i))) >= abs(DB.Pr_Amplitude_Weight*DB.Pr_Amplitude)
        DB.R_peak_Detect1(end+1) = DB.Peak_Candi1(i);
    else
        DB.Peak_Candi2(end+1) = DB.Peak_Candi1(i);
    end
end
%=======================================================================%
function DB = Find_Peak_Candi_2(DB)
DB.RR_Check_Size = round(0.7*median(DB.R_peak_Detect1(2:end)-DB.R_peak_Detect1(1:end-1)));

DB.Peak_Candi3 = [];
DB.Amplitude_Candi = [];
for i = 1 : length(DB.Peak_Candi2)
    Min_Dist =  min(abs(DB.R_peak_Detect1-DB.Peak_Candi2(i)));
    if Min_Dist >= DB.RR_Check_Size
        DB.Peak_Candi3(end+1) = DB.Peak_Candi2(i);
        DB.Amplitude_Candi(end+1) = DB.Sig_Morp(DB.Peak_Candi2(i));
    end
end
%=======================================================================%
function DB = Find_QRS_Peak_Candi(DB)
Candi_Num = length(DB.Peak_Candi3);
DB.Q_peak_Candi = [];
DB.R_peak_Candi = [];
DB.S_peak_Candi = [];
DB.R_peak_Candi_Inverse = [];
if DB.Pr_Sign == 1
    for i = 1 : Candi_Num
        if sign(DB.Sig_Cur(DB.Peak_Candi3(i))) ~= DB.Pr_Sign
            %% R-peak 곡률 극대값 검출 시
            DB.R_peak_Candi(end+1) = DB.Peak_Candi3(i);
            DB.R_peak_Candi_Inverse(end+1) = 0;
            %% Q파 존재 시
            if DB.Pr1_exist == 1
                tmp = max(1,DB.R_peak_Candi(end) - DB.Pr_Length);
                [~,p] = max(DB.Sig_Cur(tmp : DB.R_peak_Candi(end)));
                DB.Q_peak_Candi(end+1) = tmp + p - 1;
            else
                %             DB.Q_peak_Candi(end+1) = NaN;
            end
            %% S파 존재 시
            if DB.Pr3_exist == 1
                tmp = min(DB.Sig_Length,DB.R_peak_Candi(end) + DB.Pr_Length);
                [~,p] = max(DB.Sig_Cur(DB.R_peak_Candi(end) : tmp));
                DB.S_peak_Candi(end+1) = DB.R_peak_Candi(end) + p - 1;
            else
                %             DB.S_peak_Candi(end+1) = NaN;
            end
        else
            if DB.Pr1_exist == 1
                if DB.Pr3_exist == 1 %% Q,S 모두 존재, 분리 필요
                    % 다음 극대점 검출
                    Candi_Cur_Max = -inf;
                    Peak_Candi2 = DB.Peak_Candi3(i) - DB.Pr_Length;
                    for j = max(1,DB.Peak_Candi3(i) - DB.Pr_Length) : min(DB.Sig_Length,DB.Peak_Candi3(i) + DB.Pr_Length)
                        if j == DB.Peak_Candi3(i)
                            continue
                        end
                        if DB.Sig_Cur(j)>=DB.Sig_Cur(j-1)  && DB.Sig_Cur(j)>=DB.Sig_Cur(j+1) && DB.Sig_Cur(j) > Candi_Cur_Max
                            Candi_Cur_Max = DB.Sig_Cur(j);
                            Peak_Candi2 = j;
                        end
                    end
                    % Q,S 분리
                    if Peak_Candi2 < DB.Peak_Candi3(i)
                        DB.Q_peak_Candi(end+1) = Peak_Candi2;
                        DB.S_peak_Candi(end+1) = DB.Peak_Candi3(i);
                    else
                        DB.Q_peak_Candi(end+1) = DB.Peak_Candi3(i);
                        DB.S_peak_Candi(end+1) = Peak_Candi2;
                    end
                    [~,p] = max(DB.Sig(DB.Q_peak_Candi(end) : DB.S_peak_Candi(end)));
                    DB.R_peak_Candi(end+1) = DB.Q_peak_Candi(end) + p -1;
                    DB.R_peak_Candi_Inverse(end+1) = 0;
                elseif DB.Pr3_exist == 0 %% Q파만 존재 R파 검출 시작
                    DB.Q_peak_Candi(end+1) = DB.Peak_Candi3(i);
                    DB.S_peak_Candi(end+1) = NaN;
                    tmp = min(DB.Sig_Length,DB.Q_peak_Candi(end) + DB.Pr_Length);
                    [~,p] = min(DB.Sig_Cur(DB.Q_peak_Candi(end) : tmp ));
                    DB.R_peak_Candi(end+1) = DB.Q_peak_Candi(end) + p - 1;
                    DB.R_peak_Candi_Inverse(end+1) = 0;
                end
            elseif DB.Pr1_exist == 0
                if DB.Pr3_exist == 1 %% S파만 존재 R파 검출 시작
                    DB.Q_peak_Candi(end+1) = NaN;
                    DB.S_peak_Candi(end+1) = DB.Peak_Candi3(i);
                    tmp = max(1,DB.S_peak_Candi(end) - DB.Pr_Length);
                    [~,p] = min(DB.Sig_Cur(tmp : DB.S_peak_Candi(end)));
                    DB.R_peak_Candi(end+1) = tmp + p - 1;
                    DB.R_peak_Candi_Inverse(end+1) = 0;
                elseif DB.Pr3_exist == 0 %% Q,S파 모두 없음 노이즈가 검출
                    DB.R_peak_Candi(end+1) = DB.Peak_Candi3(i);
                    DB.R_peak_Candi_Inverse(end+1) = 1;
                end
            end
        end
    end
    %% Q,S보정
    if DB.Pr1_exist == 1
        for i = 1 : Candi_Num
            [~,p] = min(DB.Sig(DB.Q_peak_Candi(i) : DB.R_peak_Candi(i)));
            DB.Q_peak_Candi(i) = DB.Q_peak_Candi(i) + p -1;
        end
    end
    if DB.Pr3_exist == 1
        for i = 1 : Candi_Num
            [~,p] = min(DB.Sig(DB.R_peak_Candi(i) : DB.S_peak_Candi(i)));
            DB.S_peak_Candi(i) = DB.R_peak_Candi(i) + p -1;
        end
    end
else
    for i = 1 : Candi_Num
        if sign(DB.Sig_Cur(DB.Peak_Candi3(i))) ~= DB.Pr_Sign
            %% R-peak 곡률 극대값 검출 시
            DB.R_peak_Candi(end+1) = DB.Peak_Candi3(i);
            DB.R_peak_Candi_Inverse(end+1) = 0;
            %% Q파 존재 시
            if DB.Pr1_exist == 1
                tmp = max(1,DB.R_peak_Candi(end) - DB.Pr_Length);
                [~,p] = min(DB.Sig_Cur(tmp : DB.R_peak_Candi(end)));
                DB.Q_peak_Candi(end+1) = tmp + p - 1;
            else
                %             DB.Q_peak_Candi(end+1) = NaN;
            end
            %% S파 존재 시
            if DB.Pr3_exist == 1
                tmp = min(DB.Sig_Length,DB.R_peak_Candi(end) + DB.Pr_Length);
                [~,p] = min(DB.Sig_Cur(DB.R_peak_Candi(end) : tmp));
                DB.S_peak_Candi(end+1) = DB.R_peak_Candi(end) + p - 1;
            else
                %             DB.S_peak_Candi(end+1) = NaN;
            end
        else
            if DB.Pr1_exist == 1
                if DB.Pr3_exist == 1 %% Q,S 모두 존재, 분리 필요
                    % 다음 극대점 검출
                    Candi_Cur_Max = inf;
                    for j = max(1,DB.Peak_Candi3(i) - DB.Pr_Length) : min(DB.Sig_Length,DB.Peak_Candi3(i) + DB.Pr_Length)
                        if j == DB.Peak_Candi3(i)
                            continue
                        end
                        if DB.Sig_Cur(j)<=DB.Sig_Cur(j-1)  && DB.Sig_Cur(j)<=DB.Sig_Cur(j+1) && DB.Sig_Cur(j) < Candi_Cur_Max
                            Candi_Cur_Max = DB.Sig_Cur(j);
                            Peak_Candi2 = j;
                        end
                    end
                    % Q,S 분리
                    if Peak_Candi2 > DB.Peak_Candi3(i)
                        DB.Q_peak_Candi(end+1) = Peak_Candi2;
                        DB.S_peak_Candi(end+1) = DB.Peak_Candi3(i);
                    else
                        DB.Q_peak_Candi(end+1) = DB.Peak_Candi3(i);
                        DB.S_peak_Candi(end+1) = Peak_Candi2;
                    end
                    [~,p] = min(DB.Sig(DB.Q_peak_Candi(end) : DB.S_peak_Candi(end)));
                    DB.R_peak_Candi(end+1) = DB.Q_peak_Candi(end) + p -1;
                    DB.R_peak_Candi_Inverse(end+1) = 0;
                elseif DB.Pr3_exist == 0 %% Q파만 존재 R파 검출 시작
                    DB.Q_peak_Candi(end+1) = DB.Peak_Candi3(i);
                    DB.S_peak_Candi(end+1) = NaN;
                    [~,p] = min(DB.Sig(DB.Q_peak_Candi(end) : DB.Q_peak_Candi(end) + DB.Pr_Length));
                    DB.R_peak_Candi(end+1) = DB.Q_peak_Candi(end) + p - 1;
                    DB.R_peak_Candi_Inverse(end+1) = 0;
                end
            elseif DB.Pr1_exist == 0
                if DB.Pr3_exist == 1 %% S파만 존재 R파 검출 시작
                    DB.Q_peak_Candi(end+1) = NaN;
                    DB.S_peak_Candi(end+1) = DB.Peak_Candi3(i);
                    [~,p] = max(DB.Sig_Cur(DB.S_peak_Candi(end) - DB.Pr_Length : DB.S_peak_Candi(end)));
                    DB.R_peak_Candi(end+1) = DB.S_peak_Candi(end) - DB.Pr_Length + p - 1;
                    DB.R_peak_Candi_Inverse(end+1) = 0;
                elseif DB.Pr3_exist == 0 %% Q,S파 모두 없음 노이즈가 검출
                    DB.R_peak_Candi(end+1) = DB.Peak_Candi3(i);
                    DB.R_peak_Candi_Inverse(end+1) = 1;
                end
            end
        end
    end
    %% Q,S보정
    if DB.Pr1_exist == 1
        for i = 1 : Candi_Num
            [~,p] = max(DB.Sig(DB.Q_peak_Candi(i) : DB.R_peak_Candi(i)));
            DB.Q_peak_Candi(i) = DB.Q_peak_Candi(i) + p -1;
        end
    end
    if DB.Pr3_exist == 1
        for i = 1 : Candi_Num
            [~,p] = max(DB.Sig(DB.R_peak_Candi(i) : DB.S_peak_Candi(i)));
            DB.S_peak_Candi(i) = DB.R_peak_Candi(i) + p -1;
        end
    end
end
%=======================================================================%
function DB = Find_QRS_Poly_Candi(DB)
Candi_Num = length(DB.Peak_Candi3);
Q_S = zeros(1,Candi_Num);
Q_E = zeros(1,Candi_Num);
R_S = zeros(1,Candi_Num);
R_E = zeros(1,Candi_Num);
S_S = zeros(1,Candi_Num);
S_E = zeros(1,Candi_Num);
DB.Q_peak_Candi_Poly_X = [];
DB.Q_peak_Candi_Poly_X2 = [];
DB.Q_peak_Candi_Poly_X_move = [];
DB.Q_peak_Candi_Poly = [];
DB.Q_peak_Candi_Poly_val = [];
DB.Q_peak_Candi_Poly_val2 = [];
DB.R_peak_Candi_Poly_X = [];
DB.R_peak_Candi_Poly_X2 = [];
DB.R_peak_Candi_Poly_X_move = [];
DB.R_peak_Candi_Poly = [];
DB.R_peak_Candi_Poly_val = [];
DB.R_peak_Candi_Poly_val2 = [];
DB.S_peak_Candi_Poly_X = [];
DB.S_peak_Candi_Poly_X2 = [];
DB.S_peak_Candi_Poly_X_move = [];
DB.S_peak_Candi_Poly = [];
DB.S_peak_Candi_Poly_val = [];
DB.S_peak_Candi_Poly_val2 = [];

if DB.Pr_Sign == 1
    if DB.Pr1_exist == 1
        for i = 1 : Candi_Num
            Q_S(i) = DB.Q_peak_Candi(i)-2;
            for j = DB.Q_peak_Candi(i)-3 : -1 : max(1,DB.Q_peak_Candi(i)-length(DB.Pr1))
                if DB.Sig_Cur2(j) <= 0
                    Q_S(i) = j+1;
                    break
                end
            end
            Q_E(i) = DB.Q_peak_Candi(i)+2;
            for j = DB.Q_peak_Candi(i)+3 : DB.R_peak_Candi(i)-1
                if DB.Sig_Cur2(j) <= 0
                    Q_E(i) = j-1;
                    break
                end
            end
            DB.Q_peak_Candi_Poly_X{i} = Q_S(i) : Q_E(i);
            DB.Q_peak_Candi_Poly_X2{i} = Q_S(i) -5: Q_E(i)+5;
            DB.Q_peak_Candi_Poly_X_move{i} = DB.Q_peak_Candi_Poly_X{i}-mean(DB.Q_peak_Candi_Poly_X{i});
            DB.Q_peak_Candi_Poly(i,:) = polyfit(DB.Q_peak_Candi_Poly_X_move{i},DB.Sig(DB.Q_peak_Candi_Poly_X{i}),2);
            DB.Q_peak_Candi_Poly_val{i} = polyval(DB.Q_peak_Candi_Poly(i,:),DB.Q_peak_Candi_Poly_X_move{i});
            DB.Q_peak_Candi_Poly_val2{i} = polyval(DB.Q_peak_Candi_Poly(i,:),DB.Q_peak_Candi_Poly_X_move{i}(1)-5 :DB.Q_peak_Candi_Poly_X_move{i}(end)+5 );
        end
%         R_S(i) = Q_E(i) + 1;
        for i = 1 : Candi_Num
            R_S(i) = floor((DB.Q_peak_Candi(i)+DB.R_peak_Candi(i))/2);
            for j = DB.R_peak_Candi(i)-3 : -1 : DB.Q_peak_Candi(i)+1
                if DB.Sig_Cur2(j) >= 0
                    R_S(i) = j+1;
                    break
                end
            end
        end
    else
        for i = 1 : Candi_Num
            if DB.R_peak_Candi_Inverse(i) == 0
                R_S(i) = max(1,DB.R_peak_Candi(i)-length(DB.Pr2));
                for j = DB.R_peak_Candi(i)-3 : -1 : max(1,DB.R_peak_Candi(i)-length(DB.Pr2))
                    if DB.Sig_Cur2(j) >= 0
                        R_S(i) = j+1;
                        break
                    end
                end
            elseif DB.R_peak_Candi_Inverse(i) == 1
                R_S(i) = max(1,DB.R_peak_Candi(i)-length(DB.Pr2));
                for j = DB.R_peak_Candi(i)-3 : -1 : max(1,DB.R_peak_Candi(i)-length(DB.Pr2))
                    if DB.Sig_Cur2(j) <= 0
                        R_S(i) = j+1;
                        break
                    end
                end
            end
        end
    end
    
    if DB.Pr3_exist == 1
        for i = 1 : Candi_Num
            S_S(i) = DB.S_peak_Candi(i)-2;
            for j = DB.S_peak_Candi(i)-3 : -1 : DB.R_peak_Candi(i)+1
                if DB.Sig_Cur2(j) <= 0
                    S_S(i) = j+1;
                    break
                end
            end
            S_E(i) = DB.S_peak_Candi(i)+2;
            for j = DB.S_peak_Candi(i)+3 : min(DB.Sig_Length,DB.S_peak_Candi(i)+length(DB.Pr3))
                if DB.Sig_Cur2(j) <= 0
                    S_E(i) = j-1;
                    break
                end
            end
            DB.S_peak_Candi_Poly_X{i} = S_S(i) : S_E(i);
            DB.S_peak_Candi_Poly_X2{i} = S_S(i)-5 : S_E(i)+5;
            DB.S_peak_Candi_Poly_X_move{i} = DB.S_peak_Candi_Poly_X{i}-mean(DB.S_peak_Candi_Poly_X{i});
            DB.S_peak_Candi_Poly(i,:) = polyfit(DB.S_peak_Candi_Poly_X_move{i},DB.Sig(DB.S_peak_Candi_Poly_X{i}),2);
            DB.S_peak_Candi_Poly_val{i} = polyval(DB.S_peak_Candi_Poly(i,:),DB.S_peak_Candi_Poly_X_move{i});
            DB.S_peak_Candi_Poly_val2{i} = polyval(DB.S_peak_Candi_Poly(i,:),DB.S_peak_Candi_Poly_X_move{i}(1)-5 :DB.S_peak_Candi_Poly_X_move{i}(end)+5 );
        end
        
        for i = 1 : Candi_Num
            R_E(i) =  ceil((DB.S_peak_Candi(i)+DB.R_peak_Candi(i))/2);
            for j = DB.R_peak_Candi(i)+3 : DB.S_peak_Candi(i)-1
                if DB.Sig_Cur2(j) >= 0
                    R_E(i) = j-1;
                    break
                end
            end
        end
    else
        for i = 1 : Candi_Num
            if DB.R_peak_Candi_Inverse(i) == 0
                R_E(i) = min(DB.Sig_Length,DB.R_peak_Candi(i)+length(DB.Pr2));
                for j = DB.R_peak_Candi(i)+3 : min(DB.Sig_Length,DB.R_peak_Candi(i)+length(DB.Pr2))
                    if DB.Sig_Cur2(j) >= 0
                        R_E(i) = j-1;
                        break
                    end
                end
            elseif DB.R_peak_Candi_Inverse(i) == 1
                R_E(i) = min(DB.Sig_Length,DB.R_peak_Candi(i)+length(DB.Pr2));
                for j = DB.R_peak_Candi(i)+3 : min(DB.Sig_Length,DB.R_peak_Candi(i)+length(DB.Pr2))
                    if DB.Sig_Cur2(j) <= 0
                        R_E(i) = j-1;
                        break
                    end
                end
            end
        end
    end
    
    for i = 1 : Candi_Num
        DB.R_peak_Candi_Poly_X{i} = R_S(i) : R_E(i);
        DB.R_peak_Candi_Poly_X2{i} = R_S(i)-5 : R_E(i)+5;
        DB.R_peak_Candi_Poly_X_move{i} = DB.R_peak_Candi_Poly_X{i}-mean(DB.R_peak_Candi_Poly_X{i});
        DB.R_peak_Candi_Poly(i,:) = polyfit(DB.R_peak_Candi_Poly_X_move{i},DB.Sig(DB.R_peak_Candi_Poly_X{i}),2);
        DB.R_peak_Candi_Poly_val{i} = polyval(DB.R_peak_Candi_Poly(i,:),DB.R_peak_Candi_Poly_X_move{i});
        DB.R_peak_Candi_Poly_val2{i} = polyval(DB.R_peak_Candi_Poly(i,:),DB.R_peak_Candi_Poly_X_move{i}(1)-5 :DB.R_peak_Candi_Poly_X_move{i}(end)+5 );
    end
else
end
%=======================================================================%
function DB = Find_QRS_On_Off_Candi(DB)
Candi_Num = length(DB.Peak_Candi3);
if DB.Pr_Sign == 1
    if DB.Pr1_exist == 1
        for i = 1 : Candi_Num
            tmp = DB.Q_peak_Candi_Poly_X{i}(1);
            DB.QRS_Candi_On(i) = tmp;
            for j = tmp : -1 : max(2,tmp - DB.Pr_Length)
                if DB.Sig_Cur2(j) <= DB.Sig_Cur2(j-1) && DB.Sig_Cur2(j) <= DB.Sig_Cur2(j+1) 
                    DB.QRS_Candi_On(i) = j;
                    break
                end
            end
        end
    else
        for i = 1 : Candi_Num
            if DB.R_peak_Candi_Inverse(i) == 0
                tmp = DB.R_peak_Candi_Poly_X{i}(1);
                DB.QRS_Candi_On(i) = tmp;
                for j = tmp : -1 : max(2,tmp - DB.Pr_Length)
                    if DB.Sig_Cur(j) >= DB.Sig_Cur(j-1) && DB.Sig_Cur(j) >= DB.Sig_Cur(j+1) && DB.Sig_Cur(j) > DB.Sig_Cur(DB.QRS_Candi_On(i))
                        DB.QRS_Candi_On(i) = j;
                        %                     break
                    end
                end
            elseif DB.R_peak_Candi_Inverse(i) == 1
                tmp = DB.R_peak_Candi_Poly_X{i}(1);
                DB.QRS_Candi_On(i) = tmp;
                for j = tmp : -1 : max(2,tmp - DB.Pr_Length)
                    if DB.Sig_Cur(j) <= DB.Sig_Cur(j-1) && DB.Sig_Cur(j) <= DB.Sig_Cur(j+1) && DB.Sig_Cur(j) < DB.Sig_Cur(DB.QRS_Candi_On(i))
                        DB.QRS_Candi_On(i) = j;
                        %                     break
                    end
                end
            end
        end
    end
    if DB.Pr3_exist == 1
        for i = 1 : Candi_Num
            tmp = DB.S_peak_Candi_Poly_X{i}(end);
            DB.QRS_Candi_Off(i) = tmp;
            for j = tmp : min(DB.Sig_Length-1,tmp + DB.Pr_Length-1)
                if DB.Sig_Cur2(j) <= DB.Sig_Cur2(j-1) && DB.Sig_Cur2(j) <= DB.Sig_Cur2(j+1)
                    DB.QRS_Candi_Off(i) = j;
                    break
                end
            end
        end
    else
        for i = 1 : Candi_Num
            if DB.R_peak_Candi_Inverse(i) == 0
                tmp = DB.R_peak_Candi_Poly_X{i}(end);
                DB.QRS_Candi_Off(i) = tmp;
                for j = tmp : min(DB.Sig_Length-1,tmp + DB.Pr_Length-1)
                    if DB.Sig_Cur(j) >= DB.Sig_Cur(j-1) && DB.Sig_Cur(j) >= DB.Sig_Cur(j+1) && DB.Sig_Cur(j) > DB.Sig_Cur(DB.QRS_Candi_Off(i))
                        DB.QRS_Candi_Off(i) = j;
                        %                     break
                    end
                end
            elseif DB.R_peak_Candi_Inverse(i) == 1
                tmp = DB.R_peak_Candi_Poly_X{i}(end);
                DB.QRS_Candi_Off(i) = tmp;
                for j = tmp : min(DB.Sig_Length-1,tmp + DB.Pr_Length-1)
                    if DB.Sig_Cur(j) <= DB.Sig_Cur(j-1) && DB.Sig_Cur(j) <= DB.Sig_Cur(j+1) && DB.Sig_Cur(j) < DB.Sig_Cur(DB.QRS_Candi_Off(i))
                        DB.QRS_Candi_Off(i) = j;
                        %                     break
                    end
                end
            end
        end
    end
end
%=======================================================================%
function DB = Find_R_peak_Detect_2(DB)

DB.R_peak_Detect2 = [];
for i = 1 : length(DB.Peak_Candi3)
    Poly_Sum = 0;
    if DB.Pr1_exist == 1
        Poly_Sum = Poly_Sum + (DB.Q_peak_Candi_Poly(i,1)/(DB.Pr1_Poly_Weight*DB.Pr1_Poly(1,1)))^2;
    end
    if DB.Pr3_exist == 1
        Poly_Sum = Poly_Sum + (DB.S_peak_Candi_Poly(i,1)/(DB.Pr3_Poly_Weight*DB.Pr3_Poly(1,1)))^2;
    end
    Poly_Sum = Poly_Sum + (DB.R_peak_Candi_Poly(i,1)/(DB.Pr2_Poly_Weight*DB.Pr2_Poly(1,1))).^2;
    Poly_Sum = Poly_Sum^(1/2);
    
    
    if Poly_Sum >= 1
        DB.R_peak_Detect2(end+1) = DB.Peak_Candi3(i);
    end
end
DB.R_peak_Detect = sort([DB.R_peak_Detect1,DB.R_peak_Detect2]);
%=======================================================================%
function DB = Check_Detection_Result(DB)
% 후보점 체크
Ms = DB.fs*0.1;
N1 = 1; % DB.R_peak Index
N2 = 1; % DB.R_peak_Candi Index
L1 = length(DB.R_peak);
L2 = length(DB.R_peak_Candi);
DB.Candi_True_Index = [];
DB.Candi_False_Index = [];
DB.F_Index_Candi = [];
while N1 <= L1 && N2 <= L2
    if abs(DB.R_peak(N1)-DB.R_peak_Candi(N2))< Ms
%         if DB.R_peak_text(N1) == 'N'
            DB.Candi_True_Index = [DB.Candi_True_Index,N2];
            N1 = N1 + 1;
            N2 = N2 + 1;
%         else
%             N1 = N1 + 1;
%             N2 = N2 + 1;
%         end
    elseif DB.R_peak(N1) > DB.R_peak_Candi(N2) 
        DB.Candi_False_Index = [DB.Candi_False_Index,N2];
        N2 = N2 + 1;
    elseif DB.R_peak(N1) < DB.R_peak_Candi(N2)
%         if DB.R_peak_text(N1) == 'N'
            DB.F_Index_Candi = [DB.F_Index_Candi,DB.R_peak(N1)];
            N1 = N1 + 1;
%         else
%             N1 = N1 + 1;
%         end
    end
end

if N1 > L1
    DB.Candi_False_Index = [DB.Candi_False_Index,N2 : L2];
end
if N2 > L2
    for i = N1 : L1
%         if DB.R_peak_text(i) == 'N'
            DB.F_Index_Candi = [DB.F_Index_Candi,DB.R_peak(i)];
%         end
    end
end



% R-peak 검출율
% TP: 정상파형 정상검출, FP: 정상&부정맥 이외의 영역 검출, FN: 정상 미검출
Ms = DB.fs*0.1;
N1 = 1; % DB.R_peak Index
N2 = 1; % DB.R_peak_Detect Index
L1 = length(DB.R_peak);
L2 = length(DB.R_peak_Detect);
DB.tp = 0;
DB.tn = 0;
DB.fp = 0;
DB.fn = 0;
DB.F_Index1 = [];
DB.F_Index2 = [];
while N1 <= L1 && N2 <= L2
    if abs(DB.R_peak(N1)-DB.R_peak_Detect(N2))< Ms
%         if DB.R_peak_text(N1) == 'N'
            DB.tp = DB.tp + 1;
            N1 = N1 + 1;
            N2 = N2 + 1;
%         else
%             N1 = N1 + 1;
%             N2 = N2 + 1;
%         end
    elseif DB.R_peak(N1) > DB.R_peak_Detect(N2) 
        DB.fp = DB.fp + 1;
        DB.F_Index2 = [DB.F_Index2,DB.R_peak_Detect(N2)];
        N2 = N2 + 1;
    elseif DB.R_peak(N1) < DB.R_peak_Detect(N2)
%         if DB.R_peak_text(N1) == 'N'
            DB.fn = DB.fn + 1;
            DB.F_Index1 = [DB.F_Index1,DB.R_peak(N1)];
            DB.F_Index2 = [DB.F_Index2,DB.R_peak(N1)];
            N1 = N1 + 1;
%         else
%             N1 = N1 + 1;
%         end
    end
end

if N1 > L1
    DB.fp = DB.fp + (L2 - N2 + 1);
    DB.F_Index2 = [DB.F_Index2,DB.R_peak_Detect(N2 : L2)];
end
if N2 > L2
    for i = N1 : L1
%         if DB.R_peak_text(i) == 'N'
            DB.fn = DB.fn + 1;
            DB.F_Index1 = [DB.F_Index1,DB.R_peak(i)];
            DB.F_Index2 = [DB.F_Index2,DB.R_peak(i)];
%         end
    end
end

if DB.fp_view == 1
    DB.F_Index = DB.F_Index2;
else
    DB.F_Index = DB.F_Index1;
end
DB.F_L = length(DB.F_Index);
DB.False_View = 0;