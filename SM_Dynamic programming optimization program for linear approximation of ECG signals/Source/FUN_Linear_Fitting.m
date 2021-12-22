function [Interp_X,Interp_Y,Interp_X_Total,Interp_Y_Total, R_peak_Interp, t2] = FUN_Linear_Fitting(X,Y,R_peak,Fs, Curve_cal, Prog_cal, Mod_Type,d_max)
R_Len = length(R_peak);
m_R_peak = 1.5*median(R_peak(2:end)-R_peak(1:end-1));
%% 근사화
Interp_X = cell(1,R_Len-1);
Interp_Y = cell(1,R_Len-1);
Interp_X_Total = [];
Interp_Y_Total = [];
R_peak_Interp = zeros(1,R_Len);
R_peak_Interp(1) = 1;

e = zeros(1,R_Len-1);
% 곡률 계산
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
        % 곡률기반 초기 정점 선택
        Type = 2;
        Interp_Cur_P = Sub_Curvature_Fitting(X(R_peak(i) : R_peak(i+1)), Sig_Cur(R_peak(i) : R_peak(i+1)), Type);
        % 정점 위치 보정
%         Interp_Cur_P = R_peak(i) + Interp_Cur_P - 1;
    else
        Interp_Cur_P = [R_peak(i) , R_peak(i+1)];
    end
%     keyboard
    % 구간별 추가 정점 선택
    for k = 1 : length(Interp_Cur_P)-1
        % 추가정점 선택
        t1 = clock;
        if Prog_cal == 1
            Interp_Pro_P = Sub_Progressive_Fitting(X(Interp_Cur_P(k):Interp_Cur_P(k+1)), Y(Interp_Cur_P(k):Interp_Cur_P(k+1)), Fs, d_max);
        else
            % 미사용시 해당 구간의 양끝점을 반환
            Interp_Pro_P = [1,Interp_Cur_P(k+1)-Interp_Cur_P(k)+1];
        end
        % 정점 위치 보정
        if Mod_Type == 1        % DP 최적화
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
        elseif Mod_Type == 2    % 지역적 최적화
            Interp_Mod_P = Sub_Modified_Fitting(X(Interp_Cur_P(k):Interp_Cur_P(k+1)), Y(Interp_Cur_P(k):Interp_Cur_P(k+1)), Interp_Pro_P, Fs);
        else
            % 미사용시 순차적 직선근사화 결과 그대로 반환
            Interp_Mod_P = Interp_Pro_P;
        end
        t2 = t2+etime(clock,t1);
        % 구간별 추가정점 결과 누적
        Interp_Mod_P = Interp_Mod_P + Interp_Cur_P(k) - 1;
        Interp_Pos = [Interp_Pos(1:end-1), Interp_Mod_P];
    end
    % RR구간 별 직선근사화 결과 누적
    Interp_X{i} = X(Interp_Pos);
    Interp_Y{i} = Y(Interp_Pos);
    Interp_X_Total = [Interp_X_Total(1:end-1), X(Interp_Pos)];
    Interp_Y_Total = [Interp_Y_Total(1:end-1), Y(Interp_Pos)];
    R_peak_Interp(i+1) = length(Interp_X_Total);
end