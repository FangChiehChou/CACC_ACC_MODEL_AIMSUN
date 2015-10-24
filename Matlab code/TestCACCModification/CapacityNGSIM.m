%% calculate the free flow capacity 

function [capacity] = CapacityNGSIM(vf, tau, theta, bf, ...
    be, headway, jamgap, len)

capacity = 0;
if bf < be
    return
end

capacity = vf/(jamgap+len+vf*headway);

% required space
temp = 1/abs(bf)-1/abs(be);
temp1 = (tau+theta)/temp;
min_travel_dis = ((vf/temp1-(-1))^2-1)*((tau+theta)^2)/2/temp;
t_hwy = min_travel_dis/vf;

capacity = min(1/t_hwy, capacity)*3600;

end

