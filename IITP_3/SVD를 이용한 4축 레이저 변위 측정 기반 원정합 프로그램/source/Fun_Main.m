%% 초기화
clc, clearvars, close all

%% 실험용 의사 데이터 생성
L = 38.5;       % 반지름 
E = 5;          % 탐색 범위
E_step = .01;   % 탐색 간격

%% 테스트 데이터set, 1-4행 실제 측정 데이터, 5행 의사데이터
% 각 방향으로 오차 표기
e_table = [
    0.034 -0.03 -0.032 0.032 ;
    -0.01 -0.767 -0.013 0.672 ;
    -0.05 -0.106 0.044 0.148 ;
    0.001 -0.796 -0.13 0.65 ;
    5  -1  2  4];

e = e_table(5,:);   % 의사데이터 호출
e = e + L;          % 반지름 추가 길이 
p = zeros(4,2);     % 각점의 x,y 좌표 환산
for i = 1 : 4
    p(i,1) = e(i)*cos((2*i-1)*pi/4); % x좌표
    p(i,2) = e(i)*sin((2*i-1)*pi/4); % y좌표
end


%% grid 기반 최적 중심점 탐색
% 0,0을 중심으로 E 범위 내부에서만 탐색
% E_step grid 간격으로 움직이며 최소 오차를 갖는 지점 탐색

%% 오차를 RMSE를 이용하여 계산

tic
E_min1 = inf;
for i = -E : E_step : E             % 최적 x좌표 탐색
    for j = -E : E_step : E         % 최적 y좌표 탐색
        for k = L-E : E_step : L+E  % 최적 반지름 탐색
            su = 0;
            for l = 1 : 4           % 4 point 오차 측정
                su = su + (sqrt((p(l,1)-i).^2 + (p(l,2)-j).^2)-k)^2;
            end
            su = sqrt(su/4);
            if su < E_min1          % 최적값 갱신
                E_min1 = su;
                X_min1 = i;
                Y_min1 = j;
                R_min1 = k;
            end

        end
    end
end
fprintf("RMSE 기반 원정합 수행시간: \n")
toc

%% 오차를 MAD를 이용하여 계산
% MAD 특성 상, 최적 반지름은 수식화하여 계산 가능
% k에 대한 for 문 생략 가능

tic
E_min2 = inf;
for i = -E : E_step : E         % 최적 x좌표 탐색
    for j = -E : E_step : E     % 최적 y좌표 탐색
        for l = 1 : 4           % 4 point 오차 측정
            d(l) = sqrt((p(l,1)-i).^2 + (p(l,2)-j).^2);
        end
        [a2,b2] = sort(d);
        
        su = (d(b2(4))+d(b2(3))-d(b2(2))-d(b2(1))); 
        if su < E_min2          % 최적값 갱신
            E_min2 = su;
            X_min2 = i;
            Y_min2 = j;
            R_min2 = (d(b2(3))+d(b2(2)))/2; % 최적 반지름 계산
        end
    end
end
fprintf("MAD 기반 원정합 수행시간: \n")
toc

%% SVD 기반 원정합
tic
M = [-2*p, ones(4,1)]\[-(p(:,1).^2+p(:,2).^2)];
X_min3 = M(1);
Y_min3 = M(2);
R_min3 = sqrt(X_min3^2+Y_min3^2-M(3));
su = 0 ;
for l = 1 : 4
    su = su + (sqrt((p(l,1)-X_min3).^2 + (p(l,2)-Y_min3).^2)-R_min3)^2;
%     su = su + (sqrt((p(l,1)-X_min3).^2 + (p(l,2)-Y_min3).^2)-R_min3)^2;
end
E_min3 = sqrt(su/4);
fprintf("SVD 기반 원정합 수행시간: \n")
toc

%% 결과 분석
% error 측정
err1 = (abs(sqrt((p(:,1)-X_min1).^2 + (p(:,2)-Y_min1).^2)-R_min1))';
err2 = (abs(sqrt((p(:,1)-X_min2).^2 + (p(:,2)-Y_min2).^2)-R_min2))';
err3 = (abs(sqrt((p(:,1)-X_min3).^2 + (p(:,2)-Y_min3).^2)-R_min3))';

% 좌표오차 및 error 분포: 1행-RMSE, 2행-MAD, 3행-SVD
Result = [X_min1, Y_min1, R_min1, E_min1,err1;
X_min2, Y_min2, R_min2, E_min2,err2;
X_min3, Y_min3, R_min3, E_min3,err3;]

%% 결과 plot
close all
figure
set(gcf,'position',[446   268   794   710])
set(gca,'position',[0.13,0.11,0.8,0.8])
hold on
grid on
xlim([-50,50])
ylim([-50,50])
line([-27.22-6,-27.22+6],[-27.22-6,-27.22+6],'color',[0,0,0],'linestyle',':')
line([-27.22-6,-27.22+6],[+27.22+6,+27.22-6],'color',[0,0,0],'linestyle',':')
line([+27.22-6,+27.22+6],[-27.22+6,-27.22-6],'color',[0,0,0],'linestyle',':')
line([+27.22-6,+27.22+6],[+27.22-6,+27.22+6],'color',[0,0,0],'linestyle',':')

line([-27.22-6,+27.22+6],[-27.22-6,27.22+6],'color',[0,0,0],'linestyle',':')
line([-27.22-6,+27.22+6],[27.22+6,-27.22-6],'color',[0,0,0],'linestyle',':')
xlabel('x position (mm)')
ylabel('y position (mm)')

p1 = plot(p(:,1),p(:,2),'k*');
p2 = plot(X_min1+R_min2*cos(0:.01:2*pi), Y_min1+R_min1*sin(0:.01:2*pi),'b--');
p3 = plot(X_min2+R_min2*cos(0:.01:2*pi), Y_min2+R_min2*sin(0:.01:2*pi),'k-.');
p4 = plot(X_min3+R_min3*cos(0:.01:2*pi), Y_min3+R_min3*sin(0:.01:2*pi),'r');

legend([p1, p2, p3, p4], {'Input data','RMSE','MAE','SVD'})

%% 결과 plot 확대
figure
subplot(2,2,1)
hold on
grid on
xlim([p(2,1)-5,p(2,1)+5])
ylim([p(2,2)-5,p(2,2)+5])
line([-27.22-6,-27.22+6],[-27.22-6,-27.22+6],'color',[0,0,0],'linestyle',':')
line([-27.22-6,-27.22+6],[+27.22+6,+27.22-6],'color',[0,0,0],'linestyle',':')
line([+27.22-6,+27.22+6],[-27.22+6,-27.22-6],'color',[0,0,0],'linestyle',':')
line([+27.22-6,+27.22+6],[+27.22-6,+27.22+6],'color',[0,0,0],'linestyle',':')

line([-27.22-6,+27.22+6],[-27.22-6,27.22+6],'color',[0,0,0],'linestyle',':')
line([-27.22-6,+27.22+6],[27.22+6,-27.22-6],'color',[0,0,0],'linestyle',':')
xlabel('x position (mm)')
ylabel('y position (mm)')

p1 = plot(p(:,1),p(:,2),'k*');
p2 = plot(X_min1+R_min2*cos(0:.01:2*pi), Y_min1+R_min1*sin(0:.01:2*pi),'b--');
p3 = plot(X_min2+R_min2*cos(0:.01:2*pi), Y_min2+R_min2*sin(0:.01:2*pi),'k-.');
p4 = plot(X_min3+R_min3*cos(0:.01:2*pi), Y_min3+R_min3*sin(0:.01:2*pi),'r');

subplot(2,2,2)
hold on
grid on
xlim([p(1,1)-5,p(1,1)+5])
ylim([p(1,2)-5,p(1,2)+5])
line([-27.22-6,-27.22+6],[-27.22-6,-27.22+6],'color',[0,0,0],'linestyle',':')
line([-27.22-6,-27.22+6],[+27.22+6,+27.22-6],'color',[0,0,0],'linestyle',':')
line([+27.22-6,+27.22+6],[-27.22+6,-27.22-6],'color',[0,0,0],'linestyle',':')
line([+27.22-6,+27.22+6],[+27.22-6,+27.22+6],'color',[0,0,0],'linestyle',':')

line([-27.22-6,+27.22+6],[-27.22-6,27.22+6],'color',[0,0,0],'linestyle',':')
line([-27.22-6,+27.22+6],[27.22+6,-27.22-6],'color',[0,0,0],'linestyle',':')
xlabel('x position (mm)')
ylabel('y position (mm)')

p1 = plot(p(:,1),p(:,2),'k*');
p2 = plot(X_min1+R_min2*cos(0:.01:2*pi), Y_min1+R_min1*sin(0:.01:2*pi),'b--');
p3 = plot(X_min2+R_min2*cos(0:.01:2*pi), Y_min2+R_min2*sin(0:.01:2*pi),'k-.');
p4 = plot(X_min3+R_min3*cos(0:.01:2*pi), Y_min3+R_min3*sin(0:.01:2*pi),'r');

subplot(2,2,3)
hold on
grid on
xlim([p(3,1)-5,p(3,1)+5])
ylim([p(3,2)-5,p(3,2)+5])
line([-27.22-6,-27.22+6],[-27.22-6,-27.22+6],'color',[0,0,0],'linestyle',':')
line([-27.22-6,-27.22+6],[+27.22+6,+27.22-6],'color',[0,0,0],'linestyle',':')
line([+27.22-6,+27.22+6],[-27.22+6,-27.22-6],'color',[0,0,0],'linestyle',':')
line([+27.22-6,+27.22+6],[+27.22-6,+27.22+6],'color',[0,0,0],'linestyle',':')

line([-27.22-6,+27.22+6],[-27.22-6,27.22+6],'color',[0,0,0],'linestyle',':')
line([-27.22-6,+27.22+6],[27.22+6,-27.22-6],'color',[0,0,0],'linestyle',':')
xlabel('x position (mm)')
ylabel('y position (mm)')

p1 = plot(p(:,1),p(:,2),'k*');
p2 = plot(X_min1+R_min2*cos(0:.01:2*pi), Y_min1+R_min1*sin(0:.01:2*pi),'b--');
p3 = plot(X_min2+R_min2*cos(0:.01:2*pi), Y_min2+R_min2*sin(0:.01:2*pi),'k-.');
p4 = plot(X_min3+R_min3*cos(0:.01:2*pi), Y_min3+R_min3*sin(0:.01:2*pi),'r');

subplot(2,2,4)
hold on
grid on
xlim([p(4,1)-5,p(4,1)+5])
ylim([p(4,2)-5,p(4,2)+5])
line([-27.22-6,-27.22+6],[-27.22-6,-27.22+6],'color',[0,0,0],'linestyle',':')
line([-27.22-6,-27.22+6],[+27.22+6,+27.22-6],'color',[0,0,0],'linestyle',':')
line([+27.22-6,+27.22+6],[-27.22+6,-27.22-6],'color',[0,0,0],'linestyle',':')
line([+27.22-6,+27.22+6],[+27.22-6,+27.22+6],'color',[0,0,0],'linestyle',':')

line([-27.22-6,+27.22+6],[-27.22-6,27.22+6],'color',[0,0,0],'linestyle',':')
line([-27.22-6,+27.22+6],[27.22+6,-27.22-6],'color',[0,0,0],'linestyle',':')
xlabel('x position (mm)')
ylabel('y position (mm)')

p1 = plot(p(:,1),p(:,2),'k*');
p2 = plot(X_min1+R_min2*cos(0:.01:2*pi), Y_min1+R_min1*sin(0:.01:2*pi),'b--');
p3 = plot(X_min2+R_min2*cos(0:.01:2*pi), Y_min2+R_min2*sin(0:.01:2*pi),'k-.');
p4 = plot(X_min3+R_min3*cos(0:.01:2*pi), Y_min3+R_min3*sin(0:.01:2*pi),'r');

lgd = legend([p1, p2, p3, p4], {'Input data','RMSE','MAE','SVD'});
set(lgd,'position',[0.22,0.947,0.59,0.043],'numcolumns',4)