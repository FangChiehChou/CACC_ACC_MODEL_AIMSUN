#include "mybehavioralModel.h"
#include "AKIProxie.h"
#include "ANGConProxie.h"
#include <stdio.h>
#include <math.h>
#include <map>

#include "myVehicleDef.h"
#include <time.h>       /* time_t, struct tm, time, localtime */




#define MAX(a,b)    (((a)>(b)) ? (a) : (b))
#define MIN(a,b)    (((a)<(b)) ? (a) : (b))

#define SAVE_SECTION 23551

//#define OUTPUT

static int file_init=1;
errno_t err;
#define len_str 256
char str_tmp[len_str]="a";

FILE *OutFile;														// Output file pointer 
FILE *VehStatFile;													// summary output file pointer 			
FILE *StatFile;													    // summary output file pointer 	
FILE *CF_Data;														//add by DW to tracking CF data
FILE *errp;

mybehavioralModel:: mybehavioralModel () : A2BehavioralModel() 
{ 
	int exp_id = ANGConnGetExperimentId();
	const unsigned short *PrintCFDataString = AKIConvertFromAsciiString( 
		"GKExperiment::PrintCFData");
	this->IsPrintCF = ANGConnGetAttributeValueBool( ANGConnGetAttribute( PrintCFDataString ), exp_id );
	if(this->IsPrintCF == true)
	{
		time_t rawtime;
		struct tm * timeinfo;

		time (&rawtime);
		timeinfo = localtime (&rawtime);
		int rep_id = ANGConnGetReplicationId();

		const unsigned short *CACC_PercentString = AKIConvertFromAsciiString( 
			"GKExperiment::CACC_Percent");
		double cacc_percent = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( CACC_PercentString ), exp_id );

		const unsigned short *ACC_PercentString = AKIConvertFromAsciiString( 
			"GKExperiment::ACC_Percent");
		double acc_percent = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( ACC_PercentString ), exp_id );

		//CheckorCreateDirs(rep_id, (int)(acc_percent*100), (int)(cacc_percent*100));
		sprintf_s(str_tmp,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector\\CF_Tracking_%d_%d_%d_%d_%d_ACC%d_CACC%d_Rep%d.txt"
			,(int)(acc_percent*100)
			,(int)(cacc_percent*100)
			,(int)rep_id
			,timeinfo->tm_year
			,timeinfo->tm_mon
			,timeinfo->tm_mday
			,timeinfo->tm_hour
			,timeinfo->tm_min
			,(int)(acc_percent*100)
			,(int)(cacc_percent*100)
			,(int)rep_id);

		/*sprintf_s(str_tmp,len_str,"C:\\CACC_Simu_Data\\CF_Tracking_%d_%d_%d_%d_%d_ACC%d_CACC%d_Rep%d.txt"
			,timeinfo->tm_year
			,timeinfo->tm_mon
			,timeinfo->tm_mday
			,timeinfo->tm_hour
			,timeinfo->tm_min
			,(int)(acc_percent*100)
			,(int)(cacc_percent)
			,(int)rep_id);*/
		
		//creat the file if not exist otherwise erase the old data
		CF_Data = fopen(str_tmp,"w+");
	}
}

mybehavioralModel::~mybehavioralModel () 
{ 
	
}

void mybehavioralModel::removedVehicle( void *handlerVehicle, unsigned short idHandler, A2SimVehicle * a2simVeh )
{

}

//void mybehavioralModel::CheckorCreateDirs(int replication, int acc, int cacc)
//{	
//	createbase(replication, acc, cacc);
//	createreplicate(replication, acc, cacc);
//	createdetector(replication, acc, cacc);
//}
//
//
//void mybehavioralModel::createbase(int replication, int acc, int cacc)
//{
//	char str_tmp1[len_str] = "a"; 
//	sprintf_s(str_tmp1,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u", acc, cacc);
//	if (dirExists(str_tmp1) == false)
//	{
//		_mkdir(str_tmp1);
//	}
//}
//
//void mybehavioralModel::createreplicate(int replication, int acc, int cacc)
//{
//	char str_tmp[len_str] = "a"; 
//	sprintf_s(str_tmp,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u", acc, cacc, replication);
//	if (dirExists(str_tmp) == false)
//	{
//		_mkdir(str_tmp);
//	}
//}
//
//void mybehavioralModel::createdetector(int replication, int acc, int cacc)
//{
//	char str_tmp2[len_str] = "a"; 
//	sprintf_s(str_tmp2,len_str, "C:\\CACC_Simu_Data\\acc%u_cacc%u\\%u\\detector", acc, cacc, replication);
//	if (dirExists(str_tmp2) == false)
//	{
//		_mkdir(str_tmp2);
//	}
//}
//
//bool mybehavioralModel::dirExists(const std::string& dirName_in)
//{
//	DWORD ftyp = GetFileAttributesA(dirName_in.c_str());
//	if (ftyp == INVALID_FILE_ATTRIBUTES)
//		return false;  //something is wrong with your path!
//
//	if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
//		return true;   // this is a directory!
//
//	return false;    // this is not a directory!
//}

void mybehavioralModel::updateVehicle( A2SimVehicle *avehicle ) 
{
	if(avehicle->getId() == 269 && avehicle->getIdCurrentSection() == 23551
		&& avehicle->getPosition(0) > 1600)
		avehicle = avehicle;
	double delta_t = getSimStep(); 
	myVehicleDef *vehicle = (myVehicleDef *)avehicle;
	/*if (vehicle->LetAimsunHandle()==true)
		return;*/
	int veh_Id = vehicle->getId();
	AKIVehSetAsTracked(veh_Id);
	StaticInfVeh veh_info = AKIVehTrackedGetStaticInf(veh_Id);
	AKIVehSetAsNoTracked(veh_Id);
	if (this->IsPrintCF == true)
	{
		PrintCACCFollowMsg(vehicle, (myVehicleDef *)(vehicle->getLeader()), veh_info.headwayMin, 
			veh_info.maxDeceleration, veh_info.maxAcceleration);
	}
	vehicle->UpdateVehicle(delta_t);
} 

double mybehavioralModel::GippsSafetyLowerBound(double lead_speed, double follow_speed, double lead_max_dec, double delta_t,
											  double buffer, double lead_pos, double follow_pos, double lead_len)
{
	double lowspeed =lead_speed + lead_max_dec * delta_t;
    double low_pos = lead_pos + delta_t*(lead_speed + lowspeed)/2;
    low_pos = low_pos - lead_len - buffer;
    double travel_dis = low_pos - follow_pos;
    double speed = travel_dis/delta_t;
    return speed*2 - follow_speed;
}

void mybehavioralModel::PrintCACCFollowMsg(myVehicleDef* vehicle, myVehicleDef* leader,
										   double headwayMin, 
										   double maxDeceleration, double maxAcceleration)
{
	int exp_id = ANGConnGetExperimentId();
	const unsigned short *PrintCFDataStart = AKIConvertFromAsciiString( 
		"GKExperiment::CFDataRecordStart");
	int begin_time = ANGConnGetAttributeValueInt( ANGConnGetAttribute( PrintCFDataStart ), exp_id );
	const unsigned short *PrintCFDataEnd = AKIConvertFromAsciiString( 
		"GKExperiment::CFDataRecordEnd");
	int end_time = ANGConnGetAttributeValueInt( ANGConnGetAttribute( PrintCFDataEnd ), exp_id );

	if(vehicle->getIdCurrentSection() != SAVE_SECTION 
		||vehicle->getPosition()<100
		/*||vehicle->getPosition()>2000*/
		|| ((int)AKIGetCurrentSimulationTime())<begin_time
		|| ((int)AKIGetCurrentSimulationTime())>end_time
		/*|| ((int)AKIGetCurrentSimulationTime())>1000||
		((int)AKIGetCurrentSimulationTime())%!=0*/
		||((int)(AKIGetCurrentSimulationTime()*10))%5!=0)
		return;
	char str[1280];
	int sectionid = vehicle->getIdCurrentSection();		
	A2KSectionInf sectioninfo = AKIInfNetGetSectionANGInf(sectionid);

	if((leader && (leader->getId() > 0) 
		&& 
		leader->getPositionReferenceVeh(vehicle)>0.0 &&
		(leader->getIdCurrentLane() == vehicle->getIdCurrentLane())))
	{
		int time_for_leader = 0;
		if (leader->isUpdated())
		{
			time_for_leader = 1; 
		}
		//format: Time, ID, Type, Section, Lane, position, speed, acc, v_f, min_headway, max_dec, max_acc, 
		//leader_ID, leader_type, leader_speed, leader_accelertion, leader_pos, 
		//        distance_gap, time_gap, 
		sprintf(str,
			"%.5f, %d, %d, %d, %d, %.5f, %.5f, %.5f, %.5f, %.5f, %.5f, %.5f, %d, %d, %.5f, %.5f, %.5f, %.5f, %.5f, %d, %d, %d, %d\n",
			AKIGetCurrentSimulationTime(), vehicle->getId(), vehicle->getVehType(), 
			vehicle->getIdCurrentSection(), vehicle->getIdCurrentLane(), 
			vehicle->getPosition(0), vehicle->getSpeed(),
			(vehicle->getSpeed(0)-vehicle->getSpeed(1))/AKIGetSimulationStepTime(),
			vehicle->getFreeFlowSpeed(), headwayMin, 
			maxDeceleration, maxAcceleration,
			leader->getId(), leader->getVehType(), leader->getSpeed(),
			leader->getAcceleration()>0?leader->getAcceleration():leader->getDeceleration(), 
			leader->getPosition(time_for_leader),leader->getPosition(time_for_leader)-vehicle->getPosition(0),
			(leader->getPosition(time_for_leader)-vehicle->getPosition(0))/vehicle->getSpeed(),
			((myVehicleDef*)vehicle)->ResumeMannual()==true?0:1,
			((myVehicleDef*)leader)->ResumeMannual()==true?0:1,
			((myVehicleDef*)vehicle)->getapplyACC()==false?0:1,
			((myVehicleDef*)leader)->getapplyACC()==false?0:1);
	}
	else
	{
		sprintf(str,
			"%.5f, %d, %d, %d, %d, %.5f, %.5f, %.5f, %.5f, %.5f, %.5f, %.5f, %d, %d, %.5f, %.5f, %.5f, %.5f, %.5f, %d, %d, %d, %d\n",
			AKIGetCurrentSimulationTime(), vehicle->getId(), vehicle->getVehType(), 
			vehicle->getIdCurrentSection(), vehicle->getIdCurrentLane(), 
			vehicle->getPosition(0), vehicle->getSpeed(),
			(vehicle->getSpeed(0)-vehicle->getSpeed(1))/AKIGetSimulationStepTime(),
			vehicle->getFreeFlowSpeed(), headwayMin, 
			maxDeceleration, maxAcceleration,
			0, 0, 0,
			0, 0, 0,0,
			1,
			0,
			((myVehicleDef*)vehicle)->getapplyACC()==false?0:1,
			0);
	}
	fprintf(CF_Data,str);	
};


// Function: readVehTypeData
// The following parameters are only required by Yeo Model not CACC Model
// But we need to set the value for all vehicles if they switch to manual modes
void mybehavioralModel::readVehTypeData( int vehTypeId)
{
    A2BehavioralVehData data;
	const unsigned short *meanJamString = AKIConvertFromAsciiString( 
		"GDrivingSimPluging::GKVehicle::Jam Gap Mean" );
    data.meanJam = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( meanJamString ), vehTypeId );
    
	const unsigned short *devJamString = AKIConvertFromAsciiString( "GDrivingSimPluging::GKVehicle::Jam Gap Deviation");
    data.devJam = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( devJamString ), vehTypeId );

	const unsigned short *maxJamString = AKIConvertFromAsciiString( "GDrivingSimPluging::GKVehicle::Jam Gap Max" );
    data.maxJam = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( maxJamString ), vehTypeId );

	const unsigned short *minJamString = AKIConvertFromAsciiString( "GDrivingSimPluging::GKVehicle::Jam Gap Minimum" );
    data.minJam = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( minJamString ), vehTypeId );

    const unsigned short *meanEString = AKIConvertFromAsciiString( "GDrivingSimPluging::GKVehicle::Mean E" );
    data.meanE = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( meanEString ), vehTypeId );
    
	const unsigned short *devEString = AKIConvertFromAsciiString( "GDrivingSimPluging::GKVehicle::dev E" );
    data.devE = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( devEString ), vehTypeId );

	const unsigned short *maxEString = AKIConvertFromAsciiString( "GDrivingSimPluging::GKVehicle::Max E" );
    data.maxE = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( maxEString ), vehTypeId );

	const unsigned short *minEString = AKIConvertFromAsciiString( "GDrivingSimPluging::GKVehicle::Min E" );
    data.minE = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( minEString ), vehTypeId );
   
	const unsigned short *meanTString = AKIConvertFromAsciiString( "GDrivingSimPluging::GKVehicle::Mean T" );
    data.meanT = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( meanTString ), vehTypeId );
    
	const unsigned short *devTString = AKIConvertFromAsciiString( "GDrivingSimPluging::GKVehicle::dev T" );
    data.devT = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( devTString ), vehTypeId );

	const unsigned short *maxTString = AKIConvertFromAsciiString( "GDrivingSimPluging::GKVehicle::Max T" );
    data.maxT = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( maxTString ), vehTypeId );

	const unsigned short *minTString = AKIConvertFromAsciiString( "GDrivingSimPluging::GKVehicle::Min T" );
    data.minT = ANGConnGetAttributeValueDouble( ANGConnGetAttribute( minTString ), vehTypeId );

	//const unsigned short *distConflictString = AKIConvertFromAsciiString( "GKVehicle::distConflict" );
 //   data.distConflict = ANGConnGetAttributeValueInt( ANGConnGetAttribute( distConflictString ), vehTypeId );

	//set maximum give way time as maximum
	const unsigned short *giveWayDev = AKIConvertFromAsciiString( "GKVehicle::giveWayDev" );
	ANGConnSetAttributeValueDouble( ANGConnGetAttribute( giveWayDev ), vehTypeId,0.0);
	const unsigned short *giveWayMax = AKIConvertFromAsciiString( "GKVehicle::giveWayMax" );
	ANGConnSetAttributeValueDouble( ANGConnGetAttribute( giveWayMax ), vehTypeId,60000000.0);
	const unsigned short *giveWayMin = AKIConvertFromAsciiString( "GKVehicle::giveWayMin" );
	ANGConnSetAttributeValueDouble( ANGConnGetAttribute( giveWayMin ), vehTypeId,60000000.0);
	const unsigned short *giveWayMean = AKIConvertFromAsciiString( "GKVehicle::giveWayMean" );
	ANGConnSetAttributeValueDouble( ANGConnGetAttribute( giveWayMean ), vehTypeId,60000000.0);

	//setup reaction time
	const unsigned short *min_reaction_String = 
		AKIConvertFromAsciiString( "reaction_time_min_" );
	data.min_reaction_time = 
		ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute( min_reaction_String ), vehTypeId );
	const unsigned short *max_reaction_String = 
		AKIConvertFromAsciiString( "reaction_time_max_" );
	data.max_reaction_time = 
		ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute( max_reaction_String ), vehTypeId );
	const unsigned short *dev_reaction_String = 
		AKIConvertFromAsciiString( "reaction_time_dev_");
	data.dev_reaction_time = 
		ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute( dev_reaction_String ), vehTypeId );
	const unsigned short *avg_reaction_String = 
		AKIConvertFromAsciiString( "reaction_time_avg_" );
	data.avg_reaction_time = 
		ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute( avg_reaction_String ), vehTypeId );

	//minimum time between lane changes
	const unsigned short *min_time_lcs_str = 
		AKIConvertFromAsciiString( "min_time_between_lc_");
	data.min_time_between_lc_ = 
		ANGConnGetAttributeValueDouble(
		ANGConnGetAttribute( min_time_lcs_str ), vehTypeId );
	data.min_time_between_lc_=
		MAX(1, data.min_time_between_lc_);

	//politeness to lane changers
	const unsigned short *politeness_str = 
		AKIConvertFromAsciiString( "polite_");
	data.politeness_ = 
		ANGConnGetAttributeValueDouble(
		ANGConnGetAttribute( politeness_str ), vehTypeId );
	data.politeness_=
		MIN(MAX(0, data.politeness_),1);

    vehTypeData[vehTypeId] = data;
}

// Function: arrivalNewVehicle
// Routine for processing a new , generate a new instance and assign  characteristics
//

A2SimVehicle *mybehavioralModel::
	arrivalNewVehicle( void *handlerVehicle,unsigned short idhandler, bool isFictitiousVeh)
{

    myVehicleDef  * res = new myVehicleDef(handlerVehicle, idhandler,isFictitiousVeh );

    if (!isFictitiousVeh) {
        std::map<int, A2BehavioralVehData>::const_iterator iter = vehTypeData.find( res->getVehType() );
        double jamGap, E, T, devT, devE, minE, minT;

        if( iter == vehTypeData.end() ){
            readVehTypeData( res->getVehType() );
            iter = vehTypeData.find( res->getVehType());
        }

        jamGap = sampleNormalDist((*iter).second.meanJam, (*iter).second.devJam);
        jamGap = std::_cpp_min((*iter).second.maxJam, std::_cpp_max(jamGap, (*iter).second.minJam));
        res->setJamGap(jamGap);

		res->setIsFromMCF(0);

		res->setMode(0);

		//sampling reaction time
		double reaction_time = sampleNormalDist(
			(*iter).second.avg_reaction_time,
			(*iter).second.dev_reaction_time
			);
		reaction_time = std::_cpp_min((*iter).second.max_reaction_time, 
			std::_cpp_max(reaction_time, (*iter).second.min_reaction_time));
		res->setReactionTime(reaction_time);


		res->ModelApplied = ReadModel();
		//**********************************************
		//parameters related to NGSIM model
		int exp_id = ANGConnGetExperimentId();

		const unsigned short *gipps_theta_str = 
			AKIConvertFromAsciiString( "gipps_theta");
		res->setGippsTheta(ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute(gipps_theta_str), exp_id));
		
		const unsigned short *leader_max_dec_est_coef_str = 
			AKIConvertFromAsciiString( "leader_max_dec_est_coef");
		res->setEstimateLeaderDecCoeff(ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute(leader_max_dec_est_coef_str), exp_id));
		
		const unsigned short *acc_smooth_factor_str = 
			AKIConvertFromAsciiString( "acc_smooth_factor");
		res->setAccSmoothCoef(ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute(acc_smooth_factor_str), exp_id));

		//**********************************************
		//parameters related to lane change
		if (res->ModelApplied.compare("IDM") == 0)
		{
			res->alpha = 0.5;
			res->beta = 0.6;
			//steps depending on the delta t
			res->ACF_Steps = 10;
			res->ACF_Step = 0;
			res->Relaxation = 1;
		}
		else
		{
			res->alpha = 0.5;
			res->beta = 0.6;
			//steps depending on the delta t
			res->ACF_Steps = 10;
			res->ACF_Step = 0;
			res->Relaxation = 1;
		}

		E = sampleNormalDist((*iter).second.meanE, (*iter).second.devE);
		E = std::_cpp_min((*iter).second.maxE, std::_cpp_max(E, (*iter).second.minE));
		res->setE(E);

		T = sampleNormalDist((*iter).second.meanT, (*iter).second.devT);
		T = std::_cpp_min((*iter).second.maxT, std::_cpp_max(T, (*iter).second.minT));
		res->setT(T);

		res->setMinTimeBtwLcs((*iter).second.min_time_between_lc_);

		res->setPoliteness((*iter).second.politeness_);
		res->setRandomPoliteness(AKIGetRandomNumber());

		res->setGapAcceptanceModel(ReadGapModel(exp_id));
		//look ahead parameters of LC
		const unsigned short *dis_lookahead_str = 
			AKIConvertFromAsciiString( "look_ahead_dis");
		res->setDLCScanRange(ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute(dis_lookahead_str), exp_id));
		const unsigned short *dis_lookahead_cars_str = 
			AKIConvertFromAsciiString( "look_ahead_cars");
		res->setDLCScanNoCars(ANGConnGetAttributeValueInt(
			ANGConnGetAttribute(dis_lookahead_cars_str), exp_id));

		//lane change desires
		const unsigned short *lane_change_desire_thrd_str = 
			AKIConvertFromAsciiString( "lane_change_desire_thrd");
		double avg_lc_desire = ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute(lane_change_desire_thrd_str), exp_id);
		
		const unsigned short *lane_change_desire_thrd_dev_str = 
			AKIConvertFromAsciiString( "lane_change_desire_thrd_dev");
		double lc_desire_dev = ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute(lane_change_desire_thrd_dev_str), exp_id);
		
		const unsigned short *lane_change_desire_thrd_min_str = 
			AKIConvertFromAsciiString( "lane_change_desire_thrd_min");
		double lc_desire_min = ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute(lane_change_desire_thrd_min_str), exp_id);

		const unsigned short *lane_change_desire_thrd_max_str = 
			AKIConvertFromAsciiString( "lane_change_desire_thrd_max");
		double lc_desire_max = ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute(lane_change_desire_thrd_max_str), exp_id);

		double lc_desire = sampleNormalDist
			(avg_lc_desire, lc_desire_dev);
		lc_desire = 
			std::_cpp_min(lc_desire_max, 
			std::_cpp_max(lc_desire, lc_desire_min));
		res->setLaneChangeDesireThrd(lc_desire);

		const unsigned short *dlc_coeff = 
			AKIConvertFromAsciiString( "dlc_coeff");
		res->setDLCWeight(ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute(dlc_coeff), exp_id));

		const unsigned short *DLC_forbid_zone_before_exit = 
			AKIConvertFromAsciiString( "DLC_forbid_zone_before_exit");
		res->setDLCForbidZoneBeforeExit(ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute(DLC_forbid_zone_before_exit), exp_id));

		const unsigned short *right_DLC_coef = 
			AKIConvertFromAsciiString( "right_DLC_coef");
		res->setRightDLCCoeff(ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute(right_DLC_coef), exp_id));

		const unsigned short *lc_gap_reduction_factor_str = 
			AKIConvertFromAsciiString( "lc_gap_reduction_factor");
		res->setLCGapReductionFactor(ANGConnGetAttributeValueDouble(
			ANGConnGetAttribute(lc_gap_reduction_factor_str), exp_id));

		const unsigned short *sequential_merging_str = 
			AKIConvertFromAsciiString( "sequential_merging");
		res->SetUnsequentialMerging((ANGConnGetAttributeValueInt(
			ANGConnGetAttribute(sequential_merging_str), exp_id)>0?true:false));

		//**********************************************

		res->delta = 4;
		res->setSourceSection(res->getIdCurrentSection());
		res->setVehID(res->getId());

		const unsigned short *debug_track_id_str = 
			AKIConvertFromAsciiString( "debug_track_veh_id");
		res->setDebugTrackID(ANGConnGetAttributeValueInt(
			ANGConnGetAttribute(debug_track_id_str), exp_id));
    }

    return res;
}


std::string mybehavioralModel::ReadModel()
{
	int scenario_id = ANGConnGetScenarioId();
	const unsigned short *Model = AKIConvertFromAsciiString( 
		"GKScenario::ModelCode");
	int modelcode= 
		ANGConnGetAttributeValueInt(ANGConnGetAttribute( Model ), scenario_id);
	switch(modelcode)
	{
		default:
			return "IDM";
		case 0:
			return "NGSIM";
		case 2:
			return "SNGSIM";
	}

}


//----------------------------------------------------------------------------------------------------------
//			Miscellaneous functions
//----------------------------------------------------------------------------------------------------------

// Function: sampleUniformDist
// Returns uniform random number between 0 and 1
//

double mybehavioralModel::sampleUniformDist(void)
{
    return AKIGetRandomNumber();
}

// Function :sampleNormalDist
// Generate a normal(mean, std) random number according to the polar method
//
double mybehavioralModel::sampleNormalDist(double mean, double std)
{
	double W=0.0;
	double V1=0.0;
	double V2=0.0;
	double X=0.0;
	double Y=0.0;
	double val=0.0;
	double U1=0.0;
	double U2=0.0;

    do{
		U1=AKIGetRandomNumber();
		U2=AKIGetRandomNumber();
		V1=2*U1-1;
		V2=2*U2-1;
		W=V1*V1+V2*V2;
    }while (W>=1);

	Y=sqrt(MAX(0.0,-2*log(W)/MAX(1.0,W)));
	X=Y*V1;
	val = mean+X*std;

    return val;
}

int mybehavioralModel::ReadGapModel(int exp_id)
{
	const unsigned short *gap_model_code = AKIConvertFromAsciiString( 
		"gap_model");
	return 
		ANGConnGetAttributeValueInt(ANGConnGetAttribute(gap_model_code), exp_id);

}


