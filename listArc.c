//
//  listArc.c
//  pendo
//
//  Created by Rui Guerra on 11/1/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listArc.h"
#include "ioptDataStructures.h"

ioptArcSet* newArcSet(){
    ioptArcSet* arcSet = malloc(sizeof(ioptArcSet));
    if(arcSet== NULL){
        printf("Error allocation arcSet\n");
    }
    arcSet->head = NULL;
    arcSet->tail = NULL;
    arcSet->size = 0;
    return arcSet;
}

ioptArc * newArc(){
    ioptArc * arc = malloc(sizeof(ioptArc));
    arc->next = NULL;
    return arc;
}

void addToArcSet(ioptArc arc, ioptArcSet* arcSet){
    ioptArc* newArc = malloc(sizeof(ioptArc));
    if(newArc == NULL){
        printf("Error allocating arc\n");
    }
    *newArc = arc;
    newArc->next = NULL;
    if(arcSet->head == NULL){
        arcSet->head = arcSet->tail = newArc;
    }
    else{
        arcSet->tail->next = newArc;
        arcSet->tail = newArc;
    }
    arcSet->size++;
    if (newArc->id > arcSet->highID) {
        arcSet->highID = newArc->id;
    }
}

ioptArc * getArc(ioptArcSet * arcSet, int nodeID){
    ioptArc * arcIterator = arcSet->head;
    while (arcIterator) {
        if (arcIterator->id == nodeID) {
            return arcIterator;
        }
        arcIterator = arcIterator->next;
    }
    return NULL;
}

ioptArc * createArcCopy(ioptArc * originalArc){
    ioptArc * copyArc = newArc();
    
    copyArc->id = originalArc->id;
    copyArc->source = originalArc->source;
    copyArc->target = originalArc->target;
    copyArc->type = strdup(originalArc->type);
    copyArc->inscription.graphicsPosition.x = originalArc->inscription.graphicsPosition.x;
    copyArc->inscription.graphicsPosition.y = originalArc->inscription.graphicsPosition.y;
    copyArc->inscription.value = originalArc->inscription.value;
    
    return copyArc;
}
