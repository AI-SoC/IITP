function [Interp_X,Interp_Y,Interp_X_Total,Interp_Y_Total, R_peak_Interp, t2] = FUN_Linear_Fitting(X,Y,R_peak,Fs, Curve_cal, Prog_cal, Mod_Type,d_max)
R_Len = length(R_peak);
m_R_peak = 1.5*median(R_peak(2:end)-R_peak(1:end-1));
%% �ٻ�ȭ
Interp_X = cell(1,R_Len-1);
Interp_Y = cell(1,R_Len-1);
Interp_X_Total = [];
Interp_Y_Total = [];
R_peak_Interp = zeros(1,R_Len);
R_peak_Interp(1) = 1;

e = zeros(1,R_Len-1);
% ��� ���
if Curve_cal == 1
    Win_H = round(Fs/100);
    Weight_x=1/Fs;
    [~, Sig_Cur] = Angle_Fun(X, Y, Win_H, Weight_x);
end
% keyboard
t2 = 0;
for i = 1 : R_Len-1
    Interp_Pos = [];
    if Curve_cal == 1
        % ������ �ʱ� ���� ����
        Type = 2;
        Interp_Cur_P = Sub_Curvature_Fitting(X(R_peak(i) : R_peak(i+1)), Sig_Cur(R_peak(i) : R_peak(i+1)), Type);
        % ���� ��ġ ����
%         Interp_Cur_P = R_peak(i) + Interp_Cur_P - 1;
    else
        Interp_Cur_P = [R_peak(i) , R_peak(i+1)];
    end
%     keyboard
    % ������ �߰� ���� ����
    for k = 1 : length(Interp_Cur_P)-1
        % �߰����� ����
        t1 = clock;
        if Prog_cal == 1
            Interp_Pro_P = Sub_Progressive_Fitting(X(Interp_Cur_P(k):Interp_Cur_P(k+1)), Y(Interp_Cur_P(k):Interp_Cur_P(k+1)), Fs, d_max);
        else
            % �̻��� �ش� ������ �糡���� ��ȯ
            Interp_Pro_P = [1,Interp_Cur_P(k+1)-Interp_Cur_P(k)+1];
        end
        % ���� ��ġ ����
        if Mod_Type == 1        % DP ����ȭ
            if R_peak(i+1)-R_peak(i) >= m_R_peak;
                Interp_Mod_P = Sub_Modified_Fitting(X(Interp_Cur_P(k):Interp_Cur_P(k+1)), Y(Interp_Cur_P(k):Interp_Cur_P(k+1)), Interp_Pro_P, Fs);
            else
%                 Interp_Mod_P = Fun_Dynamic_Programming5(X(Interp_Cur_P(k):Interp_Cur_P(k+1)), Y(Interp_Cur_P(k):Interp_Cur_P(k+1)), Interp_Pro_P, Fs);
%                 Interp_Mod_P = Fun_Dynamic_Programming6(X(Interp_Cur_P(k):Interp_Cur_P(k+1)), Y(Interp_Cur_P(k):Interp_Cur_P(k+1)), Interp_Pro_P, Fs);
%                 Interp_Mod_P = Fun_Dynamic_Programming7(X(Interp_Cur_P(k):Interp_Cur_P(k+1)), Y(Interp_Cur_P(k):Interp_Cur_P(k+1)), Interp_Pro_P, Fs);
%                 Interp_Mod_P = Fun_Dynamic_Programming8(X(Interp_Cur_P(k):Interp_Cur_P(k+1)), Y(Interp_Cur_P(k):Interp_Cur_P(k+1)), Interp_Pro_P, Fs);
%                 Interp_Mod_P = Fun_Dynamic_Programming9(X(Interp_Cur_P(k):Interp_Cur_P(k+1)), Y(Interp_Cur_P(k):Interp_Cur_P(k+1)), Interp_Pro_P, Fs);
                Interp_Mod_P = Fun_Dynamic_Programming21(X(Interp_Cur_P(k):Interp_Cur_P(k+1)), Y(Interp_Cur_P(k):Interp_Cur_P(k+1)), Interp_Pro_P, Fs);
%                 Interp_Mod_P = Fun_Dynamic_Programming(X(Interp_Cur_P(k):Interp_Cur_P(k+1)), Y(Interp_Cur_P(k):Interp_Cur_P(k+1)), Interp_Pro_P, Fs);
            end
        elseif Mod_Type == 2    % ������ ����ȭ
            Interp_Mod_P = Sub_Modified_Fitting(X(Interp_Cur_P(k):Interp_Cur_P(k+1)), Y(Interp_Cur_P(k):Interp_Cur_P(k+1)), Interp_Pro_P, Fs);
        else
            % �̻��� ������ �����ٻ�ȭ ��� �״�� ��ȯ
            Interp_Mod_P = Interp_Pro_P;
        end
        t2 = t2+etime(clock,t1);
        % ������ �߰����� ��� ����
        Interp_Mod_P = Interp_Mod_P + Interp_Cur_P(k) - 1;
        Interp_Pos = [Interp_Pos(1:end-1), Interp_Mod_P];
    end
    % RR���� �� �����ٻ�ȭ ��� ����
    Interp_X{i} = X(Interp_Pos);
    Interp_Y{i} = Y(Interp_Pos);
    Interp_X_Total = [Interp_X_Total(1:end-1), X(Interp_Pos)];
    Interp_Y_Total = [Interp_Y_Total(1:end-1), Y(Interp_Pos)];
    R_peak_Interp(i+1) = length(Interp_X_Total);
end