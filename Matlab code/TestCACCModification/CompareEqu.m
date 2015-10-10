%% Compare the equilibrium spacing between Newell and Gipps

% Gipps equilibirum speed 
% /////Ciuffo, Biagio, Vincenzo Punzo, 
% //and Marcello Montanino. 
% //"Thirty Years of Gipps' Car-Following Model: 
% //Applications, Developments, and New Features." 
% //Transportation Research Record: 
% //Journal of the Transportation Research Board 
% //2315 (2012): 89-99. (Eq. 3a)

clear;

v_max = 31.3;  %[m/s]
tau = 0.5; 
theta = 0.2;
b_f = -3;
b_e = -3*1.1;

% for newell
headway = 1.5;
jamgap = 1.5; %[m]
len = 4.5; %[m]

capacity = 2800; % [vphpl]
headway = ...
    (v_max/(capacity/3600)-jamgap-len)/v_max;

for i=6:40
    speed_newell(i-5)=min(v_max,(i-jamgap-len)/headway);
end

for i=6:40
    speed_gipps(i-5)=min(v_max, ...
        max((tau+theta)/(1/-b_f-1/(-b_e))...
        *(-1+sqrt(1+(2*(i-(len+jamgap))*(1/-b_f-1/(-b_e)))/(tau+theta)^2)),0));
end

dis = 6:40;
dis = dis - len;
plot(dis, speed_newell,'r');hold on;
plot(dis, speed_gipps,'b'); hold off;





