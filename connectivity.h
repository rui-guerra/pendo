//
//  connectivity.h
//  pendo
//
//  Created by Rui Guerra on 10/12/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//

#include "listPlace.h"
#include "listTransition.h"
#include "listArc.h"
#include "readLib.h"
#include <stdbool.h>

#include "net.h"


#ifndef pendo_connectivity_h
#define pendo_connectivity_h

typedef struct ioptNode ioptNode;
typedef struct ioptNodeSet ioptNodeSet;


struct ioptNode {
    int id;
    //bool visited;
    //bool checked;
    ioptNode * next;
};

struct ioptNodeSet {
    ioptNode * head;
    ioptNode * tail;
    int size;
};

ioptNode * newNode();
ioptNodeSet * newNodeSet();
void addToNodeSet(ioptNode node, ioptNodeSet* nodeSet);
ioptNodeSet * getNeighbourNodes(ioptArcSet * arcSet, ioptNode * origin);
void removeNode (ioptNodeSet * nodeSet, int id);
bool isConnected(ioptArcSet * arcSet, ioptNode * src, ioptNode * dest);
void search(ioptNode * origin, ioptNodeSet * nodeSet, ioptNodeSet * connected, ioptArcSet * arcSet);
bool connected(ioptNet * net);
void teste (ioptNet * net);
void loadToNodeSet(ioptNodeSet * nodeSet, ioptPlaceSet * placeSet, ioptTransitionSet * trasintionSet);
#endif




