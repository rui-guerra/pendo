//
//  listArc.h
//  pendo
//
//  Created by Rui Guerra on 11/1/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//
#include "ioptDataStructures.h"

#ifndef pendo_listArc_h
#define pendo_listArc_h

typedef struct ioptArc ioptArc;
typedef struct ioptArcSet ioptArcSet;

struct ioptArc{
    int id;
    int source;
    int target;
    char* type;
    ioptInscription inscription;
    ioptArc *next;
};

struct ioptArcSet{
    ioptArc *head;
    ioptArc *tail;
    int size;
    int highID;
};


ioptArcSet* newArcSet();
ioptArc * newArc();
void addToArcSet(ioptArc arc, ioptArcSet* arcSet);
ioptArc * getArc(ioptArcSet * arcSet, int nodeID);
ioptArc * createArcCopy(ioptArc * originalArc);


#endif
