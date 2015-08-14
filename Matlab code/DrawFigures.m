load('detector_data');

SPEED_COL = 4;
FLOW_COL = 3;
K_COL = 5;

times = unique(Detector_Data(:,1));
distance = (DetectorMainLane(:,4));
detector_id = (DetectorMainLane(:,1));
v = zeros(length(times), length(distance));
f = zeros(length(times), length(distance));
k = zeros(length(times), length(distance));

for i=1:length(Detector_Data)
    time_index = find(times == Detector_Data(i, 1));
    distance_index = find(detector_id ==Detector_Data(i,2));
    if distance_index<=0
        continue;
    end
    v(time_index,distance_index) = ...
        Detector_Data(i,SPEED_COL);
    f(time_index,distance_index) = ...
        Detector_Data(i,FLOW_COL);
    k(time_index,distance_index) = ...
        Detector_Data(i,K_COL);
end

max_dis = max(Detector_Info(:,4));
%smooth by time
for i=1:length(distance)
    v(:,i) = smooth(v(:,i));
end

%smooth by time
% v =zeros(length(times), length(distance));
% for i=1:length(times)
%     v(i,:) = interp1(distance,z(i,:),0:max_dis);
% end
fig = figure(1);
axes1 = axes('Parent',fig,'FontSize',16);
surf(times/60,distance,v','LineStyle','none');
xlabel('Time (minute)');
ylabel('Location (meter)');
zlabel('Speed (km/h)');

fig = figure(2);
axes1 = axes('Parent',fig,'FontSize',16);
surf(times/60,distance,f','LineStyle','none');
xlabel('Time (minute)');
ylabel('Location (meter)');
zlabel('Flow (vehicles per hour)');

fig = figure(3);
axes1 = axes('Parent',fig,'FontSize',16);
surf(times/60,distance,k','LineStyle','none');
xlabel('Time (minute)');
ylabel('Location (meter)');
zlabel('Density (Vehicles per km)');
% contour(z', 'LineStyle','none','LineColor',[0 0 0],'Fill','on');