%% Run Simulations to validate the modified equations from XY Lu
%% Author: Dali Wei
%% Initial Date: 7/28/2015
%% Note: This file cannot be used to evaluate the smoothing method 
%% of Hwasoo's model

close all;

CACC = 0;
CACC_LU = 1;
CACC_LU_NO_CONTRAINT = 2;
NGSIM = 3;
NGSIM_PATCH_PATH=4; % This is the model adopted
NGSIM_PATCH_IDM = 5;
IDM = 6;
%% Read Real Traj File of the leader
load('realTraj');
           
nFollower = 5; % number of Followers

modelCode = CACC_LU_NO_CONTRAINT;
[speedMat, timePointList,distHeadwayMat, headwayMat,posMat,accMat, ruleMat] = ...
    runTrip(highSpeedTraj,nFollower,modelCode);
plotCompFigure(speedMat, timePointList, 'Speed [m/s]');
plotCompFigure(distHeadwayMat, timePointList, 'Distance gap [m]');
plotCompFigure(headwayMat, timePointList, 'Time gap [s]');
plotCompFigure(posMat, timePointList, 'Position [m]');
plotCompFigure(accMat, timePointList, 'Acceleration [m/s^2]');
plotCompFigure(ruleMat, timePointList, 'Rule Applied');