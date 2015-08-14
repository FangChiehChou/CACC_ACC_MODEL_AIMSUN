% generate profile of the leading vehicle
function profile = makeprofile2()

maxtime = 1000;
profile = zeros(1,maxtime);
speed = 40;

% first keep constant speed with a little variation for the first 10 second
for i=1:100  
    profile(1, i) = 40+random('Normal',-0.1,0.1,1,1);
end
% then decelerates until zero 
stop_time = 200;
for i=101:stop_time
    acc = (1.5+random('Normal',-0.5,0.5,1,1));
    if (profile(i-1)-acc*0.1)<0
        profile(1,i) = 0;
        stop_time = i;
        break;
    else
        profile(1,i) = profile(1,i-1)-acc*0.1;
    end
end

%stop for 10 seconds
for i=stop_time:stop_time+100
    profile(1,i) = profile(1,i-1);
end

%then acceleration to the normal speed
for i = stop_time+100:maxtime
    acc = (0.9+random('Normal',-0.2,0.6,1,1));
    if (profile(1,i-1)+acc*0.1)>40
        profile(1,i) = 40+abs(random('Normal',0,0.1,1,1));
    else
        profile(1,i) = profile(1,i-1)+acc*0.1;
    end
end
