//
//  conditions.h
//  pendo
//
//  Created by Rui Guerra on 19/05/14.
//  Copyright (c) 2014 Rui Guerra. All rights reserved.
//

#ifndef pendo_conditions_h
#define pendo_conditions_h



typedef struct condition condition;
typedef struct conditionSet conditionSet;

struct condition{
    int node;
    int net;
    condition * next;
};

struct conditionSet{
    condition * head;
    condition * tail;
    int size;
};


conditionSet * newConditionSet();
condition * newCondition();
void addToConditionSet(condition placement, conditionSet * placementSet);
//condition * getCondition (conditionSet * conditions, int nodeID);

#endif
