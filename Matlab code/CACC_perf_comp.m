%% draw comparison figures for CACC ACC datasets

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%  
%%  Modified from XY LU file 07/23/15
%%  
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

clear all;
close all;

N_Cell=10;
agg_T=300;
Meas_T=30;                %% sensor measuremnt Time intervale
Delta_T=60;              %% simulation data save intervale, although the simulation time step is 0.1[s]
NumOnRamp=16;
SecSize=17;
ft_sz=11;

dmd_change=2;            %% 0: no dmd change; 1: selected onramp (0, 5, 6, 7, 8) dmd +5%;  2: dmd +5% for all onramps;
RM_ON=1; %% 0: No RM control; 1: Use Field Local RM 

t=0:agg_T:86370;         %% common time
t_pm=57600:30:70200;      %% PM peak hours
pm_0=1920;               %% PM start step
pm_e=2340;

t_start=15.0;
t_end=21.0;
SIM_DATA=1;              %% to read in simulation data
        
% ACC_Percentage = [0, 0, 0, 0, 20,50,75];% list how many pairs of acc and cacc penetrations to be drawn
% CACC_Percentage = [0,20,50,75,0,0,0]; % list how many pairs of acc and cacc penetrations to be drawn
% ACC_Percentage = [100];% list how many pairs of acc and cacc penetrations to be drawn
% CACC_Percentage = [0]; % list how many pairs of acc and cacc penetrations to be drawn
% ACC_Percentage = [0,0,];% list how many pairs of acc and cacc penetrations to be drawn
% CACC_Percentage = [0,20]; % list how many pairs of acc and cacc penetrations to be drawn
ACC_Percentage = [0, 0, 0, 0, 0,0, 20,40, 60,80,100];% list how many pairs of acc and cacc penetrations to be drawn
CACC_Percentage = [0, 20,40, 60,80,100,0, 0, 0, 0,0];

%create a cell array to indicate the rep numbers for different acc and cacc
%penetration rates
rep_cell = cell(length(ACC_Percentage),1);

% replication numbers here correspond to 
% each combination of ACC & CACC
% percentages
% rep_cell{1} = [24183,24182,24181];
% rep_cell{1} = [24170,24172,24171];
% rep_cell{3} = [24236,24237,24238];

rep_cell{1} = [24183,24182,24181];
rep_cell{2} = [24414,24415,24416];
rep_cell{3} = [24423,24424,24425];
rep_cell{4} = [24432,24433,24434];
rep_cell{5} = [24441,24443,24443];
rep_cell{6} = [24236,24237,24238];
rep_cell{7} = [24378,24379,24380];
rep_cell{8} = [24387,24388,24389];
rep_cell{9} = [24396,24397,24398];
rep_cell{10} = [24405,24406,24407];
rep_cell{11} = [24170,24172,24171];

OverAllDelay = cell(length(ACC_Percentage),1);
OverAllspd = cell(length(ACC_Percentage),1);
OverAllflw = cell(length(ACC_Percentage),1);
OverAllden = cell(length(ACC_Percentage),1);
OverAllStop = cell(length(ACC_Percentage),1);
OverAllTTT = cell(length(ACC_Percentage),1);
OverAllTTD = cell(length(ACC_Percentage),1);

PeriodicAllDelay = cell(length(ACC_Percentage),1);
PeriodicAllspd = cell(length(ACC_Percentage),1);
PeriodicAllflw = cell(length(ACC_Percentage),1);
PeriodicAllden = cell(length(ACC_Percentage),1);
PeriodicAllStop = cell(length(ACC_Percentage),1);
PeriodicAllTTT = cell(length(ACC_Percentage),1);
PeriodicAllTTD = cell(length(ACC_Percentage),1);

for cacc_ind=1:1:length(ACC_Percentage)    
    reps = rep_cell{cacc_ind};
    rep_len = length(rep_cell{cacc_ind});
    disp(strcat('Acc:', int2str(ACC_Percentage(cacc_ind)), '; CACC:', int2str(CACC_Percentage(cacc_ind))));
    if cacc_ind == 7
        reps = reps;
    end
    for rep_ind=1:1:(rep_len)  %% average over all reps    
        Replication=sprintf('%d',reps(rep_ind));
        acc_str=sprintf('%d',ACC_Percentage(cacc_ind));
        cacc_str=sprintf('%d',CACC_Percentage(cacc_ind));
        CACC_dir=['C:\CACC_Simu_Data\acc',acc_str,'_cacc',cacc_str,'\',Replication,'\','detector\'];
        syst_perf_dir=['C:\CACC_Simu_Data\acc',acc_str,'_cacc',cacc_str,'\',Replication,'\syst_perf_para'];
      
         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
         %%  Whole period System data
         %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        fid=0;
        N_CRM_syst_state=10;
        Syst_CRM_dir=[CACC_dir, 'system_run.txt']; 
        fid=0;
        while fid<1                     %% Open file for reading             
             [fid, message]=fopen(Syst_CRM_dir,'r');   %% Data entries - 3: speed; 4: flow; 5: density      
             if (fopen(fid) == -1)
                disp('The File detector_run.txt not open!');
             end
        end  
        N_dat=9999999;
        [A_CRM_syst,Count]=fscanf(fid,'%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf',...
                        [N_CRM_syst_state,N_dat]); %% format:time det_Id flow speed occ density  
        fclose(fid);
        % Format for system data: time,Flow, TTa, DTa, Sa, Sd, Density, NumStops, TTD,TTT
        A_CRM_syst=A_CRM_syst';

        t_CRM_syst=A_CRM_syst(:,1);
        flw_CRM_syst=A_CRM_syst(:,2);
        TT_CRM_syst=A_CRM_syst(:,3);
        Delay_CRM_syst=A_CRM_syst(:,4);
        spd_CRM_syst=A_CRM_syst(:,5);
        spd_dev_CRM_syst=A_CRM_syst(:,6);
        p_CRM_syst=A_CRM_syst(:,7);  % density
        N_CRM_syst_stop=A_CRM_syst(:,8);
        TTd_CRM_syst=A_CRM_syst(:,9);
        TTt_CRM_syst=A_CRM_syst(:,10);  
        data_length=length(TTd_CRM_syst);

        TT_CRM_syst_stop=zeros(data_length,1);
        TT_CRM_syst_delay=zeros(data_length,1);
        TTT_CRM_syst=zeros(data_length,1);
        %TTT_CRM_syst_1=zeros(data_length,1);  %% adding correction caused by TTD
        TTD_CRM_syst=zeros(data_length,1);

        TT_CRM_syst_stop(1)=N_CRM_syst_stop(1)*TTd_CRM_syst(1);
        TT_CRM_syst_delay(1)=Delay_CRM_syst(1)*TTd_CRM_syst(1);
        TTT_CRM_syst(1)=TTt_CRM_syst(1);
        %TTT_CRM_syst_1(1)=TTt_CRM_syst(1);
        TTD_CRM_syst(1)=TTd_CRM_syst(1);

        for I=2:data_length
           TT_CRM_syst_stop(I)=TT_CRM_syst_stop(I-1)+N_CRM_syst_stop(I)*TTd_CRM_syst(I);
           TT_CRM_syst_delay(I)=TT_CRM_syst_delay(I-1)+Delay_CRM_syst(I)*TTd_CRM_syst(I);
           TTT_CRM_syst(I)=TTT_CRM_syst(I-1)+TTt_CRM_syst(I);       
           TTD_CRM_syst(I)=TTD_CRM_syst(I-1)+TTd_CRM_syst(I);
        end

        TT_CRM_syst_delay=TT_CRM_syst_delay/3600;
        TTT_CRM_syst=TTT_CRM_syst/3600;  %% [hr]   
%         TTD_CRM_syst=TTD_CRM_syst/1.609; %% [mph]

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %% Find Mean Over All Reps
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        if rep_ind==1
             spd_CRM_syst_ave = spd_CRM_syst;
             flw_CRM_syst_ave = flw_CRM_syst;
             p_CRM_syst_ave = p_CRM_syst;
             TT_CRM_syst_stop_ave= TT_CRM_syst_stop;
             TT_CRM_syst_delay_ave=TT_CRM_syst_delay;
             TTT_CRM_syst_ave=TTT_CRM_syst;
             TTD_CRM_syst_ave=TTD_CRM_syst;
        else
             spd_CRM_syst_ave =(rep_ind-1)/rep_ind*spd_CRM_syst_ave +  spd_CRM_syst/rep_ind;
             flw_CRM_syst_ave =(rep_ind-1)/rep_ind*flw_CRM_syst_ave +  flw_CRM_syst/rep_ind;
             p_CRM_syst_ave =(rep_ind-1)/rep_ind*p_CRM_syst_ave +  p_CRM_syst/rep_ind;
             TT_CRM_syst_stop_ave= (rep_ind-1)/rep_ind*TT_CRM_syst_stop_ave + TT_CRM_syst_stop/rep_ind;
             TT_CRM_syst_delay_ave=(rep_ind-1)/rep_ind*TT_CRM_syst_delay_ave + TT_CRM_syst_delay/rep_ind;
             TTT_CRM_syst_ave=(rep_ind-1)/rep_ind*TTT_CRM_syst_ave + TTT_CRM_syst/rep_ind;
             TTD_CRM_syst_ave=(rep_ind-1)/rep_ind*TTD_CRM_syst_ave + TTD_CRM_syst/rep_ind;
        end
        
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %%  Periodic System data
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        fid=0;
        N_CRM_syst_state=10;
        Syst_Part_CRM_dir=[CACC_dir, 'system_prt_run.txt']; 
        fid=0;
        while fid<1                     %% Open file for reading             
             [fid, message]=fopen(Syst_Part_CRM_dir,'r');   %% Data entries - 3: speed; 4: flow; 5: density      
             if (fopen(fid) == -1)
                disp('The File detector_run.txt not open!');
             end
        end  

        [A_CRM_syst,Count]=fscanf(fid,'%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf',[N_CRM_syst_state,N_dat]); %% format:time det_Id flow speed occ density  
        fclose(fid);
        % Format for system data: time,Flow, TTa, DTa, Sa, Sd, Density, NumStops, TTD,TTT
        A_CRM_syst=A_CRM_syst';

        t_CRM_syst=A_CRM_syst(:,1);
        Period_flw_CRM_syst=A_CRM_syst(:,2);
        Period_TT_CRM_syst=A_CRM_syst(:,3);
        Period_Delay_CRM_syst=A_CRM_syst(:,4);
        Period_spd_CRM_syst=A_CRM_syst(:,5);
        Period_spd_dev_CRM_syst=A_CRM_syst(:,6);
        Period_p_CRM_syst=A_CRM_syst(:,7);  % density
        Period_N_CRM_syst_stop=A_CRM_syst(:,8);
        Period_TTd_CRM_syst=A_CRM_syst(:,9);
        Period_TTt_CRM_syst=A_CRM_syst(:,10);  
        data_length=length(TTd_CRM_syst);

        Period_TT_CRM_syst_stop=zeros(data_length,1);
        Period_TT_CRM_syst_delay=zeros(data_length,1);
        Period_TTT_CRM_syst=zeros(data_length,1);
        %TTT_CRM_syst_1=zeros(data_length,1);  %% adding correction caused by TTD
        Period_TTD_CRM_syst=zeros(data_length,1);

        Period_TT_CRM_syst_stop(1)=Period_N_CRM_syst_stop(1)*Period_TTd_CRM_syst(1);
        Period_TT_CRM_syst_delay(1)=Period_Delay_CRM_syst(1)*Period_TTd_CRM_syst(1);
        Period_TTT_CRM_syst(1)=Period_TTt_CRM_syst(1);
        %TTT_CRM_syst_1(1)=TTt_CRM_syst(1);
        Period_TTD_CRM_syst(1)=Period_TTd_CRM_syst(1);

        for I=2:data_length
           Period_TT_CRM_syst_stop(I)=Period_TT_CRM_syst_stop(I-1)+N_CRM_syst_stop(I)*Period_TTd_CRM_syst(I);
           Period_TT_CRM_syst_delay(I)=Period_TT_CRM_syst_delay(I-1)+Delay_CRM_syst(I)*Period_TTd_CRM_syst(I);
           Period_TTT_CRM_syst(I)=Period_TTT_CRM_syst(I-1)+Period_TTt_CRM_syst(I);       
           Period_TTD_CRM_syst(I)=Period_TTD_CRM_syst(I-1)+Period_TTd_CRM_syst(I);
        end

        Period_TT_CRM_syst_delay=Period_TT_CRM_syst_delay/3600;
        Period_TTT_CRM_syst=Period_TTT_CRM_syst/3600;  %% [hr]   
%         Period_TTD_CRM_syst=Period_TTD_CRM_syst/1.609; %% [mile]

        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %% Find Mean Over All Reps
        %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        if rep_ind==1
             Period_spd_CRM_syst_ave = Period_spd_CRM_syst;
             Period_flw_CRM_syst_ave = Period_flw_CRM_syst;
             Period_p_CRM_syst_ave = Period_p_CRM_syst;
             Period_TT_CRM_syst_stop_ave= Period_TT_CRM_syst_stop;
             Period_TT_CRM_syst_delay_ave=Period_TT_CRM_syst_delay;
             Period_TTT_CRM_syst_ave=Period_TTT_CRM_syst;
             Period_TTD_CRM_syst_ave=Period_TTD_CRM_syst;
        else
             Period_spd_CRM_syst_ave =(rep_ind-1)/rep_ind*Period_spd_CRM_syst_ave ...
                 +  Period_spd_CRM_syst/rep_ind;
             Period_flw_CRM_syst_ave =(rep_ind-1)/rep_ind*Period_flw_CRM_syst_ave + ...
                 Period_flw_CRM_syst/rep_ind;
             Period_p_CRM_syst_ave =(rep_ind-1)/rep_ind*Period_p_CRM_syst_ave +  ...
                 Period_p_CRM_syst/rep_ind;
             Period_TT_CRM_syst_stop_ave= (rep_ind-1)/rep_ind*Period_TT_CRM_syst_stop_ave + ...
                 Period_TT_CRM_syst_stop/rep_ind;
             Period_TT_CRM_syst_delay_ave=(rep_ind-1)/rep_ind*Period_TT_CRM_syst_delay_ave + ...
                 Period_TT_CRM_syst_delay/rep_ind;
             Period_TTT_CRM_syst_ave=(rep_ind-1)/rep_ind*Period_TTT_CRM_syst_ave +...
                 Period_TTT_CRM_syst/rep_ind;
             Period_TTD_CRM_syst_ave=(rep_ind-1)/rep_ind*Period_TTD_CRM_syst_ave +...
                 Period_TTD_CRM_syst/rep_ind;
        end

        
    end %% global for-loop  rep_ind   end
        
     %%  save the result
     OverAllDelay{cacc_ind} = TT_CRM_syst_delay_ave;
     OverAllspd{cacc_ind} = spd_CRM_syst_ave;
     OverAllflw{cacc_ind} = flw_CRM_syst_ave;
     OverAllden{cacc_ind} = p_CRM_syst_ave;
     OverAllStop{cacc_ind} = TT_CRM_syst_stop_ave;
     OverAllTTT{cacc_ind} = TTT_CRM_syst_ave;
     OverAllTTD{cacc_ind} = TTD_CRM_syst_ave;
     
     
     PeriodicDelay{cacc_ind} = Period_TT_CRM_syst_delay_ave;
     Periodicspd{cacc_ind} = Period_spd_CRM_syst_ave;
     Periodicflw{cacc_ind} = Period_flw_CRM_syst_ave;
     Periodicden{cacc_ind} = Period_p_CRM_syst_ave;
     PeriodicStop{cacc_ind} = Period_TT_CRM_syst_stop_ave;
     PeriodicTTT{cacc_ind} = Period_TTT_CRM_syst_ave;
     PeriodicTTD{cacc_ind} = Period_TTD_CRM_syst_ave;
     
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
    %% reuse the previous variable after averaging
    %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

    TT_CRM_syst_stop= TT_CRM_syst_stop_ave;
    TT_CRM_syst_delay=TT_CRM_syst_delay_ave;
    TTT_CRM_syst=TTT_CRM_syst_ave;
    TTD_CRM_syst=TTD_CRM_syst_ave;

    clear TT_CRM_syst_stop_ave;
    clear TT_CRM_syst_delay_ave;
    clear TTT_CRM_syst_ave;
    clear TTD_CRM_syst_ave;
     
end
 
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%%     Plot
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% color sets for plotting
ColorSet = [
0.00 0.00 1.00 % Data 1 - blue
0.00 1.00 0.00 % Data 2 - green
1.00 0.00 0.00 % Data 3 - red
0.00 1.00 1.00 % Data 4 - cyan
1.00 0.00 1.00 % Data 5 - magenta
0.75 0.75 0.00 % Data 6 - RGB
0.25 0.25 0.25 % Data 7
0.75 0.25 0.25 % Data 8
0.95 0.95 0.00 % Data 9
0.25 0.25 0.75 % Data 10
0.75 0.75 0.75 % Data 11
0.00 0.50 0.00 % Data 12
0.76 0.57 0.17 % Data 13
0.54 0.63 0.22 % Data 14
0.34 0.57 0.92 % Data 15
1.00 0.10 0.60 % Data 16
0.88 0.75 0.73 % Data 17
0.10 0.49 0.47 % Data 18
0.66 0.34 0.65 % Data 19
0.99 0.41 0.23 % Data 20
];

%% plot performance index for the whole period
t_syst = t_CRM_syst;

lengend_str = '';
for i=1:length(ACC_Percentage)
    lengend_str = strcat(lengend_str,...
        '''ACC: ', int2str(ACC_Percentage(i)), '% CACC: ', int2str(CACC_Percentage(i)),'%'',');
end
lengend_str = strcat( 'legend(', lengend_str, '''Location'',''NorthEast'');');

times = t_syst/3600;
start_t = 0;
end_t = 2;

figure_index=0;

figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,1);
for i=1:length(ACC_Percentage)
    plot(times, (OverAllStop{i}), 'color', ColorSet(i,:),'LineWidth', 3,'LineWidth', 3); hold on;
end
grid;
hold off;xlim([start_t, end_t]);
xlabel('Time');
ylabel('Total number of stops');
title('System Performance comparison');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)
% legend('Sim data with Defaul Model', 'Sim Data with CRM', 'Location', 'NorthEast');

figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,2);
for i=1:length(ACC_Percentage)
    plot(times, (OverAllDelay{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
grid;
 hold off;xlim([start_t,end_t]);
xlabel('Time (hour)');
ylabel('Total Delay [hr]');
title('System Performance comparison ');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)

figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,3);
for i=1:length(ACC_Percentage)
    plot(times, (OverAllTTD{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
grid;
 hold off;xlim([start_t,end_t]);
xlabel('Time (hour)');
ylabel('Total Travel Distance [km]');
title('System Performance comparison ');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)

figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,4);
for i=1:length(ACC_Percentage)
    plot(times, (OverAllTTT{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
grid;
 hold off;xlim([start_t,end_t]);
xlabel('Time (hour) [hr]');
ylabel('Total Travel Time [hr]');
title('System Performance comparison ');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)


figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,1);
for i=1:length(ACC_Percentage)
    plot(times, (OverAllspd{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
grid;
 hold off;xlim([start_t,end_t]);
title('System Performance comparison ');
xlabel('Time (hour) [hr]');
ylabel('Speed [km/hr]');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)

figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,2);
for i=1:length(ACC_Percentage)
    plot(times, (OverAllflw{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
grid;
 hold off;xlim([start_t,end_t]);
title('System Performance comparison ');
xlabel('Time (hour) [hr]');
ylabel('Flow [vehs/hr]');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)

figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,3);
for i=1:length(ACC_Percentage)
    plot(times, (OverAllden{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
grid;
 hold off;xlim([start_t,end_t]);
xlabel('Time (hour) [hr]');
ylabel('Density [vehs/km]');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)

%% for periodic 
figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,1);
for i=1:length(ACC_Percentage)
    plot(times, (PeriodicStop{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
 hold off;xlim([start_t,end_t]);
grid;
xlabel('Time (hour)');
ylabel('Total number of stops');
title('System Performance comparison (Averaged over 30 seconds)');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)
% legend('Sim data with Defaul Model', 'Sim Data with CRM', 'Location', 'NorthEast');

figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,2);
for i=1:length(ACC_Percentage)
    plot(times, (PeriodicDelay{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
grid;
 hold off;xlim([start_t,end_t]);
xlabel('Time (hour)');
ylabel('Total Delay [hr]');
title('System Performance comparison (Averaged over 30 seconds)');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)

figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,3);
for i=1:length(ACC_Percentage)
    plot(times, (PeriodicTTD{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
grid;
 hold off;xlim([start_t,end_t]);
xlabel('Time (hour)');
ylabel('TTD [km]');
title('System Performance comparison (Averaged over 30 seconds)');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)

figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,4);
for i=1:length(ACC_Percentage)
    plot(times, (PeriodicTTT{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
grid;
 hold off;xlim([start_t,end_t]);
xlabel('Time (hour) [hr]');
ylabel('TTT [hr]');
title('System Performance comparison (Averaged over 30 seconds)');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)

figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,1);
for i=1:length(ACC_Percentage)
    plot(times, (Periodicspd{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
grid;
 hold off;xlim([start_t,end_t]);
xlabel('Time (hour) [hr]');
ylabel('Speed [km/hr]');
title('System Performance comparison (Averaged over 30 seconds)');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)

figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,2);
for i=1:length(ACC_Percentage)
    plot(times, (Periodicflw{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
grid;
 hold off;xlim([start_t,end_t]);
xlabel('Time (hour) [hr]');
ylabel('flow [vehs/hr]');
title('System Performance comparison (Averaged over 30 seconds)');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)

figure_index=figure_index+1;
fig = figure(figure_index);
% subplot(2,2,3);
for i=1:length(ACC_Percentage)
    plot(times, (Periodicden{i}), 'color', ColorSet(i,:),'LineWidth', 3); hold on;
end
grid;
 hold off;xlim([start_t,end_t]);
xlabel('Time (hour) [hr]');
ylabel('Density [vehs/km]');
title('System Performance comparison (Averaged over 30 seconds)');
eval(lengend_str);
set(findall(fig,'-property','FontSize'),'FontSize',16)


