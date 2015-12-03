//
//  conditions.c
//  pendo
//
//  Created by Rui Guerra on 19/05/14.
//  Copyright (c) 2014 Rui Guerra. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "conditions.h"


conditionSet * newConditionSet(){
    conditionSet * placementSet = malloc(sizeof(conditionSet));
    if(placementSet== NULL){
        printf("Error allocation conditionSet\n");
    }
    placementSet->head = NULL;
    placementSet->tail = NULL;
    placementSet->size = 0;
    return placementSet;
}

condition * newCondition(){
    condition * placement = malloc(sizeof(condition));
    placement->next = NULL;
    return placement;
}

void addToConditionSet(condition placement, conditionSet * placementSet){
    condition * newPlacement = malloc(sizeof(condition));
    if (newPlacement == NULL) {
        printf("Error allocating condition\n");
    }
    * newPlacement = placement;
    if (placementSet->head == NULL) {
        placementSet->head = placementSet->tail = newPlacement;
    } else{
        placementSet->tail->next = newPlacement;
        placementSet->tail = newPlacement;
    }
    placementSet->size++;
}

//condition * getCondition (conditionSet * conditions, int nodeID){
//    condition * conditionIterator = conditions->head;
//    while (conditionIterator) {
//        if (conditionIterator->node == nodeID) {
//            return conditionIterator;
//        }
//        conditionIterator = conditionIterator->next;
//
//    }
//    return NULL;
//}

