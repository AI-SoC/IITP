function [Template_N, Template_A, Cluster_N, Cluster_A, Cluster_N_Count, Cluster_A_Count] = Fun_Template_Generation(Sig, Peak, Cluster_Length_L, Cluster_Length_R, P1, Thr_Upd)

Cluster_N(:,1) = Sig(Peak(1)-Cluster_Length_L:Peak(1)+Cluster_Length_R);  % PN번째 peak를 중심으로 신호 오려내기
Cluster_N(:,1) = Cluster_N(:,1)-mean(Cluster_N(:,1)); % pearson 유사도 측정을 위한 평균값 사전 제거
Cluster_N_P1(:,1) = Cluster_N(1:P1,1);
Cluster_N_Count(1) = 1; % Template 첫 카운팅
Cluster_Num = 1;

%% Template 갱신
for N = 2 : length(Peak)
    % N 번째 심박 신호 획득
    Sig_Temp = Sig(Peak(N)-Cluster_Length_L:Peak(N)+Cluster_Length_R); % 신호 획득
    Sig_Temp_P1 = Sig_Temp(1:P1); % P파 구간 신호 획득
    
    % 가장 유사한 Template 검색
    Diff_Max = 0; % 최대 유사도 값 저장
    Diff_Pos = 1; % 최대 유사도 Template의 번호 저장
    for j = 1 : Cluster_Num
        % 전체 pearson 유사도 계산, 0-1로 스케일링
        Sig_Diff1 = Fun_pearson(Sig_Temp, Cluster_N(:,j));
        % P파 영역 pearson 유사도 계산, 0-1로 스케일링
        Sig_Diff2 = Fun_pearson(Sig_Temp_P1, Cluster_N_P1(:,j)); 
        % 두 유사도 평균 계산
        Sig_Diff = (Sig_Diff1+Sig_Diff2)/2; 
        % 측정된 유사도값과 기존 최대유사도 값을 비교 후 스왑
        if Sig_Diff > Diff_Max 
            Diff_Max = Sig_Diff;
            Diff_Pos = j;
        end
    end
    
    % 오차가 Thr 이상이면 해당 Template로 병합하여 갱신
    % 오차가 Thr 이하이면 새로운 Template로 추가
    if Diff_Max > Thr_Upd
        % Template 갱신
        % 해당 Template의 카운팅 개수를 이용하여 가중평균 신호로 업데이트
        Cluster_N(:,Diff_Pos) = (Cluster_N_Count(Diff_Pos)*Cluster_N(:,Diff_Pos)+Sig_Temp)/(Cluster_N_Count(Diff_Pos)+1); 
        % 해당 Template의 카운팅 개수를 이용하여 가중평균 신호로 업데이트
        Cluster_N_P1(:,Diff_Pos) = (Cluster_N_Count(Diff_Pos)*Cluster_N_P1(:,Diff_Pos)+Sig_Temp_P1)/(Cluster_N_Count(Diff_Pos)+1); 
        % 카운팅 추가
        Cluster_N_Count(Diff_Pos) = Cluster_N_Count(Diff_Pos)+1; 
    else
        Cluster_Num = Cluster_Num+1; % CN 증가
        Cluster_N(:,Cluster_Num) = Sig_Temp; % Template 추가
        Cluster_N_P1(:,Cluster_Num) = Sig_Temp_P1;
        Cluster_N_Count(Cluster_Num) = 1; % 카운팅 1로 설정
    end
    N = N+1; % 심박 번호 증가
end

if Cluster_Num == 1
    Template_N = Cluster_N(:,1);
    Template_A = [];
    Cluster_A = [];
    Cluster_A_Count = [];
    return
end


%% 대표 Template 선택
% 가장 카운팅이 높은 Template은 정상심박을 가장 잘 나타낸다고 볼 수 있음

[~,Template_Normal_P] = max(Cluster_N_Count); % 카운팅이 최대인 Template 획득
Sig_Rep = Cluster_N(:,Template_Normal_P); % 대표심박을 카운팅이 최대인 p번째 Template로 결정
Sig_Rep_P1 = Cluster_N_P1(:,Template_Normal_P);


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


%% Template 제거
% 대표 Template과 유사할수록 정상 Template일 가능성이 높음
% 대표 Template과 가장 유사하지 않은 Template을  대표 비정상 Template으로 획득
% 정상과 비정상 대표 Template과의 유사도를 각각 측정하여
% 보다 유사한 쪽으로 Template을 분류


% 대표 Template 과 유사도 테이블 생성
Sim_Table = zeros(1,Cluster_Num);
for j = 1 : Cluster_Num
     Sim_Temp1 = Fun_pearson(Cluster_N(:,j), Cluster_N(:,Template_Normal_P));  % 오차계산: 차신호의 절대값의 합
     Sim_Temp2 = Fun_pearson(Cluster_N_P1(:,j), Cluster_N_P1(:,Template_Normal_P));  % 오차계산: 차신호의 절대값의 합
     Sim_Table(j) = (Sim_Temp1+Sim_Temp2)/2;
end

% 가장 유사도가 낮은 비정상 템플릿 검출
[~,Template_Abnormal_P] = min(Sim_Table);

% 템플릿 순차적으로 분류
% 검출된 템플릿은 테이블에서 검출 안되도록 값을 1로 수정
Sim_Table(Template_Abnormal_P) = 1;
% 비정상심박 번호 저장
Remove_Num = [Template_Abnormal_P];

for j = 1 : Cluster_Num-2
    [~,Sim_Temp_P] = min(Sim_Table);    
    % 정상과의 유사도
     Sim_Temp_N1 = Fun_pearson(Cluster_N(:,Sim_Temp_P), Cluster_N(:,Template_Normal_P));  % 오차계산: 차신호의 절대값의 합
     Sim_Temp_N2 = Fun_pearson(Cluster_N_P1(:,Sim_Temp_P), Cluster_N_P1(:,Template_Normal_P));  % 오차계산: 차신호의 절대값의 합
     Sim_Temp_N = (Sim_Temp_N1+Sim_Temp_N2)/2;    
     % 비정상과의 유사도
     Sim_Temp_A1 = Fun_pearson(Cluster_N(:,Sim_Temp_P), Cluster_N(:,Template_Abnormal_P));  % 오차계산: 차신호의 절대값의 합
     Sim_Temp_A2 = Fun_pearson(Cluster_N_P1(:,Sim_Temp_P), Cluster_N_P1(:,Template_Abnormal_P));  % 오차계산: 차신호의 절대값의 합
     Sim_Temp_A = (Sim_Temp_A1+Sim_Temp_A2)/2;
    % 비정상에 속하는지 확인 유사도로 분류
     
%     if Sim_Temp_N < Sim_Temp_A || Sim_Temp_N1 < Thr_Upd || Sim_Temp_N2 < Thr_Upd
    if Sim_Temp_N < Sim_Temp_A
        Remove_Num = [Remove_Num;Sim_Temp_P]; % 제거할 Template 번호 저장
        % 템플릿 카운트에 따라 대표 비정상 템플릿 업데이트
        if Cluster_N_Count(Sim_Temp_P) > Cluster_N_Count(Template_Abnormal_P)
%             Template_Abnormal_P = Sim_Temp_P;
        end
        Sim_Table(Sim_Temp_P) = 1;
    end

     
end


% 정상/비정상 Template 분리
Cluster_A = Cluster_N(:,Remove_Num);
Cluster_A_P1 = Cluster_N_P1(:,Remove_Num);
Cluster_A_Count = Cluster_N_Count(:,Remove_Num);
Cluster_A_Num = length(Remove_Num);

Cluster_N(:,Remove_Num) = [];
Cluster_N_P1(:,Remove_Num) = [];
Cluster_N_Count(Remove_Num) = [];
Cluster_Num = Cluster_Num-length(Remove_Num);
%% Template 정렬
% 카운팅이 높은 Template일수록 정상일 가능성이 높음
% 획득된 Template를 카운팅 개수 순으로 정렬함
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
