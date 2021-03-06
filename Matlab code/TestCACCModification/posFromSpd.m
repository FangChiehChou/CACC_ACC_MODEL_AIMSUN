function posMat = posFromSpd(vehId, iTime, deltaT, ...
    posMat,speedMat)
   
    posMat(iTime,vehId) = posMat(iTime-1, vehId)...
        +(speedMat(iTime-1,vehId)+speedMat(iTime,vehId))/2*deltaT;