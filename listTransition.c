//
//  listTransition.c
//  pendo
//
//  Created by Rui Guerra on 11/1/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listTransition.h"
#include "ioptDataStructures.h"
#include "listIO.h"


ioptTransitionSet* newsTransitionSet(){
    ioptTransitionSet* transitionSet = malloc(sizeof(ioptTransitionSet));
    if(transitionSet == NULL){
        printf("Error allocation transitionSet");
    }
    transitionSet->head = NULL;
    transitionSet->tail = NULL;
    transitionSet->size = 0;
    transitionSet->highID = 0;
    return transitionSet;
}

ioptTransition * newTransition(){
    ioptTransition * transition = malloc(sizeof(ioptTransition));
    transition->inputEvents = newAssociatedEventSet();
    transition->outputEvents = newAssociatedEventSet();
    transition->next = NULL;
    return transition;
}

void addToTransitionSet(ioptTransition transition, ioptTransitionSet* transitionSet){
    ioptTransition* newTransition = malloc(sizeof(ioptTransition));
    if(newTransition == NULL){
        printf("Error allocation transition");
    }
    *newTransition = transition;
    newTransition->next = NULL;
    if(transitionSet->head == NULL){
        transitionSet->head = transitionSet->tail = newTransition;
    }
    else{
        transitionSet->tail->next = newTransition;
        transitionSet->tail = newTransition;
    }
    transitionSet->size++;
    if (newTransition->id > transitionSet->highID) {
        transitionSet->highID = newTransition->id;
    }
}

ioptTransition * createTransitionCopy(ioptTransition * originalTransition){
    ioptTransition * copyTransition = newTransition();
    
    copyTransition->id = originalTransition->id;
    copyTransition->priority = originalTransition->priority;
    
    //strcpy(copyTransition->name.name, originalTransition->name.name);
    //copyTransition->name.name = originalTransition->name.name;
    copyTransition->name.name = strdup(originalTransition->name.name);
    copyTransition->name.graphicsOffset = originalTransition->name.graphicsOffset;
    
    //strcpy(copyTransition->comment.comment, originalTransition->comment.comment);
    //copyTransition->comment.comment = originalTransition->comment.comment;
    if (originalTransition->comment.comment != NULL) {
        //copyTransition->comment.comment = strdup(originalTransition->comment.comment);
    }
    copyTransition->comment.graphicsOffset = originalTransition->comment.graphicsOffset;
    
    copyTransition->graphicsPosition.page = originalTransition->graphicsPosition.page;
    copyTransition->graphicsPosition.x = originalTransition->graphicsPosition.x;
    copyTransition->graphicsPosition.y = originalTransition->graphicsPosition.y;
    
    copyTransition->next = NULL;

    return copyTransition;
}

ioptTransition * getTransition(ioptTransitionSet * transitionSet, int nodeID){
    ioptTransition * transitionIterator = transitionSet->head;
    while (transitionIterator) {
        if (transitionIterator->id == nodeID) {
            return transitionIterator;
        }
        transitionIterator = transitionIterator->next;
    }
    return NULL;
}

ioptAssociatedEvent * newAssociatedEvent(){
    ioptAssociatedEvent * newEvent = malloc(sizeof(ioptAssociatedEvent));
    newEvent->next = NULL;
    return newEvent;
}

ioptAssociatedEventSet * newAssociatedEventSet(){
    ioptAssociatedEventSet * newEventSet = malloc(sizeof(ioptAssociatedEventSet));
    newEventSet->head = NULL;
    newEventSet->tail = NULL;
    newEventSet->size = 0;
    return newEventSet;
}

void addToAssociatedEventSet(ioptAssociatedEvent event, ioptAssociatedEventSet * eventSet){
    ioptAssociatedEvent * newEvent = newAssociatedEvent();
    *newEvent = event;
    
    if (eventSet->head == NULL) {
        eventSet->head = eventSet->tail = newEvent;
    }
    else{
        eventSet->tail->next = newEvent;
        eventSet->tail = newEvent;
    }
    eventSet->size++;

}

void addEventToTransition(ioptAssociatedEvent * event, int IOtype, ioptTransition * transition){
    if (IOtype == INPUT) {
        addToAssociatedEventSet(*event, transition->inputEvents);
    }else if(IOtype == OUTPUT){
        addToAssociatedEventSet(*event, transition->outputEvents);
    }
}






















