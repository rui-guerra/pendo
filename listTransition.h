//
//  listTransition.h
//  pendo
//
//  Created by Rui Guerra on 11/1/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//

#include "ioptDataStructures.h"

#ifndef pendo_listTransition_h
#define pendo_listTransition_h

typedef struct ioptTransition ioptTransition;
typedef struct ioptTransitionSet ioptTransitionSet;

typedef struct ioptAssociatedEvent ioptAssociatedEvent;
typedef struct ioptAssociatedEventSet ioptAssociatedEventSet;

struct ioptAssociatedEvent{
    char * eventID;
    ioptAssociatedEvent * next;
};

struct ioptAssociatedEventSet{
    ioptAssociatedEvent * head;
    ioptAssociatedEvent * tail;
    int size;
};


struct ioptTransition{
    int id;
    ioptName name;
    ioptComment comment;
    int priority;
    ioptGraphicsPosition graphicsPosition;
    
    ioptAssociatedEventSet * inputEvents;
    ioptAssociatedEventSet * outputEvents;
    
    ioptTransition *next;
};

struct ioptTransitionSet{
    ioptTransition *head;
    ioptTransition *tail;
    int size;
    int highID;
};


ioptTransitionSet* newsTransitionSet();
ioptTransition * newTransition();
void addToTransitionSet(ioptTransition transition, ioptTransitionSet* transitionSet);
ioptTransition * createTransitionCopy(ioptTransition * originalTransition);
ioptTransition * getTransition(ioptTransitionSet * transitionSet, int nodeID);

ioptAssociatedEvent * newAssociatedEvent();
ioptAssociatedEventSet * newAssociatedEventSet();
void addEventToTransition(ioptAssociatedEvent * event, int IOtype, ioptTransition * transition);
#endif
