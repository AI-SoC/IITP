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

FN = size(filename,2);

%% ��� ��� ���̺� ����
Result_Table = cell(FN+1,10);
Result_Table{1,1} = 'Patient ID';
Result_Table{1,2} = 'No. of Normal Beats';
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
    
    
    R_peak_DB = importdata([pathname,'Rpeak\', filename{i}(1:end-4),'_R_peak.mat']);
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
    RR1 = Peak(2:end)-Peak(1:end-1);
    Peak = Peak(2:end-1);
    
    RR1 = RR1(1:end-1);


    
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
    
    Thr_load_flag = 1;  %������ ���ǵ� �Ӱ谪 �ҷ�����
    Thr_save_flag = 0; % ���� ������ �Ӱ谪�� ����
    
    Beat_Num = 0; % Template ������Ʈ�� ���� ���� �ɹ� ��ȣ
     
    Thr(1) = 0.95; % ���絵�� ���󿩺θ� �Ǵ��ϱ� ���� �Ӱ谪,   ���絵�� 0.1 �̻� ���̳���  �ٸ� ���� Ȥ�� ���������� �з�
    Thr(2) = 0.95; % ���絵�� ���󿩺θ� �Ǵ��ϱ� ���� �Ӱ谪,   Cluster ���Ÿ� ���� �Ӱ�ġ
    Thr(3) = 0.8; % ���絵�� ���󿩺θ� �Ǵ��ϱ� ���� �Ӱ谪,   ���絵�� 0.1 �̻� ���̳���  �ٸ� ���� Ȥ�� ���������� �з�

        
    fig_flag1 = 0; % �߰� ��� �׸� ��� ���� 1: on, 0: off
    fig_flag2 = 0; % �߰� ��� �׸� ��� ���� 1: on, 0: off
    fig_flag3 = 0; % ������ ��� �׸� ��� ���� 1: on, 0: off
    
    
    if ~Thr_load_flag
        Diff_Min = 0;
%         Thr_Learn = [0.7 0.9];
%         Thr_Diff_Cf = [0.6 0.9];
%         Thr_Learn = 0.7:0.1:0.9;
%         Thr_Diff_Cf = [0.5:0.1:0.9];
        Thr_Learn = 0.6:0.05:0.95;
        Thr_Diff_Cf = [0.6:0.05:0.95];
        Thr_Learn = 0.6:0.05:0.95;
        Thr_Diff_Cf = [0.6:0.05:0.95];
%         for Thr_Learn_N = 1 : length(Thr_Learn);
%             for Thr_Diff_Cf_N = 1 : length(Thr_Diff_Cf)
        for Thr_Learn_N = length(Thr_Learn) : -1 : 1
            for Thr_Diff_Cf_N = length(Thr_Diff_Cf) : -1 : 1
                Thr(1:2) = Thr_Learn(Thr_Learn_N);
                Thr(3) = Thr_Diff_Cf(Thr_Diff_Cf_N);
                %% ��ȣ �н�
                
                Cluster = []; % �� Ŭ������ ����
                Cluster_Count = []; % Template �� ī���� ���� ������� ����

                [Cluster, Cluster_P1 Cluster_Count, Beat_Num] =...
                    Fun_Template_Create_20200516(Sig_Learn, Peak_Learn, ...
                    Cluster_Length_L, Cluster_Length_R, Cluster_Length_P1, Cluster, Cluster_P1,Cluster_Count, ...
                    Beat_Num, Thr(1), Thr(2), fig_flag1);
                %% ��ȣ �з�
                Result = Fun_Template_Classification_20200516(Sig_Test, Peak_Test, ...
                    Cluster_Length_L, Cluster_Length_R, Cluster_Length_P1, Cluster, Cluster_P1,...
                    Thr(3), fig_flag2);
                
                %% �з� ��� �м�
                [TP,TN,FP,FN,C_Type] = Fun_Match_20200514(T_Test,Result,Max_Type); % ���� ������ �����ͺ��̽��� ��Ī�Ͽ� ����� ����
                Sn = TP/(TP+FN);
                Sp = TN/(TN+FP);
                if isnan(Sp)
                    if 2*Sn > Diff_Min
                        Diff_Min = 2*Sn;
                        Thr_Learn_Opt = Thr(1);
                        Thr_Diff_Opt = Thr(3);
                    end
                else
                    if Sn+Sp > Diff_Min
                        Diff_Min = Sp+Sn;
                        Thr_Learn_Opt = Thr(1);
                        Thr_Diff_Opt = Thr(3);
                    end
                end
                %                 [FP, FN]
                %                 Thr
            end
        end
        [Thr_Learn_Opt, Thr_Diff_Opt]
        Thr(1:2) = Thr_Learn_Opt;
        Thr(3) = Thr_Diff_Opt;
        if Thr_save_flag == 1
            save(sprintf('%sThr\\%s_Thr.mat',pathname, filename{i}(1:3)), 'Thr')
        end
        Total_Thr(i,:) = Thr;
%     else
%          Thr = importdata((sprintf('%sThr\\%s_Thr.mat',pathname, filename{i}(1:3))));
    end
    
    %% ȹ��� Thr�� ������
    
    Cluster = []; % �� Ŭ������ ����
    Cluster_Count = []; % Template �� ī���� ���� ������� ����
    
    [Cluster, Cluster_P1 Cluster_Count, Beat_Num] =...
        Fun_Template_Create_20200516(Sig_Learn, Peak_Learn, ...
        Cluster_Length_L, Cluster_Length_R, Cluster_Length_P1, Cluster, Cluster_P1,Cluster_Count, ...
        Beat_Num, Thr(1), Thr(2), fig_flag1);
    %% ��ȣ �з�
    Result = Fun_Template_Classification_20200516(Sig_Test, Peak_Test, ...
        Cluster_Length_L, Cluster_Length_R, Cluster_Length_P1, Cluster, Cluster_P1,...
        Thr(3), fig_flag2);
            
    %% �з� ��� �м�
    [TP,TN,FP,FN,C_Type] = Fun_Match_20200514(T_Test,Result,Max_Type); % ���� ������ �����ͺ��̽��� ��Ī�Ͽ� ����� ����
    
    
    Total_Beat = TP+TN+FP+FN;
    Abnormal_Beat = TN+FP;
    fprintf('Data: %s, Total Beat: %d,  Abnormal Beat: %d,  TP: %d,  TN: %d,  FP: %d,  FN: %d,  Sn: %.2f%%, Sp: %.2f%%, Ac: %.2f%%, Thr_L: %.2f, Thr_C: %.2f\n', ...
        filename{i}(1:3),Total_Beat,Abnormal_Beat, TP, TN, FP, FN, 100*TP/(TP+FN), 100*TN/(TN+FP), 100*(TP+TN)/Total_Beat, Thr(1), Thr(3));
    
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
    Result_Table{i+1,11} = Thr(1);
    Result_Table{i+1,12} = Thr(3);
end