function [] = plotresult_section(ramp, x, y, isramp)

xlab = 'Ramp volume [veh/hr]';
suffix = strcat( 'Through traffic volume = ',int2str(ramp),' veh/hr');

if isramp == 1
    xlab = 'Through traffic volume (veh per hour)';
    suffix = strcat( 'Ramp volume = ',int2str(ramp),' veh/hr');
end
    
figure(1);
plot(x,y(:,3), 'ro','LineWidth', 2); 
xlabel(xlab)
ylabel('Traffic throughput [vph]')
title(suffix);

figure(2);
plot(x,y(:,4), 'ro', 'LineWidth', 2); 
xlabel(xlab)
ylabel('Travel time [s]')
title(suffix);

figure(3);
plot(x,y(:,6), 'ro', 'LineWidth', 2); 
xlabel(xlab)
ylabel('Speed [km/h]')
title(suffix);

figure(4);
plot(x,y(:,7), 'ro', 'LineWidth', 2);
xlabel(xlab)
ylabel('Density [veh/km]')
title(suffix);





