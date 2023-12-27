function [Q_on,Q_peak, S_peak, S_off,Q_Check,S_Check] = FUN_QRS_peak_Detection(X,Y,Fs,Ex_Q, Ex_S,N)
% function [Q_on,Q_peak, S_peak, S_off,Q_Check,S_Check, Total_Score1, Total_Score2, Total_Score3, Total_Score4] = FUN_QRS_peak_Detection(X,Y,Fs,Ex_Q, Ex_S,N)

% 예외처리
if length(X) <= 3
    S_off = max(1,length(X)-1);
    S_peak = S_off;
    
    Q_on = max(1,length(X)-1);
    Q_peak = Q_on;
    Q_Check = 0;
    S_Check = 0;
    return
end
Temp_D = Y(2:end) - Y(1:end-1);

% 신호 단조증가 변형
Sig_Temp_X = X;
Sig_Temp_Y = zeros(1,length(X));

for j = 1 : length(X)-1
    Sig_Temp_Y(j+1) = Sig_Temp_Y(j) + abs(Temp_D(j));
end

% 각도 측정
Angle_Weight = 0.5;
DX = Sig_Temp_X(2:end) - Sig_Temp_X(1:end-1);
DY = Sig_Temp_Y(2:end) - Sig_Temp_Y(1:end-1);
DL = atan(Angle_Weight*Fs*DY(1:end-1)./DX(1:end-1))*180/pi;
DR = atan(Angle_Weight*Fs*DY(2:end)./DX(2:end))*180/pi;
D = DR-DL+180;

DX2 = X(2:end) - X(1:end-1);
DY2 = Y(2:end) - Y(1:end-1);
DL2 = atan(Fs*DY2(1:end-1)./DX2(1:end-1))*180/pi;
DR2 = atan(Fs*DY2(2:end)./DX2(2:end))*180/pi;
D2 = DR2-DL2+180;

% 탐색구간 설정

S_end = 6;
if S_end >= length(Sig_Temp_X)-2
    S_end = length(Sig_Temp_X)-2;
end
for j = 3 : S_end
%     if Sig_Temp_X(j) - Sig_Temp_X(1) >= 0.20*Fs
    if Sig_Temp_X(j) - Sig_Temp_X(1) >= 0.15*Fs
        S_end = j-1;
        break
    end
end



QRS_D = Y(2: S_end) - Y(1 : S_end-1);
if sum(QRS_D>= 0) == length(QRS_D) || sum(QRS_D<=0) == length(QRS_D)
    if D2(S_end-1) <= 190 && D2(S_end -1) >= 170 && (DL(S_end - 1) >= 80 || DL(S_end - 1) <= -80)
        S_end = 6;
        if S_end >= length(Sig_Temp_X)-2
            S_end = length(Sig_Temp_X)-2;
        end
        for j = 3 : S_end
            if Sig_Temp_X(j) - Sig_Temp_X(1) >= 0.25*Fs
                S_end = j-1;
                break
            end
        end
    end
end



Q_start = length(Sig_Temp_X)-5;
% Q_start = 2;
if Q_start <= 2
    Q_start = 2;
end
for j = length(Sig_Temp_X)-1 : -1 : Q_start
%     if Sig_Temp_X(end) - Sig_Temp_X(j) >= 0.3*Fs
    if Sig_Temp_X(end) - Sig_Temp_X(j) >= 0.12*Fs
%     if Sig_Temp_X(end) - Sig_Temp_X(j) >= 0.08*Fs
        Q_start = j+1;
        break
    end
end
if Q_start == length(Sig_Temp_X)
    Q_start = length(Sig_Temp_X)-1;
end

QRS_D = Y(Q_start+1 : end) - Y(Q_start : end-1);
if sum(QRS_D>= 0) == length(QRS_D) || sum(QRS_D<=0) == length(QRS_D)
    if D2(Q_start-1) <= 190 && D2(Q_start -1) >= 170 && (DR(Q_start - 1) >= 80 || DR(Q_start - 1) <= -80)
        Q_start = length(Sig_Temp_X)-5;
        if Q_start <= 2
            Q_start = 2;
        end
        for j = length(Sig_Temp_X)-1 : -1 : Q_start
%             if Sig_Temp_X(end) - Sig_Temp_X(j) >= 0.15*Fs
            if Sig_Temp_X(end) - Sig_Temp_X(j) >= 0.2*Fs
                Q_start = j+1;
                break
            end
        end
    end
end





%% S-off 검출
w_Ang = 0.5;
w_X = 1;
w_Y = 1;
% [length(DL),S_end];
Data_Angle_L = DL(1:S_end-1);
Data_Angle_R = DR(1:S_end-1);

Data_X = Sig_Temp_X(1) + round(0.3*Fs) - Sig_Temp_X(2:S_end);
Data_Y = Sig_Temp_Y(2:S_end) - Sig_Temp_Y(1);

% Norm_Angle_L = 0.5*Data_Angle_L/90;
% Norm_Angle_R = 0.5*(1-Data_Angle_R/90);
Norm_Angle_L = (sin(Data_Angle_L*pi/180).^2);
Norm_Angle_R = sqrt(cos(Data_Angle_R*pi/180));
Norm_X = Data_X/round(0.3*Fs);
Norm_Y = Data_Y/max(Data_Y);
Score = w_Ang*Norm_Angle_L + w_Ang*Norm_Angle_R + w_X*Norm_X + w_Y*Norm_Y;
% 예외처리

for j = 1 : length(Score)
    if Norm_Y(j) <= 0.5 || Norm_Angle_L(j) <= max(Norm_Angle_L)*0.8
        Score(j) = 0;
    end
end

if ~isempty(Ex_S)
    Score((Ex_S-1):end) = 0;
end

[M,p] = max(Score);

% Total_Score1 = [ Norm_Angle_L(p);Norm_Angle_R(p);Norm_X(p);Norm_Y(p)];
% Total_Score3 = [Norm_Angle_L([1:p-1, p+1:end]); Norm_Angle_R([1:p-1, p+1:end]); Norm_X([1:p-1, p+1:end]); Norm_Y([1:p-1, p+1:end])];


if M ~= 0
    S_off = p+1;
    S_Check = 1;
else
    S_off = 2;
    S_peak = 2;
    S_Check = 0;
end


if M ~= 0
    %% S_peak 검출
    if Y(1) > Y(S_off)
        [~,p] = min(Y(1:S_off));
        S_peak = p;
    else
        [~,p] = max(Y(1:S_off));
        S_peak = p;
    end
    %% S_off 보정
    if Y(1) > Y(S_off)
        [~,p] = max(Y(S_peak:S_off));
        S_off = S_peak + p -1;
    else
        [~,p] = min(Y(S_peak:S_off));
        S_off = S_peak + p -1;
    end
end



%% Q-on 검출
Data_Angle_L = DL(Q_start - 1 : end);
Data_Angle_R = DR(Q_start - 1 : end);

Data_X = Sig_Temp_X(Q_start : end-1) - (Sig_Temp_X(end) - round(0.3*Fs));
Data_Y = Sig_Temp_Y(end) - Sig_Temp_Y(Q_start : end-1);

% Norm_Angle_L = 0.5*(1-Data_Angle_L/90);
% Norm_Angle_R = 0.5*Data_Angle_R/90;
Norm_Angle_L = sqrt(cos(Data_Angle_L*pi/180));
Norm_Angle_R = (sin(Data_Angle_R*pi/180).^2);
% Norm_Angle_L = 0.5*(cos(Data_Angle_L*pi/180));
% Norm_Angle_R = 0.5*(sin(Data_Angle_R*pi/180));
Norm_X = Data_X/round(0.3*Fs);
Norm_Y = Data_Y/max(Data_Y);
Score = w_Ang*Norm_Angle_L + w_Ang*Norm_Angle_R + w_X*Norm_X + w_Y*Norm_Y;
% Score = w_Ang*Norm_Angle_L + w_Ang*Norm_Angle_R;

% 예외처리

for j = 1 : length(Score)
    if Norm_Y(j) <= 0.5 || Norm_Angle_R(j) <= max(Norm_Angle_R)*0.8
        Score(j) = 0;
    end
end

if ~isempty(Ex_Q)
    Score(1:(Ex_Q-Q_start+1)) = 0;
end


[M,p] = max(Score);
if M ~= 0
    Q_on = Q_start + p - 1;
    Q_Check = 1;
else
    Q_on = length(X) - 1;
    Q_peak = length(X) - 1;
    Q_Check = 0;
end

% Total_Score2 = [Norm_Angle_L(p);Norm_Angle_R(p);Norm_X(p);Norm_Y(p)];
% Total_Score4 = [Norm_Angle_L([1:p-1, p+1:end]); Norm_Angle_R([1:p-1, p+1:end]); Norm_X([1:p-1, p+1:end]); Norm_Y([1:p-1, p+1:end])];
if M ~= 0
    %% Q_peak 검출
    if Y(end) > Y(Q_on)
        [~,p] = min(Y(Q_on:end));
        Q_peak = Q_on + p - 1;
    else
        [~,p] = max(Y(Q_on:end));
        Q_peak = Q_on + p - 1;
    end
    %% Q_on 보정
    if Y(end) > Y(Q_on)
        [~,p] = max(Y(Q_on:Q_peak));
        Q_on = Q_on + p - 1;
    else
        [~,p] = min(Y(Q_on:Q_peak));
        Q_on = Q_on + p - 1 ;
    end
end

%% 2차 보정



% cla
% plot(X,Y,'ro-')
% hold on
% plot(X(Q_on),Y(Q_on),'k*')
% plot(X(S_off),Y(S_off),'k*')
% [Norm_Angle_L;Norm_Angle_R;Norm_X;Norm_Y;Score]
% keyboard
