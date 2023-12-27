function [Diff,Diff_vec] = Fun_PC_Comp(Result, DB, N, cut1)
% 입력값
% Result: 출력 PC값
% DB: 레퍼런스 PC값
% N: 연산 목표 차수, 1은 PC1만 계산, 2는 PC2까지 계산...

% 출력값
% Diff: 오차합계
% Diff_vec: PC별 오차값 별도 저장한 벡터


% 짧은 데이터 길이에 맞춰서 데이터 잘라냄
L1 = size(Result,1);
L2 = size(DB,1);
L = min(L1,L2);
Result = Result(1:L, :);
DB = DB(1:L, :);



% PC1값의 최대값으로, 1차 정규화
Result = Result/Result(cut1,1);
DB = DB/DB(cut1,1);

% 각각의 PC값에 대해 Reference의 최대값으로 2차 정규화
for i = 1 : size(Result,2)
    Result(:,i) = Result(:,i)/max(DB(:,i));
    DB(:,i) = DB(:,i)/max(DB(:,i));
end

% 정규화된 데이터에 대해 N번째 PC값 까지의 오차 계산
M = Result(:,1:N)-DB(:,1:N);  

M = M.^2;  

M2(1) = sqrt(sum(M(cut1:end,1)))/(size(DB,1)-cut1+1);
for i = 2 : N
    M2(i) = sqrt(sum(M(:,i)))/size(DB,1);
end
% M = sum(M);  
% M = sqrt(M)/size(DB,1);
M = M2;
Diff_vec = M*100;
Diff = sum(Diff_vec)/N;