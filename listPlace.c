//
//  listPlace.c
//  pendo
//
//  Created by Rui Guerra on 11/1/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listPlace.h"
#include "ioptDataStructures.h"
#include "connectivity.h"

ioptPlaceSet* newPlaceSet(){
    ioptPlaceSet* placeSet = malloc(sizeof(ioptPlaceSet));
    if(placeSet== NULL){
        printf("Error allocation placeSet\n");
    }
    placeSet->head = NULL;
    placeSet->tail = NULL;
    placeSet->size = 0;
    return placeSet;
}

ioptPlace * newPlace(){
    ioptPlace * place = malloc(sizeof(ioptPlace));
    place->next = NULL;
    return place;
}

void addToPlaceSet(ioptPlace place, ioptPlaceSet* placeSet){
    ioptPlace* newPlace = malloc(sizeof(ioptPlace));
    if(newPlace == NULL){
        printf("Error allocating place\n");
    }    
    *newPlace = place;
    newPlace->next = NULL;
    if(placeSet->head == NULL){
        placeSet->head = placeSet->tail = newPlace;
    }
    else{
        placeSet->tail->next = newPlace;
        placeSet->tail = newPlace;
    }
    placeSet->size++;
    if (newPlace->id > placeSet->highID) {
        placeSet->highID = newPlace->id;
    }
}

ioptPlace * getPlace(ioptPlaceSet * placeSet, int nodeID){
    ioptPlace * placeIterator = placeSet->head;
    while (placeIterator) {
        if (placeIterator->id == nodeID) {
            return placeIterator;
        }
        placeIterator = placeIterator->next;
    }
    return NULL;
}

ioptPlace * createPlaceCopy(ioptPlace * originalPlace){
    ioptPlace * copyPlace = newPlace();
    copyPlace->id = originalPlace->id;
    copyPlace->graphicsPosition.x = originalPlace->graphicsPosition.x;
    copyPlace->graphicsPosition.y = originalPlace->graphicsPosition.y;
    copyPlace->graphicsPosition.page = originalPlace->graphicsPosition.page;
    
    //copyPlace->comment.comment = strdup(originalPlace->comment.comment);
    //copyPlace->comment.graphicsOffset.x = copyPlace->comment.graphicsOffset.x;
    //copyPlace->comment.graphicsOffset.y = copyPlace->comment.graphicsOffset.y;
    
    copyPlace->bound = originalPlace->bound;
    copyPlace->initialMarking.initialMarking = originalPlace->initialMarking.initialMarking;
    copyPlace->initialMarking.graphicsOffset.x = originalPlace->initialMarking.graphicsOffset.x;
    copyPlace->initialMarking.graphicsOffset.y = originalPlace->initialMarking.graphicsOffset.y;
    
    copyPlace->name.name = strdup(originalPlace->name.name);
    //copyPlace->name.graphicsOffset.x = copyPlace->name.graphicsOffset.x;
    //copyPlace->name.graphicsOffset.y = copyPlace->name.graphicsOffset.y;
    
    return copyPlace;
}









