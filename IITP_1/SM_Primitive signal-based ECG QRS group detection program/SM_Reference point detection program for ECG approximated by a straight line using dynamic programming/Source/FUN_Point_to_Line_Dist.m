function D = FUN_Point_to_Line_Dist(X1, X2, X3, Y1, Y2, Y3,N)
% N = 1;
X1 = X1/N;
X2 = X2/N;
X3 = X3/N;

D =- (  (X3-X1).*(Y2-Y3) - (X2-X3).*(Y3-Y1) ) / sqrt(  (X2-X1)^2 + (Y2-Y1)^2 );
