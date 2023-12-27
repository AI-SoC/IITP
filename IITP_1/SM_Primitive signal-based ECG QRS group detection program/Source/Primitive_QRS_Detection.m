function DB = Primitive_QRS_Detection(DB)

Sig_Length = length(DB.Sig);
Pr_Length = length(DB.Pr);

%% Primitive Fitting
Poly_Size = round(length(DB.Pr2)/4);
DB.Pr2_Poly_X = DB.Pr2_X(DB.Pr2_Max_Pos)-Poly_Size : DB.Pr2_X(DB.Pr2_Max_Pos)+Poly_Size;
DB.Pr2_Poly_Move = mean(DB.Pr2_Poly_X);
DB.Pr2_Poly = polyfit(DB.Pr2_Poly_X-DB.Pr2_Poly_Move,DB.Sig(DB.Pr2_Poly_X),2);
DB.Pr2_Poly_val = polyval(DB.Pr2_Poly, DB.Pr2_Poly_X);

%% 후보점 검출
DB.R_peak_Candi_Temp = [];
Candi_Cur_Check_Size = round(DB.fs*0.20);
if DB.Pr_Sign == 1    
    for i = Poly_Size+1 : Sig_Length-Poly_Size
        if (min(DB.Sig_Cur(max(1,i-Candi_Cur_Check_Size) : min(Sig_Length,i+Candi_Cur_Check_Size))) == DB.Sig_Cur(i))
            DB.R_peak_Candi_Temp = [DB.R_peak_Candi_Temp, i];
        end
    end
elseif DB.Pr_Sign == -1
    if (max(DB.Sig_Cur(max(1,i-Candi_Cur_Check_Size) : min(Sig_Length,i+Candi_Cur_Check_Size))) == DB.Sig_Cur(i))
        DB.R_peak_Candi_Temp = [DB.R_peak_Candi_Temp, i];
    end
end

%% 후보점 Fitting
% 2차 피팅
DB.Move_X = [];
DB.R_peak_Candi_Temp_Poly_X = [];
DB.R_peak_Candi_Temp_Poly = [];
DB.R_peak_Candi_Temp_Poly_val = [];

for i = 1 : length(DB.R_peak_Candi_Temp)
    Temp_X = DB.R_peak_Candi_Temp(i)-Poly_Size : DB.R_peak_Candi_Temp(i)+Poly_Size;
    DB.Move_X(i) = mean(Temp_X);
    DB.R_peak_Candi_Temp_Poly_X(i,:) = Temp_X-DB.Move_X(i);
    DB.R_peak_Candi_Temp_Poly(i,:) = polyfit(DB.R_peak_Candi_Temp_Poly_X(i,:),DB.Sig(Temp_X),2);
    DB.R_peak_Candi_Temp_Poly_val(i,:) = polyval(DB.R_peak_Candi_Temp_Poly(i,:),DB.R_peak_Candi_Temp_Poly_X(i,:));
    DB.R_peak_Candi_Temp_Poly_X(i,:) = DB.R_peak_Candi_Temp_Poly_X(i,:) + DB.Move_X(i);
end

%% 후보점 수정
% 2차 피팅 극점으로 수정, 피팅 범위 내에 극점 존재
DB.R_peak_Candi = [];
DB.R_peak_Candi_Poly = [];
DB.R_peak_Candi_Poly_X = [];
DB.R_peak_Candi_Poly_val = [];
for i = 1 : length(DB.R_peak_Candi_Temp)
    DB.R_peak_Candi_Move(i) = round(DB.R_peak_Candi_Temp_Poly(i,2)/(-2*DB.R_peak_Candi_Temp_Poly(i,1)) + DB.Move_X(i));
    if abs(DB.R_peak_Candi_Move(i) - DB.R_peak_Candi_Temp(i)) < Poly_Size
        DB.R_peak_Candi(end+1) = DB.R_peak_Candi_Move(i);
        DB.R_peak_Candi_Poly(end+1,:) = DB.R_peak_Candi_Temp_Poly(i,:);
        DB.R_peak_Candi_Poly_X(end+1,:) = DB.R_peak_Candi_Temp_Poly_X(i,:);
        DB.R_peak_Candi_Poly_val(end+1,:) = DB.R_peak_Candi_Temp_Poly_val(i,:);
    end
end

%% R-peak 검출
Poly_Ratio = 0.25;
DB.Poly_Thr = DB.Pr2_Poly(1)*Poly_Ratio;

DB.R_peak_Detect = [];

if DB.Poly_Thr <= 0
    DB.R_peak_Detect = DB.R_peak_Candi(DB.R_peak_Candi_Poly(:,1)<=DB.Poly_Thr)';
    DB.R_peak_Detect_Poly = DB.R_peak_Candi_Poly(DB.R_peak_Candi_Poly(:,1)<=DB.Poly_Thr,:);
else
    DB.R_peak_Detect = DB.R_peak_Candi(DB.R_peak_Candi_Poly(:,1)>DB.Poly_Thr)';
    DB.R_peak_Detect_Poly = DB.R_peak_Candi_Poly(DB.R_peak_Candi_Poly(:,1)>DB.Poly_Thr,:);
end

%% 검출율 측정
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
        if DB.R_peak_text(N1) == 'N'
            DB.Candi_True_Index = [DB.Candi_True_Index,N2];
            N1 = N1 + 1;
            N2 = N2 + 1;
        else
            N1 = N1 + 1;
            N2 = N2 + 1;
        end
    elseif DB.R_peak(N1) > DB.R_peak_Candi(N2) 
        DB.Candi_False_Index = [DB.Candi_False_Index,N2];
        N2 = N2 + 1;
    elseif DB.R_peak(N1) < DB.R_peak_Candi(N2)
        if DB.R_peak_text(N1) == 'N'
            DB.F_Index_Candi = [DB.F_Index_Candi,DB.R_peak(N1)];
            N1 = N1 + 1;
        else
            N1 = N1 + 1;
        end
    end
end

if N1 > L1
    DB.Candi_False_Index = [DB.Candi_False_Index,N2 : L2];
end
if N2 > L2
    for i = N1 : L1
        if DB.R_peak_text(i) == 'N'
            DB.F_Index_Candi = [DB.F_Index_Candi,DB.R_peak(i)];
        end
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
DB.F_Index = [];
while N1 <= L1 && N2 <= L2
    if abs(DB.R_peak(N1)-DB.R_peak_Detect(N2))< Ms
        if DB.R_peak_text(N1) == 'N'
            DB.tp = DB.tp + 1;
            N1 = N1 + 1;
            N2 = N2 + 1;
        else
            N1 = N1 + 1;
            N2 = N2 + 1;
        end
    elseif DB.R_peak(N1) > DB.R_peak_Detect(N2) 
        DB.fp = DB.fp + 1;
        N2 = N2 + 1;
    elseif DB.R_peak(N1) < DB.R_peak_Detect(N2)
        if DB.R_peak_text(N1) == 'N'
            DB.fn = DB.fn + 1;
            DB.F_Index = [DB.F_Index,DB.R_peak(N1)];
            N1 = N1 + 1;
        else
            N1 = N1 + 1;
        end
    end
end

if N1 > L1
    DB.fp = DB.fp + (L2 - N2 + 1);
end
if N2 > L2
    for i = N1 : L1
        if DB.R_peak_text(i) == 'N'
            DB.fn = DB.fn + 1;
            DB.F_Index = [DB.F_Index,DB.R_peak(i)];
        end
    end
end

DB.F_L = length(DB.F_Index);
DB.False_View = 0;
