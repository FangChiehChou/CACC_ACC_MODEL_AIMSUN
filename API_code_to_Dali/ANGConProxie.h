//-*-Mode: C++;-*-
#ifndef _ANGConProxie_h_
#define _ANGConProxie_h_
//SDK
#include "AAPI_Util.h"

#define INTEGER_TYPE 1
#define DOUBLE_TYPE 2
#define STRING_TYPE 3

#define EXTERNAL_TEMPORAL 0
#define INTERNAL 1
#define EXTERNAL 2

#define ESONONE 0
#define ESOCANCEL 1
#define ESOREWIND 2
#define ESOSTOP 3
#define ESOSTOPAT 4


// .......................................
// 		ANGConnection   
// .......................................
extern "C" {
	/*! Returns the replication identifier */ 
	AAPIEXPORT unsigned int ANGConnGetReplicationId();

	/*! Returns the experiment identifier */ 
	AAPIEXPORT unsigned int ANGConnGetExperimentId();

	/*! Returns the scenario identifier */ 
	AAPIEXPORT unsigned int ANGConnGetScenarioId();

	#define ANGConUnknownObject		-15001
	/*! Return the object id having the same name, ANGConUnknownObject otherwise. If deleteUShortName is true, then 
		name parameter will be deleted
	*/
	AAPIEXPORT int ANGConnGetObjectId( const unsigned short * name, bool deleteUShortName  );
	
	/*! returns the name of the object, if the object exists */
	AAPIEXPORT const unsigned short * ANGConnGetObjectName( int idObject );

	/*! the object idobject will be hidden in the views if hide is true and it will be drawn if hide is false */
	AAPIEXPORT void ANGConnHideObject( int idObject, bool hide );

	/*! It returns true if the object idobject is hidden (not drawn) in any of the open views and false 
		if it is being drawn in all the views */
	AAPIEXPORT bool ANGConnIsObjectHidden( int idObject );

	/*! Returns a string with the scenario date and time using ISO 8601 extended format (YYYY-MM-DDTHH:MM:SS),
		or a empty string if no information is available. Don't delete the string.
	*/
	AAPIEXPORT const char * ANGConnGetScenarioTime();

	// Gets the attribute with the given name in utf16 and \0 terminated.
	AAPIEXPORT void * ANGConnGetAttribute( const unsigned short * internalAttrName );
	// Creates the attribute with the given names in utf16 and \0 terminated.
	AAPIEXPORT void * ANGConnCreateAttribute( const unsigned short * typeName, const unsigned short * internalAttrName, const unsigned short * externalAttrName, int attrType, int columnType = EXTERNAL);
	// Sets the string as value of the given attribute. Value has to be in utf16 and \0 terminated.
	AAPIEXPORT void ANGConnSetAttributeValueString( void * attr, unsigned int objectId, const unsigned short * value );
	// Gets the string as value of the given attribute. Value is in utf16 and \0 terminated and has to be deleted using delete[]
	AAPIEXPORT const unsigned short * ANGConnGetAttributeValueString( void * attr, unsigned int objectId );

	AAPIEXPORT void ANGConnSetAttributeValueBool( void * attr, unsigned int objectId, bool value );
	AAPIEXPORT bool ANGConnGetAttributeValueBool( void * attr, unsigned int objectId );

	AAPIEXPORT void ANGConnSetAttributeValueInt( void * attr, unsigned int objectId, int value );
	AAPIEXPORT int ANGConnGetAttributeValueInt( void * attr, unsigned int objectId );

	AAPIEXPORT void ANGConnSetAttributeValueDouble( void * attr, unsigned int objectId, double value );
	AAPIEXPORT double ANGConnGetAttributeValueDouble( void * attr, unsigned int objectId );

	AAPIEXPORT void ANGConnActivatePolicy( unsigned int objectId );
	AAPIEXPORT void ANGConnDeactivatePolicy( unsigned int objectId );
	AAPIEXPORT bool ANGConnIsPolicyActive( unsigned int objectId );

	/*! Returns the number of values in a time series
	*/
	AAPIEXPORT int ANGConnGetTimeSeriesSize( void * attr, unsigned int objectId );

	/*! Returns the value at position pos in a time series or -1 if no value if found at that position
	*/
	AAPIEXPORT double ANGConnGetTimeSeriesValue( void * attr, unsigned int objectId, unsigned int pos );

	/*! Set the text value of a GKText object. text has to be in utf16 and \0 terminated.
	*/
	AAPIEXPORT void ANGConnSetText( unsigned int objectId, const unsigned short * text );

	/*! Set a policy (non external) as active or inactive.
	*/
	AAPIEXPORT void ANGConnMarkActivatePolicy( unsigned int objectId, bool value );

	/*! Initializes the policies list before a simulation.
	*/
	AAPIEXPORT void ANGConnInitPolicies();

	/*! Returns the ID of the GKSimVehicle corresponding to the kernel vehicle aid
	*/
	AAPIEXPORT int ANGConnVehGetGKSimVehicleId(int aidVeh);

	/*! When running in batch mode GKSimVehicle objects are not created by default (to simulate faster).
		This call enables the creation in batch mode. Useful when creating attributes on these vehicles.
	*/
	AAPIEXPORT void ANGConnEnableVehiclesInBatch( bool value );
	/*! Returns the last ordered command to the microsimulator; it can be: 0 (Nothing), 1 (Cancel), 2 (rewind), 3(Stop), 4 (StopAt).
		"when" parameter returns the stop time when the command is eSOStopAt, otherwise it contains a NULL pointer.
	 */
	AAPIEXPORT int ANGGetSimulationOrder( int * when );
	/*! Programs a new command to the microsimulator.
		order parameter can be: 0 (Nothing), 1 (Cancel), 2 (rewind), 3(Stop), 4 (StopAt)
		"when" parameter should be provided when the command type is StopAt and
		it is considered in relative time, not in stationary time.
	 */
	AAPIEXPORT void ANGSetSimulationOrder( int order, int when );

	#define ANGConConversionFailed -15002
	
	AAPIEXPORT int AKIConvertLatitudeLongitudeToXY( double latitude, double longitude, double *x, double *y );
	AAPIEXPORT int AKIConvertXYToLatitudeLongitude( double x, double y, double *latitude, double *longitude);
}

#endif
