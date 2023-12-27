function Pos = Sub_Modified_Fitting(X,Y,Node, Fs)
L = length(X);
N = length(Node);

Sig_Diff_temp1 = inf;
Node_temp = Node;
t_max = 64;

% while 1
    Sig_Diff_temp2 = 0;
    for i = 2 : N-1
        Diff_Temp = inf;
        Diff_Pos = 1;
        Diff_Sum = 0;
        for j = max(Node_temp(i+1)-t_max, Node_temp(i-1)+1) : min(Node_temp(i-1)+t_max,Node_temp(i+1)-1)
%         for j = Node_temp(i-1)+1 : Node_temp(i+1)-1
            % i-1��°�� i+1��° ���� ���ο��� i��° ������ ��ġ���� ����ȭ
            % i-1��°�� i��° ���� ����
            D1 = sum(abs(FUN_Point_to_Line_Dist(X(Node_temp(i-1)), X(j), X(Node_temp(i-1):j), ...
                                                Y(Node_temp(i-1)), Y(j), Y(Node_temp(i-1):j), Fs)));
            % i��°�� i+1��° ���� ����
            D2 = sum(abs(FUN_Point_to_Line_Dist(X(j),X(Node_temp(i+1)), X(j:Node_temp(i+1)), ...
                                                Y(j), Y(Node_temp(i+1)), Y(j:Node_temp(i+1)), Fs)));
            D = D1 + D2;
            if D <= Diff_Temp
                Diff_Sum = D1;
                Diff_Temp = D;
                Diff_Pos = j;
            end
        end
        Node_temp(i) = Diff_Pos;
        Sig_Diff_temp2 = Sig_Diff_temp2 + Diff_Sum;
    end
    % ������ �� �پ���� ���� ��� ���� ���� �� ��ü ���� ��ġ ��ȯ
%     if Sig_Diff_temp1 == Sig_Diff_temp2
        Pos = Node_temp;
%         break
%     else
%         Sig_Diff_temp1 = Sig_Diff_temp2;
%     end
% end