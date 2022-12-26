function DB = Primitive_QRS_20160211(DB)
DB.Sig_Length = length(DB.Sig);
DB.Pr_Length = length(DB.Pr);

DB.Sig_Thr = DB.Sig_Prod_Smooth>=DB.Pr_Max*DB.Pr_Basic_Weight;

N = 1;
M = 1;
DB.Sig_Blob = [];

while N <=DB.Sig_Length
    if DB.Sig_Thr(N) == 1
        for i = N+1 : DB.Sig_Length
            if DB.Sig_Thr(i) == 0
                DB.Sig_Blob(M).Pos = N : i-1;
                N = i-1;
                break
            elseif i == DB.Sig_Length
                DB.Sig_Blob(M).Pos = N : DB.Sig_Length;
                N = i;
            end
        end
        M = M+1;
    end
    N = N+1;
end
N = N-1;
M = M-1;
for i = 1 : M
    tmp = -inf;
    DB.Sig_Blob(i).Max_Pos = DB.Sig_Blob(i).Pos(1);
    for j = 1 : length(DB.Sig_Blob(i).Pos)
        if DB.Sig_Filt(DB.Sig_Blob(i).Pos(j)) > tmp
            DB.Sig_Blob(i).Max_Pos = DB.Sig_Blob(i).Pos(j);
            tmp = DB.Sig_Filt(DB.Sig_Blob(i).Pos(j));
        end
    end
    DB.R_peak_Detect(i) = DB.Sig_Blob(i).Max_Pos;
end
%% 검출율 측정
DB = Check_Detection_Result(DB);
%=======================================================================%
%=======================================================================%
%=======================================================================%
function DB = Check_Detection_Result(DB)
% 후보점 체크
Ms = DB.fs*0.1;
N1 = 1; % DB.R_peak Index
L1 = length(DB.R_peak);

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