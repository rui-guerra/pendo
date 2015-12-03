//
//  rules.c
//  pendo
//
//  Created by Rui Guerra on 19/05/14.
//  Copyright (c) 2014 Rui Guerra. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "listPlace.h"
#include "listArc.h"
#include "listTransition.h"
#include "listIO.h"
#include "net.h"
#include "interface.h"
#include "conditions.h"
#include "connectivity.h"
#include "rules.h"
#include "ioptDataStructures.h"
#include "split.h"



int getNodeType (ioptNode * node, ioptNet * originalNet){
    ioptPlace * placeIterator = originalNet->placeSet->head;
    ioptTransition * transitionIterator = originalNet->transitionSet->head;
    
    while (placeIterator) {
        if (node->id == placeIterator->id) {
            return PLACE;
        }
        placeIterator = placeIterator->next;
    }
    while (transitionIterator) {
        if (node->id == transitionIterator->id) {
            return TRANSITION;
        }
        transitionIterator = transitionIterator->next;
    }
    return -1;
}

void rules(ioptNetSet * netSet, ioptNet * originalNet, ioptNodeSet * cutSet){
    ioptNode * cutNode = cutSet->head;
    while (cutNode) {
        if (getNodeType(cutNode, originalNet) == PLACE) {
            ruleOne(netSet, originalNet, getPlace(originalNet->placeSet, cutNode->id));
        } else if(getNodeType(cutNode, originalNet) == TRANSITION){
            ruleTwo(netSet, originalNet, getTransition(originalNet->transitionSet, cutNode->id));
            //rule two
            //rule three
        }
        
        cutNode = cutNode->next;
    }
    
}


void ruleOne(ioptNetSet * netSet, ioptNet * originalNet, ioptPlace * place){
    
    ioptTransition * preTransitionIterator;
    ioptTransition * auxTransition;
  //  ioptTransition * transition;
    ioptArc * arcIterator;
    ioptArc * auxArc;
    ioptSignal * signal;
    ioptEvent * event;
    ioptAssociatedEvent * associatedEvent;
    char signalID [100];
    char eventID [100];
    
    char copy [] = "_copy";
    
    int placeDestiny = getPlaceDestinySubnet(netSet, originalNet, place);
    if (placeDestiny == -1) {
        printf("\nNot on same net\n");
        exit(1);
    }
    ioptNet * destinySubNet = getNet(netSet, placeDestiny);
    addToPlaceSet(* place, destinySubNet->placeSet);
    printNets(netSet);
    reconstructArcs(netSet, originalNet);
    int IDStart = getNetSetHighID(netSet) + 1;
    ioptTransitionSet * preTransitions = getAnteTrantitions(netSet, originalNet, place->id);
    
    preTransitionIterator = preTransitions->head;
    while (preTransitionIterator) {
        auxTransition = createTransitionCopy(preTransitionIterator);
        auxTransition->id = IDStart;
 
        strcat(auxTransition->name.name, copy);
        addToTransitionSet(*auxTransition, destinySubNet->transitionSet);
        
        
        signal = newSignal();
        event = newEvent();
        
        sprintf(signalID, "signal_T_%d_%d", preTransitionIterator->id, auxTransition->id);
        signal->id =strdup(signalID);
        addSignalToIO(*signal, INPUT, destinySubNet->io);
        addSignalToIO(*signal, OUTPUT, getNet(netSet, getTransitionSubNet(netSet, preTransitionIterator))->io);
        
        sprintf(eventID, "event_%d_%d", preTransitionIterator->id, auxTransition->id);
        event->id = strdup(eventID);
        event->signal = strdup(signalID);
        addEventToIO(*event, INPUT, destinySubNet->io);
        addEventToIO(*event, OUTPUT, getNet(netSet, getTransitionSubNet(netSet, preTransitionIterator))->io);
        
        associatedEvent = newAssociatedEvent();
        associatedEvent->eventID = strdup(eventID);
        
        //transition = getTransition(getNet(netSet, getTransitionSubNet(netSet, preTransitionIterator))->transitionSet, preTransitionIterator->id);
        
        addEventToTransition(associatedEvent, OUTPUT, getTransition(getNet(netSet, getTransitionSubNet(netSet, preTransitionIterator))->transitionSet, preTransitionIterator->id));
        addEventToTransition(associatedEvent, INPUT, auxTransition);
        
        free(signal);
        free(event);
        free(associatedEvent);
        
        
        
        
        arcIterator = originalNet->arcSet->head;
        while (arcIterator) {
            if (arcIterator->source == preTransitionIterator->id && arcIterator->target == place->id) {
                auxArc = createArcCopy(arcIterator);
                auxArc->source = IDStart;
                addToArcSet(*auxArc, destinySubNet->arcSet);
                free(auxArc);
            }
            
            
            arcIterator = arcIterator->next;
        }
        
        IDStart++;
        arcIterator = originalNet->arcSet->head;
        free(auxTransition);
        preTransitionIterator = preTransitionIterator->next;
    }
    reconstructArcs(netSet, originalNet);

}

void ruleTwo(ioptNetSet * netSet, ioptNet * originalNet, ioptTransition * transition){
    char master [] = "_master";
    char slave [] = "_slave";
    int IDStart;
    char signalID [100];
    char eventID [100];
    ioptEvent * event;
    ioptSignal * signal;
    ioptPlace * postPlaceIterator;
    int auxSubNet;
    ioptTransition * auxSlaveTransition;
    ioptArc * arcIterator;
    ioptAssociatedEvent * associatedEvent;
    int transitionDestiny = getTransitionDestinySubNet(netSet, originalNet, transition);
    printNets(netSet);
    if (transitionDestiny == -1) {
        
        
        ruleThree(netSet, originalNet, transition);
        //printf("\n rule 3!\n");
       // exit (1);
    } else{
    
    ioptNet * destinySubNet = getNet(netSet, transitionDestiny);
    
    ioptTransition * auxTransition = createTransitionCopy(transition);
    strcat(auxTransition->name.name, master);
    addToTransitionSet(*auxTransition, destinySubNet->transitionSet);
    reconstructArcs(netSet, originalNet);
    
    
    ioptNet * slaveDestinySubNet;
    ioptArc * auxArc;
    IDStart = getNetSetHighID(netSet) + 1;
    
    ioptPlaceSet * postPlaces = getPostPlaces(netSet, originalNet, transition->id);
    postPlaceIterator = postPlaces->head;
    
    while (postPlaceIterator) {
        auxSubNet = getPlaceSubNet(netSet, postPlaceIterator);
        if (auxSubNet != transitionDestiny) {
            auxSlaveTransition = createTransitionCopy(transition);
            
            
            
            
            
            
            arcIterator = originalNet->arcSet->head;
            while (arcIterator) {
                if (arcIterator->source == transition->id && arcIterator->target == postPlaceIterator->id) {
                    auxArc = createArcCopy(arcIterator);
                    break;
                }
                arcIterator = arcIterator->next;
            }
            
            slaveDestinySubNet = getNet(netSet, auxSubNet);
            
            
            auxSlaveTransition->id = IDStart;
            strcat(auxSlaveTransition->name.name, slave);
            addToTransitionSet(*auxSlaveTransition, slaveDestinySubNet->transitionSet);
            
            signal = newSignal();
            event = newEvent();
            
            sprintf(signalID, "signal_T_%d_%d", transition->id, auxSlaveTransition->id);
            signal->id =strdup(signalID);
            addSignalToIO(*signal, OUTPUT, destinySubNet->io);
            addSignalToIO(*signal, INPUT, slaveDestinySubNet->io);
            
            sprintf(eventID, "event_%d_%d", transition->id, auxSlaveTransition->id);
            event->id = strdup(eventID);
            event->signal = strdup(signalID);
            addEventToIO(*event, OUTPUT, destinySubNet->io);
            addEventToIO(*event, INPUT, slaveDestinySubNet->io);
            
            associatedEvent = newAssociatedEvent();
            associatedEvent->eventID = strdup(eventID);
            addEventToTransition(associatedEvent, OUTPUT, auxTransition);
            addEventToTransition(associatedEvent, INPUT, auxSlaveTransition);
            
            free(signal);
            free(event);
            free(associatedEvent);
            
            
            
            auxArc->source = auxSlaveTransition->id;
            addToArcSet(*auxArc, slaveDestinySubNet->arcSet);
            
            arcIterator = originalNet->arcSet->head;
            free(auxArc);
            //free(slaveDestinySubNet);
            free(auxSlaveTransition);
        }
        
        postPlaceIterator = postPlaceIterator->next;
    }
    
    
    
    
    
    
    
    
    printNets(netSet);
    }
    
    
}


void ruleThree (ioptNetSet * netSet, ioptNet * originalNet, ioptTransition * transition){
    int destinySubNetID = getCondition(transition);
    
    
    if ( getNet(netSet, destinySubNetID) == NULL){
        printf("\nThere is no subnet with the chosen ID. Aborted\n");
        exit(1);
    }
    
    char master [] = "_master";
    char slave [] = "_slave";
    char copy [] = "_copy ";
    
    char signalID [100];
    char eventID [100];
    
    ioptNet * transitionCopySubNet;
    ioptPlace * antePlaceIterator;
    ioptPlace * postPlaceIterator;
    ioptPlace * auxPlace;
    ioptPlace * placeIterator;
    ioptArc * arcIterator;
    ioptArc * auxArc;
    ioptTransitionSet * auxTransitionSet;
    ioptTransition * transitionIterator;
    ioptNet * auxSubNet;
   // ioptTransition * auxTransition;
    
    ioptEvent * event;
    ioptSignal * signal;
    ioptAssociatedEvent * associatedEvent;
    
    ioptNet * destinySubNet = getNet(netSet, destinySubNetID);
    ioptTransition * auxTransition = createTransitionCopy(transition);
    strcat(auxTransition->name.name, master);
    addToTransitionSet(*auxTransition, destinySubNet->transitionSet);
    free(auxTransition);
    reconstructArcs(netSet, originalNet);
    printNets(netSet);
    int IDstart = getNetSetHighID(netSet) +1;
    
    
    ioptPlaceSet * antePlaces = getAntePlaces(netSet, originalNet, transition->id);
    
    antePlaceIterator = antePlaces->head;
    while (antePlaceIterator) {
        if (getPlaceSubNet(netSet, antePlaceIterator) != destinySubNetID) {

            auxPlace = createPlaceCopy(antePlaceIterator);
            strcat(auxPlace->name.name, copy);
            auxPlace->id = IDstart;
            IDstart++;
            addToPlaceSet(*auxPlace, destinySubNet->placeSet);
            
            arcIterator = originalNet->arcSet->head;
            while (arcIterator) {
                if (arcIterator->source == antePlaceIterator->id && arcIterator->target == transition->id) {
                    auxArc = createArcCopy(arcIterator);
                    auxArc->source = auxPlace->id;
                    auxArc->id = IDstart;
                    IDstart++;
                    addToArcSet(*auxArc, destinySubNet->arcSet);
                    free(auxArc);
                    break;
                }
                arcIterator = arcIterator->next;
            }
            
            auxTransitionSet = getAnteTrantitions(netSet, originalNet, antePlaceIterator->id);
            transitionIterator = auxTransitionSet->head;
            while (transitionIterator) {
                auxTransition = createTransitionCopy(transitionIterator);
                auxSubNet = getNet(netSet, getTransitionSubNet(netSet, transitionIterator));
                strcat(getTransition(auxSubNet->transitionSet, transitionIterator->id)->name.name, master);
                
                strcat(auxTransition->name.name, slave);
                auxTransition->id = IDstart;
                IDstart++;
                addToTransitionSet(*auxTransition, destinySubNet->transitionSet);
                
                arcIterator = originalNet->arcSet->head;
                while (arcIterator) {
                    if (arcIterator->source == transitionIterator->id && arcIterator->target == antePlaceIterator->id) {
                        auxArc = createArcCopy(arcIterator);
                        auxArc->source = auxTransition->id;
                        auxArc->target = auxPlace->id;
                        auxArc->id = IDstart;
                        IDstart++;
                        addToArcSet(*auxArc, destinySubNet->arcSet);
                        free(auxArc);
                    }
                    
                    
                    arcIterator = arcIterator->next;
                }
                
                
                /*********/
                signal = newSignal();
                event = newEvent();
                
                sprintf(signalID, "signal_T_%d_%d", auxTransition->id, transitionIterator->id);
                signal->id =strdup(signalID);
                addSignalToIO(*signal, OUTPUT, destinySubNet->io);
                addSignalToIO(*signal, INPUT, auxSubNet->io);
                
                sprintf(eventID, "event_%d_%d", auxTransition->id, transitionIterator->id);
                event->id = strdup(eventID);
                event->signal = strdup(signalID);
                addEventToIO(*event, OUTPUT, destinySubNet->io);
                addEventToIO(*event, INPUT, auxSubNet->io);
                
                associatedEvent = newAssociatedEvent();
                associatedEvent->eventID = strdup(eventID);
                addEventToTransition(associatedEvent, OUTPUT, auxTransition);
                addEventToTransition(associatedEvent, INPUT, transitionIterator);
                
                free(signal);
                free(event);
                free(associatedEvent);
                /*********/
                
                
                transitionIterator = transitionIterator->next;
            }
            transitionCopySubNet = getNet(netSet, getPlaceSubNet(netSet, antePlaceIterator));
            auxTransition = createTransitionCopy(transition);
            auxTransition->id = IDstart;
            IDstart++;
            strcat(auxTransition->name.name, slave);
            addToTransitionSet(*auxTransition, transitionCopySubNet->transitionSet);
            
            arcIterator = originalNet->arcSet->head;
            while (arcIterator) {
                if (arcIterator->source == antePlaceIterator->id && arcIterator->target == transition->id) {
                    auxArc = createArcCopy(arcIterator);
                    auxArc->id = IDstart;
                    IDstart++;
                    auxArc->target = auxTransition->id;
                    addToArcSet(*auxArc, transitionCopySubNet->arcSet);
                    free(auxArc);
                }
                
                placeIterator = transitionCopySubNet->placeSet->head;
                while (placeIterator) {
                    if (arcIterator->source == transition->id && arcIterator->target == placeIterator->id) {
                        auxArc = createArcCopy(arcIterator);
                        auxArc->source = auxTransition->id;
                        addToArcSet(*auxArc, transitionCopySubNet->arcSet);
                        free(auxArc);
                    }
                    placeIterator = placeIterator->next;
                }
                
                
                arcIterator = arcIterator->next;
            }
            signal = newSignal();
            event = newEvent();

            sprintf(signalID, "signal_T_%d_%d", transition->id, auxTransition->id);
            signal->id =strdup(signalID);
            addSignalToIO(*signal, OUTPUT, transitionCopySubNet->io);
            addSignalToIO(*signal, INPUT, destinySubNet->io);

            sprintf(eventID, "event_%d_%d", transition->id, auxTransition->id);
            event->id = strdup(eventID);
            event->signal = strdup(signalID);
            addEventToIO(*event, OUTPUT, transitionCopySubNet->io);
            addEventToIO(*event, INPUT, destinySubNet->io);

            associatedEvent = newAssociatedEvent();
            associatedEvent->eventID = strdup(eventID);
            addEventToTransition(associatedEvent, OUTPUT, transition);
            addEventToTransition(associatedEvent, INPUT, auxTransition);

            free(signal);
            free(event);
            free(associatedEvent);
            free(auxPlace);
        }
        antePlaceIterator = antePlaceIterator->next;
    }
    
    reconstructArcs(netSet, originalNet);
    
    //ioptNet * auxNet;
    
    
    
    
    
    ioptPlaceSet * postPlace = getPostPlaces(netSet, originalNet, transition->id);
    postPlaceIterator = postPlace->head;
//    while (postPlaceIterator) {
//        
//        auxNet = getNet(netSet, getPlaceSubNet(netSet, postPlaceIterator));
//        if (auxNet->id != destinySubNet->id) {
//            
//            auxTransition = createTransitionCopy(transition);
//            auxTransition->id = IDstart;
//            IDstart++;
//            strcat(auxTransition->name.name, slave);
//            addToTransitionSet(*auxTransition, auxNet->transitionSet);
//            
//            arcIterator = originalNet->arcSet->head;
//            while (arcIterator) {
//                if (arcIterator->source == postPlaceIterator->id && arcIterator->target == transition->id) {
//                    auxArc = createArcCopy(arcIterator);
//                    auxArc->id = IDstart;
//                    IDstart++;
//                    auxArc->target = auxTransition->id;
//                    addToArcSet(*auxArc, auxNet->arcSet);
//                    free(auxArc);
//                }
//                
//                placeIterator = auxNet->placeSet->head;
//                while (placeIterator) {
//                    if (arcIterator->source == transition->id && arcIterator->target == placeIterator->id) {
//                        auxArc = createArcCopy(arcIterator);
//                        auxArc->source = auxTransition->id;
//                        addToArcSet(*auxArc, auxNet->arcSet);
//                        free(auxArc);
//                    }
//                    placeIterator = placeIterator->next;
//                }
//                arcIterator = arcIterator->next;
//            }
//            
//        }
//        
//        
//        /*********/
//        
//        signal = newSignal();
//        event = newEvent();
//        
//        sprintf(signalID, "signal_T_%d_%d", transition->id, auxTransition->id);
//        signal->id =strdup(signalID);
//        addSignalToIO(*signal, OUTPUT, destinySubNet->io);
//        addSignalToIO(*signal, INPUT, auxNet->io);
//        
//        sprintf(eventID, "event_%d_%d", transition->id, auxTransition->id);
//        event->id = strdup(eventID);
//        event->signal = strdup(signalID);
//        addEventToIO(*event, OUTPUT, destinySubNet->io);
//        addEventToIO(*event, INPUT, auxNet->io);
//        
//        associatedEvent = newAssociatedEvent();
//        associatedEvent->eventID = strdup(eventID);
//        addEventToTransition(associatedEvent, OUTPUT, transition);
//        addEventToTransition(associatedEvent, INPUT, auxTransition);
//        
//        free(signal);
//        free(event);
//        free(associatedEvent);
//        /*********/
//        
//        
//        
//        
//        postPlaceIterator = postPlaceIterator->next;
//    }
    
    
    printNets(netSet);
    
    
}






int getNetSetHighID(ioptNetSet * netSet){
    ioptNet * netIterator = netSet->head;
    ioptPlace * placeIterator;
    ioptTransition * transitionIterator;
    ioptArc * arcIterator;
    int highID = -1;
    
    while (netIterator) {
        placeIterator = netIterator->placeSet->head;
        while (placeIterator) {
            if (placeIterator->id > highID) {
                highID = placeIterator->id;
            }
            placeIterator = placeIterator->next;
        }
        transitionIterator = netIterator->transitionSet->head;
        while (transitionIterator) {
            if (transitionIterator->id > highID) {
                highID = transitionIterator->id;
            }
            transitionIterator = transitionIterator->next;
        }
        arcIterator = netIterator->arcSet->head;
        while (arcIterator) {
            if (arcIterator->id > highID) {
                highID = arcIterator->id;
            }
            arcIterator = arcIterator->next;
        }
        
        netIterator = netIterator->next;
    }
    return highID;
}

int getTransitionDestinySubNet(ioptNetSet * netSet, ioptNet * originalNet, ioptTransition * transition){
    
    ioptPlaceSet * placeSet = newPlaceSet();
    ioptPlace * placeIterator = originalNet->placeSet->head;
    ioptArc * arcIterator = originalNet->arcSet->head;
    ioptPlace * auxPlace;
    int transitionDestiny = 0;
    
    while (arcIterator) {
        if (arcIterator->target == transition->id) {
            while (placeIterator) {
                if (placeIterator->id == arcIterator->source) {
                    auxPlace = createPlaceCopy(placeIterator);
                    addToPlaceSet(*auxPlace, placeSet);
                    free(auxPlace);
                    break;
                }
                placeIterator = placeIterator->next;
            }
            placeIterator = originalNet->placeSet->head;
        }
        arcIterator = arcIterator->next;
    }
    placeIterator = placeSet->head;
    transitionDestiny = getPlaceSubNet(netSet, placeIterator);
    while (placeIterator) {
        if (getPlaceSubNet(netSet, placeIterator) != transitionDestiny) {
            return -1;
        }
        placeIterator = placeIterator->next;
    }
    return transitionDestiny;
}

int getPlaceDestinySubnet(ioptNetSet * netSet, ioptNet * originalNet, ioptPlace * place){
    
    ioptTransitionSet * transitionSet = newsTransitionSet();
    ioptTransition * transitionIterator = originalNet->transitionSet->head;
    ioptArc * arcIterator = originalNet->arcSet->head;
    int placeDestiny = 0;
    ioptTransition * auxTransition;
    
    while (arcIterator) {
        if (arcIterator->source == place->id) {
            while (transitionIterator) {
                if (transitionIterator->id == arcIterator->target) {
                    //auxTransition = newTransition();
                    auxTransition = createTransitionCopy(transitionIterator);
                    //copyTransition(transitionIterator, auxTransition);
                    addToTransitionSet(* auxTransition, transitionSet);
                    free(auxTransition);
                    break;
                }
                transitionIterator = transitionIterator->next;
            }
        }
        arcIterator = arcIterator->next;
    }
    transitionIterator = transitionSet->head;
    placeDestiny = getTransitionSubNet(netSet, transitionIterator);
    while (transitionIterator) {
        if (getTransitionSubNet(netSet, transitionIterator) != placeDestiny) {
            
            return -1;
        }
        transitionIterator = transitionIterator->next;
    }
    
    
    return placeDestiny;
}

int getPlaceSubNet(ioptNetSet * netSet, ioptPlace * place){
    ioptNet * netIterator = netSet->head;
    ioptPlace * placeIterator;
    while (netIterator) {
        placeIterator = netIterator->placeSet->head;
        while (placeIterator) {
            if (place->id == placeIterator->id) {
                return netIterator->id;
            }
            placeIterator = placeIterator->next;
        }
        netIterator = netIterator->next;
    }
    return -1;
}
int getTransitionSubNet(ioptNetSet * netSet, ioptTransition * transition){
    ioptNet * netIterator = netSet->head;
    ioptTransition * transitionIterator;
    while (netIterator) {
        transitionIterator = netIterator->transitionSet->head;
        while (transitionIterator) {
            if (transition->id == transitionIterator->id) {
                return netIterator->id;
            }
            
            transitionIterator = transitionIterator->next;
        }
        netIterator = netIterator->next;
    }
    return -1;
}

ioptPlaceSet * getPostPlaces(ioptNetSet * netSet, ioptNet * originalNet, int transitionID){
    ioptPlaceSet * placeSet = newPlaceSet();
    ioptArc * arcIterator = originalNet->arcSet->head;
    ioptPlace * placeIterator = originalNet->placeSet->head;
    ioptPlace * placeCopy;
    
    while (arcIterator) {
        if (arcIterator->source == transitionID) {
            while (placeIterator) {
                if (placeIterator->id == arcIterator->target) {
                    placeCopy = createPlaceCopy(placeIterator);
                    addToPlaceSet(*placeCopy, placeSet);
                    free(placeCopy);
                    placeIterator = originalNet->placeSet->head;
                    break;
                }
                placeIterator = placeIterator->next;
            }
        }
        arcIterator = arcIterator->next;
    }
    return placeSet;
}

ioptPlaceSet * getAntePlaces(ioptNetSet * netSet, ioptNet * originalNet, int transitionID){
    ioptPlaceSet * placeSet = newPlaceSet();
    ioptArc * arcIterator = originalNet->arcSet->head;
    ioptPlace * placeIterator = originalNet->placeSet->head;
    ioptPlace * placeCopy;
    
    while (arcIterator) {
        if (arcIterator->target == transitionID) {
            while (placeIterator) {
                if (placeIterator->id == arcIterator->source) {
                    placeCopy = createPlaceCopy(placeIterator);
                    addToPlaceSet(*placeCopy, placeSet);
                    free(placeCopy);
                    placeIterator = originalNet->placeSet->head;
                    break;
                }
                placeIterator = placeIterator->next;
            }
        }
        arcIterator = arcIterator->next;
    }
    return placeSet;
}

ioptTransitionSet * getAnteTrantitions (ioptNetSet * netSet, ioptNet * originalNet, int placeID){
    ioptTransitionSet * transitionSet = newsTransitionSet();
    ioptArc * arcIterator = originalNet->arcSet->head;
    ioptTransition * transitionIterator = originalNet->transitionSet->head;
    ioptTransition * transitionCopy;
    
    while (arcIterator) {
        if (arcIterator->target == placeID) {
            while (transitionIterator) {
                if (transitionIterator->id == arcIterator->source) {
                    
                    transitionCopy = createTransitionCopy(transitionIterator);

                    addToTransitionSet( * transitionCopy, transitionSet);
                    free(transitionCopy);
                    transitionIterator = originalNet->transitionSet->head;
                    break;
                }
                transitionIterator = transitionIterator->next;
            }
        }
        arcIterator = arcIterator->next;
    }
    return transitionSet;
}


