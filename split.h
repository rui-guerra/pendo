//
//  split.h
//  pendo
//
//  Created by Rui Guerra on 15/02/14.
//  Copyright (c) 2014 Rui Guerra. All rights reserved.
//

#ifndef pendo_split_h
#define pendo_split_h

#include "listPlace.h"
#include "listTransition.h"
#include "listArc.h"
#include "readLib.h"
#include <stdbool.h>

#include "connectivity.h"

#include"net.h"


ioptNetSet * divideNet (ioptNet * originalNet, ioptNodeSet * cutSet);
bool checkCutSet (ioptNet * net, ioptNodeSet * cutSet);
//ioptArcSet * duplicateArcs(ioptArcSet * original, int id);
ioptArcSet * duplicateArcs(ioptArcSet * original, ioptNodeSet * cutSet);
void copyTransition (ioptTransition * src, ioptTransition * dest);
void reconstructArcs(ioptNetSet * netSet, ioptNet * originalNet);
void mergeSubNets(ioptNetSet * netSet, ioptNet * originalNet);
void reasignIO (ioptNet * originalNet, ioptNetSet * netSet);
#endif
