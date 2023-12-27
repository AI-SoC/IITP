function DB = Primitive_QRS_20160212(DB)
DB.Sig_Length = length(DB.Sig);
DB.Pr_Length = length(DB.Pr);

% 1차 검출
% DB = R_peak_Detect1(DB);
%% RR간격 체크 후 노이즈 영역 제거
% DB = R_peak_Delete1(DB);
%% RR간격 체크 후 추가검출
% DB = R_peak_Detect2(DB);
%% RR간격 체크 후 노이즈 제거
% DB = R_peak_Delete2(DB);
%% Ind값 전역 Threshold
% DB.R_peak_Detect = DB.R_peak_Detect(DB.Sig_Ind(DB.R_peak_Detect)>0.002);
%% R-peak 위치 보정
DB = R_peak_Correct(DB);
%% 검출율 측정
% DB = Check_Detection_Result(DB);

%% QRS 구간 검출
DB = QRS_Detect(DB);

%% QRS 오차
DB = QRS_Check(DB);


%=======================================================================%
function DB = R_peak_Detect1(DB)

DB.R_peak_Detect = [];
Max_Size = round(DB.fs*DB.Max_Size_Weight);
DB.RR_Dist_Median = round(DB.fs*0.8);
DB.RR_Dist_L = DB.RR_Dist_Median;
DB.RR_Dist_R = DB.RR_Dist_Median;
% 초기 검출
for i = 1 : DB.Sig_Length
%     if DB.Sig_Diff(i) == max(DB.Sig_Diff(max(1,i-Max_Size) : min(DB.Sig_Length,i+Max_Size))) && ...
%             DB.Sig_Diff(i) >= 0.001 
    if DB.Sig_Ind(i) == max(DB.Sig_Ind(max(1,i-Max_Size) : min(DB.Sig_Length,i+Max_Size))) && ...
            DB.Sig_Ind(i) >= DB.Pr_Max*DB.Pr_Basic_Weight
        DB.R_peak_Detect(1) = i;
        Next_Start = i;
        break
    end
end

% 이후 검출
for i = Next_Start+1 : DB.Sig_Length
%     if DB.Sig_Diff(i) == max(DB.Sig_Diff(max(1,i-Max_Size) : min(DB.Sig_Length,i+Max_Size))) && ...
%             DB.Sig_Diff(i) >= 0.001
    if DB.Sig_Ind(i) == max(DB.Sig_Ind(max(1,i-Max_Size) : min(DB.Sig_Length,i+Max_Size))) && ...
            DB.Sig_Ind(i) >= min(DB.Pr_Max,DB.Sig_Ind(DB.R_peak_Detect(end)))*DB.Ind_Thr_Weight
        DB.R_peak_Detect(end+1) = i;
    end
end
DB.R_peak_Detect1 = DB.R_peak_Detect;
%=======================================================================%
function DB = R_peak_Delete1(DB)
L = length(DB.R_peak_Detect);
DB.R_peak_Delete = [];
Thr = max(DB.Sig_Diff(DB.Pr_X));
for i = 1 : L
%     Thr = median(DB.Sig_Diff(max(1,DB.R_peak_Detect(i)-100) :min(DB.Sig_Length, DB.R_peak_Detect(i)+100)));
%     if DB.Sig_Diff(DB.R_peak_Detect(i)) <= Thr*10
%         DB.R_peak_Delete(end+1) = i;
%     end
    if DB.Sig_Diff(DB.R_peak_Detect(i)) <= Thr*DB.Ind_Delete_Weight
        DB.R_peak_Delete(end+1) = i;
    end
end
DB.R_peak_Detect(DB.R_peak_Delete) = [];
DB.R_peak_Detect2 = DB.R_peak_Detect;

%=======================================================================%
function DB = R_peak_Detect2(DB)
DB.RR_Median = median(DB.R_peak_Detect(2:end)-DB.R_peak_Detect(1:end-1));
DB.RR_Ratio = zeros(1,DB.Sig_Length);
while 1
    N = length(DB.R_peak_Detect);
    DB.RR_Diff = DB.R_peak_Detect(2:end) - DB.R_peak_Detect(1:end-1);
    for i = 1 : length(DB.RR_Diff)
%         Candi_Num = round(DB.RR_Diff(i)/DB.RR_Median)-1;
        Candi_Num = (DB.RR_Diff(i)/DB.RR_Median)-1;
        if rem(Candi_Num,1) <= DB.PVC_Weight
            Candi_Num = floor(Candi_Num);
        else
            Candi_Num = ceil(Candi_Num);
        end
        if Candi_Num == 1
%             if round((DB.RR_Diff(i)+DB.RR_Diff(i-1))/DB.RR_Median) - 1 == 1
            if i > 1 && ((DB.RR_Diff(i)+DB.RR_Diff(i-1))/DB.RR_Median) - 1 <= 1+DB.PVC_Weight
                continue
            end
            Temp_X = DB.R_peak_Detect(i) : DB.R_peak_Detect(i+1);
            Temp_RR_Ratio = RR_Ratio_Create(DB.RR_Diff(i)+1,DB.fs);
            Temp_Diff = DB.Sig_Diff(Temp_X);
            Temp = Temp_RR_Ratio.*Temp_Diff;
            [~,p] = max(Temp);
            DB.R_peak_Detect = [DB.R_peak_Detect(1:i),DB.R_peak_Detect(i)+p-1,DB.R_peak_Detect(i+1:end)];
            break
        elseif Candi_Num > 1
            if i == 1
                Temp_RR_Median = DB.RR_Median;
            else
                Temp_RR_Median = median(DB.R_peak_Detect(2:i)-DB.R_peak_Detect(1:i-1));
            end
            Temp_X = DB.R_peak_Detect(i) : DB.R_peak_Detect(i)+2*Temp_RR_Median;
            Temp_RR_Ratio = RR_Ratio_Create(length(Temp_X),DB.fs);
            Temp_Diff = DB.Sig_Diff(Temp_X);
            Temp = Temp_RR_Ratio.*Temp_Diff;
            [~,p] = max(Temp);
            DB.R_peak_Detect = [DB.R_peak_Detect(1:i),DB.R_peak_Detect(i)+p-1,DB.R_peak_Detect(i+1:end)];
            break
        end
    end
    if N == length(DB.R_peak_Detect)
        break
    end
end
%=======================================================================%
function DB = R_peak_Delete2(DB)
DB.RR_Diff = DB.R_peak_Detect(2:end) - DB.R_peak_Detect(1:end-1);
DB.RR_Diff_Sum =[0, DB.RR_Diff(1:end-1) + DB.RR_Diff(2:end), 0];
DB.RR_Diff_Median = median(DB.RR_Diff);
N = length(DB.R_peak_Detect);
DB.R_peak_Delete = [];
for i = 2 : N-1
    if DB.RR_Diff_Sum(i)<= DB.RR_Diff_Median*(1+DB.PVC_Weight) && ...
            DB.RR_Diff_Sum(i) <= DB.RR_Diff_Sum(i-1) && DB.RR_Diff_Sum(i) <= DB.RR_Diff_Sum(i+1)
        DB.R_peak_Delete(end+1) = i;
    end
end
DB.R_peak_Detect(DB.R_peak_Delete) = [];
        
%=======================================================================%
function DB = Check_Detection_Result(DB)
% 후보점 체크
Ms = DB.fs*0.1;
N1 = 1; % DB.R_peak Index
L1 = length(DB.R_peak);

% R-peak 검출율
% TP: 정상파형 정상검출, FP: 정상&부정맥 이외의 영역 검출, FN: 정상 미검출
Ms = DB.fs*0.2;
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


%=======================================================================%
function DB = QRS_Detect(DB)
L = length(DB.R_peak_Detect); % R-peak 개수

DB.Pr_Expand_size = 0;
% DB.Pr_Expand_size = min([10,DB.Pr_X(1)-1,length(DB.Sig)-(DB.Pr_X(end)+1)]);

DB.Pr_L_X = DB.Pr_X(1)-DB.Pr_Expand_size : DB.Pr_X(DB.Pr_Max_Pos);
DB.Pr_L = DB.Sig(DB.Pr_L_X);
DB.Pr_R_X = DB.Pr_X(DB.Pr_Max_Pos):DB.Pr_X(end)+DB.Pr_Expand_size;
DB.Pr_R = DB.Sig(DB.Pr_R_X);

DB.QRS_on_Detect = zeros(1,L);
DB.QRS_on_Detect_X = cell(1,L);
DB.QRS_on_Detect_Data = cell(1,L);

DB.QRS_off_Detect = zeros(1,L);
DB.QRS_off_Detect_X = cell(1,L);
DB.QRS_off_Detect_Data = cell(1,L);
for i = 1 : L
    %% 좌측 최적화
    DB = QRS_Detect_Left(DB,i);
    %% 우측 최적화
    DB = QRS_Detect_Right(DB,i);
end

%=======================================================================%
function DB = QRS_Detect_Left(DB,N)
L = length(DB.Pr_L);
Peak = DB.R_peak_Detect(N);
Size = 8;
S1 = max(1,Peak - (L-1+Size));
S2 = max(1,Peak - max(4,L-1-Size));
Size_Table = S1 : S2;
Size_Length = length(Size_Table);
Diff_Table = zeros(1,Size_Length);
Pr_Temp = cell(1,Size_Length);
for i = 1 : Size_Length
    Sig_Temp = DB.Sig(Size_Table(i) : Peak);
    L_temp = length(Sig_Temp);
    Pr_Temp{i} = DB.Pr_L*(Sig_Temp(1)-Sig_Temp(end))/(DB.Pr_L(1)-DB.Pr_L(end));
    Pr_Temp{i} = Pr_Temp{i} - Pr_Temp{i}(end) + Sig_Temp(end);
    Pr_Temp{i} = interp1(linspace(0,1,L),Pr_Temp{i},linspace(0,1,L_temp));
    Diff_Table(i) = mean(abs(Pr_Temp{i} - Sig_Temp));
%     Diff_Table(i) = max(0,1-sum( ((Pr_Temp{i}-Sig_Temp).^2)/(sum(Sig_Temp.^2)-mean(Sig_Temp)^2) ));
end
[~,p] = min(Diff_Table);
% [~,p] = max(Diff_Table);
% DB.QRS_on_Detect(N) = Size_Table(p);
DB.QRS_on_Detect(N) = Size_Table(p)+DB.Pr_Expand_size;
DB.QRS_on_Detect_X{N} = Size_Table(p) : Peak;
DB.QRS_on_Detect_Data{N} = Pr_Temp{p};

% figure
% hold on
% plot(DB.Sig,'k','linewidth',2)
% for i = 1 : Size_Length
%     plot(Size_Table(i) : Peak, Pr_Temp{i},'r')
% end
% plot(Size_Table(p):Peak,DB.Sig(Size_Table(p):Peak),'b','linewidth',4)
% plot(DB.Sig,'k','linewidth',2)
% hold off
% keyboard
%=======================================================================%
function DB = QRS_Detect_Right(DB,N)
L = length(DB.Pr_R);
Sig_L = length(DB.Sig);
Peak = DB.R_peak_Detect(N);
Size = 8;
S1 = min(Sig_L,Peak + max(4,length(DB.Pr_R)-1-Size));
S2 = min(Sig_L,Peak + length(DB.Pr_R)-1+Size);
Size_Table = S1 : S2;
Size_Length = length(Size_Table);
Diff_Table = zeros(1,Size_Length);
Pr_Temp = cell(1,Size_Length);
for i = 1 : Size_Length
    Sig_Temp = DB.Sig(Peak : Size_Table(i));
    L_temp = length(Sig_Temp);
    Pr_Temp{i} = DB.Pr_R*(Sig_Temp(1)-Sig_Temp(end))/(DB.Pr_R(1)-DB.Pr_R(end));
    Pr_Temp{i} = Pr_Temp{i} - Pr_Temp{i}(end) + Sig_Temp(end);
    Pr_Temp{i} = interp1(linspace(0,1,L),Pr_Temp{i},linspace(0,1,L_temp));
    Diff_Table(i) = mean(abs(Pr_Temp{i} - Sig_Temp));
%     Diff_Table(i) = max(0,1-sum( ((Pr_Temp{i}-Sig_Temp).^2)/(sum(Sig_Temp.^2)-mean(Sig_Temp)^2) ));
end
[~,p] = min(Diff_Table);
% [~,p] = max(Diff_Table);
% DB.QRS_off_Detect(N) = Size_Table(p);
DB.QRS_off_Detect(N) = Size_Table(p)-DB.Pr_Expand_size;
DB.QRS_off_Detect_X{N} = Peak : Size_Table(p);
DB.QRS_off_Detect_Data{N} = Pr_Temp{p};

%=======================================================================%
function DB = R_peak_Correct(DB)
N = length(DB.R_peak);
L = length(DB.Sig);
Size = 15;
for i = 1 : N
    S1 = max(1,DB.R_peak(i) - Size);
    S2 = min(L,DB.R_peak(i) + Size);
%     if DB.Sig_Filt(DB.R_peak_Detect(i)) >= 0
%     if abs(max(DB.Sig_Kur(S1:S2))) < abs(min(DB.Sig_Kur(S1:S2)))
    if abs(max(DB.Sig_Filt(S1:S2))) > abs(min(DB.Sig_Filt(S1:S2)))
        [~,p] = max(DB.Sig(S1 : S2));
    else
        [~,p] = min(DB.Sig(S1 : S2));
    end
    DB.R_peak_Detect(i) = S1+p-1;
end


%=======================================================================%
function DB = QRS_Check(DB)
% keyboard
Diff_Q_on = DB.Q_on - DB.QRS_on_Detect;
Diff_S_off = DB.S_off - DB.QRS_off_Detect;
Diff_QRS_Duration = (DB.S_off - DB.Q_on) - (DB.QRS_off_Detect-DB.QRS_on_Detect);

DB.Diff_Table = [mean(Diff_Q_on), std(Diff_Q_on), mean(Diff_S_off), std(Diff_S_off), mean(Diff_QRS_Duration), std(Diff_QRS_Duration)];



