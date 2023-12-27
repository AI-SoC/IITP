function [Template_N, Template_A, Cluster_N, Cluster_A, Cluster_N_Count, Cluster_A_Count] = Fun_Template_Generation(Sig, Peak, Cluster_Length_L, Cluster_Length_R, P1, Thr_Upd)

Cluster_N(:,1) = Sig(Peak(1)-Cluster_Length_L:Peak(1)+Cluster_Length_R);  % PN��° peak�� �߽����� ��ȣ ��������
Cluster_N(:,1) = Cluster_N(:,1)-mean(Cluster_N(:,1)); % pearson ���絵 ������ ���� ��հ� ���� ����
Cluster_N_P1(:,1) = Cluster_N(1:P1,1);
Cluster_N_Count(1) = 1; % Template ù ī����
Cluster_Num = 1;

%% Template ����
for N = 2 : length(Peak)
    % N ��° �ɹ� ��ȣ ȹ��
    Sig_Temp = Sig(Peak(N)-Cluster_Length_L:Peak(N)+Cluster_Length_R); % ��ȣ ȹ��
    Sig_Temp_P1 = Sig_Temp(1:P1); % P�� ���� ��ȣ ȹ��
    
    % ���� ������ Template �˻�
    Diff_Max = 0; % �ִ� ���絵 �� ����
    Diff_Pos = 1; % �ִ� ���絵 Template�� ��ȣ ����
    for j = 1 : Cluster_Num
        % ��ü pearson ���絵 ���, 0-1�� �����ϸ�
        Sig_Diff1 = Fun_pearson(Sig_Temp, Cluster_N(:,j));
        % P�� ���� pearson ���絵 ���, 0-1�� �����ϸ�
        Sig_Diff2 = Fun_pearson(Sig_Temp_P1, Cluster_N_P1(:,j)); 
        % �� ���絵 ��� ���
        Sig_Diff = (Sig_Diff1+Sig_Diff2)/2; 
        % ������ ���絵���� ���� �ִ����絵 ���� �� �� ����
        if Sig_Diff > Diff_Max 
            Diff_Max = Sig_Diff;
            Diff_Pos = j;
        end
    end
    
    % ������ Thr �̻��̸� �ش� Template�� �����Ͽ� ����
    % ������ Thr �����̸� ���ο� Template�� �߰�
    if Diff_Max > Thr_Upd
        % Template ����
        % �ش� Template�� ī���� ������ �̿��Ͽ� ������� ��ȣ�� ������Ʈ
        Cluster_N(:,Diff_Pos) = (Cluster_N_Count(Diff_Pos)*Cluster_N(:,Diff_Pos)+Sig_Temp)/(Cluster_N_Count(Diff_Pos)+1); 
        % �ش� Template�� ī���� ������ �̿��Ͽ� ������� ��ȣ�� ������Ʈ
        Cluster_N_P1(:,Diff_Pos) = (Cluster_N_Count(Diff_Pos)*Cluster_N_P1(:,Diff_Pos)+Sig_Temp_P1)/(Cluster_N_Count(Diff_Pos)+1); 
        % ī���� �߰�
        Cluster_N_Count(Diff_Pos) = Cluster_N_Count(Diff_Pos)+1; 
    else
        Cluster_Num = Cluster_Num+1; % CN ����
        Cluster_N(:,Cluster_Num) = Sig_Temp; % Template �߰�
        Cluster_N_P1(:,Cluster_Num) = Sig_Temp_P1;
        Cluster_N_Count(Cluster_Num) = 1; % ī���� 1�� ����
    end
    N = N+1; % �ɹ� ��ȣ ����
end

if Cluster_Num == 1
    Template_N = Cluster_N(:,1);
    Template_A = [];
    Cluster_A = [];
    Cluster_A_Count = [];
    return
end


%% ��ǥ Template ����
% ���� ī������ ���� Template�� ����ɹ��� ���� �� ��Ÿ���ٰ� �� �� ����

[~,Template_Normal_P] = max(Cluster_N_Count); % ī������ �ִ��� Template ȹ��
Sig_Rep = Cluster_N(:,Template_Normal_P); % ��ǥ�ɹ��� ī������ �ִ��� p��° Template�� ����
Sig_Rep_P1 = Cluster_N_P1(:,Template_Normal_P);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% Template ����
% ��ǥ Template�� �����Ҽ��� ���� Template�� ���ɼ��� ����
% ��ǥ Template�� ���� �������� ���� Template��  ��ǥ ������ Template���� ȹ��
% ����� ������ ��ǥ Template���� ���絵�� ���� �����Ͽ�
% ���� ������ ������ Template�� �з�


% ��ǥ Template �� ���絵 ���̺� ����
Sim_Table = zeros(1,Cluster_Num);
for j = 1 : Cluster_Num
     Sim_Temp1 = Fun_pearson(Cluster_N(:,j), Cluster_N(:,Template_Normal_P));  % �������: ����ȣ�� ���밪�� ��
     Sim_Temp2 = Fun_pearson(Cluster_N_P1(:,j), Cluster_N_P1(:,Template_Normal_P));  % �������: ����ȣ�� ���밪�� ��
     Sim_Table(j) = (Sim_Temp1+Sim_Temp2)/2;
end

% ���� ���絵�� ���� ������ ���ø� ����
[~,Template_Abnormal_P] = min(Sim_Table);

% ���ø� ���������� �з�
% ����� ���ø��� ���̺��� ���� �ȵǵ��� ���� 1�� ����
Sim_Table(Template_Abnormal_P) = 1;
% ������ɹ� ��ȣ ����
Remove_Num = [Template_Abnormal_P];

for j = 1 : Cluster_Num-2
    [~,Sim_Temp_P] = min(Sim_Table);    
    % ������� ���絵
     Sim_Temp_N1 = Fun_pearson(Cluster_N(:,Sim_Temp_P), Cluster_N(:,Template_Normal_P));  % �������: ����ȣ�� ���밪�� ��
     Sim_Temp_N2 = Fun_pearson(Cluster_N_P1(:,Sim_Temp_P), Cluster_N_P1(:,Template_Normal_P));  % �������: ����ȣ�� ���밪�� ��
     Sim_Temp_N = (Sim_Temp_N1+Sim_Temp_N2)/2;    
     % ��������� ���絵
     Sim_Temp_A1 = Fun_pearson(Cluster_N(:,Sim_Temp_P), Cluster_N(:,Template_Abnormal_P));  % �������: ����ȣ�� ���밪�� ��
     Sim_Temp_A2 = Fun_pearson(Cluster_N_P1(:,Sim_Temp_P), Cluster_N_P1(:,Template_Abnormal_P));  % �������: ����ȣ�� ���밪�� ��
     Sim_Temp_A = (Sim_Temp_A1+Sim_Temp_A2)/2;
    % ������ ���ϴ��� Ȯ�� ���絵�� �з�
     
%     if Sim_Temp_N < Sim_Temp_A || Sim_Temp_N1 < Thr_Upd || Sim_Temp_N2 < Thr_Upd
    if Sim_Temp_N < Sim_Temp_A
        Remove_Num = [Remove_Num;Sim_Temp_P]; % ������ Template ��ȣ ����
        % ���ø� ī��Ʈ�� ���� ��ǥ ������ ���ø� ������Ʈ
        if Cluster_N_Count(Sim_Temp_P) > Cluster_N_Count(Template_Abnormal_P)
%             Template_Abnormal_P = Sim_Temp_P;
        end
        Sim_Table(Sim_Temp_P) = 1;
    end

     
end


% ����/������ Template �и�
Cluster_A = Cluster_N(:,Remove_Num);
Cluster_A_P1 = Cluster_N_P1(:,Remove_Num);
Cluster_A_Count = Cluster_N_Count(:,Remove_Num);
Cluster_A_Num = length(Remove_Num);

Cluster_N(:,Remove_Num) = [];
Cluster_N_P1(:,Remove_Num) = [];
Cluster_N_Count(Remove_Num) = [];
Cluster_Num = Cluster_Num-length(Remove_Num);
%% Template ����
% ī������ ���� Template�ϼ��� ������ ���ɼ��� ����
% ȹ��� Template�� ī���� ���� ������ ������
[~,Sort_Pos] = sort(Cluster_N_Count,'descend');

Cluster_Temp = Cluster_N;
Cluster_Count_Temp = Cluster_N_Count;
for j = 1 : Cluster_Num
    Cluster_N(:,j) = Cluster_Temp(:,Sort_Pos(j));
    Cluster_N_Count(j) = Cluster_Count_Temp(Sort_Pos(j));
end

Template_N = Cluster_N(:,1);


[~,Sort_Pos] = sort(Cluster_A_Count,'descend');

Cluster_Temp = Cluster_A;
Cluster_Count_Temp = Cluster_A_Count;
for j = 1 : Cluster_A_Num
    Cluster_A(:,j) = Cluster_Temp(:,Sort_Pos(j));
    Cluster_A_Count(j) = Cluster_Count_Temp(Sort_Pos(j));
end

Template_A = Cluster_A(:,1);
