%% Run a step in simulation for orignal CACC model

function [pos, vel, error,acc] = runStepCacc(pos, vel, ....
    leadPos, leadLen, headway, ePreStep,deltaT, maxAcc, maxDec)
        
        k1 = 0.45;
        k2 = 0.2;
        error = (leadPos - leadLen - pos)-headway*vel;
        errorStar = (error-ePreStep);
        newVel = max(0,vel + k1*error+k2*errorStar);
        newVel = max(min(vel+maxAcc*deltaT, newVel), vel-maxDec*deltaT);
        pos = pos + (newVel+vel)*deltaT/2;
        acc = (newVel-vel)/deltaT;
        vel = newVel;
end