//DO NOT MODIFY THIS FILE
#ifndef _AKIProxie_h_
#define _AKIProxie_h_
//SDK
#include "AAPI_Util.h"

extern "C" {

	// .......................................
	// 		ACTIONS MANAGEMENT 
	// .......................................

	/*! newSpeed expressed in the network units, either Km/h or Mph */
	AAPIEXPORT void * AKIActionAddSpeedAction(int sectionId, double newSpeed, int VehType, double acomplianceLevel);
	/*! newSpeed expressed in the network units, either Km/h or Mph. laneId will be -1 for all lanes and between 1 and number of lanes
		for a specific lane, segmentId will be -1 for all segments and between 1 and number of segments for a specific segment */
	AAPIEXPORT void * AKIActionAddDetailedSpeedAction(int ang_sectionId, int laneId, int ang_segmentId, double newSpeed, int VehType, double acomplianceLevel);

	AAPIEXPORT void * AKIActionCloseLaneAction(int sectionId, int alane, int VehType);
	AAPIEXPORT void * AKIActionCloseLaneActionBySegment(int sectionId, int alane, int ang_segmentFromId, int ang_segmentToId, int VehType, bool apply2LanesCF, double visibilityDistance);
	AAPIEXPORT void * AKIActionCloseLaneDetailedAction(int sectionId, int alane, int VehType, bool apply2LanesCF, double visibilityDistance );
	AAPIEXPORT void * AKIActionAddNextTurningODAction(int sectionId, int anextSection, int aOrigin, int aDest, int VehType, int asectionInPath, double acomplianceLevel, double visibilityDistance);
	AAPIEXPORT void * AKIActionAddNextTurningResultAction(int sectionId, int aoldNextSection, int anewNextSection, int VehType, double acomplianceLevel);
	AAPIEXPORT void * AKIActionAddChangeDestAction(int sectionId, int anewDest, int aOrigin, int aDest, int VehType, double acomplianceLevel);
	AAPIEXPORT void * AKIActionChangeTurningProbAction(int sectionId, int nbnewProb, int *aNextSection, double *anewProb, int VehType);
	AAPIEXPORT void * AKIActionCongestionPricingODAction(int ang_sectionId, int ang_anewNextSection, int aOrigin, int aDest, int VehType, double acomplianceLevel, double avisibilityDistance, bool IsIn, bool isTolled, int anextSectionComplimentary);


	AAPIEXPORT void * AKIActionDisableReservedLaneAction(int sectionId, int alane, int ang_segmentId);
	AAPIEXPORT void AKIActionRemoveAction(void *a2kaction);
	AAPIEXPORT void AKIActionReset();

	AAPIEXPORT void * AKIActionAddNextSubPathODAction(int ang_sectionId, int nbNextSections, int * anextSections, int aOrigin, int aDest, int VehType, int asectionInPath, double acomplianceLevel, double visibilityDistance);
	AAPIEXPORT void * AKIActionAddNextSubPathResultAction(int ang_sectionId, int nbNextSections, int * anextSections, int VehType, double acomplianceLevel);
	AAPIEXPORT void * AKIActionAddNextSubPathPTAction(int ang_sectionId, int nbSections, int * aSections, int idline, int VehType, double acomplianceLevel);
	AAPIEXPORT void * AKIActionAddCloseTurningODAction(int sectionId, int anewSection2Close, int aOrigin, int aDest, int VehType, double acomplianceLevel, double visibilityDistance = 200.0, bool localEffect = true, bool isDUE = false);

	AAPIEXPORT void AKIActionModifyNextTurningODAction( void * a2kaction, double acomplianceLevel);
	AAPIEXPORT void AKIActionModifyNextTurningResultAction( void * a2kaction, double acomplianceLevel);
	AAPIEXPORT void AKIActionModifyChangeDestAction( void * a2kaction, double acomplianceLevel);
	AAPIEXPORT void AKIActionModifyNextSubPathResultAction( void * a2kaction, double acomplianceLevel);
	AAPIEXPORT void AKIActionModifyNextSubPathODAction( void * a2kaction, double acomplianceLevel);
	AAPIEXPORT void AKIActionModifyCloseTurningODAction( void * a2kaction, double acomplianceLevel);

	// .......................................
	// 		INFORMATION OF GENERAL VEHICLES
	// .......................................


	#define AKIInfVehGetMem 			-4001
	#define AKIInfVehUnknownSection 	-4002
	#define AKIInfIndexNotValid 		-4003
	#define AKIInfNotReady 				-4004
	#define AKIInfVehUnknownJunction	-4005
	#define AKIInfVehNotFound			-4006
	#define AKIInfVehInvalidParam		-4007
	#define AKIVehInvalidVehicleTypeId  -4008
	#define AKIInfVehNotAvalaible		-1

	/*!  \brief Vehicle's dynamic information.

	  */
	struct AAPIEXPORT InfVeh{
		//! 0, OK, else error code
		int report;
		//! the vehicle identifier
		int idVeh;
		//! the vehicle type (car, bus, truck, etc.) from 1 to AKIVehGetNbVehTypes ()
		int type;
		//! current id section if it's in a section
		int idSection;
		//! current id segment if it's in a section
		int segment;
		//! current id lane if it's in a section
		int numberLane;
		//! current id junction if it's in a junction
		int idJunction;
		//! the origin section identifier
		int idSectionFrom;
		//! number of lanes of the origin section where the vehicle enters the junction
		int idLaneFrom;
		//! destination section identifier
		int idSectionTo;
		//! number of lanes of the destination section where the vehicle exits the junction
		int idLaneTo;
		//! the position inside the junction = distance (m or feet, depending on the units defined in the network) from the entrance to the junction.
		double CurrentPos;
		//! the distance to end of the turning (m or feet, depending on the units defined in the network).
		double distance2End;
		//! x world coordinates of the middle point of the front bumper of the vehicle.
		double xCurrentPos;
		//! y world coordinates of the middle point of the front bumper of the vehicle.
		double yCurrentPos;
		//! z world coordinates of the middle point of the front bumper of the vehicle.
		double zCurrentPos;
		//! x world coordinates of the middle point of the back bumper of the vehicle.
		double xCurrentPosBack;
		//! y world coordinates of the middle point of the back bumper of the vehicle.
		double yCurrentPosBack;
		//! z world coordinates of the middle point of the back bumper of the vehicle.
		double zCurrentPosBack;
		//! current speed (in km/h or mph, depending on the units defined in the network
		double CurrentSpeed;
		//! speed in the previous simulation step (in km/h or mph, depending on the units defined in the network
		double PreviousSpeed;
		//!  total distance travelled (m or feet)
		double TotalDistance;
		//! the absolute entrance time of the vehicle in the system
		double SystemEntranceT;
		//! the absolute entrance time of the vehicle in last section
		double SectionEntranceT;
		//! the current stop time
		double CurrentStopTime;
	};

	/*!  \brief Vehicle's static information.
	  */
	struct AAPIEXPORT StaticInfVeh{
		//! 0, OK, else error code
		int report;
		//! vehicle identifier
		int idVeh;
		//! vehicle type (car, bus, truck, etc.)
		int type;
		//! vehicle length (m or feet, depending on the units defined in the network).
		double length;
		//! vehicle width (m or feet, depending on the units defined in the network).
		double width;
		//! Maximum desired speed of the vehicle (km/h or mph, depending on the units defined in the network).
		double maxDesiredSpeed;
		//! Maximum acceleration of the vehicle (m/s2 or ft/ s2, depending on the units defined in the network).
		double maxAcceleration;
		//! Maximum deceleration of the vehicle that can apply under normal conditions (m/s2 or ft/ s2, depending the units defined in the network).
		double normalDeceleration;
		//! Maximum deceleration of the vehicle that can apply under special conditions (m/s2 or ft/ s2, depending the units defined in the network).
		double maxDeceleration;
		//! degree of acceptance of the speed limits.
		double speedAcceptance;
		//! distance that the vehicle keeps between itself and the preceding vehicle (m or feet, depending on the units defined in the network).
		double minDistanceVeh;
		//! minimum safety distance
		double minSafetyDistance;
		//! time after which the vehicle becomes more aggressive in give-way situations (seconds).
		double giveWayTime;
		//! level of compliance of the vehicle to guidance indications (between 0 and 1).
		double guidanceAcceptance;
		//!  0 means vehicle will not change path enroute, 1 means vehicle will change path enroute depending on the percentege of enrouted vehicles defined.
		int enrouted;
		//! 0 means vehicle not equipped, 1 means vehicle equipped.
		int equipped;
		//! 0 means vehicle not tracked, 1 means vehicle tracked.
		int tracked;
		//! means vehicle keep fast lane during overtaking
		bool  keepfastLane;
		//! minimum headway to keep with its leader
		double headwayMin;
		//! estimation of the acceleration of the leader
		double sensitivityFactor;
		//! reaction time of the vehicle
		double reactionTime;
		//! reaction time at stop of the vehicle
		double reactionTimeAtStop;
		//! reaction time of the vehicle when stopped at a red traffic light and it is the first vehicle
		double reactionTimeAtTrafficLight;
		//! Identifier of centroid origin of the vehicle, when the traffic conditions are defined by an O/D matrix.
		int centroidOrigin;	
		//! Identifier of centroid destination of the vehicle, when the traffic conditions are defined by an O/D matrix.
		int centroidDest;
		//! Identifier of exit section destination of the vehicle, when the destination centroid uses percentages as destination (otherwise is ?) and the traffic conditions are defined by an O/D matrix.
		int idsectionExit;
		//! Identifier of Public Transport Line, when the vehicle has been generated as a public transport vehicle.
		int idLine;
		//!	Pointer to internal veh, only for internal use
		void * internalInfo;
	};

	/*! \brief Vehicle's world coordinates.
	  */
	struct AAPIEXPORT VehPos{
		//! x world coordinates of the middle point of the front bumper of the vehicle.
		double xPos;
		//! y world coordinates of the middle point of the front bumper of the vehicle.
		double yPos;
		//! z world coordinates of the middle point of the front bumper of the vehicle.
		double zPos;
		//!  x world coordinates of the middle point of the back bumper of the vehicle.
		double xPosBack;
		//!  y world coordinates of the middle point of the back bumper of the vehicle.
		double yPosBack;
		//!  z world coordinates of the middle point of the back bumper of the vehicle.
		double zPosBack;
	};

	/*! \brief  Vehicle's world coordiantes during last simulation step.
	  */
	struct AAPIEXPORT InfVehPos{
		//! 0, OK, otherwise an error code
		int	report;
		//! the vehicle identifier
		int idVeh;
		//! number of positions during last simulation step (input parameter).
		int Npos;
		//! Array with the positions during last simulation step.  After using this function, this array must be deallocated using “free?function.
		VehPos *vehiclePos;
	};

	/*! \brief Vehicle's dynamic graphic information.
	  */
	struct AAPIEXPORT GraphicInfVeh{
		//! 0, OK, otherwise an error code
		int report;
		//! the vehicle identifier
		int idVeh;
		//! Indicates if left turn signal is activated.
		bool leftTurnSignal;
		//! Indicates if rigth turn signal is activated.
		bool rightTurnSignal;
		//! Indicates if brake ligth is activated.
		bool brakeLight;
		//! Indicates if it's driving backwards.
		bool drivingBackwards;
	};
	
	AAPIEXPORT int 				AKIRemoveVehicle(int aidSec, int indexveh );

	/* activates the graphical info by vehicle (turn lights, brake lights...). When always false the 
		graphical inf will be updated only when simulating interactively so it will not be updated 
		when simulating in batch or in fast forward mode */
	AAPIEXPORT int 				AKIVehEnableGraphicalInf( bool always = false );
	AAPIEXPORT int 				AKIVehDisableGraphicalInf();

	AAPIEXPORT int 				AKIVehStateGetNbVehiclesSection(int aidSec, bool considerAllSegments);
	AAPIEXPORT InfVeh 			AKIVehStateGetVehicleInfSection(int aidSec, int indexveh);
	AAPIEXPORT StaticInfVeh 	AKIVehGetVehicleStaticInfSection(int aidSec, int indexveh);
	AAPIEXPORT int 				AKIVehSetVehicleStaticInfSection(int aidSec, int indexveh, StaticInfVeh staticinfVeh);
	AAPIEXPORT InfVehPos 		AKIVehGetVehicleGetPosSection( int asect, int indexveh, int nbPos );
	AAPIEXPORT int 				AKIVehGetVehicleGetPosSectionWithStruct( int asect, int indexveh, int nbPos, InfVehPos *pinfVehPos );
	AAPIEXPORT GraphicInfVeh	AKIVehGetVehicleGraphicInfSection(int asect, int indexveh);
	AAPIEXPORT int 				AKIVehSetDrivingBackwards(int aidSec, int indexveh, bool value);
	
	AAPIEXPORT int 				AKIVehStateGetNbVehiclesJunction(int ajunction);
	AAPIEXPORT InfVeh 			AKIVehStateGetVehicleInfJunction(int ajunction, int indexveh);
	AAPIEXPORT StaticInfVeh 	AKIVehGetVehicleStaticInfJunction(int ajunction, int indexveh);
	AAPIEXPORT int 				AKIVehSetVehicleStaticInfJunction(int ajunction, int indexveh, StaticInfVeh staticinfVeh);
	AAPIEXPORT InfVehPos 		AKIVehGetVehicleGetPosJunction( int ajunction, int indexveh, int nbPos );
	AAPIEXPORT int 				AKIVehGetVehicleGetPosJunctionWithStruct( int ajunction, int indexveh, int nbPos, InfVehPos *pinfVehPos );
	AAPIEXPORT GraphicInfVeh 	AKIVehGetVehicleGraphicInfJunction(int ajunction, int indexveh);

	AAPIEXPORT InfVeh 			AKIVehGetInf(int aidVeh);
	AAPIEXPORT StaticInfVeh 	AKIVehGetStaticInf(int aidVeh);
	AAPIEXPORT int 				AKIVehSetStaticInf(int aidVeh, StaticInfVeh staticinfVeh);


	AAPIEXPORT int 				AKIVehGetNbVehTypes();
	AAPIEXPORT int AKIVehTypeGetIdVehTypeANG(int VehType);
	AAPIEXPORT double 			AKIVehGetMinLengthVehType(int VehType);
	AAPIEXPORT double 			AKIVehGetMaxLengthVehType(int VehType);
	// Gets the name of the given VehType it is in utf16 and \0 terminated. The result has to be deleted with delete[]
	AAPIEXPORT const unsigned short		*AKIVehGetVehTypeName(int vehType);
	// Gets the VehType of the given name. Has to be in utf16 and \0 terminated
	AAPIEXPORT int 				AKIVehGetVehTypeInternalPosition( int aimsunVehicleTypeId );
	
	AAPIEXPORT int AKIVehStateGetNbSectionsVehiclePathJunction( int aidJunction, int indexveh, int idsection );
	AAPIEXPORT int AKIVehStateGetIdSectionVehiclePathJunction( int aidJunction, int indexveh, int idsection, int indexsection );
	AAPIEXPORT int AKIVehStateGetNbSectionsVehiclePathSection( int idSect, int indexveh, int idsection );
	AAPIEXPORT int AKIVehStateGetIdSectionVehiclePathSection( int idSect, int indexveh, int idsection, int indexsection );
	AAPIEXPORT int AKIVehTrackedGetNbSectionsVehiclePath( int idveh );
	AAPIEXPORT int AKIVehTrackedGetIdSectionVehiclePath( int idveh, int indexsection );
	AAPIEXPORT int AKIRemoveVehicleJunction( int idJunction, int indexveh );
	AAPIEXPORT int AKIVehTrackedRemove( int idveh );

	// .......................................
	// 		INFORMATION OF DETECTORS AND DETECTORS MEASURES 
	// .......................................


	#define AKIDETUnknownDetector  		-3010
	#define AKIDETIncorrectInterval  	-3011
	#define AKIDETMeasureNotGathered  	-3012
	#define AKIDETNoAggregatedDetection -3013
	#define AKIDETNoInstantDetection	-3015

	/*! \brief Internal Detector information.
	  */
	struct AAPIEXPORT structA2KDetector{
		//! 0, OK, else error code
		int report;
		//! detector identifier
		int Id;
		//! section identifier where the detector is located
		int IdSection;
		//! first lane that the detector covers
		int IdFirstLane;
		//! last lane that the detector covers
		int IdLastLane;
		//! Capability to detect distinguishing per vehicle type.
		bool DistinguishType;
		//! set of bits that codifies the detection capabilities.
		int Capabilities;
		//! position of the beginning of the detector, with respect to the beginning of the section.
		double InitialPosition;
		//! position of the end of the detector, with respect to the beginning of the section.
		double FinalPosition;
	};

	/*! \brief Vehicles dynamic eqquiped information.
	  */
	struct AAPIEXPORT EquippedInfVeh{
		//! 0, OK, else error code
		int report;
		//! the instant when the equipped vehicle crossed the detector.
		double timedetected;
		//! the vehicle identifier
		int idVeh;
		//!  instant vehicle speed (Km/h)
		int vehType;
		//! instant vehicle speed (Km/h)
		double speed;
		//! instant vehicle headway (sec)
		double headway;
		//! Public Transport line identifier (-1 when the equipped vehicle does not belongs to any PT line)
		int idptline;
	};


	AAPIEXPORT int 	AKIDetGetNumberDetectors();
	AAPIEXPORT int	AKIDetGetIdDetector(int nbdet);
	AAPIEXPORT structA2KDetector AKIDetGetPropertiesDetector(int nbdet);
	AAPIEXPORT structA2KDetector AKIDetGetPropertiesDetectorById(int iddet);

	AAPIEXPORT bool 	AKIDetIsCountGather(int Capability);
	AAPIEXPORT bool 	AKIDetIsPresenceGather(int Capability);
	AAPIEXPORT bool 	AKIDetIsSpeedGather(int Capability);
	AAPIEXPORT bool 	AKIDetIsOccupancyGather(int Capability);
	AAPIEXPORT bool 	AKIDetIsHeadwayGather(int Capability);
	AAPIEXPORT bool 	AKIDetIsDensityGather(int Capability);
	AAPIEXPORT bool 	AKIDetIsInfEquippedVehGather(int Capability);

	AAPIEXPORT double 	AKIDetGetIntervalDetection();
	AAPIEXPORT double 	AKIDetGetCycleInstantDetection();
	AAPIEXPORT int 	AKIDetGetNbMeasuresAvailableInstantDetection();
	AAPIEXPORT double 	AKIDetGetEndTimeMeasureAvailableInstantDetection(int elem);

	AAPIEXPORT int 	AKIDetGetSCOOTOccupancyCyclebyId(int iddet, int VehType);
	AAPIEXPORT double 	AKIDetGetFinTimeOccupedCyclebyId(int iddet, int elem, int VehType);
	AAPIEXPORT double 	AKIDetGetIniTimeOccupedCyclebyId(int iddet, int elem, int VehType);
	AAPIEXPORT int 	AKIDetGetNbintervalsOccupedCyclebyId(int iddet, int VehType);
	AAPIEXPORT int 	AKIDetGetCounterCyclebyId(int iddet, int VehType);
	AAPIEXPORT double 	AKIDetGetSpeedCyclebyId(int iddet, int VehType);
	AAPIEXPORT double 	AKIDetGetTimeOccupedCyclebyId(int iddet, int VehType);
	AAPIEXPORT int 	AKIDetGetPresenceCyclebyId(int iddet, int VehType);
	AAPIEXPORT double 	AKIDetGetHeadwayCyclebyId(int iddet, int VehType);
	AAPIEXPORT double 	AKIDetGetDensityCyclebyId(int iddet, int VehType);

	AAPIEXPORT int		AKIDetGetNbVehsEquippedInDetectionCyclebyId(int iddet, int VehType);
	AAPIEXPORT StaticInfVeh	AKIDetGetInfVehInDetectionStaticInfVehCyclebyId(int iddet, int elem, int VehType);
	AAPIEXPORT InfVeh	AKIDetGetInfVehInDetectionInfVehCyclebyId(int iddet, int elem, int VehType);

	AAPIEXPORT int 	AKIDetGetSCOOTOccupancyInstantDetectionbyId(int iddet, int VehType, double endtime);
	AAPIEXPORT double 	AKIDetGetIniTimeOccupedInstantDetectionbyId(int iddet, int elem, int VehType, double endtime);
	AAPIEXPORT double 	AKIDetGetEndTimeOccupedInstantDetectionbyId(int iddet, int elem, int VehType, double endtime);
	AAPIEXPORT int 	AKIDetGetNbintervalsOccupedInstantDetectionbyId(int iddet, int VehType, double endtime);
	AAPIEXPORT int 	AKIDetGetCounterInstantDetectionbyId(int iddet, int VehType, double endtime);
	AAPIEXPORT double 	AKIDetGetSpeedInstantDetectionbyId(int iddet, int VehType, double endtime);
	AAPIEXPORT double 	AKIDetGetTimeOccupedInstantDetectionbyId(int iddet, int VehType, double endtime);
	AAPIEXPORT int 	AKIDetGetPresenceInstantDetectionbyId(int iddet, int VehType, double endtime);
	AAPIEXPORT double 	AKIDetGetHeadwayInstantDetectionbyId(int iddet, int VehType, double endtime);
	AAPIEXPORT double 	AKIDetGetDensityInstantDetectionbyId(int iddet, int VehType, double endtime);
	AAPIEXPORT int		AKIDetGetNbVehsEquippedInDetectionInstantDetectionbyId(int iddet, int VehType, double endtime);
	AAPIEXPORT StaticInfVeh AKIDetGetInfVehInDetectionStaticInfVehInstantDetectionbyId(int iddet, int elem, int VehType, double endtime);
	AAPIEXPORT InfVeh AKIDetGetInfVehInDetectionInfVehInstantDetectionbyId(int iddet, int elem, int VehType, double endtime);

	AAPIEXPORT int 	AKIDetGetCounterAggregatedbyId(int iddet, int VehType);
	AAPIEXPORT double 	AKIDetGetSpeedAggregatedbyId(int iddet, int VehType);
	AAPIEXPORT double 	AKIDetGetTimeOccupedAggregatedbyId(int iddet, int VehType);
	AAPIEXPORT int 	AKIDetGetPresenceAggregatedbyId(int iddet, int VehType);
	AAPIEXPORT double 	AKIDetGetDensityAggregatedbyId(int iddet, int VehType);
	AAPIEXPORT double 	AKIDetGetHeadwayAggregatedbyId(int iddet, int VehType);
	AAPIEXPORT int AKIDetGetNbVehsInDetectionAggregatedbyId(int iddet, int VehType);
	AAPIEXPORT EquippedInfVeh AKIDetGetInfVehInDetectionAggregatedbyId(int iddet, int VehType, int elem);

	// .......................................
	// 		DETECTOR CLICKABLE EVENTS   
	// .......................................

	#define AKIDetectorEventsNoTraffic -10000

	AAPIEXPORT int AKIDetectorEventsEnable();
	AAPIEXPORT int AKIDetectorEventsDisable();
	AAPIEXPORT void AKIDetectorEventsAddEvent(int iddet, double aIniTime, double aEndTime, int vehType, double speed, double length, int idPTline);
	AAPIEXPORT int AKIDetectorEventsClear();

	// .......................................
	// 		INFORMATION OF IDENTIFIERS OF SECTIONS AND JUNCTIONS 
	// .......................................

	#define AKIInfNetGetMem 			-5001
	#define AKIInfUnknownId		 		-5002

	/*! \brief Aimsun section's information.
	  */
	struct AAPIEXPORT A2KSectionInf{
		//! 0, OK, otherwise is an error code
		int report;
		//! section identifier
		int id;
		//! section identifier in Aimsun kernel
		int angId;
		//! total number of central lanes
		int nbCentralLanes;
		//! total number of side lanes
		int nbSideLanes;
		//! speed limit of the section (Km/h o mph).
		double speedLimit;
		//! visibility distance of the section (m or feet).
		double visibilityDistance;
		//! yellow box speed of the section (Km/h o mph).
		double yellowBoxSpeed;
		//! capacity of the section (veh/h).
		double capacity;
		//! distance of zone 1 of the section (sec).
		double distance_zone1;
		//! distance of zone 2 of the section (sec).
		double distance_zone2;
		//! distance On Ramp of the section (sec).
		double distance_OnRamp;
		//! cooperation percentage On Ramp of the section.
		double cooperation_OnRamp;
		//! slope percentage of the section.
		double slope_percentage;
		//! length of the section (m or feet).
		double length;
		//! user-defined cost of the section.
		double userDefinedCost;
		//! maximum giveway time variation of the section (sec).
		double maxGivewayTimeVariation;
		//! reaction time variation of the section.
		int reactionTimeVariation;
		//! reaction time variation of the section.
		double reactionTimeAtTrafficLightVariation;
		//! reaction time variation of the section.
		double reactionTimeAtStopVariation;
		double imprudentLaneChangingFactor;
		double sensitivity2ImprudentLaneChangingFactor;
		//! total number of turnings that has as origin this section.
		int nbTurnings;
	};

	/*! \brief Section's behaviour parameters.

		This class is used to change some and more important parameters.
	  */
	struct AAPIEXPORT A2KSectionBehaviourParam{
		//! speed limit of the section (km/h or mph).
		double speedLimit;
		//! visibility distance of the section (metres or feet).
		double visibilityDistance;
		//! yellow box speed of the section (km/h or mph).
		double yellowBoxSpeed;
		//! capacity of the section (veh/h).
		double capacity;
		//! distance of zone 1 for the section (sec).
		double distance_zone1;
		//! distance of zone 2 for the section (sec).
		double distance_zone2;
		//! distance for On Ramp of the section (sec).
		double distance_OnRamp;
		//! cooperation for On Ramp of the section.
		double cooperation_OnRamp;
		//! user-defined cost of the section.
		double userDefinedCost;
		//! maximum giveway time variation of the section (sec).
		double maxGivewayTimeVariation;
		//! reaction time variation of the section.
		int reactionTimeVariation;
		double reactionTimeAtTrafficLightVariation;
		double reactionTimeAtStopVariation;
		double imprudentLaneChangingFactor;
		double sensitivity2ImprudentLaneChangingFactor;
	};

	/*! \brief Aimsun Microscopic internal centroid information.

	  */
	struct AAPIEXPORT A2KCentroidInf{
		//! 0, OK, else error code
		int report;
		//! centroid identifier
		int id;
		//! Centroid considers destination connectors percentages when the centroid acts as destination
		bool AsDestConsider_percentage;
		//! Centroid considers origin connectors percentages when the centroid acts as origin
		bool AsOrigConsider_percentage;
		//! true if the centroid acts as one origin
		bool IsOrigin;
		//! true if the centroid acts as one destination
		bool IsDestination;
		//! Number of connectors To
		int NumConnecTo;
		//! Number of connectors From
		int NumConnecFrom;
	};

	AAPIEXPORT int AKIInfNetGetUnits();
	AAPIEXPORT int AKIInfNetGetWorldCoordinates(double *min_x, double *min_y, double *max_x, double *max_y);
	AAPIEXPORT int AKIInfNetNbSectionsANG();

	AAPIEXPORT int AKIInfNetGetSectionANGId(int element);

	AAPIEXPORT A2KSectionInf AKIInfNetGetSectionANGInf(int aid);

	AAPIEXPORT int AKIInfNetGetIdSectionANGDestinationofTurning(int aid, int elem);
	AAPIEXPORT int AKIInfNetGetDestinationFromLaneofTurning(int aid, int elem);
	AAPIEXPORT int AKIInfNetGetDestinationToLaneofTurning(int aid, int elem);
	AAPIEXPORT int AKIInfNetGetOriginFromLaneofTurning(int aid, int elem);
	AAPIEXPORT int AKIInfNetGetOriginToLaneofTurning(int aid, int elem);

	AAPIEXPORT int AKIInfNetGetTurningDestinationFromLane( int originSection, int destinationSection );
	AAPIEXPORT int AKIInfNetGetTurningDestinationToLane( int originSection, int destinationSection );
	AAPIEXPORT int AKIInfNetGetTurningOriginFromLane( int originSection, int destinationSection );
	AAPIEXPORT int AKIInfNetGetTurningOriginToLane( int originSection, int destinationSection );

	AAPIEXPORT int AKIInfNetSetSectionBehaviouralParam(int aid, A2KSectionBehaviourParam behaviourParam, bool allsegments);
	AAPIEXPORT A2KSectionBehaviourParam AKIInfNetGetSectionBehaviouralParam( int idSection, int* report );
	AAPIEXPORT int AKISetSectionCapacity(int aid, double capacity);
	AAPIEXPORT int AKISetSectionUserDefinedCost(int aid, double userDefinedCost);
	AAPIEXPORT int AKISetSectionUserDefinedCost2(int aid, double userDefinedCost);
	AAPIEXPORT int AKISetSectionUserDefinedCost3(int aid, double userDefinedCost);

	AAPIEXPORT double AKIGetSectionCapacity(int aid);
	AAPIEXPORT double AKIGetSectionUserDefinedCost(int aid);
	AAPIEXPORT double AKIGetSectionUserDefinedCost2(int aid);
	AAPIEXPORT double AKIGetSectionUserDefinedCost3(int aid);

	AAPIEXPORT int AKIInfNetNbJunctions();
	AAPIEXPORT int AKIInfNetGetJunctionId(int element);

	AAPIEXPORT int AKIInfNetNbCentroids();
	AAPIEXPORT int AKIInfNetGetCentroidId(int element);
	AAPIEXPORT A2KCentroidInf AKIInfNetGetCentroidInf(int aid);
	AAPIEXPORT int AKIInfNetGetIdSectionofOriginCentroidConnector(int aid, int elem);
	AAPIEXPORT int AKIInfNetGetIdSectionofDestinationCentroidConnector(int aid, int elem);
	AAPIEXPORT int AKIInfNetGetIdSectionANGofOriginCentroidConnector(int aid, int elem);
	AAPIEXPORT int AKIInfNetGetIdSectionANGofDestinationCentroidConnector(int aid, int elem);

	// .......................................
	// 		INFORMATION OF PATH & NAMES 
	// .......................................

	#define	ODMatrixDemand				1
	#define	StateDemand					2

	// Returns utf16 encoded path of the network. The result has to be deleted with delete[]
	AAPIEXPORT const unsigned short * AKIInfNetGetNetworkPath();
	// Returns utf16 encoded name of the network. The result has to be deleted with delete[]
	AAPIEXPORT const unsigned short * AKIInfNetGetNetworkName();
	// Returns utf16 encoded name of current demand. The result has to be deleted with delete[]
	AAPIEXPORT const unsigned short * AKIInfNetGetTrafficDemandName();
	AAPIEXPORT int AKIInfNetGetTrafficDemandType(); 


	// .......................................
	// 		STATISTICAL INFORMATION  
	// .......................................


	#define AKIESTUnknownSection 	-6001
	#define AKIESTNotAvalaible 		-6002
	#define AKIESTUnknownCentroid 	-6004
	#define AKIESTUnknownStream					-6005
	#define AKIESTFuelConsumptionNotAvailable	-6006
	#define AKIESTPollutionEmissionNotAvailable	-6007
	#define AKIESTUnknownPollutant				-6008
	#define AKIESTWrongLane						-6009

	/*! \brief System/Replication statistics
	*/
	struct AAPIEXPORT StructAkiEstadSystem{
		//! 0, OK, else error code
		int report;
		//!  Flow  (veh/h)
		int Flow;	/* Flow */
		//! Average travel time(sec/km or sec/mile)
		double TTa;
		//! Deviation travel time(sec/km or sec/mile)
		double TTd;
		//! Average delay time (sec/km or sec/mile)
		double DTa;
		//! Deviation delay time (sec/km or sec/mile)
		double DTd;
		//!  Average speed (km/h or mph)
		double Sa;
		//!  Deviation speed (km/h or mph)
		double Sd;
		//! Average harmonic speed (km/h or mph)
		double SHa;
		//! Deviation harmonic speed (km/h or mph)
		double SHd;
		//! Density (Veh/km or Veh/mile)
		double Density;
		//! Average stop time (sec/km or sec/mile)
		double STa;
		//! Deviation stop time (sec/km or sec/mile)
		double STd;
		//! Number of Stops (#/Veh/km or #/Veh/mile)
		double NumStops;
		//! Average Queue Length
		double LongQueueAvg;
		//! Maximum Queue Length
		double LongQueueMax;	/* Maximum Queue Length */
		//! Total Distance travelled
		double TotalTravel;
		//! Total Time travelled
		double TotalTravelTime;
		//! Virtual average queue length
		double virtualQueueAvg;
		//! Virtual max queue length
		int virtualQueueMax;
	};

	/*! \brief Section's statistics
	*/
	struct AAPIEXPORT StructAkiEstadSection{
		//! \see StructAkiEstadSystem
		int report;	
		//! Section indentifier
		int Id;
		//! \see StructAkiEstadSystem
		int Flow;	/* Flow */
		//! \see StructAkiEstadSystem
		double TTa;
		//! \see StructAkiEstadSystem
		double TTd;		/* Travel Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double DTa;
		//! \see StructAkiEstadSystem
		double DTd;		/* Delay Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double Sa;
		//! \see StructAkiEstadSystem
		double Sd;		/* Speed  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double SHa;
		//! \see StructAkiEstadSystem
		double SHd;		/* Harmonic Speed  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double Density;
		//! \see StructAkiEstadSystem
		double STa;
		//! \see StructAkiEstadSystem
		double STd;		/* Stop Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double NumStops;		/* Number of Stops */
		//! Average Queue Length
		double LongQueueAvg;	/* Average Queue Length */
		//! Maximum Queue Length
		double LongQueueMax;	/* Maximum Queue Length */
		//! \see StructAkiEstadSystem
		double TotalTravel;	/* Total Distance  travelled */
		//! \see StructAkiEstadSystem
		double TotalTravelTime;	/* Total Time  travelled */
		//! Virtual average queue length
		double virtualQueueAvg; /*VirtualQueue Avg*/
		//! Virtual max queue length
		int virtualQueueMax; /*VirtualQueue Max*/
	};

	/*! \brief Turning's statistics
	*/
	struct AAPIEXPORT StructAkiEstadTurning{
		//! \see StructAkiEstadSystem
		int report;	
		//! Section from identifier
		int IdSectionFrom;
		//! Section to identifier
		int IdSectionTo;
		//! \see StructAkiEstadSystem
		int Flow;	/* Flow */
		//! \see StructAkiEstadSystem
		double TTa;
		//! \see StructAkiEstadSystem
		double TTd;		/* Travel Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double DTa;
		//! \see StructAkiEstadSystem
		double DTd;		/* Delay Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double Sa;
		//! \see StructAkiEstadSystem
		double Sd;		/* Speed  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double SHa;
		//! \see StructAkiEstadSystem
		double SHd;		/* Harmonic Speed  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double STa;
		//! \see StructAkiEstadSystem
		double STd;		/* Stop Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double NumStops;		/* Number of Stops */
		//! \see StructAkiEstadSection
		double LongQueueAvg;	/* Average Queue Length */
		//! \see StructAkiEstadSection
		double LongQueueMax;	/* Maximum Queue Length */
		//! \see StructAkiEstadSection
		double TotalTravel;	/* Total Distance  travelled */
		//! \see StructAkiEstadSection
		double TotalTravelTime;	/* Total Time  travelled */
		//! \see StructAkiEstadSection
	};

	struct AAPIEXPORT StructAkiEstadSectionLane{
		//! \see StructAkiEstadSystem
		int report;
		//! Section Identifier
		int IdSection;
		//! \see StructAkiEstadSystem
		int Flow;	/* Flow */
		//! \see StructAkiEstadSystem
		double TTa;
		//! \see StructAkiEstadSystem
		double TTd;		/* Travel Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double DTa;
		//! \see StructAkiEstadSystem
		double DTd;		/* Delay Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double Sa;
		//! \see StructAkiEstadSystem
		double Sd;		/* Speed  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double SHa;
		//! \see StructAkiEstadSystem
		double SHd;		/* Harmonic Speed  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double Density;
		//! Average Queue Length
		double LongQueueAvg;
		//! Maximum Queue Length
		double LongQueueMax;
	};

	/*! \brief O/D pair's statistics
	*/
	struct AAPIEXPORT StructAkiEstadODPair{
		//! \see StructAkiEstadSystem
		int report;
		//! Origin centroid identifier
		int IdOrigin;
		//! Destination centroid identifier
		int IdDest;
		//! \see StructAkiEstadSystem
		int Flow;	/* Flow */
		//! \see StructAkiEstadSystem
		double TTa;
		//! \see StructAkiEstadSystem
		double TTd;		/* Travel Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double DTa;
		//! \see StructAkiEstadSystem
		double DTd;		/* Delay Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double Sa;
		//! \see StructAkiEstadSystem
		double Sd;		/* Speed  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double SHa;
		//! \see StructAkiEstadSystem
		double SHd;		/* Harmonic Speed  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double STa;
		//! \see StructAkiEstadSystem
		double STd;		/* Stop Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double NumStops;		/* Number of Stops */
		//! \see StructAkiEstadSection
		double virtualQueueAvg;	/* Average Virtual Queue */
		//! \see StructAkiEstadSection
		double virtualQueueMax;	/* Maximum Virtual Queue */
		//! \see StructAkiEstadSystem
		double TotalTravel;	/* Total Distance  travelled */
		//! \see StructAkiEstadSystem
		double TotalTravelTime;	/* Total Time  travelled */
		//! Total number of vehicles lost that we doing this trip
		int vehLost;		/* Total number of vehicle lost */
	};

	/*! \brief Stream's statistics
	*/
	struct AAPIEXPORT StructAkiEstadStream{
		//! \see StructAkiEstadSystem
		int report;	
		//! Stream's identifier
		int Id;
		//! \see StructAkiEstadSystem
		int Flow;	/* Flow */
		//! \see StructAkiEstadSystem
		double TTa;
		//! \see StructAkiEstadSystem
		double TTd;		/* Travel Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double DTa;
		//! \see StructAkiEstadSystem
		double DTd;		/* Delay Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double Sa;
		//! \see StructAkiEstadSystem
		double Sd;		/* Speed  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double SHa;
		//! \see StructAkiEstadSystem
		double SHd;		/* Harmonic Speed  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double STa;
		//! \see StructAkiEstadSystem
		double STd;		/* Stop Time  : Average & Deviation */
		//! \see StructAkiEstadSystem
		double NumStops;		/* Number of Stops */
		//! \see StructAkiEstadSection
		double virtualQueueAvg;	/* Average Virtual Queue */
		//! \see StructAkiEstadSection
		double virtualQueueMax;	/* Maximum Virtual Queue */
		//! \see StructAkiEstadSection
		double TotalTravel;	/* Total Distance  travelled */
		//! \see StructAkiEstadSection
		double TotalTravelTime;	/* Total Time  travelled */
	};


	AAPIEXPORT int AKIIsGatheringStatistics();
	AAPIEXPORT double AKIEstGetIntervalStatistics();
	AAPIEXPORT bool 					AKIEstIsNewStatisticsAvailable();
	AAPIEXPORT StructAkiEstadSection 	AKIEstGetGlobalStatisticsSection(int aidarc, int VehType);
	AAPIEXPORT StructAkiEstadSection 	AKIEstGetParcialStatisticsSection(int aidarc, double time, int VehType);
	AAPIEXPORT StructAkiEstadSectionLane AKIEstGetGlobalStatisticsSectionLane(int aidarc, int indexLane, int VehType);
	AAPIEXPORT StructAkiEstadSectionLane AKIEstGetParcialStatisticsSectionLane(int aidarc, int indexLane, double time, int VehType);
	AAPIEXPORT StructAkiEstadTurning 	AKIEstGetGlobalStatisticsTurning(int aidsectionFrom, int aidsectionTo, int VehType);
	AAPIEXPORT StructAkiEstadTurning 	AKIEstGetParcialStatisticsTurning(int aidsectionFrom, int aidsectionTo, double time, int VehType);
	AAPIEXPORT StructAkiEstadTurning 	AKIEstGetGlobalStatisticsLink(int aidsectionFrom, int aidsectionTo, int VehType);
	AAPIEXPORT StructAkiEstadTurning 	AKIEstGetParcialStatisticsLink(int aidsectionFrom, int aidsectionTo, double time, int VehType);
	AAPIEXPORT StructAkiEstadSystem 	AKIEstGetGlobalStatisticsSystem(int VehType);
	AAPIEXPORT StructAkiEstadSystem 	AKIEstGetParcialStatisticsSystem(double time, int VehType);
	AAPIEXPORT StructAkiEstadODPair 	AKIEstGetGlobalStatisticsODPair(int idOrigin, int idDestination, int VehType);
	AAPIEXPORT StructAkiEstadODPair 	AKIEstGetParcialStatisticsODPair(int idOrigin, int idDestination, double time, int VehType);
	AAPIEXPORT StructAkiEstadStream 	AKIEstGetParcialStatisticsStream(int aidstream, double time, int VehType);
	AAPIEXPORT StructAkiEstadStream 	AKIEstGetGlobalStatisticsStream(int aidstream, int VehType);

	AAPIEXPORT double					AKIEstGetInstantVirtualQueueSection(int aidarc,int VehType);

	//Pollutants and emission statistics
	AAPIEXPORT double 	AKIEstGetGlobalStatisticsSectionFuelCons(int aidarc, int VehType);
	AAPIEXPORT double 	AKIEstGetParcialStatisticsSectionFuelCons(int aidarc, double time, int VehType);
	AAPIEXPORT double 	AKIEstGetGlobalStatisticsTurningFuelCons(int aidsectionFrom, int aidsectionTo, int VehType);
	AAPIEXPORT double 	AKIEstGetParcialStatisticsTurningFuelCons(int aidsectionFrom, int aidsectionTo, double time, int VehType);
	AAPIEXPORT double 	AKIEstGetGlobalStatisticsLinkFuelCons(int aidsectionFrom, int aidsectionTo, int VehType);
	AAPIEXPORT double 	AKIEstGetParcialStatisticsLinkFuelCons(int aidsectionFrom, int aidsectionTo, double time, int VehType);
	AAPIEXPORT double 	AKIEstGetGlobalStatisticsSystemFuelCons(int VehType);
	AAPIEXPORT double 	AKIEstGetParcialStatisticsSystemFuelCons(double time, int VehType);
	AAPIEXPORT double 	AKIEstGetGlobalStatisticsODPairFuelCons(int idOrigin, int idDestination, int VehType);
	AAPIEXPORT double 	AKIEstGetParcialStatisticsODPairFuelCons(int idOrigin, int idDestination, double time, int VehType);
	AAPIEXPORT double 	AKIEstGetParcialStatisticsStreamFuelCons(int aidstream, double time, int VehType);
	AAPIEXPORT double 	AKIEstGetGlobalStatisticsStreamFuelCons(int aidstream, int VehType);

	AAPIEXPORT int 	AKIEstGetNbPollutants();
	// Gets the name in utf16 and \0 terminated. The result has to be deleted with delete[]
	AAPIEXPORT const unsigned short *AKIEstGetPollutantName(int index);

	AAPIEXPORT double 	AKIEstGetGlobalStatisticsSectionPollution(int indexPol, int aidarc, int VehType);
	AAPIEXPORT double 	AKIEstGetParcialStatisticsSectionPollution(int indexPol, int aidarc, double time, int VehType);
	AAPIEXPORT double 	AKIEstGetGlobalStatisticsTurningPollution(int indexPol, int aidsectionFrom, int aidsectionTo, int VehType);
	AAPIEXPORT double 	AKIEstGetParcialStatisticsTurningPollution(int indexPol, int aidsectionFrom, int aidsectionTo, double time, int VehType);
	AAPIEXPORT double 	AKIEstGetGlobalStatisticsLinkPollution(int indexPol, int aidsectionFrom, int aidsectionTo, int VehType);
	AAPIEXPORT double 	AKIEstGetParcialStatisticsLinkPollution(int indexPol, int aidsectionFrom, int aidsectionTo, double time, int VehType);
	AAPIEXPORT double 	AKIEstGetGlobalStatisticsSystemPollution(int indexPol, int VehType);
	AAPIEXPORT double 	AKIEstGetParcialStatisticsSystemPollution(int indexPol, double time, int VehType);
	AAPIEXPORT double 	AKIEstGetGlobalStatisticsODPairPollution(int indexPol, int idOrigin, int idDestination, int VehType);
	AAPIEXPORT double 	AKIEstGetParcialStatisticsODPairPollution(int indexPol, int idOrigin, int idDestination, double time, int VehType);
	AAPIEXPORT double 	AKIEstGetParcialStatisticsStreamPollution(int indexPol, int aidstream, double time, int VehType);
	AAPIEXPORT double 	AKIEstGetGlobalStatisticsStreamPollution(int indexPol, int aidstream, int VehType);

	// .......................................
	// 		VEHICLE ENTRANCE  
	// .......................................

	#define AKIEnterVehUnknownSection 		-7001
	#define AKIEnterVehUnFeasibleLane		-7003
	#define AKIEnterVehNotSpace		 		-7004
	#define AKIEnterVehUnknownCentroid		-7005
	#define AKIEnterVehUnFeasiblePath		-7006
	#define AKIEnterVehNoTrafficFlow		-7008
	#define AKIEnterVehNoTrafficOD			-7009
	#define AKIEnterVehUnknownLane			-7010
	#define AKIEnterVehUnknownNextSection	-7011
	#define AKIEnterVehUnknownPTLine		-7015
	#define AKIEnterVehUnknownVehType		-7016

	#define RIGHT	-1
	#define NOCHANGE 0
	#define LEFT	1

	AAPIEXPORT int		AKIEnterVehTrafficFlow(int asection, int VehType, int tracking = 0);
	AAPIEXPORT int		AKIEnterVehTrafficOD(int asection, int VehType, int idCentroidOrigin, int idCentroidDest, int tracking = 0);
	/* initPosition and initSpeed in network units (metric: m and km/h and english: feet and mph */
	AAPIEXPORT int		AKIPutVehTrafficFlow(int asection, int idLane, int VehType, double initPosition, double initSpeed, int nextSection, int tracking);
	/* initPosition and initSpeed in network units (metric: m and km/h and english: feet and mph */
	AAPIEXPORT int		AKIPutVehTrafficOD(int asection, int idLane, int VehType, int idCentroidOr, int idCentroidDest, double initPosition, double initSpeed, int tracking);

	/*! \brief Vehicle's arrival information.
	  */
	struct AAPIEXPORT InfArrival{
		//! 0, OK, else error code
		int report;
		//! >0 vehicle identifier entered in the system
		int idVeh;
		//! true the vehicle has not enter in the system but has been stored in the virtual entrance Queue, false otherwise
		bool inVirtualQueue;
		//! entrance section identifier where the vehicle has been entered or stored in the virtual entrance queue.
		int entranceSection;
	}; 



	AAPIEXPORT InfArrival AKIGenerateArrivalTrafficFlow(int asection, int VehType, int useVirtualQueue);
	AAPIEXPORT InfArrival AKIGenerateArrivalTrafficOD(int VehType, int idCentroidOrigin, int idCentroidDest, int useVirtualQueue);

	// .......................................
	// 		VEHICLE TRACKED MANAGE  
	// .......................................

	#define AKIVehNotTracked				-7012
	#define AKIVehInvalidParameter			-7013
	#define AKIVehNextSectionUnreachable	-7014

	/*! newSpeed expressed in the network units, either Km/h or Mph */
	AAPIEXPORT int		AKIVehTrackedModifySpeed(int aidVeh, double newSpeed);
	AAPIEXPORT int		AKIVehTrackedModifyLane(int aidVeh, int nextLane);
	AAPIEXPORT int		AKIVehTrackedModifyNextSection(int aidVeh, int nextSection);
	AAPIEXPORT int		AKIVehTrackedModifyNextTargetLaneInNextSection(int aidVeh, int nextSection, int nextlane);
	AAPIEXPORT int		AKIVehTrackedDelete(int aidVeh);
	AAPIEXPORT int		AKIVehSetAsTracked(int aidVeh);
	AAPIEXPORT int		AKIVehSetAsTrackedbyPointer(void *aVeh);
	AAPIEXPORT int		AKIVehSetAsNoTracked(int aidVeh);




	// .......................................
	// 		INFORMATION OF VEHICLE TRACKED   
	// .......................................

	/*! \brief Dynamic vehicle's position.

	  This struct is used to modify vehicle's position.
  */
	struct AAPIEXPORT DynInfVeh{
		//! x world coordinates of the middle point of the vehicle front bumper for the current position
		double xCurrentPos;
		//! y world coordinates of the middle point of the vehicle front bumper for the current position
		double yCurrentPos;
		//! x world coordinates of the middle point of the vehicle front bumper for the previous position
		double xCurrentPosBack;
		//! y world coordinates of the middle point of the vehicle front bumper for the previous position
		double yCurrentPosBack;
		//! current speed (km/h or mph, depending the units defined in the network).
		double currentSpeed;
		//! next turning selected by the vehicle (-1 right turning, 1 left turning, 0 other cases)
		int turning;
	};

 
	AAPIEXPORT StaticInfVeh 	AKIVehTrackedGetStaticInf(int aidVeh);
	AAPIEXPORT InfVeh 			AKIVehTrackedGetInf(int aidVeh);
	AAPIEXPORT int 			AKIVehTrackedSetStaticInf(int aidVeh, StaticInfVeh staticinfVeh);
	AAPIEXPORT InfVehPos 		AKIVehTrackedGetPos( int anIdVeh, int nbPos );
	AAPIEXPORT int 			AKIVehTrackedGetPosWithStruct( int anIdVeh, int nbPos , InfVehPos *pinfVehPos);
	AAPIEXPORT int 			AKIVehSetVehicleTrackedDynInf( int anIdVeh, DynInfVeh dynInfVeh);
	AAPIEXPORT GraphicInfVeh 	AKIVehTrackedGetGraphicInf(int aidVeh);

	// .......................................
	// 		INFORMATION OF RUN TIME SIMULATION   
	// .......................................
	AAPIEXPORT double 	AKIGetCurrentSimulationTime();	
	AAPIEXPORT double 	AKIGetTimeSta();	
	AAPIEXPORT double 	AKIGetIniSimTime();
	AAPIEXPORT double 	AKIGetEndSimTime();
	AAPIEXPORT double 	AKIGetDurationTransTime();
	AAPIEXPORT double 	AKIGetSimulationStepTime();
	AAPIEXPORT int 	AKISetEndSimTime(double atime);

	// .......................................
	// 		INFORMATION OF REPLICATION   
	// .......................................

	AAPIEXPORT double 	AKIGetRandomNumber();

	// .......................................
	// 		MANAGING OF INCIDENTS   
	// .......................................

	#define AKIIncidentWrongIniTime		-8001
	#define AKIIncidentWrongPosition	-8002
	#define AKIIncidentUnknownLane		-8003
	#define AKIIncidentUnknownSection	-8004
	#define AKIIncidentNotPresent		-8005
	#define AKIIncidentWrongLength		-8006
	#define AKIIncidentWrongDuration	-8007

	AAPIEXPORT int AKIGenerateIncident(int asection, int alane, double position, double length, double initime, double duration, double visibilityDistance, bool updateIdGroup);
	AAPIEXPORT int AKIRemoveIncident(int asection, int alane, double position);
	AAPIEXPORT int AKIRemoveAllIncidentsInSection(int asection); 
	AAPIEXPORT int AKIResetAllIncidents();

	// .......................................
	// 		MANAGING OF Public Transport   
	// .......................................

	#define AKIPTNotLoaded					-9001
	#define AKIPTVehUnknown					-9002
	#define AKIPTStopUnknown				-9003
	#define AKIPTLineUnknown				-9005
	#define AKIPTVehNotSpace		 		-9006
	#define AKIPTIndexNotValid		 		-9007
	#define AKIPTVehUnFeasibleLane		 	-9008
	#define AKIPTVehCapacityOverflow	 	-9009
	#define AKIPTNotAvalaible		 		-1

	/*! \brief Public transport vehicle's dynamic information.
	  */
	struct AAPIEXPORT InfPTVeh{
		//! \see InfVeh
		int report; // Retorna 0, si OK, sino codi error
		//! \see InfVeh
		int idVeh;
		//! \see InfVeh
		int type;
		//! \see InfVeh
		// Information in Vehicle when it is in a section
		//! \see InfVeh
		int idSection;
		//! \see InfVeh
		int segment;
		//! \see InfVeh
		int numberLane;
		//! \see InfVeh
		// Information in Vehicle when it is in a node
		int idJunction;
		//! \see InfVeh
		int idSectionFrom;
		//! \see InfVeh
		int idLaneFrom;
		//! \see InfVeh
		int idSectionTo;
		//! \see InfVeh
		int idLaneTo;
		//! \see InfVeh
		double CurrentPos;
		//! \see InfVeh
		double distance2End;
		//! \see InfVeh
		double xCurrentPos;
		//! \see InfVeh
		double yCurrentPos;
		//! \see InfVeh
		double zCurrentPos;
		//! \see InfVeh
		double xCurrentPosBack;
		//! \see InfVeh
		double yCurrentPosBack;
		//! \see InfVeh
		double zCurrentPosBack;
		//! \see InfVeh
		double CurrentSpeed;
		//! \see InfVeh
		double PreviousSpeed;
		//! \see InfVeh
		double TotalDistance;
		//! \see InfVeh
		double SystemEntranceT;
		//! \see InfVeh
		double SectionEntranceT;
		//! \see InfVeh
		double CurrentStopTime;


		// Information about PT
		//! the number of stops done.
		int nbStopsDone;
		//! the identifier of next stop
		int nextStopId;
		//! the distance to next stop (metres or feet).
		double distanceNextStop;
		//! the next stop time in the next stop(seconds).
		double nextServiceTime;
		//! the stopped time in a bus stop(seconds).
		double currentStoppedTimeInBusStop;

	};

	/*!  \brief Public transport vehicle's static information.
	 */
	struct AAPIEXPORT StaticInfPTVeh{
		//! \see StaticInfVeh
		int report; 
		//! \see StaticInfVeh
		int idVeh;
		//! \see StaticInfVeh
		int type;
		//! \see StaticInfVeh
		double length;
		//! \see StaticInfVeh
		double width;
		//! \see StaticInfVeh
		double maxDesiredSpeed;
		//! \see StaticInfVeh
		double maxAcceleration;
		//! \see StaticInfVeh
		double normalDeceleration;
		//! \see StaticInfVeh
		double maxDeceleration;
		//! \see StaticInfVeh
		double speedAcceptance;
		//! \see StaticInfVeh
		double minDistanceVeh;
		//! \see StaticInfVeh
		double minSafetyDistance;
		//! \see StaticInfVeh
		double giveWayTime;
		//! \see StaticInfVeh
		double guidanceAcceptance;
		//! \see StaticInfVeh
		int enrouted;
		//! \see StaticInfVeh
		int equipped;
		//! \see StaticInfVeh
		int tracked;
		//! \see StaticInfVeh
		bool  keepfastLane;
		//! \see StaticInfVeh
		double headwayMin;
		//! \see StaticInfVeh
		double sensitivityFactor;
		//! \see StaticInfVeh
		double reactionTime;
		//! \see StaticInfVeh
		double reactionTimeAtStop;
		//! \see StaticInfVeh
		double reactionTimeAtTrafficLight;
		//! Maximum number of people inside the ptvehicle
		int	maxCapacity;
		//! Current number of people inside the ptvehicle
		int currentLoad;
		//! Public line identifier
		int idLine;
	};


	AAPIEXPORT int		AKIPTGetNumberLines();
	AAPIEXPORT int		AKIPTGetIdLine(int elem);

	AAPIEXPORT int		AKIPTGetNumberSectionsInLine(int lineId);
	AAPIEXPORT int		AKIPTGetIdSectionInLine(int lineId, int elem);

	AAPIEXPORT int		AKIPTGetNumberStopsInLine(int lineId);
	AAPIEXPORT int		AKIPTGetIdStopsInLine(int lineId, int elem);

	AAPIEXPORT int		AKIPTEnterVeh( int lineId, int VehType, bool tracked );
	AAPIEXPORT int		AKIPTVehModifyStopTime( int aidVeh, int nstop, double stopTime );	


	AAPIEXPORT StaticInfPTVeh 	AKIPTVehGetStaticInf(int aidVeh);
	AAPIEXPORT InfPTVeh 		AKIPTVehGetInf(int aidVeh);
	AAPIEXPORT double			AKIPTGetServiceTimeStopsInLine(int aidVeh, int nstop);
	AAPIEXPORT int AKIPTVehSetCurrentLoad( int aidVeh, int currentLoad );
	// .......................................
	// 		PRINT INFORMATION IN XCONSOLE / LOG WINDOW
	// .......................................

	AAPIEXPORT int AKIPrintString(char *string);
	AAPIEXPORT void AKIPrintAsUNICODEString(const unsigned short *string);
	// remember to delete[] the const char *
	AAPIEXPORT const char *AKIConvertToAsciiString(const unsigned short *string, bool deleteUshortString, bool *anyNonAsciiChar);
	// remember to delete[] the const unsigned short *
	AAPIEXPORT const unsigned short *AKIConvertFromAsciiString(const char *ascii);
	AAPIEXPORT void AKIDeleteUNICODEString( const unsigned short *string );
	//.........................................
	//     ODDemand
	//........................................

	#define AKIODDemandNoTrafficOD 			-11000
	#define AKIODDemandIncorrectNumSlice 	-11002
	#define AKIODDemandUnknownCentroid 		-11003
	#define AKIODDemandUnknownODPair 		-11004


	AAPIEXPORT int AKIODDemandGetNumSlicesOD(int vehType);
	AAPIEXPORT double AKIODDemandGetIniTimeSlice(int vehType, int slice);
	AAPIEXPORT double AKIODDemandGetEndTimeSlice(int vehType, int slice);
	AAPIEXPORT int AKIODDemandGetDemandODPair(int origen, int desti, int VehType, int numslice);
	AAPIEXPORT int AKIODDemandSetDemandODPair(int origen, int desti, int VehType, int numslice, int anewdemand);

	//.........................................
	//     TrafficDemand
	//........................................

	#define AKIStateDemandNoTrafficState		-15000
	#define AKIStateDemandIncorrectNumSlice 	-15002
	#define AKIStateDemandUnknownCentroid 		-15003
	#define AKIStateDemandUnknownVehType 		-15004

	AAPIEXPORT int AKIStateDemandGetNumSlices();
	AAPIEXPORT double AKIStateDemandGetIniTimeSlice(int numslice);
	AAPIEXPORT double AKIStateDemandGetEndTimeSlice(int numslice);
	AAPIEXPORT double AKIStateDemandGetDemandSection(int idSection, int VehType, int numslice);
	AAPIEXPORT int AKIStateDemandSetDemandSection(int idSection, int VehType, int numslice, double anewflow);

	//.........................................
	//     PastCosts
	//........................................
	#define AKIPastCostUnknownLink 			-12001
	#define AKIPastCostNoPerVehType		 	-12002
	#define AKIPastCostPerVehType 			-12003
	#define AKIPastCostNoReaded 			-12004
	#define AKIPastCostIncorrectTypeName	-12005

	AAPIEXPORT int AKIPastCostAreCostsPerVehicleType();
	AAPIEXPORT double AKIPastCostGetIniTimeReaded();
	AAPIEXPORT double AKIPastCostGetIntervalReaded();
	AAPIEXPORT int AKIPastCostGetNbIntervalsReaded();
	AAPIEXPORT int AKIPastCostSetPastCost(int sectorig, int sectdest, double aTime, int idVehType, double acost, double aocost);
	AAPIEXPORT double AKIPastCostGetPastCost(int sectorig, int sectdest, double aTime, int idVehType);
	AAPIEXPORT double AKIPastCostGetPastOutputCost(int sectorig, int sectdest, double aTime, int idVehType);

	//.........................................
	//     Fleet Management
	//........................................
 
	#define AKIFleetNotLoaded					-14001
	#define AKIFleetVehUnknown					-14002
	#define AKIFleetStopUnknown					-14003
	#define AKIFleetVehNotSpace		 			-14005
	#define AKIFleetVehUnFeasibleLane		 	-14006
	#define AKIFleetSectUnknown		 			-14007
	#define AKIFleetStopTypeUnknown		 		-14008
	#define AKIFleetInvalidFleetId		 		-14009
	#define AKIFleetRouteAlreadyAssigned 		-14010
	#define AKIFleetSectNotConnected	 		-14011
	#define AKIFleetRouteUnknown		 		-14012
	#define AKIFleetFleetVehNotGenerated 		-14013
	#define AKIFleetInvalidFirstSection 		-14014
	#define AKIFleetInvalidDistance		 		-14015
	#define AKIFleetInvalidLane			 		-14016
 
	AAPIEXPORT int AKIFleetGetNbFleet();
	AAPIEXPORT int AKIFleetGetFleetId(int index);
 
	AAPIEXPORT int AKIFleetGetNbFleetStops();
	AAPIEXPORT int AKIFleetGetFleetStopId(int index);
 
	AAPIEXPORT int AKIFleetGetNbFleetVehicles(int idFleet);
	// Gets the name in utf16 and \0 terminated. The result has to be deleted with delete[]
	AAPIEXPORT const unsigned short *AKIFleetGetFleetVehicleName(int idFleet, int index);
 
	AAPIEXPORT int AKIFleetAddFleet( int id );
	AAPIEXPORT int AKIFleetAddFleetStop(int id, int type, int idSect, int aidLane, double adistance, double alenght, double aMaxVeh);
	AAPIEXPORT int AKIFleetModifyFleetStopTimeWindow( int id, double openTW, double closeTW );
	AAPIEXPORT int AKIFleetAddFleetAssignment(int idFleet, const unsigned short *nameFleetVeh, int VehType, double departureTime, double deviation);
	AAPIEXPORT int AKIFleetAddFleetAssignmentRoute(int idFleet, const unsigned short *nameFleetVeh, int idRoute, int nbsect, int *idsect);
	AAPIEXPORT int AKIFleetAddFleetAssignmentStop(int idFleet, const unsigned short *nameFleetVeh, int nbstops, int *idstops, double *avgtime, double *devtime);
	AAPIEXPORT int AKIFleetIsFleetVehicleGenerated(int idFleet, int idFleetVeh);
	AAPIEXPORT int AKIFleetGetCurrentSectionIdFleetVehicle(int idFleet, const unsigned short *nameFleetVeh);
	AAPIEXPORT int AKIFleetModifyRouteStopsFleetVehicle(int idFleet, const unsigned short *nameFleetVeh, int nbsect, int *idsect, int nbstops, int *idstops, double *avgtime, double *devtime);
	AAPIEXPORT int AKIFleetGetNbStopsFleetVeh(int idFleet, const unsigned short *nameFleetVeh);
	AAPIEXPORT int AKIFleetChangeStopTimeFleetVeh(int idFleet, const unsigned short *nameFleetVeh, int indexStop, double serviceTime);
	AAPIEXPORT int AKIFleetGetNbStopsDoneFleetVeh(int idFleet, const unsigned short *nameFleetVeh);
	AAPIEXPORT int AKIFleetIsDoingStopFleetVeh(int idFleet, const unsigned short *nameFleetVeh);
	AAPIEXPORT double AKIFleetGetStopTimeFleetVeh(int idFleet, const unsigned short *nameFleetVeh, int indexStop);

// Functions relative to path information
	#define AKIInfVehPathNotAvailable	-13001
	#define AKIInfVehDestinationUnreachable	-13002
	#define AKIInfVehInvalidDestinationCentroid	-13003
	
	/*! \brief Path vehicle's information.
	  */
	struct AAPIEXPORT PathInfVeh {
		//! 0 OK, otherwise check the error code.
		int report;
		//! Vehicle identifier.
		int idVeh;
		/*! Path type:
				-# Route Choice Path
				-# User-Defined Route
				-# User-Defined Shortest Path Tree
		  */
		int type;
		//! Entrance section identifier. This is the section from where the vehicle enters the network.
		int entranceSectionId;
		//! Total number of sections that have	the path.
		int numSectionsInPath;
		//! Total distance of the path in m.
		double totalDistance;
	};

	AAPIEXPORT PathInfVeh AKIVehInfPath( int idveh );
	AAPIEXPORT PathInfVeh AKIVehInfPathSection(int aidSec, int indexveh);
	AAPIEXPORT int AKIVehInfPathGetNextSection(int idveh, int fromsection);
	AAPIEXPORT int AKIVehInfPathGetNextSectionInSection(int aidSec, int indexveh, int fromsection);
}		



#endif
