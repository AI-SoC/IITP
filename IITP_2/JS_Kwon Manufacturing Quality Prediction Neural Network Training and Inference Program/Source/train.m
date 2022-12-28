close all
clear
clc

shuffleDataset = 1;
dataCutIdx = 500; 

num_epoch       = 50;
miniBatchSize   = 500;

%% 신경망 Layer 정의
layers = [
    featureInputLayer(31, 'Normalization', 'rescale-symmetric')
    fullyConnectedLayer(15, 'WeightsInitializer', 'zeros', 'BiasInitializer', 'zeros')
    sigmoidLayer
    fullyConnectedLayer(4)
    regressionLayer]

% analyzeNetwork(layers)

%% 데이터 read

Dataset1 = readmatrix("data/0_input.xlsx");
Dataset1 = [Dataset1 readmatrix("data/0_output.xlsx")];

Dataset2 = readmatrix("data/1_input.xlsx");
Dataset2 = [Dataset2 readmatrix("data/1_output.xlsx")];

Dataset3 = readmatrix("data/2_input.xlsx");
Dataset3 = [Dataset3 readmatrix("data/2_output.xlsx")];

Dataset4 = readmatrix("data/3_input.xlsx");
Dataset4 = [Dataset4 readmatrix("data/3_output.xlsx")];


DatasetAllOriginal = [Dataset1 ; Dataset2 ; Dataset3 ; Dataset4];


% matrix row random mixing
if shuffleDataset == 1
    for i = 1 : floor(height(DatasetAllOriginal) / 15)
        DatasetAll = DatasetAllOriginal(randperm(size(DatasetAllOriginal, 1)), :);
    end
end


%% 학습 데이터
if shuffleDataset == 1
    XTrain = DatasetAll(1:dataCutIdx , 1:end-4);
    YTrain = DatasetAll(1:dataCutIdx , end-3:end);
else
    XTrain =    [Dataset1(1:dataCutIdx , 1:end-4) ;
                 Dataset2(1:dataCutIdx , 1:end-4) ;
                 Dataset3(1:dataCutIdx , 1:end-4) ;
                 Dataset4(1:dataCutIdx , 1:end-4) ];
    YTrain =    [Dataset1(1:dataCutIdx , end-3:end) ;
                 Dataset2(1:dataCutIdx , end-3:end) ;
                 Dataset3(1:dataCutIdx , end-3:end) ;
                 Dataset4(1:dataCutIdx , end-3:end) ];
end

%% 학습
options = trainingOptions('adam', ...
    'MiniBatchSize',miniBatchSize, ...
    'MaxEpochs',num_epoch, ...
    'InitialLearnRate',0.1, ...
    'Shuffle','every-epoch', ...
    'Verbose',false, ...
    'Plots','training-progress', ...
    'VerboseFrequency',floor(num_epoch / 10) ...
    )

net = trainNetwork(XTrain,YTrain,layers,options);

% net.Layers


%% 학습된 신경망 테스트
result_1 = predict(net, Dataset1(:,1:end-4));
result_2 = predict(net, Dataset2(:,1:end-4));
result_3 = predict(net, Dataset3(:,1:end-4));
result_4 = predict(net, Dataset4(:,1:end-4));

%% 

fig = figure;
subplot(1,8,1)
plot(Dataset1(:,end-3:end))
title('1번_ fit (label)','FontSize',15)
ylim([-3 10])
if shuffleDataset == 0
    xline(dataCutIdx,'-r',{string(dataCutIdx) + '개 학습 데이터 사용'},'FontSize',15, 'LineWidth', 2.5);
end

subplot(1,8,2)
plot(Dataset2(:,end-3:end))
title('2번_ fit (label)','FontSize',15)
ylim([-3 10])
if shuffleDataset == 0
    xline(dataCutIdx,'-r',{string(dataCutIdx) + '개 학습 데이터 사용'},'FontSize',15, 'LineWidth', 2.5);
end

subplot(1,8,3)
plot(Dataset3(:,end-3:end))
title('3번_ fit (label)','FontSize',15)
ylim([-3 10])
if shuffleDataset == 0
    xline(dataCutIdx,'-r',{string(dataCutIdx) + '개 학습 데이터 사용'},'FontSize',15, 'LineWidth', 2.5);
end

subplot(1,8,4)
plot(Dataset4(:,end-3:end))
title('6번_ fit (label)','FontSize',15)
ylim([-3 10])
if shuffleDataset == 0
    xline(dataCutIdx,'-r',{string(dataCutIdx) + '개 학습 데이터 사용'},'FontSize',15, 'LineWidth', 2.5);
end

% 추론 결과
subplot(1,8,5)
plot(result_1)
title('1번_ fit (prediction)','FontSize',15)
ylim([-3 10])

subplot(1,8,6)
plot(result_2)
title('2번_ fit (prediction)','FontSize',15)
ylim([-3 10])

subplot(1,8,7)
plot(result_3)
title('3번_ fit (prediction)','FontSize',15)
ylim([-3 10])

subplot(1,8,8)
plot(result_4)
title('6번_ fit (prediction)','FontSize',15)
ylim([-3 10])
legend('편심x', '편심y', '편심률', 'Location','best','FontSize',15,'AutoUpdate','off')
