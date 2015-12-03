//
//  net.h
//  pendo
//
//  Created by Rui Guerra on 11/1/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//

#include "listPlace.h"
#include "listArc.h"
#include "listTransition.h"
#include "listIO.h"

#ifndef pendo_net_h
#define pendo_net_h


typedef struct ioptNet ioptNet;
typedef struct ioptNetSet ioptNetSet;

struct ioptNet {
    ioptPlaceSet * placeSet;
    ioptTransitionSet * transitionSet;
    ioptArcSet * arcSet;
    
    ioptIO * io;
    ioptNet * next;
    int id;
};

struct ioptNetSet{
    ioptNet * head;
    ioptNet * tail;
    int size;
    int highID;
};


ioptNet * newNet();
ioptNetSet * newNetSet();
void addToNetSet(ioptNet net, ioptNetSet * netSet);
ioptNet * getNet(ioptNetSet * netSet, int id);
void removeNet(int id, ioptNetSet * netSet);
#endif
