#pragma once
#include<stdio.h>

bool moveData(detData* detection) // move historical detector data to avoid been over-written; piggy-back
{
	int i;
	for(i=0;i<Np-1;i++)
	{
		
		detection->data[i].DataTime=detection->data[i+1].DataTime;
		detection->data[i].density=detection->data[i+1].density;
		detection->data[i].flow=detection->data[i+1].flow;
		detection->data[i].instant_density=detection->data[i+1].instant_density;
		detection->data[i].instant_flow=detection->data[i+1].instant_flow;
		detection->data[i].instant_speed=detection->data[i+1].instant_speed;
		detection->data[i].occupancy=detection->data[i+1].occupancy;
		detection->data[i].speed=detection->data[i+1].speed;
		detection->data[i].vehType=detection->data[i+1].vehType;
	}
	return true;
}
bool moveData_sec(secData* section) // move historical section data to avoid been over-written; piggy-back
{
	int i;
	for(i=0;i<Np-1;i++)
	{
		
		section->data[i].DataTime=section->data[i+1].DataTime;
		section->data[i].density=section->data[i+1].density;
		section->data[i].flow=section->data[i+1].flow;
		section->data[i].speed=section->data[i+1].speed;
		section->data[i].Harmonic_speed=section->data[i+1].Harmonic_speed;
		section->data[i].vehType=section->data[i+1].vehType;
	}
	return true;
}

bool getData(detData* detection,double time,int vehType) // get all sensor data; sensor ID defined in parameters.h
{
	StructAkiEstadSection secinfo;	

	secinfo=AKIEstGetParcialStatisticsSection(detection->sectionId,time,0);
	
	detection->data[Np-1].DataTime=time;
	detection->data[Np-1].vehType=vehType;
if (detection->detId>0)
{
	detection->data[Np-1].density=(double)AKIDetGetDensityAggregatedbyId(detection->detId,vehType);
	
	detection->data[Np-1].flow=double(AKIDetGetCounterAggregatedbyId(detection->detId,vehType))*3600.0/detInterval;

	detection->data[Np-1].occupancy=AKIDetGetTimeOccupedAggregatedbyId(detection->detId,vehType);
	detection->data[Np-1].speed=AKIDetGetSpeedAggregatedbyId(detection->detId,vehType);
	if(detection->data[Np-1].speed<0.5)
		detection->data[Np-1].speed=105;
	StructAkiEstadSection stat_info=AKIEstGetParcialStatisticsSection(detection->sectionId,time,vehType);

	detection->data[Np-1].instant_density=AKIDetGetDensityCyclebyId(detection->detId,vehType);
	detection->data[Np-1].instant_flow=double(AKIDetGetCounterCyclebyId(detection->detId,vehType))*3600.0/detInterval;
	detection->data[Np-1].instant_speed=AKIDetGetSpeedCyclebyId(detection->detId,vehType);
}	
	return true;
}

bool get_Data_Onramp(detData* detection,double time,int vehType) // get all sensor data; sensor ID defined in parameters.h
{
	int j;
	double tmp;
	double tmp_flw=0.0;
	
	StructAkiEstadSection secinfo;
	

	secinfo=AKIEstGetParcialStatisticsSection(detection->sectionId,time,0);
	

	detection->data[Np-1].DataTime=time;
	detection->data[Np-1].vehType=vehType;
	for (j=0;j<max_onramp_ln;j++)  // for separated lanes of onramps
	{
		if (detection->detId_ln[j] > 0)
		{
			tmp=AKIDetGetDensityAggregatedbyId(detection->detId_ln[j],vehType);
			detection->data[Np-1].density=(j/(j+1))*(detection->data[Np-1].density)+(double)(tmp/(j+1));
		}
	}
	
	//fprintf(rm_f, "%10.5lf ", detection->practical_flow);

	detection->data[Np-1].flow=0.0;
	tmp_flw=0.0;
	for (j=0;j<max_onramp_ln;j++)  // for separated lanes of onramps
	{
		if (detection->detId_ln[j] > 0)
		{
			
			detection->ln_flow[j]=double(AKIDetGetCounterAggregatedbyId(detection->detId_ln[j],vehType))*3600.0/Maxd(detInterval,30.0);
			tmp_flw+=detection->ln_flow[j];
		}

	}
	(detection->data[Np-1].flow)=tmp_flw;

	
	detection->practical_flow=double(AKIDetGetCounterAggregatedbyId(detection->detId,vehType))*3600.0/Maxd(detInterval,30.0); // flow after metering
	

	detection->data[Np-1].occupancy=AKIDetGetTimeOccupedAggregatedbyId(detection->detId,vehType);                  // occ after metering
	detection->data[Np-1].speed=AKIDetGetSpeedAggregatedbyId(detection->detId,vehType);                            // spd after metring	
	if(detection->data[Np-1].speed<0.5)
		detection->data[Np-1].speed=105;
	StructAkiEstadSection stat_info=AKIEstGetParcialStatisticsSection(detection->sectionId,time,vehType);
	detection->data[Np-1].instant_density=AKIDetGetDensityCyclebyId(detection->detId,vehType);              // instant density after metring
	detection->data[Np-1].instant_flow=double(AKIDetGetCounterCyclebyId(detection->detId,vehType))*3600.0/detInterval;  // instant flow after metering 
	detection->data[Np-1].instant_speed=AKIDetGetSpeedCyclebyId(detection->detId,vehType);                  // instant speed after metering
	
	return true;
}
bool getData_sec(secData* section,double time, int vehType) // Get section data
{
	section->data[Np-1].DataTime=time;
	section->data[Np-1].vehType=vehType;
	StructAkiEstadSection sec_info=AKIEstGetParcialStatisticsSection(section->sectionId,time,vehType);
	if(sec_info.report!=0)
	{
		AKIPrintString("read section error");
		return false;
	}
	else
	{
		section->data[Np-1].density=sec_info.Density;
		section->data[Np-1].speed=sec_info.Sa;
		if(section->data[Np-1].speed<0.5)
			section->data[Np-1].speed=105;
		section->data[Np-1].flow=double(sec_info.Flow);
		section->data[Np-1].Harmonic_speed=sec_info.SHa;
	}

	
	return true;
}

bool storeData_sec(secData* section) // not used anymore;  07/10/13
{
	fprintf(sec_outfile,"%10.2lf \t%i \t%10.2lf \t%10.2lf \t%10.2lf \t%10.2lf \n",
		section->data[Np-1].DataTime,
		section->data[Np-1].vehType,
		section->data[Np-1].density,
		section->data[Np-1].flow,
		section->data[Np-1].speed,
		section->data[Np-1].Harmonic_speed);
	return true;
}



bool moveprofile(data_profile *p)
{
	int i;
	
	for(i=0;i<Np-1;i++)
	{
		p->data[i].DataTime=p->data[i+1].DataTime;
		p->data[i].density=p->data[i+1].density;
		p->data[i].flow=p->data[i+1].flow;
		p->data[i].occupancy=p->data[i+1].occupancy;
		p->data[i].speed=p->data[i+1].speed;
		p->data[i].vehType=p->data[i+1].vehType;
		p->data[i].instant_density=p->data[i+1].instant_density;
		p->data[i].instant_flow=p->data[i+1].instant_flow;
		p->data[i].instant_speed=p->data[i+1].instant_speed;

	}
	return true;
}

bool call_moveprofile(data_profile *p,int num)
{
	int i;
	for(i=0;i<num;i++)
	{
		moveprofile(p);                            // this is necessary to update buffer of historical data buffer; 05_30_13		
	}
	return true;
}





bool readSection(double time)
{
	int i;
	if(AKIEstIsNewStatisticsAvailable()==true)
	{
		for(i=0;i<sectionNum;i++)
		{
			moveData_sec(section[i]);
			getData_sec(section[i],time,ALLvehType);
			//storeData_sec(section[i]);  // removed on 07/10/13
		}
	}
	else
		AKIPrintString("statistics data not available");
	return true;
}



bool Init_detData_s(detData* detection)   // onramp and off-ramp
{
	
	if(detection->detId!=-1)
	{
		structA2KDetector det_info=AKIDetGetPropertiesDetectorById(detection->detId);
		detection->sectionId=det_info.IdSection;
	}
	else
		detection->sectionId=-1;
	
	return true;
}


double get_flow(int id,int state)
{
	return detection[id]->data[Np-1-state].flow;
}


double get_speed(int id,int state)
{
	return detection[id]->data[Np-1-state].speed;
}
double get_density(int id,int state)
{
	return detection[id]->data[Np-1-state].density;
}

double get_onramp(int id,int state)		// the onramp demand
{
	return detection_onramp[id]->data[Np-1-state].flow;
}
double get_offramp(int id,int state)
{
	return detection_offramp[id]->data[Np-1-state].flow;
}

bool InitRealtimeDetection() //
{
	int i;
	

	detection=new detData* [detectorNum];	
	for(i=0;i<detectorNum;i++)
	{
		detection[i]=new detData;
		detection[i]->detId=AKIDetGetIdDetector(i);			
	}
	return true;
}

bool InitRealTimeSection()
{
	int i;
	section=new secData* [sectionNum];

	for(i=0;i<sectionNum;i++)
	{
		section[i]=new secData;
		section[i]->sectionId=AKIInfNetGetSectionANGId(i);
		
//		AKIPrintString("initial section");
	}

	return true;
}

bool getDetSec()
{
	char filename[len_str],str[len_str];
	FILE* p;
	errno_t err;

	if (use_RM ==0)
		sprintf_s(filename,len_str, "C:\\xylu\\Test_dat\\CACC_Sim\\sim_data\\detector\\%u\\SectionIdforDetectorSmallSection.txt",replication);
	

	err=fopen_s(&p,filename,"w+");
	int i;
	int detId,secId;
	structA2KDetector det_info;
	for(i=0;i<detectorNum;i++)
	{
		detId=AKIDetGetIdDetector(i);
		det_info=AKIDetGetPropertiesDetectorById(detId);
		secId=det_info.IdSection;
		sprintf_s(str,len_str, "%i \t%i \n",detId,secId);
		fprintf(p,str);  //tmp remove
	}

	return true;
}

bool InitRealtimeDetection_s()
{
	int i,j;
	
	detection_s=new detData* [SecSize];
	for(i=0;i<SecSize;i++)
	{
		detection_s[i]=new detData;
		detection_s[i]->detId=CellDetectionId[i]; //		
		Init_detData_s(detection_s[i]);
		
	}
	
	detection_onramp=new detData* [NumOnRamp];  // changed on 01/03/13
	for(i=0;i<NumOnRamp;i++)
	{
		detection_onramp[i]=new detData;
		detection_onramp[i]->detId=OnRampDetectionEndId[i];  // single det after metering for all lanes; added on 10_23_13
		detection_onramp[i]->practical_flow=0.0;

		for (j=0;j<max_onramp_ln;j++)
			detection_onramp[i]->detId_ln[j]=OnRamp_Ln_AdvanceDetEndId[i][j];
		Init_detData_s(detection_onramp[i]);
	}
	
	detection_offramp=new detData* [NumOnRamp];
	for (i=0;i<NumOnRamp;i++)
	{
		detection_offramp[i]=new detData;
		detection_offramp[i]->detId=OffRampDetectionId[i];
		Init_detData_s(detection_offramp[i]);
	}
	return true;
}


bool readDetector_s(double time)   // read only those for control: each cell with one detector; detector ID assigned
{
	int i;
	for(i=0;i<SecSize;i++) 
	{
		moveData(detection_s[i]);
		getData(detection_s[i],time,ALLvehType);
		//storeData(detection_s[i]);
	}
	for (i=0;i<NumOnRamp;i++)
	{
		if (detection_onramp[i]->detId > 0)
		{
			moveData(detection_onramp[i]);
			get_Data_Onramp(detection_onramp[i],time,ALLvehType);  // changed on 09/22/13			
		}
	}
	for (i=0;i<NumOnRamp;i++)
	{
		if (detection_offramp[i]->detId > 0)
		{
			moveData(detection_offramp[i]);
			getData(detection_offramp[i],time,ALLvehType);			
		}
	}
	return true;
}



bool InitDataProfile_s()  // Not used 10_20_13
{
	int i,j;
	
//	char filename[128];
	dataProfileUp=new data_profile;
	
	dataProfileUp->detId=572;
	for(j=0;j<Np;j++)
	{
		dataProfileUp->data[j].DataTime=0;
		dataProfileUp->data[j].density=0;
		dataProfileUp->data[j].flow=0;
		dataProfileUp->data[j].occupancy=0;
		dataProfileUp->data[j].speed=0;
		dataProfileUp->data[j].vehType=ALLvehType;
		dataProfileUp->data[j].instant_density=0.0;
		dataProfileUp->data[j].instant_flow=0.0;
		dataProfileUp->data[j].instant_speed=0.0;
	}

	dataProfileDown=new data_profile;
	
	
	dataProfileDown->detId=581;
	for (j=0;j<Np;j++)
	{
		dataProfileDown->data[j].DataTime=0;  // read historical data at each step and put in this boundary data array for calculation
		dataProfileDown->data[j].density=0;
		dataProfileDown->data[j].flow=0;
		dataProfileDown->data[j].occupancy=0;
		dataProfileDown->data[j].speed=0;
		dataProfileDown->data[j].vehType=ALLvehType;
		dataProfileDown->data[j].instant_density=0.0;
		dataProfileDown->data[j].instant_flow=0.0;
		dataProfileDown->data[j].instant_speed=0.0;
	}
	
	dataProfileOnRamp=new data_profile *[NumOnRamp];
	for (i=0;i<NumOnRamp;i++)
	{
		dataProfileOnRamp[i]=new data_profile;		
		for(j=0;j<Np;j++)
		{
			dataProfileOnRamp[i]->data[j].DataTime=0;
			dataProfileOnRamp[i]->data[j].density=0;
			dataProfileOnRamp[i]->data[j].flow=0;
			dataProfileOnRamp[i]->data[j].occupancy=0;
			dataProfileOnRamp[i]->data[j].speed=0;
			dataProfileOnRamp[i]->data[j].vehType=ALLvehType;
			dataProfileOnRamp[i]->data[j].instant_density=0.0;
			dataProfileOnRamp[i]->data[j].instant_flow=0.0;
			dataProfileOnRamp[i]->data[j].instant_speed=0.0;
		}
	}

	dataProfileOffRamp=new data_profile *[NumOnRamp];
	for (i=0;i<NumOnRamp;i++)
	{
		dataProfileOffRamp[i]=new data_profile;
		dataProfileOffRamp[i]->detId=OffRampDetectionId[i];
	}

	return true;
}



bool update_queue(double time)  // This will not be used since we have onramp length available;
{
	int i,j;
	double w;
	StructAkiEstadSection onramp_info;

	for(i=0;i<NumOnRamp;i++)
	{
		w=0;
		for(j=0;j<max_onramp_ln;j++)
		{
			
			if(OnRamp_Ln_Id[i][j]>0)
			{
				onramp_info=AKIEstGetParcialStatisticsSection(OnRamp_Ln_Id[i][j],time,0);
				if(onramp_info.report!=0)
					AKIPrintString("read section stat error");
				else
					w=Maxd(w,onramp_info.LongQueueAvg);  // use the max queue of the lane as onramp queue
			}
		}
		queue[i]=w;
	}

	return true;
}

