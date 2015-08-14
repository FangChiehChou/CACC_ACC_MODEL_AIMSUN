% generate profile of the leading vehicle
function profile = makeprofile()

maxtime = 1000; %every .1 second
delta_t = 0.1;
profile = zeros(maxtime, 2);

col_speed = 2;
col_time = 1;

for i=1:maxtime
    profile(i,col_time) = (i-1)*delta_t;
end

% first keep constant speed for 10 seconds
for i=1:100 
    profile( i,col_speed) = 30;
end
% then decelerates until zero 
stop_time = 0;
for i=101:600
    acc = 1.5;
    if (profile(i-1, col_speed)-acc*delta_t)<0
        profile(i,col_speed) = 0;
        stop_time = i;
        break;
    else
        profile( i,col_speed) = profile( i-1, col_speed)-acc*delta_t;
    end
end

% stop for ten seconds
for i=stop_time:stop_time+100 
    profile(i,col_speed) = 0;
end

%then acceleration to the normal speed
for i = stop_time+100:maxtime
    acc = 0.9;
    if (profile( i-1, col_speed)+acc*delta_t)>20
        profile( i,col_speed) = 20;
    else
        profile( i,col_speed) = profile( i-1, col_speed)+acc*delta_t;
    end
end
