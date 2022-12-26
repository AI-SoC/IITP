clc, clear all, close all
%% ������ ����
[filename_temp, pathname] = uigetfile('*.mat','Multiselect','on');
if pathname == 0
    return
elseif ~iscell(filename_temp)
    filename{1,1} = filename_temp;
else
    filename = filename_temp;
end
% filename{1} = '234m.mat';
% pathname = [pwd,'\'];
FN = size(filename,2);

%% ��� ��� ���̺� ����
Result_Table = cell(FN+1,10);
Result_Table{1,1} = 'Patient ID';
Result_Table{1,2} = 'No. of Total Beats';
Result_Table{1,3} = 'No. of PVC and PAC Beats';
Result_Table{1,4} = 'TP';
Result_Table{1,5} = 'TN';
Result_Table{1,6} = 'FP';
Result_Table{1,7} = 'FN';
Result_Table{1,8} = 'Sensitivity';
Result_Table{1,9} = 'Specificity';
Result_Table{1,10} = 'Accuracy';

for i = 1 : size(filename,2)
    %% Data load 
    Sig_Ori = importdata([pathname, filename{i}]);
    if size(Sig_Ori,1) <= 2 % �����ͷ� ��ȯ
        Sig_Ori = Sig_Ori';
    end
    
    base = 1024;
    gain = 200;
    Sig_Ori = (Sig_Ori - base) / gain;
    
    
%     R_peak_DB = importdata(['234m_R_peak.mat']);
    R_peak_DB = importdata([pathname,'Rpeak\', filename{i}(1:end-4), '_R_peak.mat']);
    Peak = R_peak_DB.data;
    Peak_N2 = R_peak_DB.text;
    
    Sampling_f=360;
    Butterworth_Type=2;
    Order_n=3;
    Cutoff_f=[1 25];
    [~, Sig] = Butterworth_Filter(1:length(Sig_Ori), Sig_Ori, Sampling_f, Butterworth_Type, Order_n, Cutoff_f);
    
    %% Peak ����
    
    t_s = 15;
    LL = length(Sig);
    for j = 1 : length(Peak)
        LB = max(1,Peak(j)-t_s);
        RB = min(LL,Peak(j)+t_s);
        PP = Peak(j)-LB+1;
        
        [M,P] = max(Sig(LB:RB));
%         [m,p] = min(Sig(LB:RB));
%         if abs(P-PP) <= abs(p-PP)
            Peak(j) = Peak(j) + P-PP;
%         else
%             Peak(j) = Peak(j) + p-PP;
%         end
    end
    Peak = Peak(2:end-1);
    

    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %%  �н��� �׽�Ʈ�� �з�
    Learn_Ratio = 1; % �ʱ� �н� ������ ���� ���� 0.1�� 10% �н�, 
    Test_Ratio = 0; % �׽�Ʈ�� ������ ���� 0.1�� 10% ������ �����͸� �׽�Ʈ
    Cut_L1 = round(length(Sig)*Learn_Ratio);
    Cut_L2 = round(length(Sig)*Test_Ratio);
    
    Sig_Learn = Sig(1:Cut_L1);
    Sig_Test = Sig(Cut_L2+1:end);
    
    Peak_Learn = Peak(Peak < Cut_L1);
    Peak_Learn = Peak_Learn(1:end-1); % ������ ���� ������ �����ʹ� ����
    Peak_Test = Peak(Peak > Cut_L2)-Cut_L2; % ����� �����ʹ� �н��κ��� ���ŵǾ����Ƿ�, �߸���ŭ ��ǥ����
    Peak_Test = Peak_Test(2:end); % ������ ���� ù �����ʹ� ����

    T = R_peak_DB.text; % Annotation ����, ���� ����
    T = T(2:end-1);
    T_Test = T(Peak > Cut_L2);
    T_Test = T_Test(2:end);
    
    % DB�� ��ǥŸ�� ����
    Max_Type = 'N'; % ����: N, PVC: V, PAC: A �� ���ؼ��� ������ ����
    Max_Type = T(1);
    Max_Num = 0;
    ut = unique(T);
    for j = 1 : length(ut)
        Temp = sum(T==ut(j));
        if Temp > Max_Num
            Max_Num = Temp;
            Max_Type = ut(j);
        end
    end
    if Max_Type == 'V' || Max_Type == 'A'
        Max_Type = 'R';
    end
    
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% Template ���� �ʱ�ȭ
    Cluster_Length_L = 360*275/1000; % R-peak �߽� ���� Template ����
    Cluster_Length_R = 360*375/1000; % R-peak �߽� ���� Template ���� 
    Cluster_Length_P1 = 80;
    
    % Ŭ�����Ͱ� �߰��� ��� ���� �����ϹǷ� �����Ҵ��� �ʿ��� ������
    Cluster = []; % �� Ŭ������ ����
    Cluster_P1 = []; % �� Ŭ������ ����
    Cluster_Count = []; % Template �� ī���� ���� ������� ����
    
    
    Beat_Num = 0; % Template ������Ʈ�� ���� ���� �ɹ� ��ȣ
     
    Thr_Upd = 0.9; % ���絵�� ���󿩺θ� �Ǵ��ϱ� ���� �Ӱ谪,   ���絵�� 0.1 �̻� ���̳���  �ٸ� ���� Ȥ�� ���������� �з�
    Thr_Rem = 0.9; % ���絵�� ���󿩺θ� �Ǵ��ϱ� ���� �Ӱ谪,   Cluster ���Ÿ� ���� �Ӱ�ġ
    Thr_Det = 0.9; % ���絵�� ���󿩺θ� �Ǵ��ϱ� ���� �Ӱ谪,   ���絵�� 0.1 �̻� ���̳���  �ٸ� ���� Ȥ�� ���������� �з�

        
    fig_flag1 = 0; % �߰� ��� �׸� ��� ���� 1: on, 0: off
    fig_flag2 = 1; % �߰� ��� �׸� ��� ���� 1: on, 0: off
    fig_flag3 = 1; % ������ ��� �׸� ��� ���� 1: on, 0: off
    
    %% ��ȣ �н�
    t1 = clock;
%     [Cluster, Cluster_Count, Beat_Num] =...
%         Fun_Template_Create_20200514(Sig_Learn, Peak_Learn, ...
%         Cluster_Length_L, Cluster_Length_R, Cluster, Cluster_Count, ...
%         Beat_Num, Thr_Upd, Thr_Rem, fig_flag1);
    
    [Cluster, Cluster_P1 Cluster_Count, Beat_Num] =...
        Fun_Template_Create_20200516(Sig_Learn, Peak_Learn, ...
        Cluster_Length_L, Cluster_Length_R, Cluster_Length_P1, Cluster, Cluster_P1,Cluster_Count, ...
        Beat_Num, Thr_Upd, Thr_Rem, fig_flag1);
    
    t2(1,i) = etime(clock,t1);
    t3(1,i) = t2(1,i)/length(Peak_Learn);
    %% ��ȣ �з�
    
    t1 = clock;
%     Result = Fun_Template_Classification_20200514(Sig_Test, Peak_Test, ...
%         Cluster_Length_L, Cluster_Length_R, Cluster, ...
%         Thr_Det, fig_flag2);
    Result = Fun_Template_Classification_20200516(Sig_Test, Peak_Test, ...
        Cluster_Length_L, Cluster_Length_R, Cluster_Length_P1, Cluster, Cluster_P1,...
        Thr_Det, fig_flag2);
    t2(2,i) = etime(clock,t1);
    t3(2,i) = t2(2,i)/length(Peak_Test);
    %% �з� ��� �м�
    [TP,TN,FP,FN,C_Type] = Fun_Match_20200514(T_Test,Result,Max_Type); % ���� ������ �����ͺ��̽��� ��Ī�Ͽ� ����� ����
    
    
    Total_Beat = TP+TN+FP+FN;
    Abnormal_Beat = TN+FP;
    fprintf('Data: %s, Total Beat: %d,  Abnormal Beat: %d,  TP: %d,  TN: %d,  FP: %d,  FN: %d,  Sn: %.2f%%, Sp: %.2f%%, Ac: %.2f%%\n', ...
        filename{i}(1:3),Total_Beat,Abnormal_Beat, TP, TN, FP, FN, 100*TP/(TP+FN), 100*TN/(TN+FP), 100*(TP+TN)/Total_Beat);
    
    Result_Table{i+1,1} = str2double(filename{i}(1:3));
    Result_Table{i+1,2} = Total_Beat;
    Result_Table{i+1,3} = Abnormal_Beat;
    Result_Table{i+1,4} = TP;
    Result_Table{i+1,5} = TN;
    Result_Table{i+1,6} = FP;
    Result_Table{i+1,7} = FN;
    Result_Table{i+1,8} = TP/(TP+FN);
    Result_Table{i+1,9} = TN/(TN+FP);
    Result_Table{i+1,10} = (TP+TN)/Total_Beat;
    
    %% �з� ��� ���
    if fig_flag3 == 1
        figure
        subplot(1,2,1)
        hold on
        subplot(1,2,2)
        hold on
        for j = 1 : length(Peak_Test)
            if C_Type(j) == 3
                subplot(1,2,1)
                plot(Sig_Test(Peak_Test(j)-360*275/1000 : Peak_Test(j)+360*375/1000))
                hold on
                f_temp1 = plot(Cluster,'k','linewidth',2);
            elseif C_Type(j) == 4
                subplot(1,2,2)
                plot(Sig_Test(Peak_Test(j)-360*275/1000 : Peak_Test(j)+360*375/1000))
            end
        end
    end
%     save('Result_Table.mat','Result_Table')
end