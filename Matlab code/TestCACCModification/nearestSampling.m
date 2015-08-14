function [timePointList, speedList] = nearestSampling(delta, x, y)
    
    
    startTime = round(min(x),1);
    endTime = max(x);
    
    timePointList = startTime:delta:endTime;
    timePointList = timePointList';
    
    speedList = zeros(length(timePointList), 1);
    %find the nearst point
    for i=1:length(speedList)    
        [~,index] = min(abs(x-timePointList(i,1)));
        speedList(i,1) = y(index,1);
    end
    
end