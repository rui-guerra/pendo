//
//  interface.h
//  pendo
//
//  Created by Rui Guerra on 19/05/14.
//  Copyright (c) 2014 Rui Guerra. All rights reserved.
//

#include "conditions.h"
#include "connectivity.h"

#ifndef pendo_interface_h
#define pendo_interface_h

void printNet (ioptNet * net);
void printNets (ioptNetSet * netSet);
//void printGetPlacements(conditionSet * placementSet);
ioptNodeSet * getCuttingSet();
int getCondition (ioptTransition * transition);
conditionSet * getConditions(ioptNodeSet * cuttingSet, ioptNet * originalNet);
#endif
