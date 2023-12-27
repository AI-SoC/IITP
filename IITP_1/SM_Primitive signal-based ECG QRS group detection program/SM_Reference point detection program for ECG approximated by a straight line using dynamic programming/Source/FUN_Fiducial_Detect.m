function [Q_on, Q_on_Index, S_off, S_off_Index] = FUN_Fiducial_Detect(Interp_X,Interp_Y, Interp_R_peak,Fs)

N = length(Interp_R_peak);

Q_on = [];
S_off = [];
if N <= 2
    return
end
Q_on = zeros(1,N-1);
Q_peak = zeros(1,N-1);
S_off = zeros(1,N-1);
S_peak = zeros(1,N-1);
Q_on_Index = zeros(1,N-1);
Q_peak_Index = zeros(1,N-1);
S_off_Index = zeros(1,N-1);
S_peak_Index = zeros(1,N-1);

% Score_Result_off = [];
% Score_Result_on = [];
% Score_Result_off2 = [];
% Score_Result_on2 = [];
for i = 1 : N-1
    Interp_X_Temp = Interp_X(Interp_R_peak(i):Interp_R_peak(i+1));
    Interp_Y_Temp = Interp_Y(Interp_R_peak(i):Interp_R_peak(i+1));
    
    Ex_Q = [];
    Ex_S = [];
    if length(Interp_X_Temp) == 1
        Q_on(i) = Interp_X_Temp(1);
        Q_peak(i) = Interp_X_Temp(1);
        S_peak(i) = Interp_X_Temp(1);
        S_off(i) = Interp_X_Temp(1);
        continue
    end
    
    [Q_on_Index(i),Q_peak_Index(i), S_peak_Index(i), S_off_Index(i),Q_Check,S_Check] = FUN_QRS_peak_Detection(Interp_X_Temp, Interp_Y_Temp, Fs,Ex_Q,Ex_S,i);
%     [Q_on_Index(i),Q_peak_Index(i), S_peak_Index(i), S_off_Index(i),Q_Check,S_Check, Score1, Score2,Score3,Score4] = FUN_QRS_peak_Detection(Interp_X_Temp, Interp_Y_Temp, Fs,Ex_Q,Ex_S,i);
    Q_on(i) = Interp_X_Temp(Q_on_Index(i));
    Q_peak(i) = Interp_X_Temp(Q_peak_Index(i));
    S_peak(i) = Interp_X_Temp(S_peak_Index(i));
    S_off(i) = Interp_X_Temp(S_off_Index(i));
%     Score_Result_off= [Score_Result_off, Score1];
%     Score_Result_on= [Score_Result_on, Score2];
%     Score_Result_off2= [Score_Result_off2, Score3];
%     Score_Result_on2= [Score_Result_on2, Score4];
%     keyboard
end
% keyboard

