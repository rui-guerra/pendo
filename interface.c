//
//  interface.c
//  pendo
//
//  Created by Rui Guerra on 19/05/14.
//  Copyright (c) 2014 Rui Guerra. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ioptDataStructures.h"
#include "listPlace.h"
#include "listArc.h"
#include "listTransition.h"
#include "net.h"
#include "interface.h"
#include "conditions.h"
#include "connectivity.h"
#include "rules.h"


void printNets (ioptNetSet * netSet){
    //system("clear");
	printf("\033[2J");
    ioptNet * net = netSet->head;
    while (net) {
        printNet(net);
        net = net->next;
    }
}

void printNet (ioptNet * net){
    ioptPlace * place = net->placeSet->head;
    ioptTransition * transition = net->transitionSet->head;
    ioptArc * arc = net->arcSet->head;
    ioptEvent * event;
    ioptSignal * signal;
    printf("******************************************************************\n\n");
    printf("NET %2d\n", net->id);
    printf("***************\n\n");
    printf("PLACE ID             PLACE NAME\n");
    while (place) {
        printf("%4d               %s\n", place->id, place->name.name);
        place= place->next;
    }
    printf("\n");
    printf("TRANSITION ID        TRANSITION NAME\n");
    while (transition) {
        printf("%4d               %s\n", transition->id, transition->name.name);
        transition = transition->next;
    }
    printf("\n");
    printf("ARC ID      SOURCE     TARGET\n");
    while (arc) {
        printf("%4d       %2d     %2d\n", arc->id, arc->source, arc->target);
        arc = arc->next;
    }
    event = net->io->input->eventSet->head;

    printf("\n");
    printf("\n");
    printf("INPUT\n");
    printf("\n");
    printf("EVENT\n");
    while (event) {
        printf("%s        %s\n", event->id, event->signal);
        event = event->next;
    }
    signal = net->io->input->signalSet->head;
    printf("\n");
    printf("SIGNAL\n");
    while (signal) {
        printf("%s\n", signal->id);
        signal = signal->next;
    }
    event=net->io->output->eventSet->head;
    printf("\n");
    printf("\n");
    printf("OUTPUT\n");
    printf("\n");
    printf("EVENT\n");
    while (event) {
        printf("%s        %s\n", event->id, event->signal);
        event = event->next;
    }
    signal = net->io->output->signalSet->head;
    printf("\n");
    printf("SIGNAL\n");
    while (signal) {
        printf("%s\n", signal->id);
        signal = signal->next;
    }

    
    
    printf("\n");
}

ioptNodeSet * getCuttingSet(){
    
    ioptNodeSet * cuttingSet = newNodeSet();
    
    char input [256];
    char * ptr;
    char delimiters[] = " ,.-";
    ioptNode * node;
    printf("\nInsert cutting set IDs, separate by commas.");
    printf("\nCutting Set:");
    if (scanf("%[^\n]s", input) > 0){
        ptr = strtok(input, delimiters);
        while (ptr != NULL) {
            if((int)strtol(ptr, NULL, 10) > 0){
                node = newNode();
                node->id = (int)strtol(ptr, NULL, 10);
                addToNodeSet(*node, cuttingSet);
                free(node);
            }
            ptr = strtok(NULL, delimiters);
        }
        return cuttingSet;
    }
    return NULL;
}

conditionSet * getConditions(ioptNodeSet * cuttingSet, ioptNet * originalNet){
    conditionSet * conditions = newConditionSet();
    
    condition * cond;
    ioptNode * iterator = cuttingSet->head;
    
    while (iterator) {

        if (getNodeType(iterator, originalNet) == TRANSITION) {
            
            cond = newCondition();
            printf("\nInsert node %d in net:", iterator->id);
            scanf("%d", &cond->net);
            cond->node = iterator->id;
            addToConditionSet(*cond, conditions);
            free(cond);
        }
        iterator = iterator->next;
    }
    return conditions;
}

int getCondition (ioptTransition * transition){
    printf("\nDestiny subnet for transition %d: ", transition->id);
    int destiny;
    scanf("%d", &destiny);
    return destiny;
}
