%% Script that tests the DLC function

V_dlc = 5;
Delta_V_dlc = 5;

v_max = 30;
desire = zeros(v_max,v_max);
v_ant = zeros(v_max, v_max);
v = zeros(v_max, v_max);
for i=1:v_max
    for j=1:v_max
        v_ant(i,j) = i;
        v(i,j)= j;
%         if v_ant(i,j)-v(i,j)>Delta_V_dlc
            desire(i,j) = (v_ant(i,j)-max(v(i,j), V_dlc))/max(v(i,j), V_dlc);
%         else
%             desire(i,j) = 0;
%         end
        desire(i,j) = min(1, max(0,desire(i,j)));
    end
end

surf(v, v_ant - v, desire); hold on;