//
//  split.c
//  pendo
//
//  Created by Rui Guerra on 15/02/14.
//  Copyright (c) 2014 Rui Guerra. All rights reserved.
//

#include <stdio.h>


#include "listPlace.h"
#include "listTransition.h"
#include "listArc.h"
#include "readLib.h"
#include <stdbool.h>
#include <string.h>

#include "net.h"
#include "connectivity.h"

#include "split.h"



bool checkCutSet (ioptNet * net, ioptNodeSet * cutSet){
//    ioptNodeSet * neigh;
//    ioptNode * iterator = cutSet->head;
//    
//    // CONDITION 1
//    
//    while (iterator && (iterator->next != NULL)) {
//        if (isConnected(net->arcSet, iterator, iterator->next)) {
//            return false;
//        }
//        iterator = iterator->next;
//        
//        
//        
//        
//        
//    }
//    return true;
    ioptNode * iteratorOne = cutSet->head;
    ioptNode * iteratorTwo = cutSet->head;
    while(iteratorOne){
        while(iteratorTwo){
            if (isConnected(net->arcSet, iteratorOne, iteratorTwo)){
                return false;
            }
            iteratorTwo = iteratorTwo->next;
        }
        iteratorOne = iteratorOne->next;
        iteratorTwo = cutSet->head;
    }
    return true;
}

ioptArcSet * duplicateArcs(ioptArcSet * original, ioptNodeSet * cutSet){//int id){
    ioptArcSet * duplicated = newArcSet();
    ioptArc * iterator = original->head;
    ioptNode * cutIterator;
    
    bool flag = true;
    
    while (iterator) {
        
        
        cutIterator = cutSet->head;
        while (cutIterator) {
            
            if (!((iterator->source != cutIterator->id) && (iterator->target != cutIterator->id))) {
                flag = false;
                break;
            }
            
            
            
            
            
            
            cutIterator = cutIterator->next;
        }
        if (flag) {
            addToArcSet(* iterator, duplicated);
            
        }
        
        flag = true;
        
        
        
        
        
        
        iterator = iterator->next;
    }
    return duplicated;
}

        
        
        
        
        
        
/*
        if ((iterator->source != id) && (iterator->target != id)) {
            addToArcSet( * iterator, duplicated);
        }
        iterator = iterator->next;
    }
    return duplicated;
}*/

void copyPlace (ioptPlace * src, ioptPlace * dest){
    dest->id = src->id;
    dest->name = src->name;
    dest->initialMarking = src->initialMarking;
    dest->graphicsPosition = src->graphicsPosition;
    dest->comment = src->comment;
    dest->bound = src->bound;
    dest->next = NULL;
}

void copyTransition (ioptTransition * src, ioptTransition * dest){
    dest->id = src->id;
    dest->name = src->name;
    dest->graphicsPosition = src->graphicsPosition;
    dest->comment = src->comment;
    dest->priority = src->priority;
    dest->next = NULL;
}


void addNodeToNet(ioptNet * newNet, ioptNet * originalNet, ioptNodeSet * nodeSet)
{
    ioptNode * node = nodeSet->head;
    ioptPlace * place;// = originalNet->placeSet->head;
    ioptTransition * transition;// = originalNet->transitionSet->head;
    
    ioptPlace * auxPlace;
    ioptTransition * auxTransition;
    
    
    while (node) {
        place = originalNet->placeSet->head;
        transition = originalNet->transitionSet->head;
        
        while (place) {
            if (place->id == node->id) {
                auxPlace = newPlace();
                //auxPlace = place;
                copyPlace(place, auxPlace);
                auxPlace->next = NULL;
                addToPlaceSet(*auxPlace, newNet->placeSet);
                free(auxPlace);
                break;
            }
            place = place->next;
        }
        while (transition) {
            if (transition->id == node->id) {
                auxTransition = newTransition();
                //auxTransition = transition;
                copyTransition(transition, auxTransition);
                auxTransition->next = NULL;
                addToTransitionSet(*auxTransition, newNet->transitionSet);
                free(auxTransition);
                break;
            }
            transition = transition->next;
        }
        
        
        
        
        
        
        node = node->next;
    }


}





ioptNetSet * divideNet (ioptNet * originalNet, ioptNodeSet * cutSet){
    
    ioptNetSet * netSet = newNetSet();
    ioptNodeSet * nodeSet = newNodeSet();
    ioptNodeSet * connected;
    ioptNode * iterator = cutSet->head;
    ioptNode * origin;
    ioptNet * auxNet;
    
    //ioptArcSet * auxArcSet;
    loadToNodeSet (nodeSet, originalNet->placeSet, originalNet->transitionSet);
    
    while (iterator) {
        removeNode (nodeSet, iterator->id);
        iterator = iterator->next;
    }
    
    //iterator = cutSet->head;
    
    while (nodeSet->size > 0) {
        
    
    
        connected = newNodeSet();
        origin = nodeSet->head;
    
    
        search(origin, nodeSet, connected, duplicateArcs(originalNet->arcSet, cutSet));//iterator->id));
        iterator = connected->head;
        while (iterator) {
            removeNode(nodeSet, iterator->id);
            iterator = iterator->next;
        }
        
        //netSet->head = newNet();
        
        auxNet = newNet();
        
        addNodeToNet(auxNet, originalNet, connected);
        addToNetSet(*auxNet, netSet);
        
        
        
        // add cnnected to new net
        free(connected);
    }
    
    return netSet;
}


bool abcconnected(ioptNet * net){
    ioptNodeSet * connected = newNodeSet();
    ioptNodeSet * nodeSet = newNodeSet();
    ioptNode * origin;
    
    loadToNodeSet(nodeSet, net->placeSet, net->transitionSet);
    origin = nodeSet->head;
    
    search(origin, nodeSet, connected, net->arcSet);
    
    return false;
}


void reconstructArcs(ioptNetSet * netSet, ioptNet * originalNet){
    ioptNet * netIterator = netSet->head;
    ioptArc * arcIterator = originalNet->arcSet->head;
    ioptArc * auxArc;
    
    while (netIterator) {
        while (arcIterator) {
            if (getArc(netIterator->arcSet, arcIterator->id) == NULL) {
                if ((getPlace(netIterator->placeSet, arcIterator->source) != NULL && getTransition(netIterator->transitionSet, arcIterator->target) != NULL) || (getPlace(netIterator->placeSet, arcIterator->target) != NULL && getTransition(netIterator->transitionSet, arcIterator->source) != NULL)) {
                    auxArc = createArcCopy(arcIterator);
                    addToArcSet(*auxArc, netIterator->arcSet);
                    free(auxArc);
                }
            }
            arcIterator = arcIterator->next;
        }
        arcIterator = originalNet->arcSet->head;
        netIterator = netIterator->next;
    }
}



void mergeSubNets(ioptNetSet * netSet, ioptNet * originalNet){
    
    int id1;
    int id2;
    
    while (true) {
        printf("\nPress '0' at any time to exit");
        printf("\n Merge subnet: ");
        scanf("%d", &id1);
    
        if (id1 !=0) {
            printf("\n With subnet: ");
            scanf("%d", &id2);
            
            if(id2 == 0){
                break;
            }
    
    
    
            ioptNet * net1 = getNet(netSet, id1);
    
            ioptNet * net2 = getNet(netSet, id2);
            
            if (net1 == NULL || net2 == NULL){
                printf("\nOne of the chosen subnets does not exist. Aborted.");
                printf("\n\n");
                exit(1);
            }
    
            ioptPlace * placeIterator = net1->placeSet->head;
            ioptTransition * transitionIterator = net1->transitionSet->head;
            ioptArc * arcIterator = net1->arcSet->head;
    
            while (placeIterator) {
                addToPlaceSet(*placeIterator, net2->placeSet);
                placeIterator = placeIterator->next;
            }
            while (transitionIterator) {
                addToTransitionSet(*transitionIterator, net2->transitionSet);
                transitionIterator = transitionIterator->next;
            }
            while (arcIterator) {
                addToArcSet(*arcIterator, net2->arcSet);
                arcIterator = arcIterator->next;
            }
            removeNet(id1, netSet);
        } else{
            return;
        }
    }
}

void reasignIO (ioptNet * originalNet, ioptNetSet * netSet){
    ioptNet * net = netSet->head;
    ioptTransition * transition;
    ioptTransition * originalTransition;
    //ioptPlace * place;
    ioptAssociatedEvent * associatedEvent;
    ioptAssociatedEvent * auxAssociatedEvent;
    ioptEvent * event;
    
    while (net) {
        transition = net->transitionSet->head;
        while (transition) {
            originalTransition = getTransition(originalNet->transitionSet, transition->id);
            if (originalTransition != NULL && originalTransition->inputEvents->size > 0) {
                associatedEvent = originalTransition->inputEvents->head;
                while (associatedEvent) {
                    event = originalNet->io->input->eventSet->head;
                    while (event) {
                        if (!strcmp(associatedEvent->eventID, event->id)) {
                            addEventToIO(*event, INPUT, net->io);
                            auxAssociatedEvent = newAssociatedEvent();
                            auxAssociatedEvent->eventID = strdup(event->id);
                            addEventToTransition(auxAssociatedEvent, INPUT, transition);
                        }
                        event = event->next;
                    }
                    associatedEvent = associatedEvent->next;
                }
            }
            if (originalTransition != NULL && originalTransition->outputEvents->size > 0) {
                associatedEvent = originalTransition->outputEvents->head;
                while (associatedEvent) {
                    event = originalNet->io->output->eventSet->head;
                    while (event) {
                        if (!strcmp(associatedEvent->eventID, event->id)) {
                            addEventToIO(*event, OUTPUT, net->io);
                            auxAssociatedEvent = newAssociatedEvent();
                            auxAssociatedEvent->eventID = strdup(event->id);
                            addEventToTransition(auxAssociatedEvent, OUTPUT, transition);
                        }
                        event = event->next;
                    }
                    associatedEvent = associatedEvent->next;
                }
            }
            transition = transition->next;
        }
        net = net->next;
    }
}








