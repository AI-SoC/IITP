function P = test_pearson(S1, S2)
N = length(S1);
P = sum((S1-mean(S1)).*(S2-mean(S2))/(std(S1)*std(S2)))/(N-1);
P = (P+1)/2;
