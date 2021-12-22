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

    %% ��ȣ �н�
    t1 = clock
    [Cluster, Cluster_P1 Cluster_Count, Beat_Num] =...
        Fun_Template_Create_20200516(Sig_Learn, Peak_Learn, ...
        Cluster_Length_L, Cluster_Length_R, Cluster_Length_P1, Cluster, Cluster_P1,Cluster_Count, ...
        Beat_Num, Thr_Upd, Thr_Rem, 1);
    
    
end