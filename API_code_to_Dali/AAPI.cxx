
#include "AKIProxie.h"

int count=1;

#include "CIProxie.h"
#include "ANGConProxie.h"
#include "AAPI.h"
#include <stdio.h>
#include <string.h>

#define _CRT_SECURE_NO_DEPRECATE
#include "parameters.h"
#include "sim_data_io.h"
#include "dmd_modify.h"
#include "data_save.h"
#include "RM.h"

char str[len_str];

int AAPILoad()
{
	AKIPrintString("LOAD");
	return 0;
}

int AAPIInit()
{	
	AKIPrintString("\tInit");
	ANGConnEnableVehiclesInBatch(true);
	
	Init();  


	//****************Added by Dali Wei. Read percentages and volume
	int mainlane=0;
	int ramp=0;
	double acc_percent = 0;
	double cacc_percent = 0;
	read_precentage(acc_percent,cacc_percent);
	ACC_percent = acc_percent;
	CACC_percent = cacc_percent;
	//****************

	Init_sim_data_out(acc_percent, cacc_percent);

	//****************Added by Dali Wei modify demand for all slices for generating CACC and ACC penetration rates
	//dmd_modify_cacc_linear(acc_percent, cacc_percent);
	//ModifyMatrixDemand(acc_percent, cacc_percent);
	//modify cooperate factor for on ramps
	/*double coop = 1;
	modify_section_cooperation(coop);*/
	
	//read direct outputs from the pems dataset and transfer it into states
	dmd_create_pems(acc_percent, cacc_percent);
	

	//****************
	init_data_saving(replication,(int)( acc_percent*100),(int)(cacc_percent*100));

	//disable the two-lane car following model and overtaking
	DisableCertainBehavior();
	
	return 0;
}

int AAPIManage(double time, 
			   double timeSta, double timTrans, 
			   double acicle)
{	
	/*dmd_generate_matrix(time, 
		timeSta, timTrans, 
		acicle);*/
	dmd_generate_section(time, 
		timeSta, timTrans, 
		acicle);
	return 0;
}

int AAPIPostManage(double time, double timeSta, double timTrans, double acicle)
{	
	if(time>timTrans && (time-timTrans)==count*detInterval)
	{
		//readDetector_s(time-timTrans);  
		/*if(IsBatchMode()==true)
		{*/
			/*if(save_data(time-timTrans) == QUIT)
			{
				AKISetEndSimTime(AKIGetCurrentSimulationTime()+1);
			}*/
		//}
		//else
		{
			save_data(time-timTrans);
		}
		count++;
	}
	
	return 0;
}

int AAPIFinish()
{
	
	Finish();
	finish_data_saving();
	Finish_sim_data_out();
	/*AKIPrintString("\tFinish");*/
	
	return 0;
}

int AAPIUnLoad()
{
	AKIPrintString("UNLOAD");
	return 0;
}

int AAPIEnterVehicle(int idveh, int idsection)
{
	return 0;
}

int AAPIExitVehicle(int idveh, int idsection)
{
	return 0;
}



int AAPIPreRouteChoiceCalculation(double time, double timeSta)
{
	AKIPrintString("\tPreRouteChoice Calculation");
	return 0;
}
