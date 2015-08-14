%% Run a step in simulation for CACC model with XY Lu model 

function [pos, vel, error, acc] = runStepCaccXiaoyun(pos, vel, ....
    leadPos, leadLen, headway, ePreStep, ePrePreStep, deltaT,...
    maxAcc, maxDec)

        disHeadway = (leadPos - leadLen - pos);
        error = disHeadway - headway*vel;        
        errorStar = (ePreStep-ePrePreStep);
        
        if disHeadway>60            
            newVel = vel + 0.45*ePreStep+0.2*errorStar;
			newVel=max(min(newVel,vel+0.98*deltaT),vel-2.0*deltaT);   
            
        elseif disHeadway>50            
            newVel = vel + 0.4*ePreStep+0.18*errorStar;
			newVel=max(min(newVel,vel+0.8*deltaT),vel-2.0*deltaT);
                    
        elseif disHeadway>40            
            newVel = vel + 0.38*ePreStep+0.16*errorStar;
			newVel=max(min(newVel,vel+0.7*deltaT),vel-2.0*deltaT);
           
        elseif disHeadway>30            
            newVel = vel + 0.36*ePreStep+0.15*errorStar;
			newVel=max(min(newVel,vel+0.5*deltaT),vel-2.0*deltaT);
           
        elseif disHeadway>20            
            newVel = vel + 0.35*ePreStep+0.14*errorStar;
			newVel=max(min(newVel,vel+0.3*deltaT),vel-2.0*deltaT);
           
        elseif disHeadway>15            
            newVel = vel + 0.35*error+0.13*ePreStep;
			newVel=max(min(newVel,vel+0.2*deltaT),vel-2.0*deltaT);
            
        elseif disHeadway>10            
            newVel = vel + 0.35*error+0.1*ePreStep;
			newVel=max(min(newVel,vel+0.1*deltaT),vel-2.0*deltaT);
            
        elseif disHeadway>5            
            newVel = vel + 0.3*error+0.08*ePreStep;
			newVel=max(min(newVel,vel+0.05*deltaT),vel-2.0*deltaT);
                
        else
            newVel = vel + 0.3*error+0.06*ePreStep;
			newVel=max(min(newVel,vel+0.1*deltaT),vel-2.0*deltaT);
        end
                
        newVel = max(0,newVel);
        pos = pos+(newVel+vel)*deltaT/2;
        
        acc = (newVel - vel)/deltaT;
        vel = newVel;
        
end