clc, clear all, close all
%% 데이터 선택
load('Test_filename.mat')

FN = size(filename,2);

L = 470;

t1 = clock;
net = importdata('net.mat');
Result = cell(1,FN);
for i = 1 : FN
    %% Data load
    Sig_Ori = importdata([pwd,'\DB\', filename{i}]);
    if size(Sig_Ori,1) <= 2 % 열벡터로 전환
        Sig_Ori = Sig_Ori';
    end
    L = length(Sig_Ori);
    Fs = 360;
    base = 1024;
    gain = 200;

    Sig_Ori = (Sig_Ori - base) / gain;

    %% R-peak 불러오기
    R_peak_DB = importdata([pwd,'\DB\', filename{i}(1:4),'_R_peak.mat']);
    Peak = R_peak_DB.data;  % R-peak의 위치정보
    Peak_N2 = R_peak_DB.text; % R-peak의 심박 종류
    %     [~,Peak] = Fun_pan_tompkin_Fig(Sig,Fs,0);

    %% Butterworth 필터
    Butterworth_Type=2; % bandpass
    Order_n=3; %3차
    Cutoff_f=[1 25]; % Cut-off frequency 1-25 Hz
    [~, Sig] = Fun_Butterworth_Filter(1:L, Sig_Ori, Fs, Butterworth_Type, Order_n, Cutoff_f);

    %% Peak 보정
    t_s = 5;  % 로칼 범위 +-5 샘플
    % 지역적 최대값 보정
    for j = 1 : length(Peak)
        LB = max(1,Peak(j)-t_s);
        RB = min(L,Peak(j)+t_s);
        [~,P] = max(Sig(LB:RB));
        Peak(j) = LB+P-1;
    end
    % 양끝 심박 제거
    Peak = Peak(2:end-1);
    Peak_N2 = Peak_N2(2:end-1);

    % DB의 대표타입 결정
    Max_Type = mode(Peak_N2);
    Annot_N = find(Peak_N2==Max_Type);
    Annot_A = find(Peak_N2=='V');
    %% Template 생성
    % 입력 변수
    Cluster_Length_L = Fs*275/1000; % R-peak 중심 좌측 Template 길이 275ms
    Cluster_Length_R = Fs*375/1000; % R-peak 중심 우측 Template 길이 375ms
    Cluster_Length_P1 = 80; % P파 구간길이

    Thr_Upd = 0.9; % 유사도의 정상여부를 판단하기 위한 임계값,   유사도가 0.1 이상 차이나면  다른 유형 혹은 비정상으로 분류
    t1 = clock;
    [Template_N, Template_A, Cluster_N, Cluster_A, Cluster_N_Count, Cluster_A_Count] =...
        Fun_Template_Generation(Sig, Peak, ...
        Cluster_Length_L, Cluster_Length_R, Cluster_Length_P1, Thr_Upd);
    fprintf('%s번 데이터 대표심박 획득 시간: %.2fs\n',filename{i}(1:3), etime(clock,t1) )

    %% 통합 영상으로 정규화
    t = [];
    for j = 1 : length(Annot_N)
        Sig_Cut = Sig(Peak(Annot_N(j))-Cluster_Length_L:Peak(Annot_N(j))+Cluster_Length_R);
        Sig_Temp = [Template_N; Sig_Cut];
        t1 = clock;
        [YPred,probs] = classify(net,Sig_Temp);
        t(end+1) = etime(clock,t1);
        if YPred == 'Normal'
            Result{i}(end+1) = 3;
        else
            Result{i}(end+1) = 4;
        end
    end
    for j = 1 : length(Annot_A)
        Sig_Cut = Sig(Peak(Annot_A(j))-Cluster_Length_L:Peak(Annot_A(j))+Cluster_Length_R);

        Sig_Temp = [Template_N; Sig_Cut];
        t1 = clock;
        [YPred,probs] = classify(net,Sig_Temp);
        t(end+1) = etime(clock,t1);
        if YPred == 'Abnormal'
            Result{i}(end+1) = 1;
        else
            Result{i}(end+1) = 2;
        end
    end
    Total(i,:) = [str2num(filename{i}(1:3)), sum(Result{i}==1), sum(Result{i}==2), sum(Result{i}==3), sum(Result{i}==4)];

    fprintf('%s번 데이터 전체 분류 시간: %.2fs\n',filename{i}(1:3), sum(t) )
end
for i = 1 : FN
    fprintf('Name: %d,  TP: %d  FP: %d  TN: %d  FN: %d\n',Total(i,:))
end
