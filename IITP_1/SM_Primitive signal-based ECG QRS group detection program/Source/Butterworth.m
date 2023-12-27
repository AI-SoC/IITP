function Filter_Data = Butterworth(Filter_Data,FS)

% %% Butterworth Low Pass Filter
Nc_L = 4;   % 차수
fc_L = 40;  % 차단주파수
Tc_L = fc_L/(FS/2);
[Bu_B_L,Bu_A_L]=butter(Nc_L,Tc_L,'low');
Filter_Data=filtfilt(Bu_B_L,Bu_A_L,Filter_Data);
%% Butterworth High Pass Filter
Nc_H = 4;  % 차수
fc_H = 5;  % 차단주파수
Tc_H = fc_H/(FS/2);
[Bu_B_H,Bu_A_H]=butter(Nc_H,Tc_H,'high');
Filter_Data=filtfilt(Bu_B_H,Bu_A_H,Filter_Data);