
#include "A2BehavioralModelUtil.h"
#include "mybehavioralModelCreator.h"
#include "mybehavioralModel.h"

A2BehavioralModelCreator * mybehavioralModelFactory()
{

	A2BehavioralModelCreator	* res = new mybehavioralModelCreator();
	return res;
}

mybehavioralModelCreator::mybehavioralModelCreator () : A2BehavioralModelCreator(){}

mybehavioralModelCreator::~mybehavioralModelCreator (){}

A2BehavioralModel * mybehavioralModelCreator::newModel()
{
	A2BehavioralModel *res = new mybehavioralModel();
	return res;
}

