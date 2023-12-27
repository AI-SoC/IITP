function [Cluster, Cluster_P1, Cluster_Count, N] = Fun_Template_Create_20200516(Sig, Peak, Cluster_Length_L, Cluster_Length_R, P1, Cluster, Cluster_P1, Cluster_Count, N, Thr_Diff, Thr_Diff2, fig_flag)
Cluster_Num = size(Cluster,2);

if fig_flag
    Sig_Cut = zeros(Cluster_Length_L+Cluster_Length_R+1,length(Peak)); % 전체 심박 군집 plot을 위한 공간 생성, 실제로는 불필요
end

%% 초기 Template 획득
if isempty(Cluster)
    Cluster(:,1) = Sig(Peak(1)-Cluster_Length_L:Peak(1)+Cluster_Length_R);  % PN번째 peak를 중심으로 신호 오려내기
    Cluster(:,1) = Cluster(:,1)-mean(Cluster(:,1)); % pearson 유사도 측정을 위한 평균값 사전 제거
    Cluster_P1(:,1) = Cluster(1:P1,1);
    Cluster_Count(1) = 1; % Template 첫 카운팅
end

if fig_flag
    Sig_Cut(:,1) = Cluster(:,1); % plot을 위한 신호복사, 실제로는 불필요
    Cluster_Init = Cluster; % 첫 Template 정보 plot을 위한 별도 저장, 실제로는 불필요
end


%% Template 갱신
for N = 2 : length(Peak)
    % N 번째 심박 신호 획득
    Sig_Temp = Sig(Peak(N)-Cluster_Length_L:Peak(N)+Cluster_Length_R); % 신호 획득
    Sig_Temp_P1 = Sig_Temp(1:P1);
    if fig_flag
        Sig_Cut(:,N) = Sig_Temp; % plot을 위한 신호복사, 실제로는 불필요
    end
    
    % 가장 유사한 Template 검색
    % 0-1로 스케일링 된 pearson 유사도와 RR간격의 길이 비를 이용
    % RR간격 비가 Thr_RR보다 커 유사한 길이를 가질 경우, pearson 유사도로 형태를 비교하며,
    % RR간격 비가 Thr_RR보다 작을 경우 pearson 유사도에서 RR간격 비를 가중치로 곱하여 유사도를 비교
    Diff_Max = 0; % 최대 유사도 값 저장
    Diff_Pos = 1; % 최대 유사도 Template의 번호 저장
    for j = 1 : Cluster_Num
        Sig_Diff1 = test_pearson(Sig_Temp, Cluster(:,j)); % pearson 유사도 계산, 0-1로 스케일링
        Sig_Diff2 = test_pearson(Sig_Temp_P1, Cluster_P1(:,j)); % pearson 유사도 계산, 0-1로 스케일링
        Sig_Diff = (Sig_Diff1+Sig_Diff2)/2;
%         Sig_Diff = Sig_Diff1;
        if Sig_Diff > Diff_Max % 측정된 유사도값과 기존 최대유사도 값을 비교 후 스왑
            Diff_Max = Sig_Diff;
            Diff_Pos = j;
        end
    end
    
    % 오차가 Thr 이상이면 해당 Template로 병합하여 갱신
    % 오차가 Thr 이하이면 새로운 Template로 추가
    if Diff_Max > Thr_Diff
        % Template 갱신
        Cluster(:,Diff_Pos) = (Cluster_Count(Diff_Pos)*Cluster(:,Diff_Pos)+Sig_Temp)/(Cluster_Count(Diff_Pos)+1); % 해당 Template의 카운팅 개수를 이용하여 가중평균 신호로 업데이트
        Cluster_P1(:,Diff_Pos) = (Cluster_Count(Diff_Pos)*Cluster_P1(:,Diff_Pos)+Sig_Temp_P1)/(Cluster_Count(Diff_Pos)+1); % 해당 Template의 카운팅 개수를 이용하여 가중평균 신호로 업데이트
        Cluster_Count(Diff_Pos) = Cluster_Count(Diff_Pos)+1; % 카운팅 추가
    else
        Cluster_Num = Cluster_Num+1; % CN 증가
        Cluster(:,Cluster_Num) = Sig_Temp; % Template 추가
        Cluster_P1(:,Cluster_Num) = Sig_Temp_P1;
        Cluster_Count(Cluster_Num) = 1; % 카운팅 1로 설정
    end
    N = N+1; % 심박 번호 증가
end

if fig_flag
    Cluster_Upd = Cluster; % 업데이트 후의 Cluster를 plot 하기 위한 저장공간, 실제로는 불필요
    CC_Upd = Cluster_Count;
end
%% Template 제거
% Template 중 일부는 정상에서 벗어난 비정상심박이 Template로 획득됨
% 이를 제거하여 정상심박의 primtivie만 남기도록 후처리를 진행
% 대표 Template를 선택하고 이와 충분히 유사하지 않은 심박들은
% 변형이 크게 발생한 비정상심박의 Template로 보고 제거

% 대표 Template 선택
[~,p] = max(Cluster_Count); % 카운팅이 최대인 Template 획득
Sig_Rep = Cluster(:,p); % 대표심박을 카운팅이 최대인 p번째 Template로 결정
Sig_Rep_P1 = Cluster_P1(:,p);

% 대표 Template와의 pearson 유사도나 RR 유사도가 Thr 이하이면 Cluster에서 제거
Remove_Num = []; % 제거할 Template 번호 저장공간 생성
for j = 1 : length(Cluster_Count)
    Sig_Diff1(j) = test_pearson(Cluster(:,j), Cluster(:,p));  % 오차계산: 차신호의 절대값의 합
    Sig_Diff2(j) = test_pearson(Cluster_P1(:,j), Cluster_P1(:,p));  % 오차계산: 차신호의 절대값의 합
        Sig_Diff = (Sig_Diff1+Sig_Diff2)/2;
%         Sig_Diff = Sig_Diff1;
    if Sig_Diff(j) < Thr_Diff2
        Remove_Num = [Remove_Num;j]; % 제거할 Template 번호 저장
    end
end

% Template 제거
Cluster(:,Remove_Num) = [];
Cluster_P1(:,Remove_Num) = [];
Cluster_Count(Remove_Num) = [];
Cluster_Num = Cluster_Num-length(Remove_Num);
%% Template 정렬
% 카운팅이 높은 Template일수록 정상일 가능성이 높음
% 획득된 Template를 카운팅 개수 순으로 정렬함
[~,Sort_Pos] = sort(Cluster_Count,'descend');
% [~,Sort_Pos] = sort(Cluster_Count,'ascend');

Cluster_Temp = Cluster;
Cluster_Count_Temp = Cluster_Count;
for j = 1 : Cluster_Num
    Cluster(:,j) = Cluster_Temp(:,Sort_Pos(j));
    Cluster_Count(j) = Cluster_Count_Temp(Sort_Pos(j));
end

if fig_flag
    Cluster_Rem = Cluster; % 제거 후의 최종 cluster를 plot하기 위한 저장 공간, 실제로는 불필요
    CC_Rem = Cluster_Count;
end

if fig_flag
    figure % 1) 전체 심박 및 초기 cluster 출력, 2) 확장 cluster, 3) 최종 cluster
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