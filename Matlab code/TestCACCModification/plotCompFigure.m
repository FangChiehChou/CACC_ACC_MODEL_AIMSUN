function []=plotCompFigure(speedMat, timePointList, yLableStr)

    fig = figure('Name', 'Speed Comparison');
    axes('Parent',fig,'FontSize',16);
    [~, nVeh] = size(speedMat);
    for iFollower = 1:nVeh-1
        plot(timePointList,speedMat(:,1+iFollower),'r','LineWidth',0.5);hold on;
    end
    plot(timePointList,speedMat(:,1),'b','LineWidth',0.5);hold off;
    xlabel('Time [s]');
    ylabel(yLableStr);
end