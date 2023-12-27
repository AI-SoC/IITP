function Interp_Mod_P = Fun_Dynamic_Programming(X,Y, Node,Fs)
L = length(Y); % ��ü ����
N = length(Node); % ��� ����Ʈ

% ��尣 ���� ��� �� ��� ������ ���� ������ 0 ~ N-2�� �� �� ����
% �ʱⰪ�� ���Ѵ�
Node_Mat = inf*ones(L,L,N-1); 

% �ּҿ��� ��� ����
Way_Mat = zeros(L,L,N-1);

% �ڱ��ڽű��� ������ 0
% �ٷ� ���ǰ���+1�� �Ÿ� ������ ������ ������ 0
% �ּ� ��δ� ������ ����
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
% ���̰��� ���� ����
if N == -1
    D = 0;
    return
end
% �Ÿ����� ���Ѵ밡 �ƴϸ� �� �� ������ ������ �̹� ���Ϸ�
if ~isinf(Node_Mat(X1,X2,N+1))
    D = Node_Mat(X1,X2,N+1);
    return
end

% �� �� ���� ���� ���� ��� �������ϱ�
if isinf(Node_Mat(X1,X2,N+1)) && N == 0
    D = sum(abs(FUN_Point_to_Line_Dist(X(X1),X(X2),X(X1:X2),Y(X1),Y(X2),Y(X1:X2),Fs))); 
    Node_Mat(X1,X2,N+1) = D;
%     Way_Mat(X1,X2,N+1) = X1+P-1;
%     P = [X1,X2];
    return
end

% �� �� ������ ���� ���� ��� �������ϱ�
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