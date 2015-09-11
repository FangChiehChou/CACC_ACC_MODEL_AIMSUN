function [] = plotresult_section(ramp, x, y, isramp)

xlab = 'Ramp volume [veh/hr]';
suffix = strcat( 'Through traffic volume = ',int2str(ramp),' veh/hr');

if isramp == 1
    xlab = 'Through traffic volume (veh per hour)';
    suffix = strcat( 'Ramp volume = ',int2str(ramp),' veh/hr');
end
    
subplot(2,2,1);
plot(x,y(:,3), 'ro','LineWidth', 2); 
xlabel(xlab)
ylabel('Traffic throughput [vph]')
ylim([0,9000]);
title(suffix);

subplot(2,2,2);
plot(x,y(:,4), 'ro', 'LineWidth', 2); 
xlabel(xlab);
ylabel('Travel time [s]');
ylim([0,300]);
title(suffix);

subplot(2,2,3);
plot(x,y(:,6), 'ro', 'LineWidth', 2); 
xlabel(xlab);
ylabel('Speed [km/h]');
ylim([0,120]);
title(suffix);

subplot(2,2,4);
plot(x,y(:,7), 'ro', 'LineWidth', 2);
xlabel(xlab);
ylabel('Density [veh/km]');
ylim([0,100]);
title(suffix);





