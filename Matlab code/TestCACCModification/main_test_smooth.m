%% Run Simulations to test the smoothing to Hwasoo Model
%% Author: Dali Wei
%% Initial Date: 08/08/2015

close all;

CACC = 0;
CACC_LU = 1;
CACC_LU_NO_CONTRAINT = 2;
NGSIM = 3;
NGSIM_PATCH_1=4;
NGSIM_PATCH_IDM = 5;
IDM = 6;
%% Read Real Traj File of the leader
SpeedTraj = makeprofile();
           
nFollower = 1; % number of Followers

modelCode = NGSIM_PATCH_1;
[speedMat, timePointList,distHeadwayMat, headwayMat,posMat,accMat, ruleMat,...
    dist_safety_mat] = ...
    runTrip(SpeedTraj,nFollower,modelCode);
plotCompFigure(speedMat, timePointList, 'Speed [m/s]');
plotCompFigure(distHeadwayMat, timePointList, 'Distance gap [m]');
plotCompFigure(headwayMat, timePointList, 'Time gap [s]');
plotCompFigure(posMat, timePointList, 'Position [m]');
plotCompFigure(accMat, timePointList, 'Acceleration [m/s^2]');
plotCompFigure(ruleMat, timePointList, 'Rule Applied');
plotCompFigure(dist_safety_mat, timePointList, 'Safety Distance [m]');