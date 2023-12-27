function [Cluster, Cluster_P1, Cluster_Count, N] = Fun_Template_Create_20200516(Sig, Peak, Cluster_Length_L, Cluster_Length_R, P1, Cluster, Cluster_P1, Cluster_Count, N, Thr_Diff, Thr_Diff2, fig_flag)
Cluster_Num = size(Cluster,2);

if fig_flag
    Sig_Cut = zeros(Cluster_Length_L+Cluster_Length_R+1,length(Peak)); % ��ü �ɹ� ���� plot�� ���� ���� ����, �����δ� ���ʿ�
end

%% �ʱ� Template ȹ��
if isempty(Cluster)
    Cluster(:,1) = Sig(Peak(1)-Cluster_Length_L:Peak(1)+Cluster_Length_R);  % PN��° peak�� �߽����� ��ȣ ��������
    Cluster(:,1) = Cluster(:,1)-mean(Cluster(:,1)); % pearson ���絵 ������ ���� ��հ� ���� ����
    Cluster_P1(:,1) = Cluster(1:P1,1);
    Cluster_Count(1) = 1; % Template ù ī����
end

if fig_flag
    Sig_Cut(:,1) = Cluster(:,1); % plot�� ���� ��ȣ����, �����δ� ���ʿ�
    Cluster_Init = Cluster; % ù Template ���� plot�� ���� ���� ����, �����δ� ���ʿ�
end


%% Template ����
for N = 2 : length(Peak)
    % N ��° �ɹ� ��ȣ ȹ��
    Sig_Temp = Sig(Peak(N)-Cluster_Length_L:Peak(N)+Cluster_Length_R); % ��ȣ ȹ��
    Sig_Temp_P1 = Sig_Temp(1:P1);
    if fig_flag
        Sig_Cut(:,N) = Sig_Temp; % plot�� ���� ��ȣ����, �����δ� ���ʿ�
    end
    
    % ���� ������ Template �˻�
    % 0-1�� �����ϸ� �� pearson ���絵�� RR������ ���� �� �̿�
    % RR���� �� Thr_RR���� Ŀ ������ ���̸� ���� ���, pearson ���絵�� ���¸� ���ϸ�,
    % RR���� �� Thr_RR���� ���� ��� pearson ���絵���� RR���� �� ����ġ�� ���Ͽ� ���絵�� ��
    Diff_Max = 0; % �ִ� ���絵 �� ����
    Diff_Pos = 1; % �ִ� ���絵 Template�� ��ȣ ����
    for j = 1 : Cluster_Num
        Sig_Diff1 = test_pearson(Sig_Temp, Cluster(:,j)); % pearson ���絵 ���, 0-1�� �����ϸ�
        Sig_Diff2 = test_pearson(Sig_Temp_P1, Cluster_P1(:,j)); % pearson ���絵 ���, 0-1�� �����ϸ�
        Sig_Diff = (Sig_Diff1+Sig_Diff2)/2;
%         Sig_Diff = Sig_Diff1;
        if Sig_Diff > Diff_Max % ������ ���絵���� ���� �ִ����絵 ���� �� �� ����
            Diff_Max = Sig_Diff;
            Diff_Pos = j;
        end
    end
    
    % ������ Thr �̻��̸� �ش� Template�� �����Ͽ� ����
    % ������ Thr �����̸� ���ο� Template�� �߰�
    if Diff_Max > Thr_Diff
        % Template ����
        Cluster(:,Diff_Pos) = (Cluster_Count(Diff_Pos)*Cluster(:,Diff_Pos)+Sig_Temp)/(Cluster_Count(Diff_Pos)+1); % �ش� Template�� ī���� ������ �̿��Ͽ� ������� ��ȣ�� ������Ʈ
        Cluster_P1(:,Diff_Pos) = (Cluster_Count(Diff_Pos)*Cluster_P1(:,Diff_Pos)+Sig_Temp_P1)/(Cluster_Count(Diff_Pos)+1); % �ش� Template�� ī���� ������ �̿��Ͽ� ������� ��ȣ�� ������Ʈ
        Cluster_Count(Diff_Pos) = Cluster_Count(Diff_Pos)+1; % ī���� �߰�
    else
        Cluster_Num = Cluster_Num+1; % CN ����
        Cluster(:,Cluster_Num) = Sig_Temp; % Template �߰�
        Cluster_P1(:,Cluster_Num) = Sig_Temp_P1;
        Cluster_Count(Cluster_Num) = 1; % ī���� 1�� ����
    end
    N = N+1; % �ɹ� ��ȣ ����
end

if fig_flag
    Cluster_Upd = Cluster; % ������Ʈ ���� Cluster�� plot �ϱ� ���� �������, �����δ� ���ʿ�
    CC_Upd = Cluster_Count;
end
%% Template ����
% Template �� �Ϻδ� ���󿡼� ��� ������ɹ��� Template�� ȹ���
% �̸� �����Ͽ� ����ɹ��� primtivie�� ���⵵�� ��ó���� ����
% ��ǥ Template�� �����ϰ� �̿� ����� �������� ���� �ɹڵ���
% ������ ũ�� �߻��� ������ɹ��� Template�� ���� ����

% ��ǥ Template ����
[~,p] = max(Cluster_Count); % ī������ �ִ��� Template ȹ��
Sig_Rep = Cluster(:,p); % ��ǥ�ɹ��� ī������ �ִ��� p��° Template�� ����
Sig_Rep_P1 = Cluster_P1(:,p);

% ��ǥ Template���� pearson ���絵�� RR ���絵�� Thr �����̸� Cluster���� ����
Remove_Num = []; % ������ Template ��ȣ ������� ����
for j = 1 : length(Cluster_Count)
    Sig_Diff1(j) = test_pearson(Cluster(:,j), Cluster(:,p));  % �������: ����ȣ�� ���밪�� ��
    Sig_Diff2(j) = test_pearson(Cluster_P1(:,j), Cluster_P1(:,p));  % �������: ����ȣ�� ���밪�� ��
        Sig_Diff = (Sig_Diff1+Sig_Diff2)/2;
%         Sig_Diff = Sig_Diff1;
    if Sig_Diff(j) < Thr_Diff2
        Remove_Num = [Remove_Num;j]; % ������ Template ��ȣ ����
    end
end

% Template ����
Cluster(:,Remove_Num) = [];
Cluster_P1(:,Remove_Num) = [];
Cluster_Count(Remove_Num) = [];
Cluster_Num = Cluster_Num-length(Remove_Num);
%% Template ����
% ī������ ���� Template�ϼ��� ������ ���ɼ��� ����
% ȹ��� Template�� ī���� ���� ������ ������
[~,Sort_Pos] = sort(Cluster_Count,'descend');
% [~,Sort_Pos] = sort(Cluster_Count,'ascend');

Cluster_Temp = Cluster;
Cluster_Count_Temp = Cluster_Count;
for j = 1 : Cluster_Num
    Cluster(:,j) = Cluster_Temp(:,Sort_Pos(j));
    Cluster_Count(j) = Cluster_Count_Temp(Sort_Pos(j));
end

if fig_flag
    Cluster_Rem = Cluster; % ���� ���� ���� cluster�� plot�ϱ� ���� ���� ����, �����δ� ���ʿ�
    CC_Rem = Cluster_Count;
end

if fig_flag
    figure % 1) ��ü �ɹ� �� �ʱ� cluster ���, 2) Ȯ�� cluster, 3) ���� cluster
    set(gcf,'position',[554   618   260   360]);
    set(gcf,'color',[1 1 1]);    
    set(gca,'position',[ 0.19    0.122    0.762    0.85 ]);
    
%     f1 = plot(Sig_Cut,'b');
    f1 = plot(Sig_Cut(:,1:min(size(Sig_Cut,2),300)),'b');
    hold on
    f2 = plot(Cluster_Init,'r','linewidth',2);
    legend([f1(1),f2],'Total Beat', 'First Beat')
    hold off
    xlabel('Sample[N]')
    ylabel('Amplitude[mV]')
    xlim([0,size(Cluster,1)])
% keyboard
    figure
    set(gcf,'position',[554   618   260   360]);
    set(gcf,'color',[1 1 1]);    
    set(gca,'position',[ 0.19    0.122    0.762    0.85 ]);
    
    plot(Cluster_Upd)
    s = [];
    for j = 1 : length(CC_Upd)
        s{j} = sprintf('Count: %d',round(CC_Upd(j)));
    end
    legend(s)
    xlabel('Sample[N]')
    ylabel('Amplitude[mV]')
    xlim([0,size(Cluster,1)])
    set(gcf,'position',[680   627   373   351])
%     set(L,'fontsize',13)
    xlim([-25,265])

    figure
    set(gcf,'position',[554   618   260   360]);
    set(gcf,'color',[1 1 1]);    
    set(gca,'position',[ 0.19    0.122    0.762    0.85 ]);
    plot(Cluster_Rem)
    s = [];
    for j = 1 : length(CC_Rem)
        s{j} = sprintf('Count: %d',round(CC_Rem(j)));
    end
    L = legend(s);
    xlabel('Sample[N]')
    ylabel('Amplitude[mV]')
    xlim([0,size(Cluster,1)])
    set(gcf,'position',[680   627   373   351])
    set(L,'fontsize',13)
    xlim([-25,265])
%     keyboard
end