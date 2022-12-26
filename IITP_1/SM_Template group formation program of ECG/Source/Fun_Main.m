clc, clear all, close all
%% 데이터 선택
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

%% 결과 출력 테이블 생성
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
    if size(Sig_Ori,1) <= 2 % 열벡터로 전환
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
    
    %% Peak 보정
    
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
    %%  학습용 테스트용 분류
    Learn_Ratio = 1; % 초기 학습 데이터 비율 결정 0.1은 10% 학습, 
    Test_Ratio = 0; % 테스트용 데이터 비율 0.1은 10% 이후의 데이터만 테스트
    Cut_L1 = round(length(Sig)*Learn_Ratio);
    Cut_L2 = round(length(Sig)*Test_Ratio);
    
    Sig_Learn = Sig(1:Cut_L1);
    Sig_Test = Sig(Cut_L2+1:end);
    
    Peak_Learn = Peak(Peak < Cut_L1);
    Peak_Learn = Peak_Learn(1:end-1); % 여백을 위해 마지막 데이터는 버림
    Peak_Test = Peak(Peak > Cut_L2)-Cut_L2; % 실험용 데이터는 학습부분이 제거되었으므로, 잘린만큼 좌표수정
    Peak_Test = Peak_Test(2:end); % 여백을 위해 첫 데이터는 버림

    T = R_peak_DB.text; % Annotation 정보, 위와 동일
    T = T(2:end-1);
    T_Test = T(Peak > Cut_L2);
    T_Test = T_Test(2:end);
    
    % DB의 대표타입 결정
    Max_Type = 'N'; % 정상: N, PVC: V, PAC: A 에 대해서만 실험을 진행
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
    %% Template 변수 초기화
    Cluster_Length_L = 360*275/1000; % R-peak 중심 좌측 Template 길이
    Cluster_Length_R = 360*375/1000; % R-peak 중심 우측 Template 길이 
    Cluster_Length_P1 = 80;
    
    % 클러스터가 추가될 경우 열이 증가하므로 동적할당이 필요한 변수들
    Cluster = []; % 빈 클러스터 생성
    Cluster_P1 = []; % 빈 클러스터 생성
    Cluster_Count = []; % Template 별 카운팅 개수 저장공간 생성
    
    
    Beat_Num = 0; % Template 업데이트를 위한 현재 심박 번호
     
    Thr_Upd = 0.9; % 유사도의 정상여부를 판단하기 위한 임계값,   유사도가 0.1 이상 차이나면  다른 유형 혹은 비정상으로 분류
    Thr_Rem = 0.9; % 유사도의 정상여부를 판단하기 위한 임계값,   Cluster 제거를 위한 임계치
    Thr_Det = 0.9; % 유사도의 정상여부를 판단하기 위한 임계값,   유사도가 0.1 이상 차이나면  다른 유형 혹은 비정상으로 분류

    %% 신호 학습
    t1 = clock
    [Cluster, Cluster_P1 Cluster_Count, Beat_Num] =...
        Fun_Template_Create_20200516(Sig_Learn, Peak_Learn, ...
        Cluster_Length_L, Cluster_Length_R, Cluster_Length_P1, Cluster, Cluster_P1,Cluster_Count, ...
        Beat_Num, Thr_Upd, Thr_Rem, 1);
    
    
end