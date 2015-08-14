/***************************************************************************

	this is modified from Dongyan's code for metering at SR87-Taylor
	for coordination; 08_30_13

***************************************************************************/


double safety_gap2=1.0;
int last_green_metering=0;	//the metering index that turns green at last time
int HOV_last_green_metering=0;
//double metering_rate_change_time;	//the time when metering rate changes; not used
double CYCLE;	//simulation step duration
double last_green;	//the time when the last green happens; used in metering 2
double HOV_last_green;
bool can_turn_green=false;	/*check which metering should be green,0 to NUMBER_METERING-1, if -1, no one should be green*/  // used in metering 2
bool HOV_can_turn_green=false;
//double last_red;	//the time when the last red happens
//double HOV_last_red;
//double safety_gap=0.1;

double red_green_safety_gap=0.5;
double max_green=5.0;

//////////////////// for Taylor St intersection///////////////////////

#define NUMBER_METERING 2
//int meteringID[NUMBER_METERING]={380,379};
int meteringID[NUMBER_METERING]={20139,20138};
//int detectorID[NUMBER_METERING][2]={{388,390},{389,391}};
int detectorID[NUMBER_METERING][2]={{20142,20146},{20141,20145}};
bool arrival[NUMBER_METERING]={false,false};
int presence[NUMBER_METERING]={0};
int down_presence[NUMBER_METERING]={0};
bool show_green[NUMBER_METERING]={false,false};
bool show_green_last_step[NUMBER_METERING]={false,false};
double last_red_time[NUMBER_METERING]={-1.0,-1.0};
double last_green_time[NUMBER_METERING]={-1.0,-1.0};

#define HOV_NUMBER_METERING 1
//\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\//int HOV_meteringID[HOV_NUMBER_METERING]={434};
int HOV_meteringID[HOV_NUMBER_METERING]={20140};
int HOV_detectorID[HOV_NUMBER_METERING][2]={20147,20147};  // {b4, after}
bool HOV_arrival[HOV_NUMBER_METERING]={false};
int HOV_presence[HOV_NUMBER_METERING]={0};
int HOV_down_presence[HOV_NUMBER_METERING]={0};
bool HOV_show_green[HOV_NUMBER_METERING]={false};
bool HOV_show_green_last_step[HOV_NUMBER_METERING]={false};
double HOV_last_red_time[HOV_NUMBER_METERING]={-1.0};
double HOV_last_green_time[HOV_NUMBER_METERING]={-1.0};

int mainline_detector=2;
int mainline_detectorID[2]={673,674};
double rate=1650.0;
double HOV_rate=900.0;

#define TRANS_RATE 1800.0;
#define EFFECTIVE_RATE_1 1.0
#define EFFECTIVE_RATE_3 1.0

double meter_rate;

double get_metering_rate(int type, double occ, double time) // we do not need this for SR99
{
	char str[128];
	sprintf(str,"type=%d, occ=%lf, time=%lf",type,occ,time);
//	AKIPrintString(str);
	double meter_rate;
	if(type==1)	//generate metering rate for the HOV at taylor
	{
		if(occ<4 && occ>=0)
			meter_rate=900;
		else
			if(occ>=4 && occ<6)
				meter_rate=820.0;
			else
				if(occ>=6)
					meter_rate=820.0;
				else
					meter_rate=820.0;
		return meter_rate;

	}
	if(type==2)	//generating metring rate at the general lanes at taylor
	{
		if(time<=15*60)
		{
			if(occ<4 && occ>=0)
				meter_rate=900*2;
			else
				if(occ>=4 && occ<6)
					meter_rate=800.0*2;
				else
					if(occ>=6 && occ<9)
						meter_rate=720.0*2;
					else
						if(occ>=9)
							meter_rate=720.0*2;
						else
							meter_rate=720.0*2;
			return meter_rate*EFFECTIVE_RATE_1;
		}
		if(time>15*60 && time<45*60)
		{
			if(occ<4 && occ>=0)
				meter_rate=900*2;
			else
				if(occ>=4 && occ<6)
					meter_rate=800.0*2;
				else
					if(occ>=6 && occ<9)
						meter_rate=660.0*2;
					else
						if(occ>=9)
							meter_rate=660.0*2;
						else
							meter_rate=660.0*2;
			return meter_rate;
		}
		if(time>=45*60)
		{
			if(occ<4 && occ>=0)
				meter_rate=900*2;
			else
				if(occ>=4 && occ<6)
					meter_rate=800.0*2;
				else
					if(occ>=6 && occ<9)
						meter_rate=660.0*2;
					else
						if(occ>=9 && occ<10)
							meter_rate=600.0*2;
						else
							if(occ>=10)
								meter_rate=600.0*2;
							else
								meter_rate=600.0*2;
			return meter_rate*EFFECTIVE_RATE_3;
		}
	}
	return 1000.0;
}

bool is_green2(double rate,double time)
{
	if(time-last_green<3600.0/rate)
	{
		if(3600.0/rate-(time-last_green)<CYCLE/2)
			return true;
		else
			return false;
	}
	else
		return true;
}
bool is_green2HOV(double rate,double time)
{
	if(time-HOV_last_green<3600.0/rate)
	{
		if(3600.0/rate-(time-HOV_last_green)<CYCLE/2)
			return true;
		else
			return false;
	}
	else
		return true;
}

int metering2(double time)	// for the meterings that can be green at the same time
{
	char str[128];

	int i;
	//see whether there are vehicles waiting to merge
	for(i=0;i<NUMBER_METERING;i++)
	{
		if(AKIDetGetPresenceCyclebyId(detectorID[i][1],0)==1)
			down_presence[i]++;
		else
			down_presence[i]=0;
		if(arrival[i]==false)
		{
			if(AKIDetGetPresenceCyclebyId(detectorID[i][0],0)==1)
				presence[i]++;
			else
				presence[i]=0;
//			if(AKIDetGetPresenceCyclebyId(detectorID[i][0],0)>0)
			if(AKIDetGetCounterCyclebyId(detectorID[i][0],0)>0)
			{
				arrival[i]=true;
				presence[i]=0;
			}
			else
				if(presence[i]>=10 && down_presence[i]>=5)
				{
					arrival[i]=true;
					presence[i]=0;
				}
		}

	sprintf(str,"arrival 0 = %d, arrival 1 = %d",arrival[0],arrival[1]);
//	AKIPrintString(str);
	}


//	sprintf(str,"show 1 = %d, show 2 = %d, show 3 = %d",show_green[0],show_green[1],show_green[2]);
//	AKIPrintString(str);


//now it is green, see whether there the current green light should turn red
	for(i=0;i<NUMBER_METERING;i++)
	{
		if(ECIGetCurrentStateofMeteringById(meteringID[i],1)==1)
		{
			show_green_last_step[i]=true;
			if(AKIDetGetCounterCyclebyId(detectorID[i][1],0)>0  || time-last_green_time[i]>max_green)
			{
				show_green[i]=false;
				last_red_time[i]=time;
			}
		}
		else
			show_green_last_step[i]=false;
	}

	//it is red now, so check whether should turn green
	if(time-last_green>=safety_gap2)
	{
		if(can_turn_green==true)
		{
			int i;
			for(i=0;i<NUMBER_METERING;i++)
			{
				int new_green_metering=(last_green_metering+1+i)%(NUMBER_METERING);
//				sprintf(str,"new green %d",new_green_metering);
//				AKIPrintString(str);
				if(arrival[new_green_metering]==true && show_green_last_step[new_green_metering]==false && time-last_red_time[new_green_metering]>red_green_safety_gap)// && down_presence[new_green_metering]==0)
				{
					//ECIChangeStateMeteringById(meteringID[new_green_metering],1,time,CYCLE,1);
					show_green[new_green_metering]=true;
					can_turn_green=false;
					last_green_metering=new_green_metering;
					last_green=time;
					last_green_time[new_green_metering]=time;
//					sprintf(str,"last green = %lf",last_green);
//					AKIPrintString(str);
					arrival[new_green_metering]=false;
//					sprintf(str,"green %d",new_green_metering);
//					AKIPrintString(str);
					break;
				}
			}
		}
		else
		{
			double occ=0.0;
			int s;
			for(s=0;s<mainline_detector;s++)
				occ=occ+AKIDetGetTimeOccupedAggregatedbyId(mainline_detectorID[s],0);
			occ=occ/mainline_detector;
			rate=get_metering_rate(2, occ, time);
//			char str[128];
//			sprintf(str,"rate=%lf, occ=%lf, time=%lf",rate,occ,time);
//			AKIPrintString(str);
			if( rate>0.0)
			{
				can_turn_green=is_green2(rate, time);
				if(can_turn_green)
				{
					int i;
					for(i=0;i<NUMBER_METERING;i++)
					{
						int new_green_metering=(last_green_metering+1+i)%(NUMBER_METERING);
	//					sprintf(str,"new green %d",new_green_metering);
	//					AKIPrintString(str);
						if(arrival[new_green_metering]==true && show_green_last_step[new_green_metering]==false && time-last_red_time[new_green_metering]>red_green_safety_gap)// && down_presence[new_green_metering]==0)
						{
							//ECIChangeStateMeteringById(meteringID[new_green_metering],1,time,CYCLE,1);
							show_green[new_green_metering]=true;
							can_turn_green=false;
							last_green_metering=new_green_metering;
							last_green=time;
							last_green_time[new_green_metering]=time;
	//						sprintf(str,"last green = %lf",last_green);
	//						AKIPrintString(str);
							arrival[new_green_metering]=false;
	//						sprintf(str,"green %d",new_green_metering);
	//						AKIPrintString(str);
							break;
						}
					}
				}
			}
			else
			{//problem?
				int i;
				for(i=0;i<NUMBER_METERING;i++)
				{
					show_green[i]=true;
					can_turn_green=false;
					last_green=time;
					last_green_time[i]=time;
				}
			}
		}
	}

//	sprintf(str,"show 1 = %d, show 2 = %d",show_green[0],show_green[1]);
//	AKIPrintString(str);

//	sprintf(str,"show 1 = %d, show 2 = %d, show 3 = %d",show_green[0],show_green[1],show_green[2]);
//	AKIPrintString(str);
/*	for(i=0;i<NUMBER_METERING;i++)
		if(time-last_red_time[i]>5)
			show_green[i]=true;*/
	for(i=0;i<NUMBER_METERING;i++)
		if(show_green[i]==true)
		{
			if(ECIGetCurrentStateofMeteringById(meteringID[i],1)==0)
				ECIChangeStateMeteringById(meteringID[i],1,time,CYCLE,1);
		}
		else
		{
			if(ECIGetCurrentStateofMeteringById(meteringID[i],1)==1)
				ECIChangeStateMeteringById(meteringID[i],0,time,CYCLE,1);
		}
	return 0;
}


int metering2HOV(double time)	// for the meterings that can be green at the same time
{
	char str[128];

	int i;
	//see whether there are vehicles waiting to merge
	for(i=0;i<HOV_NUMBER_METERING;i++)
	{
		if(AKIDetGetPresenceCyclebyId(HOV_detectorID[i][1],0)==1)
			HOV_down_presence[i]++;
		else
			HOV_down_presence[i]=0;
		if(HOV_arrival[i]==false)
		{
			if(AKIDetGetPresenceCyclebyId(HOV_detectorID[i][0],0)==1)
				HOV_presence[i]++;
			else
				HOV_presence[i]=0;
//			if(AKIDetGetPresenceCyclebyId(detectorID[i][0],0)>0)
			if(AKIDetGetCounterCyclebyId(HOV_detectorID[i][0],0)>0)
			{
				HOV_arrival[i]=true;
				HOV_presence[i]=0;
			}
			else
				if(HOV_presence[i]>=10 && HOV_down_presence[i]>=5)
				{
					HOV_arrival[i]=true;
					HOV_presence[i]=0;
				}
		}

	sprintf(str,"arrival 0 = %d",HOV_arrival[0]);
//	AKIPrintString(str);
	}


//	sprintf(str,"show 1 = %d, show 2 = %d, show 3 = %d",show_green[0],show_green[1],show_green[2]);
//	AKIPrintString(str);


//now it is green, see whether there the current green light should turn red
	for(i=0;i<HOV_NUMBER_METERING;i++)
	{
		if(ECIGetCurrentStateofMeteringById(HOV_meteringID[i],1)==1)
		{
			HOV_show_green_last_step[i]=true;
			if(AKIDetGetCounterCyclebyId(HOV_detectorID[i][1],0)>0  || time-HOV_last_green_time[i]>max_green)
			{
				HOV_show_green[i]=false;
				HOV_last_red_time[i]=time;
			}
		}
		else
			HOV_show_green_last_step[i]=false;
	}

	//it is red now, so check whether should turn green
	if(time-HOV_last_green>=safety_gap2)
	{
		if(HOV_can_turn_green==true)
		{
			int i;
			for(i=0;i<HOV_NUMBER_METERING;i++)
			{
				int HOV_new_green_metering=(HOV_last_green_metering+1+i)%(HOV_NUMBER_METERING);
//				sprintf(str,"new green %d",new_green_metering);
//				AKIPrintString(str);
				if(HOV_arrival[HOV_new_green_metering]==true && HOV_show_green_last_step[HOV_new_green_metering]==false && time-HOV_last_red_time[HOV_new_green_metering]>red_green_safety_gap)// && down_presence[HOV_new_green_metering]==0)
				{
					//ECIChangeStateMeteringById(meteringID[new_green_metering],1,time,CYCLE,1);
					HOV_show_green[HOV_new_green_metering]=true;
					HOV_can_turn_green=false;
					HOV_last_green_metering=HOV_new_green_metering;
					HOV_last_green=time;
					HOV_last_green_time[HOV_new_green_metering]=time;
//					sprintf(str,"last green = %lf",last_green);
//					AKIPrintString(str);
					HOV_arrival[HOV_new_green_metering]=false;
//					sprintf(str,"green %d",new_green_metering);
//					AKIPrintString(str);
					break;
				}
			}
		}
		else
		{
			double occ=0.0;
			int s;
			for(s=0;s<mainline_detector;s++)
				occ=occ+AKIDetGetTimeOccupedAggregatedbyId(mainline_detectorID[s],0);
			occ=occ/mainline_detector;
			HOV_rate=get_metering_rate(1, occ, time);
			if( HOV_rate>0.0)
			{
				HOV_can_turn_green=is_green2HOV(HOV_rate, time);
				if(HOV_can_turn_green)
				{
					int i;
					for(i=0;i<HOV_NUMBER_METERING;i++)
					{
						int HOV_new_green_metering=(HOV_last_green_metering+1+i)%(HOV_NUMBER_METERING);
	//					sprintf(str,"new green %d",new_green_metering);
	//					AKIPrintString(str);
						if(HOV_arrival[HOV_new_green_metering]==true && HOV_show_green_last_step[HOV_new_green_metering]==false && time-HOV_last_red_time[HOV_new_green_metering]>red_green_safety_gap)// && down_presence[HOV_new_green_metering]==0)
						{
							//ECIChangeStateMeteringById(meteringID[new_green_metering],1,time,CYCLE,1);
							HOV_show_green[HOV_new_green_metering]=true;
							HOV_can_turn_green=false;
							HOV_last_green_metering=HOV_new_green_metering;
							HOV_last_green=time;
							HOV_last_green_time[HOV_new_green_metering]=time;
	//						sprintf(str,"last green = %lf",last_green);
	//						AKIPrintString(str);
							HOV_arrival[HOV_new_green_metering]=false;
	//						sprintf(str,"green %d",new_green_metering);
	//						AKIPrintString(str);
							break;
						}
					}
				}
			}
			else
			{//problem?
				int i;
				for(i=0;i<HOV_NUMBER_METERING;i++)
				{
					HOV_show_green[i]=true;
					HOV_can_turn_green=false;
					HOV_last_green=time;
					HOV_last_green_time[i]=time;
				}
			}
		}
	}

	sprintf(str,"show 1 = %d",HOV_show_green[0]);
//	AKIPrintString(str);

//	sprintf(str,"show 1 = %d, show 2 = %d, show 3 = %d",show_green[0],show_green[1],show_green[2]);
//	AKIPrintString(str);
	for(i=0;i<HOV_NUMBER_METERING;i++)
		if(HOV_show_green[i]==true)
		{
			if(ECIGetCurrentStateofMeteringById(HOV_meteringID[i],1)==0)
				ECIChangeStateMeteringById(HOV_meteringID[i],1,time,CYCLE,1);
		}
		else
		{
			if(ECIGetCurrentStateofMeteringById(HOV_meteringID[i],1)==1)
				ECIChangeStateMeteringById(HOV_meteringID[i],0,time,CYCLE,1);
		}
	return 0;
}

int init_metering(void)
{
	int i;
	CYCLE=AKIGetSimulationStepTime();
	metering_rate_change_time=0.0;
	last_green=0.0;
	//last_red=0.0;
	HOV_last_green=0.0;
	//HOV_last_red=0.0;
	ECIChangeStateMeteringById(meteringID[0],0,0.0,CYCLE,1);
	ECIChangeStateMeteringById(meteringID[1],0,0.0,CYCLE,1);
	ECIChangeStateMeteringById(HOV_meteringID[0],0,0.0,CYCLE,1);
	for (i=0;i<NumOnRamp;i++)
		ECIChangeStateMeteringById(meteringId[i],0,0.0,CYCLE,1);
	meter_rate=900.0;	//initial metering rate
	return 0;
}

int switch_metering(double time,double timeSta)
{
	ECIChangeStateMeteringById(20138,1,time,CYCLE,1);
	ECIChangeStateMeteringById(20139,1,time,CYCLE,1);
	ECIChangeStateMeteringById(70140,1,time,CYCLE,1);
	metering2(timeSta);
	metering2HOV(timeSta);
	return 0;
}
