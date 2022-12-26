function Interp_Mod_P = Fun_Dynamic_Programming21(X,Y, Node,Fs)
% X, Y: Time and Amplitude of Input signal
% Node: Initial node
% Fs: Frequency
% Interp_Mod_P: Index of optimized node

%% Initialize
L = length(Y); % Signal length
N = length(Node)-2; % Number of internal vertex
N_Bit = 32; % Limitation of time difference. If bit is 5, N_Bit is 32 = 2^5

if N == 0
    Interp_Mod_P = Node;
    return
end

if L/(N_Bit) > N+1
    N = ceil(L/(N_Bit)); % If the N_bit is not met, N is increased to the minimum
end
W = L-N-1; % If the signal is short, N determines the matrix size of Cost_Mat and Base_Mat.
L2 = min(W,N_Bit); % If the signal if long and N is small, N_Bit determines the matrix size


%% Main process
Base_Mat = inf*ones(L2,1,'uint16');  % initialized as 16 bit unsigned int type
Diff = FUN_Point_to_Line_Dist(X(1),X(end),X,Y(1),Y(end),Y,Fs); % calculate the approximation error
Diff_Max = max(Diff)-min(Diff); % calculate the difference between the maximum error and the minimum error
Diff_Node = sqrt((X(end)-X(1))^2 + (Y(end)-Y(1))^2); % calculate the distance between two endpoints
Dist_Max = Diff_Max*Diff_Node; % calculate the area of the rectangle
Dist_Weight = floor(65535/Dist_Max); % aquire the weight according to the area of the rectangle on uint16 type

clear Diff Diff_Max Diff_Node Dist_Max

Cost_Mat = inf*ones(N,L,'uint16');
Way_Mat = zeros(N,L2);

% Cost_Mat�� 1�� ��� �ÿ��� Base_Mat�� ���и��� �̿��ϳ�, ���� Base_Mat�� 1���� �����ϹǷ� �� �� ����
% Base_Mat�� 1�� �κ��� ���� ����
Cost_Mat_Temp = inf*ones(1,W,'uint16');

i = 1;
Cost_Mat_Temp(1,1) = 0;
for j = 3 : 1+min(N_Bit,W)
    Cost_Mat_Temp(1,j-1) = (Dist_Weight*(sum(abs(FUN_Point_to_Line_Dist(X(i),X(j),X(i:j),Y(i),Y(j),Y(i:j),Fs)))));
end

% ������ ������ 2�� �̻��� ���
if N >= 2
    % Cost_Mat�� ���� ���������� ���, Cost_Mat�� �ּ� 3������ �����ϸ�, ������ L��°�� ���� ���
    for Node_Col = 3 : L-1
        % Cost_Mat�� �� ������ Base_Mat ������Ʈ
        j = Node_Col;
        for i = max(1,Node_Col-L2) : Node_Col-1
            Base_Mat(i-Node_Col+L2+1,1) = (Dist_Weight*(sum(abs(FUN_Point_to_Line_Dist(X(i),X(j),X(i:j),Y(i),Y(j),Y(i:j),Fs)))));
%             [i-Node_Col+L2+1, length(Base_Mat), L2]
        end
        % �־��� Cost_Mat�� ���� ���� ���� ��ĭ�� �����̸� �������
        for Dep = max(1,Node_Col-W-1) : min(N-1, Node_Col-2)
            % 1���� ���, Cost_Mat_Temp�� �̿�
            if Dep == 1
                Temp_Min = inf;
                Temp_p = 2;
                for i = max(2,Node_Col-L2) : Node_Col-1
                    Temp = Cost_Mat_Temp(1,i-1)+Base_Mat(i-Node_Col+L2+1,1);
                    if Temp < Temp_Min 
                        Temp_Min = Temp;
                        Temp_p = i;
                    end
                end
                Cost_Mat(Dep,Node_Col) = Temp_Min;
                Way_Mat(Dep,Node_Col) = Temp_p;
            % 2����ʹ� Cost_Mat�� ���� ��� Base_Mat ������� ���� �ּҰ� ���
            else
                Temp_Min = inf;
                Temp_p = Dep+1;
                for i = max(Dep+1,Node_Col-L2) : Node_Col-1
                    Temp = Cost_Mat(Dep-1,i)+Base_Mat(i-Node_Col+L2+1,1);
                    if Temp < Temp_Min 
                        Temp_Min = Temp;
                        Temp_p = i;
                    end
                end
                Cost_Mat(Dep,Node_Col) = Temp_Min;
                Way_Mat(Dep,Node_Col) = Temp_p;
            end
        end
    end
    % ������ L��° ���� �� ���� ����ϸ� �ǹǷ� ���� �и�
    Node_Col = L;
    Dep = N;
    j = L;
    for i = max(1,Node_Col-L2) : Node_Col-1
        Base_Mat(i-Node_Col+L2+1,1) = (Dist_Weight*(sum(abs(FUN_Point_to_Line_Dist(X(i),X(j),X(i:j),Y(i),Y(j),Y(i:j),Fs)))));
    end
    
    Temp_Min = inf;
    Temp_p = Dep+1;
    for i = max(Dep+1,Node_Col-L2) : Node_Col-1
        Temp = Cost_Mat(Dep-1,i)+Base_Mat(i-Node_Col+L2+1,1);
        if Temp < Temp_Min
            Temp_Min = Temp;
            Temp_p = i;
        end
    end
%     Cost_Mat(Dep,Node_Col) = Temp_Min;
    Way_Mat(Dep,Node_Col) = Temp_p;

% ������ ������ 1���� ���
else
    
    Node_Col = L;
    Dep = 1;
    j = L;
    for i = max(1,Node_Col-L2) : Node_Col-1
        Base_Mat(i-Node_Col+L2+1,1) = (Dist_Weight*(sum(abs(FUN_Point_to_Line_Dist(X(i),X(j),X(i:j),Y(i),Y(j),Y(i:j),Fs)))));
    end
    
    Temp_Min = inf;
    Temp_p = 2;
    for i = max(2,Node_Col-L2) : Node_Col-1
        Temp = Cost_Mat_Temp(1,i-1)+Base_Mat(i-Node_Col+L2+1,1);
        if Temp < Temp_Min
            Temp_Min = Temp;
            Temp_p = i;
        end
    end
%     Cost_Mat(Dep,Node_Col) = Temp_Min;
    Way_Mat(Dep,Node_Col) = Temp_p;
end
%% �ּ� ���� ��� ����
Way = zeros(1,N+2);
Way(1) = 1;
Way(N+2) = L;
for i = N : -1 : 1
    Way(i+1) = Way_Mat(i,Way(i+2));
end
Interp_Mod_P = Way; % ��θ� ��ȯ, �������� �̹�ȯ