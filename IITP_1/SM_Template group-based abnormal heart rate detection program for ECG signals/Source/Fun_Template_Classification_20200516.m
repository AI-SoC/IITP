function [Result] = Fun_Template_Classification_20200516(Sig, Peak,  ...
    Cluster_Length_L, Cluster_Length_R, P1, Cluster, Cluster_P1, Thr_Diff, fig_flag)
%% 신호입력 및 판별
% 다시 1번 심박부터 신호입력
% Template 들과 순차적 비교
% 유사도가 Thr 이상인 Template가 있을 경우 정상
Cluster_Num = size(Cluster,2);
Result = zeros(1,length(Peak)); % 판정 결과 저장
Sig_Norm = [];
Sig_Abnorm = []; % 비정상 심박 plot을 위한 저장공간, 실제로는 불필요
for j = 1 : length(Peak)
    Sig_Temp = Sig(Peak(j)-Cluster_Length_L:Peak(j)+Cluster_Length_R);
    Sig_Temp = Sig_Temp-mean(Sig_Temp);
    Sig_Temp_P1 = Sig_Temp(1:P1);
    
    % 가장 유사한 Template 검색
    Sim_flag = 0;
    for k = 1 : Cluster_Num
        Sig_Diff1 = test_pearson(Sig_Temp,Cluster(:,k));
        Sig_Diff2 = test_pearson(Sig_Temp_P1,Cluster_P1(:,k));
        Sig_Diff = (Sig_Diff1+Sig_Diff2)/2;
%         Sig_Diff = Sig_Diff1;
        if Sig_Diff < Thr_Diff
            continue % 임계값 이하일 경우 다음 Template와 비교
        else
            Result(j) = 1; % 임계값 이상일 경우 정상으로 결과 저장
            Sim_flag = 1; % flag 수정 후 비교 종료
            break
        end
    end
    if Sim_flag == 0
        Result(j) = 0; % flag가 0으로 나올 경우, 모든 Template와 유사하지 않으므로 비정상으로 결과 저장
        Sig_Abnorm = [Sig_Abnorm, Sig_Temp]; % 비정상 심박 plot을 위한 결과저장, 실제로는 불필요
    else
        Sig_Norm = [Sig_Norm, Sig_Temp];
    end
end


if fig_flag    
    figure % 1) 최종 cluster 및 초기 cluster 비교 2) 검출된 비정상심박과 최종 cluster 비교
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