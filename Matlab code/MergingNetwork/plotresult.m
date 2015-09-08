function [] = plotresult(ramp, x, y, isramp)

xlab = 'Ramp volume [veh/hr]';
suffix = strcat( 'Through traffic volume = ',ramp,' veh/hr');

if isramp == 1
    xlab = 'Through traffic volume (veh per hour)';
    suffix = strcat( 'Ramp volume = ',ramp,' veh/hr');
end
    
figure(1);
plot(x,y(:,1), 'LineWidth', 2); 
xlabel(xlab)
ylabel('Speed of inner two lanes [mph]')
title(strcat('Speed of inner two lanes [mph]', suffix));

figure(2);
plot(x,y(:,2), 'LineWidth', 2); 
xlabel(xlab)
ylabel('Speed of outer two lanes [mph]')
title(strcat('Speed of outer two lanes [mph]', suffix));

figure(3);
plot(x,y(:,3), 'LineWidth', 2);
xlabel(xlab)
ylabel('Density of inner two lanes [veh/mi]')
title(strcat('Density of inner two lanes [veh/mi]', suffix));

figure(4);
plot(x,y(:,4), 'LineWidth', 2);
xlabel(xlab)
ylabel('Density of outer two lanes [veh/mi]')
title(strcat('Density of outer two lanes [veh/mi]', suffix));





