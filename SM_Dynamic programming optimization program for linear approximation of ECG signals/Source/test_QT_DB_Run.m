clc, clear, close all

if exist('pathname.mat')==2
    pathname_temp = importdata('pathname.mat');
else
    pathname_temp = pwd;
end


[filename_temp, pathname] = uigetfile('*.mat','multiselect','on',[],pathname_temp);

if pathname == 0
    return
else
    save('pathname.mat','pathname')
end
if ~iscell(filename_temp)
    filename{1} = filename_temp;
else
    filename = filename_temp;
end


file_num = length(filename);

% 오차 테이블 생성
Diff_Table.Q_on = cell(1,file_num);
Diff_Table.S_off = cell(1,file_num);
Diff_Table.Q_on_mean = zeros(1,file_num);
Diff_Table.Q_on_std = zeros(1,file_num);
Diff_Table.S_off_mean = zeros(1,file_num);
Diff_Table.S_off_std = zeros(1,file_num);
Diff_Table.Q_on_mean_mean = 0;
Diff_Table.Q_on_std_mean = 0;
Diff_Table.S_off_mean_mean = 0;
Diff_Table.S_off_std_mean = 0;

% 초기화
Fs = 250;
Base = 0;
Gain = 200;

Nc_H = 5;  % High Pass Filter 차수
fc_H = 1;  % High Pass Filter 차단주파수
Tc_H = fc_H/(Fs/2);
Nc_L = 5;   % Low Pass Filter 차수
fc_L = 25;  % Low Pass Filter 차단주파수
Tc_L = fc_L/(Fs/2);
[Bu_B_H,Bu_A_H]=butter(Nc_H,Tc_H,'high');
[Bu_B_L,Bu_A_L]=butter(Nc_L,Tc_L,'low');
gw = gausswin(15,5);
gw = gw/sum(gw);
% 직선근사화 설정
Curve_cal = 0; % 곡률기반 직선근사화 on: 1, off: 0
Prog_cal = 0; % 순차적 직선근사화 on: 1, off: 0
Mod_Type = 1; % 추가정점 최적화   DP: 1, 지역적 최적화: 2,  off: 0
% 추가정점 최적화 1혹은 2일 경우, 순차적 직선근사화는 무조건 on으로 재조정
if Mod_Type ~= 0 && Prog_cal == 0
    Prog_cal = 1;
end
tic
for fn = 1 : file_num
    fn
    % 신호 입력
    Sig_Raw = importdata([pathname,'\',filename{fn}]);
    Sig_Raw = (Sig_Raw-Base)/Gain;
    
    % 필터링
    Sig_Filt = zeros(size(Sig_Raw));
    for sn = 1 : 2
        Sig_Filt(sn,:)=filtfilt(Bu_B_H,Bu_A_H,Sig_Raw(sn,:));
        Sig_Filt(sn,:)=filtfilt(Bu_B_L,Bu_A_L,Sig_Filt(sn,:));
%         Sig_Filt(sn,:)=filtfilt(Bu_B_L,Bu_A_L,Sig_Raw(sn,:));
%         Sig_Filt(sn,:) = conv(Sig_Raw(sn,:),gw,'same');
    end
    X1 = 1:length(Sig_Filt(1,:));
    Y1 = Sig_Filt(1,:);
    X2 = 1:length(Sig_Filt(2,:));
    Y2 = Sig_Filt(2,:);
%     Y1 = conv(Y1,gw,'same');
%     Y2 = conv(Y2,gw,'same');
    
    % Annotation 입력
    Annot = Fun_Annotation_Read(pathname,filename{fn});
    R_peak = Annot.R_peak;
    
    % R_peak 보정
%     DB.R_peak_Raw = R_peak;
%     DB.Sig_Len = length(Y1(1,:));
%     DB.Sig_Raw = Y1(1,:);
%     DB.Sig_Filt = Y1(1,:);
%     DB.RN = length(DB.R_peak_Raw); % R-peak 개수
%     DB.Sig_Len = length(DB.Sig_Raw);
%     
%     % 곡률계산
%     DB.Sig_Cur = zeros(1,DB.Sig_Len);
%     DY = zeros(1,DB.Sig_Len);
%     DY2 = zeros(1,DB.Sig_Len);
%     
%     DY(3:end-2) = (-DB.Sig_Filt(5:end) + 8*DB.Sig_Filt(4:end-1) - 8*DB.Sig_Filt(2:end-3) + DB.Sig_Filt(1:end-4) )/12;
%     DY2(3:end-2) = (-DB.Sig_Filt(5:end) + 16*DB.Sig_Filt(4:end-1) -30*DB.Sig_Filt(3:end-2) + 16*DB.Sig_Filt(2:end-3) - DB.Sig_Filt(1:end-4) )/12;
%     %     DY_Size = 5;
%     %
%     %     DY(DY_Size+1 : end) = DB.Sig_Filt(DY_Size+1 : end) - DB.Sig_Filt(1:end-DY_Size);
%     %     DY2(DY_Size+1 : end-DY_Size) = DY(2*DY_Size+1 : end) - DY(DY_Size+1 : end-DY_Size);
%     
%     DB.Sig_Cur = -(DY2)./( (1+DY.^2).^(3/2) );
%     
%     DB.Sig_Filt_Cur = DB.Sig_Filt.*DB.Sig_Cur;
%     
%     QCS = round(0.15*Fs); % QRS 체크 너비 사이즈
%     for j = 1 : DB.RN
%         QCS1 = min(DB.R_peak_Raw(j)-Annot.Q_on(j)-1, round(0.15*Fs)); % QRS 체크 너비 사이즈
%         QCS2 = min(Annot.S_off(j)-DB.R_peak_Raw(j)-1, round(0.15*Fs)); % QRS 체크 너비 사이즈
%         Temp = DB.Sig_Filt(DB.R_peak_Raw(j) - QCS1 : DB.R_peak_Raw(j)+QCS2);
%         Temp_Filt = Temp - min(Temp);
%         Temp_Cur = DB.Sig_Cur(DB.R_peak_Raw(j) - QCS1 : DB.R_peak_Raw(j)+QCS2);
%         Temp_Filt_Cur = Temp_Filt.*Temp_Cur;
%         [~,p] = max(Temp_Filt_Cur);
%         DB.R_peak_Raw(j) = DB.R_peak_Raw(j) - QCS1 + p -1;
%     end
%     DB.R_peak_Filt = DB.R_peak_Raw;
%     R_peak = DB.R_peak_Filt;
        
    
    
    
    
    % 직선근사화
%     tic
    [Interp_X1,Interp_Y1,Interp_X_Total1,Interp_Y_Total1, Interp_R_peak1,t1] = FUN_Linear_Fitting(X1,Y1,R_peak,Fs, Curve_cal, Prog_cal, Mod_Type,2/Fs);
    [Interp_X2,Interp_Y2,Interp_X_Total2,Interp_Y_Total2, Interp_R_peak2,t2] = FUN_Linear_Fitting(X2,Y2,R_peak,Fs, Curve_cal, Prog_cal, Mod_Type,2/Fs);
%     [Interp_X1,Interp_Y1,Interp_X_Total1,Interp_Y_Total1, Interp_R_peak1,t1] = FUN_Linear_Fitting2(X1,Y1,R_peak,Fs, Curve_cal, Prog_cal, Mod_Type,2/Fs);
%     [Interp_X2,Interp_Y2,Interp_X_Total2,Interp_Y_Total2, Interp_R_peak2,t2] = FUN_Linear_Fitting2(X2,Y2,R_peak,Fs, Curve_cal, Prog_cal, Mod_Type,2/Fs);
%     toc
    % 기준점 검출
    [Q_on1,Q_on_Index1, S_off1, S_off_Index1] = FUN_Fiducial_Detect(Interp_X_Total1,Interp_Y_Total1, Interp_R_peak1,Fs);
    Q_on_Index1 =  Q_on_Index1 + Interp_R_peak1(1:end-1)-1;
    S_off_Index1 =  S_off_Index1 + Interp_R_peak1(1:end-1)-1;
    [Q_on2,Q_on_Index2, S_off2, S_off_Index2] = FUN_Fiducial_Detect(Interp_X_Total2,Interp_Y_Total2, Interp_R_peak2,Fs);
    Q_on_Index2 =  Q_on_Index2 + Interp_R_peak2(1:end-1)-1;
    S_off_Index2 =  S_off_Index2 + Interp_R_peak2(1:end-1)-1;
    
    % 데이터 선별
    
    Q_on_Diff1 = (Annot.Q_on(2:end)-Interp_X_Total1(Q_on_Index1));
    Q_on_Diff2 = (Annot.Q_on(2:end)-Interp_X_Total2(Q_on_Index2));
    Diff_Table.Q_on{fn} = Q_on_Diff1;
    for i = 1 : length(Q_on_Diff1)
        if abs(Q_on_Diff1(i)) > abs(Q_on_Diff2(i))
            Diff_Table.Q_on{fn}(i) = Q_on_Diff2(i);
        end
    end
    
    S_off_Diff1 = (Annot.S_off(1:end-1)-Interp_X_Total1(S_off_Index1));
    S_off_Diff2 = (Annot.S_off(1:end-1)-Interp_X_Total2(S_off_Index2));
    Diff_Table.S_off{fn} = S_off_Diff1;
    for i = 1 : length(S_off_Diff1)
        if abs(S_off_Diff1(i)) > abs(S_off_Diff2(i))
            Diff_Table.S_off{fn}(i) = S_off_Diff2(i);
        end
    end
    
    if 0
        Sort_Size = round(0.9*length(R_peak));
        
        [~,Q_on_Temp_Pos] = sort(abs(Diff_Table.Q_on{fn}));
        Diff_Table.Q_on{fn} = Diff_Table.Q_on{fn}(Q_on_Temp_Pos(1:Sort_Size));
                
        [~,S_off_Temp_Pos] = sort(abs(Diff_Table.S_off{fn}));
        Diff_Table.S_off{fn} = Diff_Table.S_off{fn}(S_off_Temp_Pos(1:Sort_Size));
    end
    Diff_Table.Q_on_mean(fn) = mean(Diff_Table.Q_on{fn});
    Diff_Table.Q_on_std(fn) = std(Diff_Table.Q_on{fn});
    Diff_Table.S_off_mean(fn) = mean(Diff_Table.S_off{fn});
    Diff_Table.S_off_std(fn) = std(Diff_Table.S_off{fn});
    % Figure
    
    if 1
        figure
        plot(X1,Y1)
        hold on
        line([Annot.Q_on; Annot.Q_on], [Y1(Annot.Q_on)-0.1; Y1(Annot.Q_on)+0.1],'color','k')
        line([Annot.S_off; Annot.S_off], [Y1(Annot.S_off)-0.1; Y1(Annot.S_off)+0.1],'color','k')
        line([Annot.R_peak; Annot.R_peak], [Y1(Annot.R_peak)-0.1; Y1(Annot.R_peak)+0.1],'color','k')
%         line([Annot.P_on; Annot.P_on], [Y1(Annot.P_on)-0.1; Y1(Annot.P_on)+0.1],'color','k')
%         line([Annot.P_off; Annot.P_off], [Y1(Annot.P_off)-0.1; Y1(Annot.P_off)+0.1],'color','k')
%         line([Annot.T_on; Annot.T_on], [Y1(Annot.T_on)-0.1; Y1(Annot.T_on)+0.1],'color','k')
%         line([Annot.T_off; Annot.T_off], [Y1(Annot.T_off)-0.1; Y1(Annot.T_off)+0.1],'color','k')
        
        plot(Interp_X_Total1,Interp_Y_Total1,'ro-')
        plot(Interp_X_Total1(Q_on_Index1),Interp_Y_Total1(Q_on_Index1),'k>')
        plot(Interp_X_Total1(S_off_Index1),Interp_Y_Total1(S_off_Index1),'k<')
        plot(Interp_X_Total1(Interp_R_peak1), Interp_Y_Total1(Interp_R_peak1), 'kv')
        xlim([X1(round(R_peak(1)-Fs)),X1(round(R_peak(end)+Fs))]);
        hold off
    end
    
    % DB 저장
    clear DB
    DB.Sig_Raw = Sig_Raw;
    DB.Sig_Filt = Sig_Filt;
    DB.Annot = Annot;
    DB.Fs = Fs;
    DB.Base = Base;
    DB.Gain = Gain;
    DB.R_peak = R_peak;
    DB.Interp_X_Total1 = Interp_X_Total1;
    DB.Interp_X_Total2 = Interp_X_Total2;
    DB.Interp_Y_Total1 = Interp_Y_Total1;
    DB.Interp_Y_Total2 = Interp_Y_Total2;
    DB.Q_on_Index1 = Q_on_Index1;
    DB.Q_on_Index2 = Q_on_Index2;
    DB.S_off_Index1 = S_off_Index1;
    DB.S_off_Index2 = S_off_Index2;
    DB.Q_on1 = Interp_X_Total1(Q_on_Index1);
    DB.Q_on2 = Interp_X_Total2(Q_on_Index2);
    DB.S_off1 = Interp_X_Total1(S_off_Index1);
    DB.S_off2 = Interp_X_Total2(S_off_Index2);
    
    save([filename{fn}(1:end-4),'_result.mat']','DB')
end
% Diff_Table.Q_on_mean_mean = 0;
% Diff_Table.S_off_mean_mean = 0;
% N = 0;
% for i = 1 : file_num
%     for j = 1 : length(Diff_Table.Q_on{i})
%         Diff_Table.Q_on_mean_mean = Diff_Table.Q_on_mean_mean + Diff_Table.Q_on{i}(j);
%         Diff_Table.S_off_mean_mean = Diff_Table.S_off_mean_mean + Diff_Table.S_off{i}(j);
%         N = N+1;
%     end
% end
        
% Diff_Table.Q_on_mean_mean = Diff_Table.Q_on_mean_mean/N;
Diff_Table.Q_on_mean_mean = mean(Diff_Table.Q_on_mean);
Diff_Table.Q_on_std_mean = mean(Diff_Table.Q_on_std);
% Diff_Table.S_off_mean_mean = Diff_Table.S_off_mean_mean/N;
Diff_Table.S_off_mean_mean = mean(Diff_Table.S_off_mean);
Diff_Table.S_off_std_mean = mean(Diff_Table.S_off_std);
Diff_Table.filename = filename;
4*[Diff_Table.Q_on_mean_mean,   Diff_Table.Q_on_std_mean, Diff_Table.S_off_mean_mean,  Diff_Table.S_off_std_mean]
[Diff_Table.Q_on_mean_mean,   Diff_Table.Q_on_std_mean, Diff_Table.S_off_mean_mean,  Diff_Table.S_off_std_mean]
toc
save('Diff_Table.mat','Diff_Table')