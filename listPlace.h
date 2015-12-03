//
//  listPlace.h
//  pendo
//
//  Created by Rui Guerra on 11/1/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//

#include "ioptDataStructures.h"

#ifndef pendo_listPlace_h
#define pendo_listPlace_h

typedef struct ioptPlace ioptPlace;
typedef struct ioptPlaceSet ioptPlaceSet;

struct ioptPlace{
    int id;
    ioptName name;
    ioptComment comment;
    ioptInitialMarking initialMarking;
    int bound;
    ioptGraphicsPosition graphicsPosition;
    ioptPlace* next;
};

struct ioptPlaceSet{
    ioptPlace *head;
    ioptPlace *tail;
    int size;
    int highID;
};

ioptPlace * newPlace();
ioptPlaceSet * newPlaceSet();
void addToPlaceSet(ioptPlace place, ioptPlaceSet* placeSet);
ioptPlace * getPlace(ioptPlaceSet * placeSet, int nodeID);
ioptPlace * createPlaceCopy(ioptPlace * originalPlace);

#endif
