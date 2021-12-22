function DB = Primitive_QRS_20160202(DB)
DB.Sig_Length = length(DB.Sig);
DB.Pr_Length = length(DB.Pr);

%% Primitive Fitting
DB = Primitive_Fitting(DB);
%% 전위값 기반 후보점 검출
DB = Find_Peak_Candi_1(DB);
%% 후보점 Q,R,S peak 검출
DB = Find_QRS_Peak_Candi(DB);
%% 후보점 Poly Fit
DB = Find_QRS_Poly_Candi(DB);
% DB.R_peak_Detect = DB.R_peak_Candi;
%% R-peak 1차 검출
DB = Find_R_peak_Detect_1(DB);
%% RR 간격 측정
% DB = Find_RR_Interval(DB);
%% R-peak 과검 제거
% DB = Delete_R_peak_Detect_1(DB);

% L = length(DB.R_peak_Detect);
% % while 1
%     %% RR 간격 측정
%     DB = Find_RR_Interval(DB);
%     %% 정상 RR 간격 내부 후보 제거
%     DB = Find_Peak_Candi_2(DB);
%     %% 비정상 RR 간격 내부 추가 검출
%     DB = Find_R_peak_Detect_2(DB);
% %     if L == length(DB.R_peak_Detect)
% %         break
% %     else
% %         L = length(DB.R_peak_Detect);
% %     end
% % end



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
    DB.Pr1_Max_Pos_Index = DB.Pr1_X(DB.Pr1_Max_Pos);
    Poly_size = DB.Sig_Kur_Size(DB.Pr1_Max_Pos_Index);
    A = DB.Sig_Kur(DB.Pr1_Max_Pos_Index);
    C = DB.Sig(DB.Pr1_Max_Pos_Index);
    
%     DB.Pr1_Poly_X = - Poly_size : Poly_size;
%     DB.Pr1_Poly_val = polyval([A,0,C], DB.Pr1_Poly_X);
%     DB.Pr1_Poly_X = DB.Pr1_Poly_X + DB.Pr1_Max_Pos_Index;
    
end
if DB.Pr_Sign == 1
    [~,DB.Pr2_Max_Pos] = max(DB.Pr2);
elseif DB.Pr_Sign == -1
    [~,DB.Pr2_Max_Pos] = min(DB.Pr2);
end
DB.Pr2_Max_Pos_Index = DB.Pr2_X(DB.Pr2_Max_Pos);
Poly_size = DB.Sig_Kur_Size(DB.Pr2_Max_Pos_Index);
A = DB.Sig_Kur(DB.Pr2_Max_Pos_Index);
C = DB.Sig(DB.Pr2_Max_Pos_Index);

DB.Pr2_Poly_X = - Poly_size : Poly_size;
DB.Pr2_Poly_val = polyval([A,0,C], DB.Pr2_Poly_X);
DB.Pr2_Poly_X = DB.Pr2_Poly_X + DB.Pr2_Max_Pos_Index;

if DB.Pr3_exist == 1
    if DB.Pr_Sign == 1
        [~,DB.Pr3_Max_Pos] = min(DB.Pr3);
    elseif DB.Pr_Sign == -1
        [~,DB.Pr3_Max_Pos] = max(DB.Pr3);
    end
    DB.Pr3_Max_Pos_Index = DB.Pr3_X(DB.Pr3_Max_Pos);
    Poly_size = DB.Sig_Kur_Size(DB.Pr3_Max_Pos_Index);
    A = DB.Sig_Kur(DB.Pr3_Max_Pos_Index);
    C = DB.Sig(DB.Pr3_Max_Pos_Index);
    
%     DB.Pr3_Poly_X = - Poly_size : Poly_size;
%     DB.Pr3_Poly_val = polyval([A,0,C], DB.Pr3_Poly_X);
%     DB.Pr3_Poly_X = DB.Pr3_Poly_X + DB.Pr3_Max_Pos_Index;
    
end
%=======================================================================%
function DB = Find_Peak_Candi_1(DB)

DB.Peak_Candi1 = [];
Last_x = 1;
Last_y = 1;
for i = 2 : DB.Sig_Length-1
%     if sign(DB.Sig(i-1)-DB.Sig(i))*sign(DB.Sig(i)-DB.Sig(i+1)) == -1        
%         if (abs(DB.Sig_Filt(i)) > abs(DB.Pr_Amplitude)*DB.Pr_Basic_Weight) && ...
%                 (abs(DB.Sig_Kur(i)) > abs(DB.Pr_Kurtosis)*DB.Pr_Basic_Weight)
%             DB.Peak_Candi1(end+1) = i;
%             Last_x = i;
%             Last_y = DB.Sig_Filt(i);
%         end
%     elseif (sign(DB.Sig(i-1)-DB.Sig(i))~=0 && sign(DB.Sig(i)-DB.Sig(i+1)) == 0)
%         for j = 2 : min(DB.Sig_Length,i+5)
%             if sign(DB.Sig(i-1)-DB.Sig(i))*sign(DB.Sig(i)-DB.Sig(i+j)) == -1
%                 if (abs(DB.Sig_Filt(i)) > abs(DB.Pr_Amplitude)*DB.Pr_Basic_Weight) && ...
%                         (abs(DB.Sig_Kur(i)) > abs(DB.Pr_Kurtosis)*DB.Pr_Basic_Weight)
%                     if (Last_x + 5 < i || Last_y ~= DB.Sig_Filt(i)) || ...
%                             isempty(DB.Peak_Candi1)
%                         DB.Peak_Candi1(end+1) = i;
%                         Last_x = i;
%                         Last_y = DB.Sig_Filt(i);
%                     end
%                 end
%                 break
%             elseif sign(DB.Sig(i-1)-DB.Sig(i))*sign(DB.Sig(i)-DB.Sig(i+j)) == 1
%                 break
%             end
%         end
%     end
    if (min(DB.Sig_Filt(max(1,i-10):min(DB.Sig_Length,i+10))) == DB.Sig_Filt(i) || max(DB.Sig_Filt(max(1,i-10):min(DB.Sig_Length,i+10))) == DB.Sig_Filt(i)) && abs(DB.Sig_Filt(i)) >= abs(DB.Pr_Amplitude)*DB.Pr_Basic_Weight
        DB.Peak_Candi1(end+1) = i;
    end
end

%=======================================================================%
function DB = Find_QRS_Peak_Candi(DB)
Candi_Num = length(DB.Peak_Candi1);
DB.Q_peak_Candi = [];
DB.R_peak_Candi = [];
DB.S_peak_Candi = [];
DB.R_peak_Candi_Inverse = [];
if DB.Pr_Sign == 1
    for i = 1 : Candi_Num
        if sign(DB.Sig_Kur(DB.Peak_Candi1(i))) ~= DB.Pr_Sign
            %% R-peak 곡률 극대값 검출 시
            DB.R_peak_Candi(end+1) = DB.Peak_Candi1(i);
            DB.R_peak_Candi_Inverse(end+1) = 0;
            %% Q파 존재 시
            if DB.Pr1_exist == 1
                tmp = max(1,DB.R_peak_Candi(end) - DB.Pr_Length);
                [~,p] = min(DB.Sig_Filt(tmp : DB.R_peak_Candi(end)));
                DB.Q_peak_Candi(end+1) = tmp + p - 1;
            end
            %% S파 존재 시
            if DB.Pr3_exist == 1
                tmp = min(DB.Sig_Length,DB.R_peak_Candi(end) + DB.Pr_Length);
                [~,p] = min(DB.Sig_Filt(DB.R_peak_Candi(end) : tmp));
                DB.S_peak_Candi(end+1) = DB.R_peak_Candi(end) + p - 1;
            end
        else
            if DB.Pr1_exist == 1
                if DB.Pr3_exist == 1 %% Q,S 모두 존재, 분리 필요
                    % 다음 극대점 검출
                    tmp = max(1,DB.Peak_Candi1(i) - DB.Pr_Length);
                    tmp2 = min(DB.Sig_Length,DB.Peak_Candi1(i) + DB.Pr_Length);
                    [~,p] = max(DB.Sig_Filt(tmp:tmp2));
                    DB.R_peak_Candi(end+1) = tmp + p -1;
                    if DB.Peak_Candi1(i) > DB.R_peak_Candi(end)
                        DB.S_peak_Candi(end+1) = DB.Peak_Candi1(i);
                        tmp = max(1,DB.R_peak_Candi(end) - DB.Pr_Length);
                        [~,p] = min(DB.Sig_Filt(tmp : DB.R_peak_Candi(end)));
                        DB.Q_peak_Candi(end+1) = tmp + p - 1;                        
                    else
                        DB.Q_peak_Candi(end+1) = DB.Peak_Candi1(i);
                        tmp = min(DB.Sig_Length,DB.R_peak_Candi(end) + DB.Pr_Length);
                        [~,p] = min(DB.Sig_Filt(DB.R_peak_Candi(end) : tmp));
                        DB.S_peak_Candi(end+1) = DB.R_peak_Candi(end) + p - 1;                        
                    end
                    DB.R_peak_Candi_Inverse(end+1) = 0;
                elseif DB.Pr3_exist == 0 %% Q파만 존재
                    DB.Q_peak_Candi(end+1) = DB.Peak_Candi1(i);
                    tmp = min(DB.Sig_Length,DB.Peak_Candi1(i) + DB.Pr_Length);
                    [~,p] = max(DB.Sig_Filt(DB.Peak_Candi1(i):tmp));
                    DB.R_peak_Candi(end+1) = DB.Peak_Candi1(i) + p -1;
                    DB.R_peak_Candi_Inverse(end+1) = 0;
                end
            elseif DB.Pr1_exist == 0
                if DB.Pr3_exist == 1 %% S파만 존재 R파 검출 시작
                    DB.S_peak_Candi(end+1) = DB.Peak_Candi1(i);
                    tmp = max(1,DB.Peak_Candi1(i) - DB.Pr_Length);
                    [~,p] = max(DB.Sig_Filt(tmp:DB.Peak_Candi1(i)));
                    DB.R_peak_Candi(end+1) = tmp + p -1;
                    DB.R_peak_Candi_Inverse(end+1) = 0;
                elseif DB.Pr3_exist == 0 %% Q,S파 모두 없음 노이즈가 검출
                    DB.R_peak_Candi(end+1) = DB.Peak_Candi1(i);
                    DB.R_peak_Candi_Inverse(end+1) = 1;
                end
            end
        end
    end    
else
    for i = 1 : Candi_Num
        if sign(DB.Sig_Kur(DB.Peak_Candi1(i))) ~= DB.Pr_Sign
            %% R-peak 곡률 극대값 검출 시
            DB.R_peak_Candi(end+1) = DB.Peak_Candi1(i);
            DB.R_peak_Candi_Inverse(end+1) = 0;
            %% Q파 존재 시
            if DB.Pr1_exist == 1
                tmp = max(1,DB.R_peak_Candi(end) - DB.Pr_Length);
                [~,p] = max(DB.Sig_Filt(tmp : DB.R_peak_Candi(end)));
                DB.Q_peak_Candi(end+1) = tmp + p - 1;
            end
            %% S파 존재 시
            if DB.Pr3_exist == 1
                tmp = min(DB.Sig_Length,DB.R_peak_Candi(end) + DB.Pr_Length);
                [~,p] = max(DB.Sig_Filt(DB.R_peak_Candi(end) : tmp));
                DB.S_peak_Candi(end+1) = DB.R_peak_Candi(end) + p - 1;
            end
        else
            if DB.Pr1_exist == 1
                if DB.Pr3_exist == 1 %% Q,S 모두 존재, 분리 필요
                    % 다음 극대점 검출
                    tmp = max(1,DB.Peak_Candi1(i) - DB.Pr_Length);
                    tmp2 = min(DB.Sig_Length,DB.Peak_Candi1(i) + DB.Pr_Length);
                    [~,p] = min(DB.Sig_Filt(tmp:tmp2));
                    DB.R_peak_Candi(end+1) = tmp + p -1;
                    if DB.Peak_Candi1(i) > DB.R_peak_Candi(end+1)
                        DB.S_peak_Candi(end+1) = DB.Peak_Candi1(i);
                        tmp = max(1,DB.R_peak_Candi(end) - DB.Pr_Length);
                        [~,p] = max(DB.Sig_Filt(tmp : DB.R_peak_Candi(end)));
                        DB.Q_peak_Candi(end+1) = tmp + p - 1;                        
                    else
                        DB.Q_peak_Candi(end+1) = DB.Peak_Candi1(i);
                        tmp = min(DB.Sig_Length,DB.R_peak_Candi(end) + DB.Pr_Length);
                        [~,p] = max(DB.Sig_Filt(DB.R_peak_Candi(end) : tmp));
                        DB.S_peak_Candi(end+1) = DB.R_peak_Candi(end) + p - 1;                        
                    end
                    DB.R_peak_Candi_Inverse(end+1) = 0;
                elseif DB.Pr3_exist == 0 %% Q파만 존재
                    DB.Q_peak_Candi(end+1) = DB.Peak_Candi1(i);
                    tmp = min(DB.Sig_Length,DB.Peak_Candi1(i) + DB.Pr_Length);
                    [~,p] = min(DB.Sig_Filt(DB.Peak_Candi1(i):tmp));
                    DB.R_peak_Candi(end+1) = DB.Peak_Candi1(i) + p -1;
                    DB.R_peak_Candi_Inverse(end+1) = 0;
                end
            elseif DB.Pr1_exist == 0
                if DB.Pr3_exist == 1 %% S파만 존재 R파 검출 시작
                    DB.S_peak_Candi(end+1) = DB.Peak_Candi1(i);
                    tmp = max(1,DB.Peak_Candi1(i) - DB.Pr_Length);
                    [~,p] = min(DB.Sig_Filt(tmp:DB.Peak_Candi1(i)));
                    DB.R_peak_Candi(end+1) = tmp + p -1;
                    DB.R_peak_Candi_Inverse(end+1) = 0;
                elseif DB.Pr3_exist == 0 %% Q,S파 모두 없음 노이즈가 검출
                    DB.R_peak_Candi(end+1) = DB.Peak_Candi1(i);
                    DB.R_peak_Candi_Inverse(end+1) = 1;
                end
            end
        end
    end
end
%중복 제거
[~,p] = unique(DB.R_peak_Candi);
if DB.Pr1_exist == 1
    DB.Q_peak_Candi = DB.Q_peak_Candi(p);
end
DB.R_peak_Candi = DB.R_peak_Candi(p);
if DB.Pr3_exist == 1
    DB.S_peak_Candi = DB.S_peak_Candi(p);
end
DB.Peak_Candi1 = DB.Peak_Candi1(p);


%=======================================================================%
function DB = Find_QRS_Poly_Candi(DB)
Candi_Num = length(DB.Peak_Candi1);
DB.Q_peak_Candi_Poly_X = [];
DB.Q_peak_Candi_Poly_val = [];
DB.Q_peak_Candi_Poly_val2 = [];
DB.R_peak_Candi_Poly_X = [];
DB.R_peak_Candi_Poly_val = [];
DB.R_peak_Candi_Poly_val2 = [];
DB.S_peak_Candi_Poly_X = [];
DB.S_peak_Candi_Poly_val = [];
DB.S_peak_Candi_Poly_val2 = [];

for i = 1 : Candi_Num
    if DB.Pr1_exist == 1
        Poly_size = DB.Sig_Kur_Size(DB.Q_peak_Candi(i));
        A = DB.Sig_Kur(DB.Q_peak_Candi(i));
        C = DB.Sig(DB.Q_peak_Candi(i));
        DB.Q_peak_Candi_Poly_X{i} = - Poly_size : Poly_size;
        DB.Q_peak_Candi_Poly_val{i} = polyval([A,0,C], DB.Q_peak_Candi_Poly_X{i});
        DB.Q_peak_Candi_Poly_X{i} = DB.Q_peak_Candi_Poly_X{i} + DB.Q_peak_Candi(i);
        A = DB.Sig_Kur2(DB.Q_peak_Candi(i));
        C = DB.Sig_Filt(DB.Q_peak_Candi(i));
        DB.Q_peak_Candi_Poly_X{i} = - Poly_size : Poly_size;
        DB.Q_peak_Candi_Poly_val2{i} = polyval([A,0,C], DB.Q_peak_Candi_Poly_X{i});
        DB.Q_peak_Candi_Poly_X{i} = DB.Q_peak_Candi_Poly_X{i} + DB.Q_peak_Candi(i);
    end
    Poly_size = DB.Sig_Kur_Size(DB.R_peak_Candi(i));
    A = DB.Sig_Kur(DB.R_peak_Candi(i));
    C = DB.Sig(DB.R_peak_Candi(i));
    DB.R_peak_Candi_Poly_X{i} = - Poly_size : Poly_size;
    DB.R_peak_Candi_Poly_val{i} = polyval([A,0,C], DB.R_peak_Candi_Poly_X{i});
    DB.R_peak_Candi_Poly_X{i} = DB.R_peak_Candi_Poly_X{i} + DB.R_peak_Candi(i);
    A = DB.Sig_Kur2(DB.R_peak_Candi(i));
    C = DB.Sig_Filt(DB.R_peak_Candi(i));
    DB.R_peak_Candi_Poly_X{i} = - Poly_size : Poly_size;
    DB.R_peak_Candi_Poly_val2{i} = polyval([A,0,C], DB.R_peak_Candi_Poly_X{i});
    DB.R_peak_Candi_Poly_X{i} = DB.R_peak_Candi_Poly_X{i} + DB.R_peak_Candi(i);
    if DB.Pr3_exist == 1
        Poly_size = DB.Sig_Kur_Size(DB.S_peak_Candi(i));
        A = DB.Sig_Kur(DB.S_peak_Candi(i));
        C = DB.Sig(DB.S_peak_Candi(i));
        DB.S_peak_Candi_Poly_X{i} = - Poly_size : Poly_size;
        DB.S_peak_Candi_Poly_val{i} = polyval([A,0,C], DB.S_peak_Candi_Poly_X{i});
        DB.S_peak_Candi_Poly_X{i} = DB.S_peak_Candi_Poly_X{i} + DB.S_peak_Candi(i);
        A = DB.Sig_Kur(DB.S_peak_Candi(i));
        C = DB.Sig_Filt(DB.S_peak_Candi(i));
        DB.S_peak_Candi_Poly_X{i} = - Poly_size : Poly_size;
        DB.S_peak_Candi_Poly_val2{i} = polyval([A,0,C], DB.S_peak_Candi_Poly_X{i});
        DB.S_peak_Candi_Poly_X{i} = DB.S_peak_Candi_Poly_X{i} + DB.S_peak_Candi(i);
    end
end
%=======================================================================%
function DB = Find_R_peak_Detect_1(DB)
DB.Peak_Candi2 = [];
DB.Peak_Candi2_Index = [];
DB.R_peak_Detect = [];
DB.R_peak_Detect_Index = [];
for i = 1 : length(DB.R_peak_Candi)
    if abs(DB.Sig_Filt(DB.R_peak_Candi(i))) >= abs(DB.Pr_Amplitude_Weight*DB.Pr_Amplitude) && ...
            abs(DB.Sig_Kur(DB.R_peak_Candi(i))) >= abs(DB.Pr_Kurtosis_Weight*DB.Pr_Kurtosis)
        DB.R_peak_Detect(end+1) = DB.R_peak_Candi(i);
        DB.R_peak_Detect_Index(end+1) = i;
    else
        DB.Peak_Candi2(end+1) = DB.R_peak_Candi(i);
        DB.Peak_Candi2_Index(end+1) = i;
    end
end
DB.R_peak_Detect1 = DB.R_peak_Detect;
DB.R_peak_Detect1_Index = DB.R_peak_Detect_Index;
%=======================================================================%
function DB = Delete_R_peak_Detect_1(DB)
L = length(DB.R_peak_Detect);
DB.R_peak_Detect_Temp = [];
DB.R_peak_Detect_Temp_Index = [];
for i = 1 : L
    if DB.Sig_Kur(DB.R_peak_Candi(DB.R_peak_Detect_Index(i))) >= 0
        if DB.Sig_Kur(DB.Q_peak_Candi(DB.R_peak_Detect_Index(i))) <= 0 && ...
                DB.Sig_Kur(DB.S_peak_Candi(DB.R_peak_Detect_Index(i))) <= 0
            DB.R_peak_Detect_Temp(end+1) = DB.R_peak_Detect(i);
            DB.R_peak_Detect_Temp_Index(end+1) = DB.R_peak_Detect_Index(i);
        end
    elseif DB.Sig_Kur(DB.R_peak_Candi(DB.R_peak_Detect_Index(i))) <= 0
        if DB.Sig_Kur(DB.Q_peak_Candi(DB.R_peak_Detect_Index(i))) >= 0 && ...
                DB.Sig_Kur(DB.S_peak_Candi(DB.R_peak_Detect_Index(i))) >= 0
            DB.R_peak_Detect_Temp(end+1) = DB.R_peak_Detect(i);
            DB.R_peak_Detect_Temp_Index(end+1) = DB.R_peak_Detect_Index(i);
        end
    end
end
DB.R_peak_Detect = DB.R_peak_Detect_Temp;
DB.R_peak_Detect_Index = DB.R_peak_Detect_Temp_Index;

%=======================================================================%
function DB = Find_RR_Interval(DB)
DB.RR_Diff1 = DB.R_peak_Detect(2:end)-DB.R_peak_Detect(1:end-1);
DB.RR_Interval_Median = median(DB.RR_Diff1);
%=======================================================================%
function DB = Find_Peak_Candi_2(DB)
DB.Peak_Candi3 = DB.Peak_Candi2;
DB.Peak_Candi3_Index = DB.Peak_Candi2_Index;

for i = 1 : length(DB.RR_Diff1)
    if DB.RR_Diff1(i) < DB.RR_Interval_Median*DB.Candi_Max_Size
        Tmp = DB.Peak_Candi3<DB.R_peak_Detect(i) | DB.Peak_Candi3>DB.R_peak_Detect(i+1);
        DB.Peak_Candi3 = DB.Peak_Candi3(Tmp == 1);
        DB.Peak_Candi3_Index = DB.Peak_Candi3_Index(Tmp == 1);
    end
end
% DB.R_peak_Detect = sort([DB.R_peak_Detect, DB.Peak_Candi3]);
% DB.R_peak_Detect_Index = sort([DB.R_peak_Detect_Index, DB.Peak_Candi3_Index]);
%=======================================================================%
function DB = Find_R_peak_Detect_2(DB)
DB.R_peak_Detect2 = [];
DB.R_peak_Detect2_Index = [];
DB.RR_Ratio = [];
DB.RR_Ratio_X = [];
for i = 1 : length(DB.RR_Diff1)
    if DB.RR_Diff1(i) >= DB.RR_Interval_Median*DB.Candi_Max_Size
        Tmp = DB.Peak_Candi3>DB.R_peak_Detect(i) & DB.Peak_Candi3<DB.R_peak_Detect(i+1);
        Temp = DB.Peak_Candi3(Tmp == 1);
        Temp_Index = DB.Peak_Candi3_Index(Tmp == 1);
        Kur_Thr = mean(DB.Sig_Kur(DB.R_peak_Detect(i:i+1)));
        Cur_Thr = mean(DB.Sig_Cur(DB.R_peak_Detect(i:i+1)));
        if ~isempty(Temp)
            Candi_Num = round(DB.RR_Diff1(i)/DB.RR_Interval_Median)-1;
            if length(Temp) <= Candi_Num
                DB.R_peak_Detect2 = [DB.R_peak_Detect2,Temp];
                DB.R_peak_Detect2_Index = [DB.R_peak_Detect2_Index,Temp_Index];
            elseif Candi_Num~=0
                Temp_RR_Ratio_V = RR_Ratio_Create(DB.RR_Diff1(i)+1,Candi_Num,DB.fs);
                Temp_RR_Ratio = Temp_RR_Ratio_V(Temp-DB.R_peak_Detect(i)+1);
                Temp_Kur = min(abs(DB.Sig_Kur(Temp)/Kur_Thr),1);
%                 Temp_Cur = min(abs(DB.Sig_Cur(Temp)/Cur_Thr),1);
%                 Temp_Sum = Temp_Cur.*Temp_Kur.*Temp_RR_Ratio;
                Temp_Sum = Temp_Kur.*Temp_RR_Ratio;
                DB.RR_Ratio_X = [DB.RR_Ratio_X, Temp];
                DB.RR_Ratio = [DB.RR_Ratio, Temp_RR_Ratio];
                [~,p] = sort(Temp_Sum,'descend');
%                 DB.R_peak_Detect2 = [DB.R_peak_Detect2,sort(Temp(p(1:Candi_Num)))];
%                 DB.R_peak_Detect2_Index = [DB.R_peak_Detect2_Index,sort(Temp_Index(p(1:Candi_Num)))];
                DB.R_peak_Detect2 = [DB.R_peak_Detect2,sort(Temp(p(1)))];
                DB.R_peak_Detect2_Index = [DB.R_peak_Detect2_Index,sort(Temp_Index(p(1)))];
            end
        end
    end
end
DB.R_peak_Detect = sort([DB.R_peak_Detect, DB.R_peak_Detect2]);
DB.R_peak_Detect_Index = sort([DB.R_peak_Detect_Index, DB.R_peak_Detect2_Index]);
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