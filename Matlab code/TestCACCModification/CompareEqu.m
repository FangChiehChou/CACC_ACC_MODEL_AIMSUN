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

v_max = 40;
tau = 0.7;
theta = 0.3;
b_f = -3;
b_e = -3*1.1;

% for newell
headway = 1.5;
jamgap = 1;
len = 4;

for i=10:40
    speed_newell(i)=min(v_max,(i-jamgap)/headway);
end

for i=10:40
    speed_gipps(i)=min(v_max, ...
        max((tau+theta)/(1/-b_f-1/(-b_e))...
        *(-1+sqrt(1+(2*(i-(len+jamgap))*(1/-b_f-1/(-b_e)))/(tau+theta)^2)),0));
end

plot(speed_newell,'r');hold on;
plot(speed_gipps,'b'); hold off;





