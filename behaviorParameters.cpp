#include "behaviorParameters.h"

#define  RAMP_LC_TIME_DEFAULT 67 //[s]
#define  RAMP_LC_DIS_DEFAULT 300 //[m]

behaviorParameters::behaviorParameters(void)
{
}

behaviorParameters::~behaviorParameters(void)
{
}

double behaviorParameters::getRampLcTime()
{
	return this->rampLcTime<=0?RAMP_LC_TIME_DEFAULT:this->rampLcTime; 
}

double behaviorParameters::getRampLcDis()
{
	return this->rampLcDis<=0?RAMP_LC_DIS_DEFAULT:this->rampLcDis; 
}
