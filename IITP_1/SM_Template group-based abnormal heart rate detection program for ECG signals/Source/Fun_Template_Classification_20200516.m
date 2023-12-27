function [Result] = Fun_Template_Classification_20200516(Sig, Peak,  ...
    Cluster_Length_L, Cluster_Length_R, P1, Cluster, Cluster_P1, Thr_Diff, fig_flag)
%% ��ȣ�Է� �� �Ǻ�
% �ٽ� 1�� �ɹں��� ��ȣ�Է�
% Template ��� ������ ��
% ���絵�� Thr �̻��� Template�� ���� ��� ����
Cluster_Num = size(Cluster,2);
Result = zeros(1,length(Peak)); % ���� ��� ����
Sig_Norm = [];
Sig_Abnorm = []; % ������ �ɹ� plot�� ���� �������, �����δ� ���ʿ�
for j = 1 : length(Peak)
    Sig_Temp = Sig(Peak(j)-Cluster_Length_L:Peak(j)+Cluster_Length_R);
    Sig_Temp = Sig_Temp-mean(Sig_Temp);
    Sig_Temp_P1 = Sig_Temp(1:P1);
    
    % ���� ������ Template �˻�
    Sim_flag = 0;
    for k = 1 : Cluster_Num
        Sig_Diff1 = test_pearson(Sig_Temp,Cluster(:,k));
        Sig_Diff2 = test_pearson(Sig_Temp_P1,Cluster_P1(:,k));
        Sig_Diff = (Sig_Diff1+Sig_Diff2)/2;
%         Sig_Diff = Sig_Diff1;
        if Sig_Diff < Thr_Diff
            continue % �Ӱ谪 ������ ��� ���� Template�� ��
        else
            Result(j) = 1; % �Ӱ谪 �̻��� ��� �������� ��� ����
            Sim_flag = 1; % flag ���� �� �� ����
            break
        end
    end
    if Sim_flag == 0
        Result(j) = 0; % flag�� 0���� ���� ���, ��� Template�� �������� �����Ƿ� ���������� ��� ����
        Sig_Abnorm = [Sig_Abnorm, Sig_Temp]; % ������ �ɹ� plot�� ���� �������, �����δ� ���ʿ�
    else
        Sig_Norm = [Sig_Norm, Sig_Temp];
    end
end


if fig_flag    
    figure % 1) ���� cluster �� �ʱ� cluster �� 2) ����� ������ɹڰ� ���� cluster ��
    set(gcf,'position',[554   618   260   360]);
    set(gcf,'color',[1 1 1]);    
    set(gca,'position',[ 0.19    0.122    0.762    0.85 ]);
    
%     f_temp2 = plot(Sig_Norm,'r');
    f_temp2 = plot(Sig_Norm(:,1000:size(Sig_Norm,2)),'r');
%     f_temp2 = plot(Sig_Norm(:,1:min(size(Sig_Norm,2),300)),'r');
    hold on
    f_temp1 = plot(Cluster,'k','linewidth',2);
    L = legend([f_temp1(1), f_temp2(1)], 'Cluster','Normal');
    hold off
    xlabel('Sample[N]')
    ylabel('Amplitude[mV]')
    xlim([0,size(Cluster,1)])
    set(gcf,'position',[680   627   468   351])
    set(L,'fontsize',13)
    xlim([-25,265])
    
    figure
    set(gcf,'position',[554   618   260   360]);
    set(gcf,'color',[1 1 1]);    
    set(gca,'position',[ 0.19    0.122    0.762    0.85 ]);
    
    f_temp2 = plot(Sig_Abnorm,'r');
%     f_temp2 = plot(Sig_Abnorm(:,1:min(size(Sig_Abnorm,2),300)),'r');
    hold on
    f_temp1 = plot(Cluster,'k','linewidth',2);
    L = legend([f_temp1(1), f_temp2(1)], 'Cluster','Abnormal');
    hold off
    xlabel('Sample[N]')
    ylabel('Amplitude[mV]')
    xlim([0,size(Cluster,1)])
    
    
    set(gcf,'position',[680   627   468   351])
    set(L,'fontsize',13)
    xlim([-25,265])
    
%     keyboard
end