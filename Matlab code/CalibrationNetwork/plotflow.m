function [] = plotflow(temp, temp1, detid, figureid)

figure(figureid);
plot(temp, 'r', 'LineWidth', 2); 
title(strcat(int2str(detid), ' flow [vphpl]; GEH: ', ...
    num2str(GEH(temp1, temp))));
hold on;
ylim([0, 2200]);
plot(temp1, 'b', 'LineWidth', 2); 
legend('simulation data', 'field data');hold off;