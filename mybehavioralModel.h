//-*-Mode: C++;-*-
#ifndef _mybehavioralModel_h_
#define _mybehavioralModel_h_

#include "A2BehavioralModelUtil.h"
#include "A2BehavioralModel.h"
#include "myVehicleDef.h"
#include <stdio.h>
#include <map>

class A2BehavioralVehData
{
public:
	A2BehavioralVehData(){};
	~A2BehavioralVehData(){};

	//Jam corresponds to jam gap
	double  minJam;
	double maxJam; 
	double meanJam; 
	double devJam;
	//T corresponds to the slope parameters T_n in Eq. (18)
	double  minT;
	double maxT;
	double meanT; 
	double devT;
	//E corresponds to the target distance E in Eq. (18)
	double  minE;
	double maxE; 
	double meanE;
	double devE;
	double distConflict; // distance to conflict point (e.g. end of ramp) at which driver starts trying to change lanes
	bool Asym;

	double min_reaction_time;
	double max_reaction_time;
	double avg_reaction_time;
	double dev_reaction_time;
	double min_time_between_lc_;
	double politeness_;
};


class A2BEHAVIORALEXPORT mybehavioralModel: public A2BehavioralModel{
public:
	mybehavioralModel ();
	~ mybehavioralModel ();

	virtual void updateVehicle( A2SimVehicle *vehicle );
	
	virtual A2SimVehicle * arrivalNewVehicle( void *handlerVehicle, unsigned short idHandler, bool isFictitiousVeh); 

	virtual void removedVehicle( void *handlerVehicle, unsigned short idHandler, A2SimVehicle * a2simVeh );

	double GippsSafetyLowerBound(double lead_speed, double follow_speed, double lead_max_dec, double delta_t,
											  double buffer, double lead_pos, double follow_pos, double lead_len);

	
	bool LetAimsunHandle(A2SimVehicle *avehicle, double buffer, double aimsun_activate_speed);
	bool ApplyNGSIMModel(A2SimVehicle *avehicle, double buffer, double aimsun_activate_speed);
	void PrintCACCFollowMsg(myVehicleDef* vehicle, myVehicleDef* leader,
		double headwayMin, 
		double maxDeceleration, double maxAcceleration);

	void readVehTypeData( int vehTypeId );
	static double sampleNormalDist(double mean, double dev);
	static double sampleUniformDist();

	std::map<int, A2BehavioralVehData> vehTypeData;

	std::string ReadModel();
	int ReadGapModel(int exp_id);
	bool IsPrintCF;

	/*void CheckorCreateDirs(int replication, int acc, int cacc);

	void createbase(int replication, int acc, int cacc);

	void createreplicate(int replication, int acc, int cacc);

	void createdetector(int replication, int acc, int cacc);

    bool dirExists(const std::string& dirName_in);*/
};


#endif
