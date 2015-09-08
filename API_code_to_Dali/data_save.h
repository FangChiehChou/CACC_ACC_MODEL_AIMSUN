#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <vector>
#include <numeric>

//#include "parameters.h"
#define len_str 1280
#define CONTR_SW 0

#define MERGE_SECTION 332

//char data_saving_dir[len_str]="a";

char data_saving_detector[len_str]="a";
char data_saving_section[len_str]="a";
char data_saving_system[len_str]="a";   
char data_saving_prt_system[len_str]="a";  
char data_saving_networkinfo[len_str]="a";
char data_saving_detector_instant[len_str]="a";
char data_saving_merge_section[len_str]="a";
char data_saving_section_instant[len_str]="a";
char data_saving_signal[len_str]="a";
char data_saving_meter[len_str]="a";

bool READ_DETECTOR_AGGREGATED=true;
bool READ_SECTION_AGGREGATED=true;
bool READ_DETECTOR_INSTANT=true;
bool READ_MERGE_SECTION=true;
bool READ_SECTION_INSTANT=false;
bool READ_SIGNAL=false;
bool READ_METER=false;
bool READ_SYSTEM=true;
int control_intersection=255;
int watch_phase=3;

int phase_state=-1;

//// for reading metering
int meter_id=380;
int meter_state=-1;


int *detIds, *sectIds, *nodeIds;

FILE *dfp, *sfp,*systfp,*difp,*sifp,*sigfp,*metfp,*prtsystfp, *merge_fp; 


int open_detector(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent);
int open_section(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent);
int open_system(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent);
int open_prt_system(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent);
int open_detector_instant(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent);
int open_section_instant(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent);
int open_merge_section(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent);
int open_network(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent);
int open_signal(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent);
int open_meter(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent);
int init_data_saving(unsigned int replica, int acc_percent, int cacc_percent);
int get_detIds(void);
int get_sectIds(void);
int get_nodeIds(void);
int read_detector(double);
int read_detector_instant(double);
int read_section(double);
int read_system(double);
int read_prt_system(double);
int read_section_instant(double);
int read_meter_state(double);
int read_signal_state(double);
int save_networkinfo(char * data_saving, unsigned replic, int accp, int caccp);
int save_detector_info();
//int data_dir(char* data_saving_dir, unsigned int contr_sw);


void DisableCertainBehavior()
{ 
	int exp_id = ANGConnGetExperimentId();
	const unsigned short *twolanecfString = AKIConvertFromAsciiString( 
		"GKExperiment::applyTwoLanesAtt" );
	ANGConnSetAttributeValueBool(ANGConnGetAttribute( twolanecfString ), exp_id, false);

	const unsigned short *overtakeString = AKIConvertFromAsciiString( 
		"GKExperiment::overtakeAtt" );
	ANGConnSetAttributeValueDouble(ANGConnGetAttribute( overtakeString ), exp_id, 1e8);

	const unsigned short *recoverString = AKIConvertFromAsciiString( 
		"GKExperiment::recoverAtt" );
	ANGConnSetAttributeValueDouble(ANGConnGetAttribute( recoverString ), exp_id, 1e8);


}


/********************************************
           Open files for writing
*********************************************/
int init_data_saving(unsigned int replica, int acc_percent, int cacc_percent)
{
	errno_t err;
	//save_networkinfo(data_saving_networkinfo, replica, acc_percent, cacc_percent);
	save_detector_info();
		
	if(READ_DETECTOR_AGGREGATED)
	{		
		open_detector(data_saving_detector, replica, acc_percent, cacc_percent);     // changed on 07/24/12 XYLu
		err=fopen_s(&dfp,data_saving_detector,"w+");
		if(err!=0)
			READ_DETECTOR_AGGREGATED=false;
	}

	if(AKIIsGatheringStatistics()<=0)
	{
		AKIPrintString("statistics data is not gathering or error");
		READ_SECTION_AGGREGATED=false;
	}
	
	if(READ_SECTION_AGGREGATED)
	{
		sectStatInterval=AKIEstGetIntervalStatistics();
		last_sect_readtime=0.0;
		open_section(data_saving_section, replica, acc_percent, cacc_percent);    // changed on 07/24/12 XYLu
		err=fopen_s(&sfp,data_saving_section,"w+");	
		if(err!=0)
			READ_SECTION_AGGREGATED=false;
	}
	if(READ_SYSTEM)
	{
		systStatInterval=AKIEstGetIntervalStatistics();
		last_syst_readtime=0.0;
		open_system(data_saving_system, replica, acc_percent, cacc_percent);    // changed on 07/24/12 XYLu
		err=fopen_s(&systfp,data_saving_system,"w+");	
		if(err!=0)
			READ_SYSTEM=false;
		
		open_prt_system(data_saving_prt_system, replica, acc_percent, cacc_percent);    
		err=fopen_s(&prtsystfp,data_saving_prt_system,"w+");	
		if(err!=0)
			READ_SYSTEM=false; 
	}
	if(READ_DETECTOR_INSTANT)
	{
		detInstantInterval=AKIDetGetCycleInstantDetection();
		last_det_inst_readtime=0.0;
		open_detector_instant(data_saving_detector_instant, replica, acc_percent, cacc_percent);  // changed on 07/24/12 XYLu
		err=fopen_s(&difp,data_saving_detector_instant,"w+");
		if(err!=0)
			READ_DETECTOR_INSTANT=false;

		open_merge_section(data_saving_merge_section, replica, acc_percent, cacc_percent);  
		err=fopen_s(&merge_fp,data_saving_merge_section,"w+");
		if(err!=0)
			READ_MERGE_SECTION=false;
	}
	if(READ_SECTION_INSTANT)
	{
		sectInstantInterval=AKIGetSimulationStepTime();
		last_sect_inst_readtime=0.0;
		open_section_instant(data_saving_section_instant, replica, acc_percent, cacc_percent);   // changed on 07/24/12 XYLu
		err=fopen_s(&sifp,data_saving_section_instant,"w+");
		if(err!=0)
			READ_SECTION_INSTANT=false;
	}
	if(READ_SIGNAL)
	{
		open_signal(data_saving_signal, replica, acc_percent, cacc_percent);
		err=fopen_s(&sigfp,data_saving_signal,"w+");                // changed on 07/24/12 XYLu
		if(err!=0)
			READ_SIGNAL=false;
	}
	if(READ_METER)
	{
		open_meter(data_saving_meter, replica, acc_percent, cacc_percent);
		err=fopen_s(&metfp,data_saving_meter,"w+");
		if(err!=0)
			READ_METER=false;
	}
	last_det_readtime=0.0;	
	get_detIds();
	get_sectIds();
	get_nodeIds();
	return 0;
}


int open_detector(char *data_saving, unsigned int replic, int acc_percent, int cacc_percent)
{

	const int N_files=50;	
	errno_t err;
	FILE* fname;
	
	if (use_RM == 0)
	{
		if(IsBatchMode() == false)
		{
			sprintf_s(data_saving,len_str,"C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\detector_run.txt"
				,acc_percent, cacc_percent, replic);
		}
		else
		{	
			int total_through = 0;
			int off_ramp = 0; 
			int on_ramp = 0;
			read_volumn(total_through,on_ramp,off_ramp);
			sprintf_s(data_saving,len_str,"C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\detector_run_%u_%u_%u.txt"
				,acc_percent, cacc_percent, replic, on_ramp,total_through,off_ramp);			
		}
	}

	err=fopen_s(&fname, data_saving,"w+");
	if (err == 0)
		fclose(fname);
	else		
	{	
		fprintf(stderr,"File detector_run not open!");	
		
	}
	
	return 1;
}

int open_section(char *data_saving, unsigned int replic, int acc_percent, int cacc_percent)
{
	
	const int N_files=50;
	errno_t err;
	FILE* fname;
	char str_tmp[len_str]="a";

	if(IsBatchMode() == false)
	{
		sprintf_s(data_saving,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\section_run.txt"
			,acc_percent, cacc_percent, replic);
	}
	else
	{	
		int total_through = 0;
		int off_ramp = 0; 
		int on_ramp = 0;
		read_volumn(total_through,on_ramp,off_ramp);
		sprintf_s(data_saving,len_str,"C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\section_run_%u_%u_%u.txt"
			,acc_percent, cacc_percent, replic, on_ramp,total_through,off_ramp);			
	}
	
	err=fopen_s(&fname, data_saving,"w+");
	if (err == 0)
		fclose(fname);
	else		
	{	
		fprintf(stderr,"File section_run not open!");					
	}
	
	return 1;
}

int open_prt_system(char *data_saving, unsigned int replic, int acc_percent, int cacc_percent)
{
	const int N_files=50;
	errno_t err;
	FILE* fname;
	char str_tmp[len_str]="a";

	
	if (use_RM == 0)
		sprintf_s(data_saving,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\system_prt_run.txt"
			,acc_percent, cacc_percent, replic);
		

		err=fopen_s(&fname, data_saving,"w+");
		if (err == 0)
			fclose(fname);
		else		
		{	
			fprintf(stderr,"File system_run not open!");					
		}	
	return 1;
}

int open_system(char *data_saving, unsigned int replic, int acc_percent, int cacc_percent)
{	
	const int N_files=50;
	errno_t err;
	FILE* fname;
	char str_tmp[len_str]="a";

	
	if (use_RM == 0)
		sprintf_s(data_saving,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\system_run.txt"
			,acc_percent, cacc_percent, replic);
		

		err=fopen_s(&fname, data_saving,"w+");
		if (err == 0)
			fclose(fname);
		else
		
		{	
			fprintf(stderr,"File system_run not open!");		
			
		}	
	return 1;
}

int open_detector_instant(char *data_saving, unsigned int replic, int acc_percent, int cacc_percent)
{
	const int N_files=50;
	errno_t err;
	FILE* fname;
	char str_tmp[len_str]="a";
	
	if (use_RM == 0)
		sprintf_s(data_saving,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\detector_instant_run.txt"
			,acc_percent, cacc_percent, replic);

		err=fopen_s(&fname, data_saving,"w+");
		if (err == 0)
			fclose(fname);
		else		
		{	
			fprintf(stderr,"File detector_instant_run not open!");				
		}	
	return 1;
}


int open_merge_section(char *data_saving, unsigned int replic, 
						int acc_percent, int cacc_percent)
{
	const int N_files=50;
	errno_t err;
	FILE* fname;
	char str_tmp[len_str]="a";

	if (use_RM == 0)
	{
		if(IsBatchMode() == false)
		{
			sprintf_s(data_saving,len_str,"C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\merge_section.txt"
				,acc_percent, cacc_percent, replic);
		}
		else
		{	
			int total_through = 0;
			int off_ramp = 0; 
			int on_ramp = 0;
			read_volumn(total_through,on_ramp,off_ramp);
			sprintf_s(data_saving,len_str,"C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\merge_section_%u_%u_%u.txt"
				,acc_percent, cacc_percent, replic, on_ramp,total_through,off_ramp);			
		}
	}

	err=fopen_s(&fname, data_saving,"w+");
	if (err == 0)
		fclose(fname);
	else		
	{	
		fprintf(stderr,"File merge_section not open!");				
	}	
	return 1;
}

int open_section_instant(char *data_saving, unsigned int replic, int acc_percent, int cacc_percent)
{
	
	const int N_files=50;
	errno_t err;
	FILE* fname;
	char str_tmp[len_str]="a";

	
	if (use_RM == 0)
		sprintf_s(data_saving,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\section_instant_run.txt"
			,acc_percent,cacc_percent, replic);

		err=fopen_s(&fname, data_saving,"w+");
		if (err == 0)
			fclose(fname);
		else
		
		{	
			fprintf(stderr,"File section_instant_run not open!");				
		}
	
	return 1;
}

int open_network(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent)
{	
	const int N_files=50;
	errno_t err;
	FILE* fname;
	char str_tmp[len_str]="a";
	
	if (use_RM == 0)
		sprintf_s(data_saving,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\network_run.txt"
			,acc_percent,cacc_percent, replic);

		err=fopen_s(&fname, data_saving,"w+");
		if (err == 0)
			fclose(fname);
		else		
		{	
			fprintf(stderr,"File network_run not open!");	
			
		}
	
	return 1;
}

int open_signal(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent)
{
	//int i;
	const int N_files=50;
	errno_t err;
	FILE* fname;
	char str_tmp[len_str]="a";

	if (use_RM == 0)
		sprintf_s(data_saving,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\signal_run.txt"
			,acc_percent,cacc_percent, replic);

		err=fopen_s(&fname, data_saving,"w+");
		if (err == 0)
			fclose(fname);
		else		
		{	
			fprintf(stderr,"File singal_run not open!");				
		}

	return 1;
}

int open_meter(char* data_saving, unsigned int replic, int acc_percent, int cacc_percent)
{
	const int N_files=50;
	errno_t err;
	FILE* fname;
	char str_tmp[len_str]="a";
	
	if (use_RM == 0)
		sprintf_s(data_saving,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\meter_run.txt"
			,acc_percent,cacc_percent, replic);
			
		err=fopen_s(&fname, data_saving,"w+");
		if (err == 0)
			fclose(fname);
		else		
		{	
			fprintf(stderr,"File meter_run not open!");				
		}
	return 1;
}


//save detector information such as location, section_id, and lane_id
int save_detector_info()
{
	FILE *fp;
	errno_t err;
	char str_tmp[len_str]="a";

	char detector_folder[len_str]="a";
	sprintf_s(detector_folder,len_str, "C:\\CACC_Simu_Data\\detecotr_info.txt");
	err=fopen_s(&fp, detector_folder,"w+");

	if (err!=0)
	{
		AKIPrintString("can not open detector info ");
		return 1;
	}

	for(int i=0; i<detectorNum;i++)
	{
		int detector_id = AKIDetGetIdDetector(i);
		structA2KDetector info =
			AKIDetGetPropertiesDetectorById(detector_id);
		//write info 
		fprintf(fp,"%d,%d,%d,%.1f\n",
			detector_id, 
			info.IdSection,
			info.IdFirstLane,
			info.InitialPosition);
	}

	fflush(fp);
	fclose(fp);
	return 0;
}

/**************************************************
         Save network info
***************************************************/
int save_networkinfo(char * data_saving, unsigned int replic, int accp, int caccp)
{
    FILE *fp;
	errno_t err;
	char str_tmp[len_str]="a";

	if (use_RM == 0)
		sprintf_s(data_saving,len_str, "C:\\CACC_Simu_Data\\acc%d_cacc%d\\%u\\detector\\network_info.txt"
			,accp,caccp, replic);	
	
	err=fopen_s(&fp, data_saving,"w+");
	//if(fp==NULL)
	if (err!=0)
	{
		AKIPrintString("can not open  data_saving_networkinfo");
		return 1;
	}
//	char str[128];
	fprintf(fp,"number of detector: %d\n",detectorNum);
	fprintf(fp,"number of section: %d\n",sectionNum);
	fprintf(fp,"number of node: %d\n",nodeNum);
	fprintf(fp,"Replication: %d\n",replication);
	fprintf(fp,"detector aggregation interval: %5.2lf\n",detInterval);
	fprintf(fp,"detector instant data interval: %5.2lf\n",detInstantInterval);
	fprintf(fp,"section aggregation interval: %5.2lf\n",sectStatInterval);
	fprintf(fp,"section instant data interval: %5.2lf\n",sectInstantInterval);
	fprintf(fp,"number of emission factor: %d\n",N_emission);
	fflush(fp);
	fclose(fp);
	return 0;
}

//////////////////////////////////////////////////////////////////////////
// save space mean speed and density of the section
//////////////////////////////////////////////////////////////////////////
void read_space_mean_speed_density(double absolute_time, int section_id)
{
	A2KSectionInf section_info = AKIInfNetGetSectionANGInf(section_id);
	double length = section_info.length/1000.0;
	int num_lane = section_info.nbCentralLanes;
	if(section_info.nbSideLanes == 0)
	{
		return;
	}
	int num_veh = AKIVehStateGetNbVehiclesSection(section_id,true);
	std::vector<double> speed_inner;
	std::vector<double> speed_outer;
	for(int i=0;i<num_veh;i++)
	{
		InfVeh veh_info = AKIVehStateGetVehicleInfSection(section_id,i);
		if(veh_info.numberLane == 1)
			continue;
		if (veh_info.numberLane<=3)
		{
			speed_inner.push_back(veh_info.CurrentSpeed);
		} 
		else
		{
			speed_outer.push_back(veh_info.CurrentSpeed);
		}
	}

	double avg_speed_inner =  0;
	double avg_speed_outer =  0;
	double density_inner = 0;
	double density_outer = 0;
	if(speed_inner.size() >0)
	{
		avg_speed_inner =  std::accumulate(speed_inner.begin(), speed_inner.end(), 0.0)
			/speed_inner.size();
		density_inner = speed_inner.size()/length/2.0;
	}
	if(speed_outer.size()>0)
	{
		avg_speed_outer = std::accumulate(speed_outer.begin(), speed_outer.end(), 0.0)
			/speed_outer.size();
		density_outer = speed_outer.size()/length/(double)(num_lane-2);
	}
	
	fprintf(merge_fp,"%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\n",
			absolute_time, avg_speed_inner,avg_speed_outer,density_inner,density_outer);
	
}



/****************************************
   Read sensor data and save to  files
******************************************/
int save_data(double absolute_time)
{
	if(READ_DETECTOR_AGGREGATED)
	{
		if(fabs(absolute_time-last_det_readtime-detInterval)<1e-3)
		{
			read_detector(absolute_time);

			//meanwhile record the space mean speed and density by averaging over all the vehicles on 
			//inner and outer lanes
			//read_space_mean_speed_density(absolute_time, MERGE_SECTION);

			last_det_readtime=absolute_time;
		}
	}
	if(READ_SECTION_AGGREGATED)
	{
		if(fabs(absolute_time-last_sect_readtime-sectStatInterval)<1e-3)
		{
			read_section(absolute_time);

			last_sect_readtime=absolute_time;
		}
	}
	if(READ_SYSTEM)
	{
		if(fabs(absolute_time-last_syst_readtime-systStatInterval)<1e-3)
		{
			read_system(absolute_time);
			read_prt_system(absolute_time);
			last_syst_readtime=absolute_time;
		}
	}
	if(READ_DETECTOR_INSTANT)
	{
		if(fabs(absolute_time-last_det_inst_readtime-detInstantInterval)<1e-3)
		{
			read_detector_instant(absolute_time);

			last_det_inst_readtime=absolute_time;
		}
		
		
	}
	if(READ_SECTION_INSTANT)
	{
		if(fabs(absolute_time-last_sect_inst_readtime-sectInstantInterval)<1e-3)
		{
			read_section_instant(absolute_time);
			last_sect_inst_readtime=absolute_time;
		}
	}
	if(READ_SIGNAL)
	{
		read_signal_state(absolute_time);
	}
	if(READ_METER)
	{
		read_meter_state(absolute_time);
	}
	return 0;
}

int read_detector(double time)
{
	int i;
	
	for(i=0;i<detectorNum;i++)
	{
		int id = AKIDetGetIdDetector(i);
		double speed = AKIDetGetSpeedAggregatedbyId(id,0);
		if(speed >=0 && speed<200)
			fprintf(dfp,"%.1lf,%d,%d,%d,%.1lf,%.1lf,%.1lf\n",
				time,
				id,
				AKIDetGetPropertiesDetectorById(id).IdFirstLane,
				AKIDetGetCounterAggregatedbyId(id,0),
				speed,
				AKIDetGetTimeOccupedAggregatedbyId(id,0),
				AKIDetGetDensityAggregatedbyId(id,0));
		else
			fprintf(dfp,"%.1lf,%d,%d,%d,%.1lf,%.1lf,%.1lf\n",
			time,
			id,
			AKIDetGetPropertiesDetectorById(id).IdFirstLane,
			0,
			0,
			0,
			0);
					
	}
	
	return 0;
}

int get_detIds()
{
	int i;
	detIds=new int [detectorNum];
	for(i=0;i<detectorNum;i++)
		detIds[i]=AKIDetGetIdDetector(i);
	return 0;
}

int get_sectIds()
{
	int i;
	sectIds=new int [sectionNum];
	for(i=0;i<sectionNum;i++)
		sectIds[i]=AKIInfNetGetSectionANGId(i);
	return 0;
}

int get_nodeIds()
{
	int i;
	nodeIds=new int [nodeNum];
	for(i=0;i<nodeNum;i++)
		nodeIds[i]=ECIGetJunctionId(i);
	return 0;
}

int read_section(double time)
{
	int i;
	StructAkiEstadSection secinfo;  // This is just local variable
	
	for(i=0;i<sectionNum;i++)
	{
		secinfo=AKIEstGetParcialStatisticsSection(sectIds[i],time,0);
		
		if(secinfo.report!=0)
			AKIPrintString("section report error");
		else
		{
			fprintf(sfp,"%10.2lf\t%d\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\n",
				time,secinfo.Id,double(secinfo.Flow),secinfo.TTa,secinfo.DTa,secinfo.Sa,secinfo.Density,secinfo.LongQueueAvg,secinfo.TotalTravel,secinfo.TotalTravelTime);
		}
	}
	return 0;
}

int read_prt_system(double time)
{
	static int i=0;
	double fuel=0.0, CO2=0.0, NOx=0.0, VOC=0.0, PM=0.0;
	StructAkiEstadSystem systinfo;

	systinfo=AKIEstGetParcialStatisticsSystem(time,0);
	
	fuel=AKIEstGetGlobalStatisticsSystemFuelCons(0);	
	CO2=AKIEstGetGlobalStatisticsSystemPollution(0, 0);
	NOx=AKIEstGetGlobalStatisticsSystemPollution(1, 0);
	VOC=AKIEstGetGlobalStatisticsSystemPollution(2, 0);
	PM=AKIEstGetGlobalStatisticsSystemPollution(3, 0);

	if(systinfo.report!=0)
		AKIPrintString("system report error");
	else
		{			
			//if (i<=1)   // to avoid data reading error
			//{
			//	i++;
			//	fflush(prtsystfp);
			//	fprintf(prtsystfp,"%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\n",
			//	time,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
			//}
			//else
			//{
				fflush(prtsystfp);	
				/*fprintf(prtsystfp,"%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\n",
				time,double(systinfo.Flow),systinfo.TTa,systinfo.DTa,systinfo.Sa,systinfo.Sd,
				systinfo.Density,0,
				systinfo.TotalTravel,systinfo.TotalTravelTime);*/
				//fprintf(prtsystfp,
				//	"%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\n",
				//	time,double(systinfo.Flow),0,/*systinfo.TTa,systinfo.DTa,*/0,systinfo.Sa,systinfo.Sd,
				//	systinfo.Density,0,
				//	systinfo.TotalTravel,systinfo.TotalTravelTime);
				fprintf(prtsystfp,
					"%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\n",
					time,
					systinfo.Flow!=systinfo.Flow?0:systinfo.Flow,
					0,
					/*systinfo.TTa,systinfo.DTa,*/0,
					systinfo.Sa!=systinfo.Sa?0:systinfo.Sa,
					systinfo.Sd!=systinfo.Sd?0:systinfo.Sd,
					systinfo.Density!=systinfo.Density?0:systinfo.Density,
					0,
					systinfo.TotalTravel!=systinfo.TotalTravel?0:systinfo.TotalTravel,
					systinfo.TotalTravelTime!=systinfo.TotalTravelTime?0:systinfo.TotalTravelTime);
			//}
		}
	
	return 0;
}

int read_system(double time) // added on 07_08_13
{
	static int i=0;
	double fuel=0.0, CO2=0.0, NOx=0.0, VOC=0.0, PM=0.0;
	StructAkiEstadSystem systinfo;

	systinfo=AKIEstGetGlobalStatisticsSystem(0);

	
	fuel=AKIEstGetGlobalStatisticsSystemFuelCons(0);	
	CO2=AKIEstGetGlobalStatisticsSystemPollution(0, 0);
	NOx=AKIEstGetGlobalStatisticsSystemPollution(1, 0);
	VOC=AKIEstGetGlobalStatisticsSystemPollution(2, 0);
	PM=AKIEstGetGlobalStatisticsSystemPollution(3, 0);

	if(systinfo.report!=0)
		AKIPrintString("system report error");
	else
		{			
			if (i<=1)   // to avoid data reading error
			{
				i++;
				fflush(systfp);
				fprintf(systfp,"%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\n",
				time,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0);
			}
			else
			{
				fflush(systfp);	
				/*fprintf(systfp,"%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\n",
				time,double(systinfo.Flow),systinfo.TTa,systinfo.DTa,systinfo.Sa,systinfo.Sd,systinfo.Density,0,
				systinfo.TotalTravel,systinfo.TotalTravelTime);*/
				//fprintf(systfp,"%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\n",
				//	time,double(systinfo.Flow),0,/*systinfo.TTa,systinfo.DTa*/0,systinfo.Sa,systinfo.Sd,systinfo.Density,0,
				//	systinfo.TotalTravel,systinfo.TotalTravelTime);
				//fprintf(systfp,"%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\n",
				//	time,double(systinfo.Flow),0,/*systinfo.TTa,systinfo.DTa*/0,0,0,0,0,
				//	0,0);
				fprintf(systfp,"%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\t%10.2lf\n",
					time,
					double(systinfo.Flow),
					systinfo.TTa!=systinfo.TTa?0:systinfo.TTa,
					systinfo.DTa!=systinfo.DTa?0:systinfo.DTa,
					systinfo.Sa!=systinfo.Sa?0:systinfo.Sa,
					systinfo.Sd!=systinfo.TTa?0:systinfo.Sd,
					systinfo.Density!=systinfo.Density?0:systinfo.Density,
					0,
					systinfo.TotalTravel!=systinfo.TotalTravel?0:systinfo.TotalTravel,
					systinfo.TotalTravelTime!=systinfo.TotalTravelTime?0:systinfo.TotalTravelTime);
			}
		}
	
	return 0;
}

int read_detector_instant(double time)
{
	int i;
	for(i=0;i<detectorNum;i++)
	{
		int id = AKIDetGetIdDetector(i);
		fprintf(difp,"%.2f,%d,%d,%.1f,%.1f,%.1f\n",
				time,
				id,
				AKIDetGetCounterCyclebyId(detIds[i],0),
				AKIDetGetSpeedCyclebyId(detIds[i],0),
				AKIDetGetTimeOccupedCyclebyId(detIds[i],0),
				AKIDetGetDensityCyclebyId(detIds[i],0));
	}
	
	return 0;
}

int read_section_instant(double time)
{
	int i;
	for(i=0;i<sectionNum;i++)
	{
		fprintf(sifp,"%10.2lf\t%d\t%d\n",
			time,sectIds[i],AKIVehStateGetNbVehiclesSection(sectIds[i],true));
	}
	for(i=0;i<nodeNum;i++)
	{
		fprintf(sifp,"%10.2lf\t%d\t%d\n",
			time,nodeIds[i],AKIVehStateGetNbVehiclesJunction(nodeIds[i]));
	}
	return 0;
}


int read_signal_state(double time)
{
	static float buff=0.0, buff_1=0.0;
	static int J=1;
	int new_phase_state=ECIGetCurrentStateofSignalGroup(control_intersection,watch_phase);
	if(new_phase_state!=phase_state)
	{
		switch(new_phase_state)
		{
		//case 0: fprintf(sigfp,"%5.2lf: change to red\n",time);break;
		case 1:
			buff_1=(float)((time-buff)+(J-1)*buff_1)/J;
			fprintf(sigfp,"Change to green at: %5.2lf; Cycle=%5.2lf; Mean Cycle=%5.2lf \n",time, time-buff, buff_1);
			buff =(float)time;	
			J++;
		break;
		//case 2: fprintf(sigfp,"%5.2lf: change to yellow\n",time);break;
			
		}
		phase_state=new_phase_state;
	}
	return 0;
}

int read_meter_state(double time)
{
	int new_meter_state=ECIGetCurrentStateofMeteringById(meter_id,0);
	if(new_meter_state!=meter_state)
	{
		switch(new_meter_state)
		{
		case 0: fprintf(metfp,"%5.2lf: change to red\n",time);break;
		case 1: fprintf(metfp,"%5.2lf: change to green\n",time);break;
		case 2: fprintf(metfp,"%5.2lf: change to yellow\n",time);break;
		}
		meter_state=new_meter_state;
	}
	return 0;
}





/****************************************
   Closing files after data saving
******************************************/

int finish_data_saving()
{
	delete detIds;
	delete sectIds;
	delete nodeIds;

	if(READ_DETECTOR_AGGREGATED)
	{
		fflush(dfp);
		fclose(dfp);
	}
	if(READ_SECTION_AGGREGATED)
	{
		fflush(sfp);
		fclose(sfp);
	}
	if(READ_SYSTEM)
	{
		fflush(systfp);
		fclose(systfp);
		fflush(prtsystfp);
		fclose(prtsystfp);
	}
	if(READ_DETECTOR_INSTANT)
	{
		fflush(difp);
		fclose(difp);
		fflush(merge_fp);
		fclose(merge_fp);
	}
	if(READ_SECTION_INSTANT)
	{
		fflush(sifp);
		fclose(sifp);
	}
	if(READ_SIGNAL)
	{
		fflush(sigfp);
		fclose(sigfp);
	}
	if(READ_METER)
	{
		fflush(metfp);
		fclose(metfp);
	}
	return 0;
}