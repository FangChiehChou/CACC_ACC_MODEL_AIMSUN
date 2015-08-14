%% ngsim model with smoothing patch of IDM model

function [pos, vel, error, acc, rule]...
        = runStepNGSIM_Patch_IDM(pos_follower, ...
                       speed_follower, ....
                       pos_leader, ...
                       speed_leader,...
                       vehLen, ...
                       leader_past_pos, ...
                       deltaT,maxAcc,maxDec,...
                       reaction_time,...
                       jamGap,...
                       vf)
             
COMF_LEVEL = 0.5;
maxDec = -maxDec;	
headway = pos_leader-pos_follower;

% new acceleration
acc = maxAcc*(1-min((speed_follower/vf),1)^4);

if speed_follower>speed_leader
    % new safety distance
    time_collsion = (headway - vehLen)/(speed_follower-speed_leader);
    Dec = speed_follower^2/...
        (4*time_collsion^2*maxDec*COMF_LEVEL);
    
    %Newell
    dist_CF = leader_past_pos - vehLen - jamGap; 
    Newell_speed = (dist_CF - pos_follower)/deltaT*2-speed_follower;
    acc_Newell = (Newell_speed - speed_follower)/deltaT;
    
    acc = acc+acc_Newell+Dec;

end

acc = min(maxAcc, max(maxDec,acc));
dist = ...
    speed_follower * deltaT + 0.5*acc* deltaT * deltaT;

pos = max(pos_follower,pos_follower+dist); 
vel = (pos - pos_follower)/deltaT*2-speed_follower;
acc = (vel - speed_follower)/deltaT;

rule = 0;

error=0;