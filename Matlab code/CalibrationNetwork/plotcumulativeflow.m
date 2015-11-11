function [] = plotcumulativeflow(temp, temp1, detid, figureid,...
    figure_title)

[row, col] = size(temp);

for i =1:row-1
    for j=1:col
        temp(i+1, j) = temp(i, j)+temp(i+1, j);
        temp1(i+1, j) = temp1(i, j)+temp1(i+1, j);
    end
end

if nargin<5
    figure_title = '';
end

figure(figureid);
plot(temp, 'r', 'LineWidth', 2); 
title(strcat(figure_title, int2str(detid), ' cumulative flow [vphpl]; GEH: ', ...
    num2str(GEH(temp1, temp))));
hold on;
ylim([0, 12000]);
plot(temp1, 'b', 'LineWidth', 2); 
legend('simulation data', 'field data');hold off;