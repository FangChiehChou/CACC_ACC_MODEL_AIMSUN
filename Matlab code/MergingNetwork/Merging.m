%% Calculate the density and speed of 
%% on-ramp merging areas based on 
%% HCM method

function [d_r, s_r, s_o, s, v_12_prop] = Merging(l_a,n_lane,v_r,v_f,...
    s_ff, s_fr)

n_o = n_lane - 2; % number of outer lanes

% calculate the flow on the inner two lanes
v_12 = v_f*(0.2178-0.000125*v_r+0.01115*l_a/s_fr);
v_r12 = v_12 + v_r;
v_12_prop = v_12/v_f;
v = v_r+v_f;

% initialize output variables
[d_r, s_r, s_o, s] = deal(0);

if(v>=9400 || v_r12>=4600 || n_lane<2)
    display('Flow exceeds capacity!');
    return;
end

d_r = 5.475+0.00734*v_r+0.0078*v_12-0.00627*l_a; % [veh/mi/ln]
s_r = s_ff-(s_ff-42)*(0.321+0.0039*exp(v_r12/1000)-0.002*(l_a*s_fr/1000)); %[mph]
if(n_o>0)
    v_oa = (v_f-v_12)/n_o;
    if v_oa<500
        s_o=s_ff;
    elseif v_oa < 2300
        s_o=s_ff-0.0036*(v_oa-500);
    else
        s_o=s_ff-6.53-0.006*(v_oa-2300);       
    end
    s = (v_r12+v_oa*n_o)/((v_r12/s_r)+(v_oa*n_o/s_o));
else
    s=s_r;
end
