//-*-Mode: C++;-*-
#ifndef _myVehicleDef_h_
#define _myVehicleDef_h_

#include "A2BehavioralModelUtil.h"
#include "A2BehavioralModel.h"
#include "A2SimVehicle.h"
#include "akiproxie.h"
#include "behaviorParameters.h"
#include <stdio.h>
#include <deque>
#include <hash_map>

//#define USE_CACC   2 // 1: using DYSu algorithm; 2: use Vicente model


//#define CARveh  53
//#define HOVveh  17859      
//#define HIAveh  21350  // 811
#define ACCveh  23053  // 812
#define CACCveh 23057  // 813

#define A_lim		1.0
#define D_lim		2.0
#define DEBUG_TRACK_ID 0
//#define Max_ACC_TGap 2.0
//#define Min_ACC_TGap 1.0
//#define Max_CACC_TGap 0.6
//#define Min_CACC_TGap 1.1


class A2BEHAVIORALEXPORT myVehicleDef: public A2SimVehicle
{
private:
	bool applyACC_CACC; //determines whether follows an Automatic Cruise Control 

	//*********************************************************
	//the following private variables are defined for Yeo's model
	int state;
	int mode;
	double jam_gap;
	double E;
	double T;
	double distConflict;
	int target_lane;
	int RMode;  // Relaxation Mode 0: No relaxation, 1: relaxation 2: relaxation transition
	int CoopTime;
	int LCTime;
	int LCType;  //Lane changing type 1= Exit or turning, LC, 2= ELC, 3=DLC
	double xExit;
	int nLC;
	bool CoopRequest;
	int CoopLane;
	int idDEBUG;
	//dongyan add, to avoid MCF and BCF loop
	int isFromMCF;	//if 0, means that it is not from MCF mode, if 1, it from MCF mode
	//jhsuh add
	int VehID;
	int MCFtoBCF;
	int numupdate;
	double dLC_forbidzone_before_exit_;
	double desireLC;
	double reaction_time_;
	double desireLC_force_left;
	double desireLC_force_right;
	double desireLC_option_left;
	double desireLC_option_right;
	double mandatory_LC_weight;
	double discretionary_LC_weight;
	double min_time_btw_lcs;
	double last_lc_time;
	double politeness_;
	double random_politeness_;
	double dLC_scan_range_;
	int dLC_scan_no_cars_;
	int source_section;


	//****************************

public:
	myVehicleDef ( void *handlerVehicle, unsigned short idhandler, bool isFictitiousVeh );
	~myVehicleDef ();

	double lane_change_prob;
	bool getapplyACC();
	const A2SimVehicle* leader ;
	InfVeh info;
	StaticInfVeh staticinfo;
	InfVeh leader_info;
	const A2SimVehicle* getLeader();
	void RunNGSIM();
	void RunACCCACC();
	void UpdateVehicle(double simu_step);
	bool ApplyNGSIMModel() ;
	bool has_leader;
	double getMAXacc();
	double getMAXdec();

	double BaseCfModel(double a_L, double a_U, 
		double tau, double headway, double jamGap, double d_leader, 
		double l_leader, double vf, double v, double x, double x_leader, 
		double x_leader_steps_early,
		double lead_v, double min_headway);

	double BaseCfModel (double a_L, double a_U, 
		double tau, double headway, double jamGap, double d_leader, 
		double l_leader, double vf, double v, double x, 
		double x_leader, double x_leader_steps_early, 
		double lead_v, double min_headway, 
		double &target_pos );

	int gapAccepted(const A2SimVehicle *vehUp, const A2SimVehicle *vehDown, 
		double desire);
	int GapAcceptDecision_Sync_First();
	double RouteDesireLC();

	//*********the following functions and variables are defined for Yeo's model
	
	int getMode(){return mode;};
	int setMode(int avalue){mode = avalue;return mode;};
	myVehicleDef* CoopRequester;

	double IX;				// Initial Location of vehicle
	double IT;				// Initial time of  vehicle
	double FX;				// Final Location of vehicle >> 4,000m 
	double FT;				// Final time of vehicle >> 4,000m  

	double CoopDist;
	int CoopRequesterID;
	void setCodist(double codist){CoopDist=codist;}     
	double getCodist(){return CoopDist;}
	void setVehID(int aVehID){VehID = aVehID;}
	double getJamGap(){return jam_gap;};
	void setJamGap(double avalue){jam_gap = avalue;};

	double getE(){return E;};
	void setE(double avalue){E = avalue;};

	double getT(){return T;};
	void setT(double avalue){T = avalue;};

	int getnLC(){return nLC;};
	void setnLC(int value){nLC=value;};

	double getxExit(){return xExit;};
	void setxExit(double avalue){xExit = avalue;};

	int getCoopTime(){return CoopTime;};
	void setCoopTime(int avalue){CoopTime=avalue;}

	int getLCTime(){return LCTime;};
	void setLCTime(int avalue){LCTime=avalue;};

	double getDistConflict(){return distConflict;};
	void setDistConflict(double avalue){distConflict = avalue;};

	int getLCType(){return LCType;};
	void setLCType(int avalue){ LCType= avalue;};

	int getRMode(){return RMode;};
	void setRMode(int avalue){RMode = avalue;};

	int getTargetLane(){return target_lane;};
	void setTargetLane(int avalue){target_lane = avalue;};

	int getCoopLane(){return CoopLane;};
	void setCoop(int colane,double codist){CoopRequest=true; CoopLane=colane;CoopDist=codist;};
	void ReleaseCoop(){CoopRequest=false;CoopLane=0;CoopRequester=0;CoopDist=0;};
	void setCoopRequesterID(int covehID){CoopRequesterID=covehID;}
	int getCoopRequesterID(){return CoopRequesterID;}
	bool setIsFromMCF(int value){isFromMCF=value;return true;};
	int getIsFromMCF(){return isFromMCF;};
	void setMCFtoBCF(int value){MCFtoBCF=value;};
	int getMCFtoBCF(){return MCFtoBCF;};
	double PosCfSkipGap(const A2SimVehicle* leader);
	double PosCfSkipGap(const A2SimVehicle* potential_leader, bool apply_creep_speed);
	void cntract(double sample_t, double mag_rate, double e_rate, double a_val, double b_val, 
				double *int_val, double *int_val_d);
	double SIGN1(double x);
	double PosCf(const A2SimVehicle* leader, int shortGap, double beta, double alpha, double relaxation);
	
	bool NeedLC();
	bool NeedDlc( );
	bool NeedRampLc();
	bool NeedLcExit();
	bool NeedLc4Turning();
	int NeedMCF();
	bool NeedCoop();
	int GetConflictLane(myVehicleDef *vehicle);
	void updateVehicle( A2SimVehicle *avehicle );
	void readVehTypeData( int vehTypeId );
	A2SimVehicle *arrivalNewVehicle( void *handlerVehicle,unsigned short idhandler, bool isFictitiousVeh);
	void removedVehicle(void *handlerVehicle, unsigned short idHandler, A2SimVehicle * a2simVeh );
	
	
	double getSpeed();
	double getPosition();
	void setNewPosition(double pos, double velocity);
	double getReactionTime(){
		return this->reaction_time_;
	};
	void setReactionTime(double val);

	double getPosition(int state);
	double getSpeed(int state);
	void Report();
	int getStopTime();
	bool NeedELC();

	double delta_t;
	void X_ECF(myVehicleDef* LCleader );
	double getELCdist();

	double cycle,left,right;
	int seed;
	bool Asym;
	double alpha;
	double beta;
	bool ShortgapMode;
	double MaxSpeedDifference;
	myVehicleDef *lvehicle;

	int ACF_Steps, ACF_Step;
	double Relaxation;
	std::string ModelApplied;

	bool LetAimsunHandle();

	bool ResumeMannual();
	bool ResumeAutomatic();

	//IDM parameters
	double delta;
	void RunIDM();
	double Safe_Speed();

	bool Turn2Mannual;

	double getFreeFlowSpeed();
	int determineDrivingMode();
	int DetermineLcOrMergeOrCoop();
	int Determine2ndLcAfterLc();
	int determineCoopOrLc();
	int DetermineReceiveOrLc();
	void UpdateMergeCf();
	void UpdateBeforeLaneChangeCf();
	void UpdateEmergentCf();
	void UpdateDuingLaneChangeCf();
	void UpdateAfterLaneChangeCf();
	void updateCoopCf();
	void UpdateReceiveCf();
	bool isAfterLaneChangeFinish();
	bool NeedReceive();
	int determineGapOrGiveup();
	void observeCoopSignal();
	std::deque<double> position_queue;
	stdext::hash_map<int, myVehicleDef*> cooperRequestList;
	double right_dlc_coeff_;
	double estimated_leader_dec_coeff_;
	double acc_smooth_coeff_;
	int last_lc_target;
	double lc_gap_reduction_factor;
	int gap_model;
	int debug_track_id;
	double gipps_theta;
	bool allow_unseqential_merging;
	int mandatory_type;
	double off_ramp_e;
	double off_ramp_t;
	double penalty_dlc_no_exitlane;
	double b_gap_reduction_factor_offramp;
	double b_gap_reduction_factor_onramp;
	double f_gap_reduction_factor_onramp;
	double f_gap_reduction_factor_offramp;
	double increase_DLC_close_ramp;
	bool new_need_adjust;
	bool first_cycle_after_adjust;
	double desire_headway;
	void BeforeOnRampLcSlowDown();
	void BeforeOnRampLcSync();
	double PosCf2EndofRamp();
	double posEndAccLane();
	void updateRegularCf();
	int RampCfDecision();
	double getLaneChangeDesire();
	double distance2EndAccLane();
	void UpdateLc();
	bool Willing2Coop(myVehicleDef *coop_veh);
	int ExitCfDecision();
	void setLaneChangeDesire(double incentive);
	void setLaneChangeDesireForce(double incentive_left, double incentive_right);
	void setLaneChangeDesireOption(double incentive_left, double incentive_right);
	bool CombineLCDesires();
	void ResetDesires();
	double CalculateDesireForce(int n_lc, double d_exit, double speed
								,bool is_for_on_ramp);
	void setMinTimeBtwLcs(double val){this->min_time_btw_lcs = 
		(val>1?val:1);};
	double getMinTimeBtwLcs(){return min_time_btw_lcs;};
	double getLastLCTime(){return this->last_lc_time<=0?0:this->last_lc_time;};
	void setLastLCTime(double val){this->last_lc_time = val;};
	void SetInitialVal();
	//override the default to add the lane change time
	void applyLaneChanging
		(const A2SimVehicle *vehDown, 
		const int targetLane, const double newpos, 
		const double newspeed);
	void setPoliteness(double val){this->politeness_ = val;};
	double getPoliteness(){return politeness_;}
	void setRandomPoliteness(double val){this->random_politeness_ = val;};
	double getRandomPoliteness(){return random_politeness_;};
	double NewtonianPos(double v, double a_f, double x);
	double GetPastPos(double reaction_time);
	//get past reference position
	double getPastPositionReferenceVehs
		(double reaction_time_ref, myVehicleDef* ref_veh, 
		 double reaction_time_this);
	double getPositionReferenceVeh //override the existing function
		(myVehicleDef* ref_veh);
	double GetPositionRelativeFake
		(myVehicleDef* fake_veh, double reaction_time_fake, 
		bool downstream);
	double AnticipatedMinGap(double acc_self,
		double Gap_AC_Thrd, double desire, 
		double up_speed,
		double headway);
	bool IsCoopEffectMuch(myVehicleDef * coop_veh);
	int DLCCFDecision();
	void BeforeDLcSlowDown();
	void BeforeDLcSync();

	double getDLCForbidZoneBeforeExit(){return dLC_forbidzone_before_exit_;};
	void setDLCForbidZoneBeforeExit(double val){dLC_forbidzone_before_exit_=val;};
	
	double getDLCScanRange(){return dLC_scan_range_;};
	void setDLCScanRange(double val){dLC_scan_range_=val;};
	int getDLCScanNoCars(){return dLC_scan_no_cars_;};
	void setDLCScanNoCars(int val){dLC_scan_no_cars_=val;};
	void setDLCWeight(double val){this->discretionary_LC_weight=val;
						this->mandatory_LC_weight = 1.0-val;};
	double getRightDLCCoeff(){return this->right_dlc_coeff_;};
	void setRightDLCCoeff(double val);
	void setLaneChangeDesireThrd(double val);
	double getEstimateLeaderDecCoeff(){return this->estimated_leader_dec_coeff_;};
	void setEstimateLeaderDecCoeff(double val)
				{this->estimated_leader_dec_coeff_=val;};
	double getAccSmoothCoef(){return this->acc_smooth_coeff_;};
	void setAccSmoothCoef(double val)
				{this->acc_smooth_coeff_=val;};
	void setLastLCTarget(const int targetLane)
				{this->last_lc_target = targetLane;};
	int getLastLCTarget()
				{return (this->last_lc_target==LEFT||this->last_lc_target==RIGHT)?
						 this->last_lc_target:NOCHANGE;};
	void setSourceSection(int val)
						{this->source_section = val;};
	int getSourceSection()
						{return this->source_section;};
	void setLCGapReductionFactor(double val){this->lc_gap_reduction_factor=
		((val>0&&val<=1)?val:1);};
	double getLCGapReductionFactor(){return this->lc_gap_reduction_factor;};
	int getGapAcceptanceModel(){return this->gap_model;};
	void setGapAcceptanceModel(int val){this->gap_model=val;};
	bool DisGapAccepted(double a_L, double a_U, double tau, 
		double headway, double jamGap, 
		double d_leader, 
		double l_leader, double vf, double v, 
		double x, 
		double x_leader, 
		double x_leader_steps_early,
		double lead_v, double min_headway,
		double Gap_AC_Thrd,
		double desire, 
		bool on_ramp, bool forward);
	bool AccGapAccepted(double a_L, 
		double a_U, double tau, double headway, 
		double jamGap, double d_leader, double l_leader, double vf, 
		double v, double x, double x_leader, double x_leader_steps_early, 
		double lead_v, double min_headway, double Gap_AC_Thrd, 
		double desire);
	int getDebugTrackID(){return this->debug_track_id;};
	void setDebugTrackID(int val){this->debug_track_id = val;};
	double getGippsTheta(){return this->gipps_theta;};
	void setGippsTheta(double val){this->gipps_theta=val;};
	bool AllowUnsequentialMerging(){return allow_unseqential_merging;};
	void SetUnsequentialMerging(bool val){allow_unseqential_merging=val;};
	double GippsDecelerationTerm(
		double maxDec,double reaction_time,double theta,
		double x_leader,double x,double jamGap,
		double l_leader,double v,double lead_v,double b_estimate);
	bool GippsGap(double maxDec,double reaction_time,
		double theta, double x_leader,double x,
		double jamGap, double l_leader, double v,
		double lead_v,double b_estimate);
	bool GippsGap(double maxDec,double reaction_time,double theta, double x_leader,double x,double jamGap, double l_leader, double v,double lead_v,double b_estimate, bool on_ramp, bool forward);
	bool HwasooGap(double maxDec,double reaction_time,double theta, 
		double x_leader,double x,double jamGap, double l_leader, double v,
		double lead_v,double b_estimate);
	double getLaneChangeDesireThrd(){return lane_change_prob;}
	void setMandatoryType(int type){this->mandatory_type = type;};
	int getMandatoryType(){return this->mandatory_type;}
	void BeforeExitorTurningLcSync();
	void BeforeExitorTurningLcSlowDown();
	double PosCf2EndofExitTurning();
	void setOffRampE(double val){this->off_ramp_e = val;};
	double getOffRampE(){return this->off_ramp_e;};
	void setOffRampT(double val){this->off_ramp_t = val;};
	double getOffRampT(){return this->off_ramp_t;};
	double GetAdditionalDlcDesire(int target_lane);
	double Bound_Function(double param1);
	double getPenaltyDLCNoExitLane(){return penalty_dlc_no_exitlane;};
	void setPenaltyDLCNoExitLane(double val){this->penalty_dlc_no_exitlane=val;};
	void setForwardGapReductionOnRamp(double val)
	{this->f_gap_reduction_factor_onramp = ((val<=0||val>1)?1:val);};
	void setBackwardGapReductionOnRamp(double val)
		{this->b_gap_reduction_factor_onramp = ((val<=0||val>1)?1:val);};
	void setForwardGapReductionOffRamp(double val)
		{this->f_gap_reduction_factor_offramp = ((val<=0||val>1)?1:val);};
	void setBackwardGapReductionOffRamp(double val)
		{this->b_gap_reduction_factor_offramp = ((val<=0||val>1)?1:val);};
	double getForwardGapReductionOnRamp()
		{return this->f_gap_reduction_factor_onramp;};
	double getBackwardGapReductionOnRamp()
		{return this->b_gap_reduction_factor_onramp;};
	double getForwardGapReductionOffRamp()
		{return this->f_gap_reduction_factor_offramp;};
	double getBackwardGapReductionOffRamp()
		{return this->b_gap_reduction_factor_offramp;};
	double OnRampAddCoef(double ahead_speed, int num_lane_2_rightmost);
	double getIncreaseDLCCloseRamp();
	void setIncreaseDLCCloseRamp(double val);
	int GetRampType(int sec_id);
	int GetOnRampFlow(int next_sec, double *ramp_length);
	int GetOnAccLaneFlow(int next_sec);
	double GetEquPosition(double leader_pos, double leader_l);
	void setNewArrivalAdjust(bool needadjust){this->new_need_adjust = needadjust;};
	bool getNewArrivalAdjust(){return this->new_need_adjust;};
	void AjustArrivalVehicle();
	void setFirstCycleAfterAdjust(bool val)
			{first_cycle_after_adjust = val;};
	bool getFirstCycleAfterAdjust(){return first_cycle_after_adjust;};
	void setHeadwayTime(double headway_time)
		{this->desire_headway = headway_time;};
	double getDesireHeadway()
		{return this->desire_headway>0.5?this->desire_headway:0.5;};
};
#endif
