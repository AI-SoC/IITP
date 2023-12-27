function Interp_Mod_P = Fun_Dynamic_Programming(X,Y, Node,Fs)
L = length(Y); % 전체 길이
N = length(Node); % 노드 포인트

% 노드간 오차 행렬 두 노드 사이의 점의 개수가 0 ~ N-2개 일 때 오차
% 초기값은 무한대
Node_Mat = inf*ones(L,L,N-1); 

% 최소오차 경로 저장
Way_Mat = zeros(L,L,N-1);

% 자기자신까지 오차는 0
% 바로 점의개수+1개 거리 까지의 점과의 오차는 0
% 최소 경로는 좌측점 저장
for i = 1 : N-1
    for j = 1 : L
        Node_Mat(j,j:min(L,j+i),i) = 0;
        Way_Mat(j,j:min(L,j+i),i) = j;
    end
end

[~,Node_Mat,Way_Mat]= Dist_Fun(X,Y,Node_Mat,Way_Mat,1,L,N-2,Fs);
Way = [1,L];
for i = N-1 : -1 : 2
    Way = [Way(1), Way_Mat(1,Way(2),i), Way(2:end)];
end
Interp_Mod_P = Way;
% keyboard
function [D,Node_Mat,Way_Mat] = Dist_Fun(X,Y,Node_Mat,Way_Mat,X1,X2,N,Fs)
% 사이값이 없음 종료
if N == -1
    D = 0;
    return
end
% 거리값이 무한대가 아니면 두 점 사이의 오차가 이미 계산완료
if ~isinf(Node_Mat(X1,X2,N+1))
    D = Node_Mat(X1,X2,N+1);
    return
end

% 두 점 사이 점이 없을 경우 오차구하기
if isinf(Node_Mat(X1,X2,N+1)) && N == 0
    D = sum(abs(FUN_Point_to_Line_Dist(X(X1),X(X2),X(X1:X2),Y(X1),Y(X2),Y(X1:X2),Fs))); 
    Node_Mat(X1,X2,N+1) = D;
%     Way_Mat(X1,X2,N+1) = X1+P-1;
%     P = [X1,X2];
    return
end

% 두 점 사이의 점이 있을 경우 오차구하기
D_min = inf;
D_pos = X1;
for i = X1  : X2 - 1
    [D_Temp1,Node_Mat,Way_Mat] = Dist_Fun(X,Y,Node_Mat,Way_Mat,X1,i,N-1,Fs);
    [D_Temp2,Node_Mat,Way_Mat] = Dist_Fun(X,Y,Node_Mat,Way_Mat,i,X2,0,Fs);
    D_Temp = D_Temp1 + D_Temp2;
    if D_Temp< D_min
        D_min = D_Temp;
        D_pos = i;
    end
end
% [X1,X2,N]
D = D_min;
Node_Mat(X1,X2,N+1) = D;
Way_Mat(X1,X2,N+1) = D_pos;