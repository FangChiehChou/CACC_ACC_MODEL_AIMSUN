//-*-Mode: C++;-*-
#ifndef _mybehavioralModelCreator_h_
#define _mybehavioralModelCreator_h_

#include "A2BehavioralModelUtil.h"
#include "A2BehavioralModelCreator.h"

class A2BehavioralModel;
extern "C" A2BEHAVIORALEXPORT A2BehavioralModelCreator * mybehavioralModelFactory();

class A2BEHAVIORALEXPORT mybehavioralModelCreator : public A2BehavioralModelCreator
{
public:
	mybehavioralModelCreator();
	~mybehavioralModelCreator();

	virtual A2BehavioralModel * newModel();
};

#endif