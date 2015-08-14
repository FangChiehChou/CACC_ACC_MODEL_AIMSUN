/******************************************************************

                Version from "combine"
******************************************************************/

#include <math.h>
#include <string.h>
#include "readdetectordata.h"


double Mind(double a,double b)
{
	if(a<=b)
		return a;
	else
		return b;
}
double Maxd(double a,double b)
{
	if(a>=b)
		return a;
	else
		return b;
}
double get_min(double a, double b)
{
	if(a<=b)
		return a;
	else
		return b;
}




bool get_s()     // get off-ramp flow
{
	int i;	
	for(i=0;i<NumOnRamp;i++)
	{
		if (detection_offramp[i]->detId > 0)		
			//s[i]=exp_flt*(detection_offramp[i]->data[0].flow)+(1-exp_flt)*s[i]; 
			s[i]=exp_flt*(detection_offramp[i]->data[Np-1].flow)+(1-exp_flt)*s[i]; // changed on 03/03/14			
		else
			s[i]=0.0;
	}

	return true;
}


bool get_dmd()
{
	int i;
	
	for(i=0;i<NumOnRamp;i++);	
	{				
		dmd[i]=float(AKIDetGetCounterAggregatedbyId(OnRampDetectionEndId[i],0))*3600.0/Maxd(detInterval,30.0);	
	}


	return true;
}



bool get_meas(double T)
{
	int i;


	for(i=0;i<SecSize;i++)
	{		
		v[i]=exp_flt*(detection_s[i]->data[Np-1].speed)+(1.0-exp_flt)*v[i];
		o[i]=exp_flt*(detection_s[i]->data[Np-1].occupancy)+(1.0-exp_flt)*o[i];		
	}
	return true;
}




bool Set_Default_Meter(double time,double time2,double timeSta)   // this implemenmtation is correct 03_05_14
{
	int i,j,k, tmp_err, tmp_err1;
	double flow=0.0;
	double amax=0.0;
	double amin=0.0;
	

	tmp_err=0; tmp_err1=0; 
	if(ISUPDATE2>=0)  // set every step
	{
		for(i=0;i<NumOnRamp;i++)
		{			
			for (j=0;j<max_mainline_ln;j++)
			{
				if (Mainline_Ln_Det_Id[i][j]>0)
				{					
					RM_occ[i]+=AKIDetGetTimeOccupedAggregatedbyId(Mainline_Ln_Det_Id[i][j],0);											
			
				}
			}
			RM_occ[i]=RM_occ[i]/N_Mainline_Ln_RM[i];
			fprintf(dbg_f,"%10.2f ", RM_occ[i]);
			
			for (j=0;j<N_interv-1;j++)
			{
				if (RM_occ[i]<SR99_RM_occ_tbl[0][i])
				{
					for (k=0;k<max_onramp_ln;k++)
					{
						if (ln_meteringId[i][k]>0)
						{
							tmp_err=ECIGetParametersFlowMeteringById(ln_meteringId[i][k],timeSta,&amax,&flow,&amin);
							if (k<2)
								actual_r[i][k]=SR99_RM_rate_tbl[0][i];
							else  // HOV lane								
								actual_r[i][k]=amax;							
						}
						else
							actual_r[i][k]=0.0;
					}
				}
				else if (RM_occ[i]>=SR99_RM_occ_tbl[j][i] && RM_occ[i]<=SR99_RM_occ_tbl[j+1][i])
				{					
					for (k=0;k<max_onramp_ln;k++)
					{
						if (ln_meteringId[i][k]>0)
						{
							tmp_err=ECIGetParametersFlowMeteringById(ln_meteringId[i][k],timeSta,&amax,&flow,&amin);
							if (k<2)
								actual_r[i][k]=SR99_RM_rate_tbl[j][i];								
							else  // HOV lane														
								actual_r[i][k]=amax;							
						}
						else
							actual_r[i][k]=0.0;
					}
				}
				else
				{
					for (k=0;k<max_onramp_ln;k++)
					{
						if (ln_meteringId[i][k]>0)
						{
							tmp_err=ECIGetParametersFlowMeteringById(ln_meteringId[i][k],timeSta,&amax,&flow,&amin);
							if (k<2)
								actual_r[i][k]=SR99_RM_rate_tbl[N_interv-1][i];
							else  // HOV lane							{								
								actual_r[i][k]=amax;							
						}
						else
							actual_r[i][k]=0.0;
					}
				}			
			} // for j-loop
			for (k=0;k<max_onramp_ln;k++)
			{
				if (actual_r[i][k]>max_RM_rt)
					actual_r[i][k]=max_RM_rt;
			}
		} // for i-loop
		fprintf(dbg_f,"\n");

		// activation of Feild RM
		for(i=0;i<NumOnRamp;i++)
		{	
			for (j=0;j<max_onramp_ln;j++)
			{				
				if(ln_meteringId[i][j]>0)				
					tmp_err1=ECIChangeParametersFlowMeteringById(ln_meteringId[i][j],timeSta,max_RM_rt,actual_r[i][j],amin); 			
			} // for j-loop end			
		}// for i-loop end	
		fprintf(rm_f, "\n");
		ISUPDATE2=0;

		fprintf(rm_f,"%10.2f\t", timeSta);
		for(i=0;i<NumOnRamp;i++)
			 fprintf(rm_f,"%10.2f\t", (float)(actual_r[i][0]+actual_r[i][1]+actual_r[i][2]));
		fprintf(rm_f,"\n");

	
	} // if ISUPDATE2 loop 
	else
		ISUPDATE2++;	
	return true;
}






bool get_state(double time)   // a Major function
{
	get_dmd();          // onramp flow not considered	
	get_meas(time);		
	get_s();
	
	return true;
}


bool Meter(double time,double time2,double timeSta)
{
	
	Set_Default_Meter(time,time2,timeSta); 
	
	return true;
}




bool RM_Control(double time,double time2,double timeSta)
{
	if(count>=Np)
	{	

		get_state(time);
		
		if (use_RM == 0)
			Meter(time,time2,timeSta);
	}
	return true;
}


bool Finish()	//should be all the fclose()
{
	delete detection;
	delete section;
	delete detection_s;
	delete detection_onramp;
	delete detection_offramp;

	return true;	
}