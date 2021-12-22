for loop = 1:10000
    %필드 초기화
    field = ones(50,50);
    field(:,:) = 6;

    %모델 생성
    trans = randi(25,5,2);
    receive = randi(25,5,2);

    %점찍기
    for i=1:5
        field(trans(i,1),trans(i,2)) = field(trans(i,1),trans(i,2)) + 1;
        field(receive(i,1)+25,receive(i,2)+25) = field(receive(i,1)+25,receive(i,2)+25) - 1;
    end

    %거리값
    dis = 300;
    for i = 1:5
        for j = 1:5
            if(i==j) 
                continue;
            end
            for k = 1:5
                if(i==k || j==k) 
                    continue;
                end
                for l = 1:5
                    if(i==l || j==l || k==l) 
                        continue;
                    end
                    for m = 1:5
                        if(i==m || j==m || k==m || l==m) 
                            continue;
                        end
                        % 거리계산
                        buf = 0;
                        buf = buf + sqrt((trans(1,1)-receive(i,1))^2+(trans(1,2)-receive(i,2))^2);
                        buf = buf + sqrt((trans(2,1)-receive(j,1))^2+(trans(2,2)-receive(j,2))^2);
                        buf = buf + sqrt((trans(3,1)-receive(k,1))^2+(trans(3,2)-receive(k,2))^2);
                        buf = buf + sqrt((trans(4,1)-receive(l,1))^2+(trans(4,2)-receive(l,2))^2);
                        buf = buf + sqrt((trans(5,1)-receive(m,1))^2+(trans(5,2)-receive(m,2))^2);

                        if(buf<dis)
                            dis = buf;
                        end
                    end
                end
            end
        end
    end

    colormap(mul_w);
    xticks([]);
    yticks([]);
    grid off;
    fname = sprintf('%0.0f-%d.jpg',dis,loop);
    im = image(field,'CDataMapping','direct');
    cdata = print('-RGBImage');
    imwrite(cdata,fname);
    fprintf('%0.2f\n',loop/10000);
end
fprintf('DONE!');
