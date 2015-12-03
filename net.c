//
//  net.c
//  pendo
//
//  Created by Rui Guerra on 11/1/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "listPlace.h"
#include "listArc.h"
#include "listTransition.h"
#include "listIO.h"
#include "net.h"

ioptNet * newNet(){
    ioptNet * net = malloc(sizeof(ioptNet));
    if(net == NULL){
        printf("Error creating net\n");
    }
    net->placeSet = newPlaceSet();
    net->arcSet = newArcSet();
    net->transitionSet = newsTransitionSet();
    net->io = newIO();
    net->next = NULL;
    net->id = 0;
    return net;    
}

ioptNetSet * newNetSet(){
    ioptNetSet * netSet = malloc(sizeof(ioptNetSet));
    netSet->head = NULL;
    netSet->tail = NULL;
    netSet->size = 0;
    netSet->highID = 0;
    return netSet;
}

void addToNetSet(ioptNet net, ioptNetSet * netSet){
    ioptNet * newNet = malloc(sizeof(ioptNet));
    if(newNet == NULL){
        printf("Error allocating net\n");
    }
    * newNet = net;
    newNet->id = netSet->size + 1;
    if(netSet->head == NULL){
        netSet->head = netSet->tail = newNet;
    }
    else{
        netSet->tail->next = newNet;
        netSet->tail = newNet;
    }
    netSet->size++;
}

ioptNet * getNet(ioptNetSet * netSet, int id){
    ioptNet * netIterator = netSet->head;
    while (netIterator) {
        if (netIterator->id == id) {
            return netIterator;
        }
        netIterator = netIterator->next;
    }
    return NULL;
}

void removeNet(int id, ioptNetSet * netSet){
    ioptNet * anteNet = NULL;
    ioptNet * postNet = NULL;
    
    ioptNet * netIterator = netSet->head;
    if (netIterator->id == id) {
        postNet = netIterator->next;
        free(netIterator);
        netSet->head = postNet;
        netSet->size--;
    }else{
        anteNet = netIterator;
        netIterator = netIterator->next;
        postNet = netIterator->next;
        while (netIterator) {
            if (netIterator->id == id) {
                if (netIterator->next == netSet->tail) {
                    netSet->tail = anteNet;
                }
                
                anteNet->next = postNet;
                free(netIterator);
                netSet->size--;
                break;
            }
        anteNet = netIterator;
        netIterator = netIterator->next;
        postNet = netIterator->next;
        }
    }
    
    
}