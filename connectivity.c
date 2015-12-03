//
//  connectivity.c
//  pendo
//
//  Created by Rui Guerra on 10/12/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



#include "connectivity.h"
#include "ioptDataStructures.h"

#include "listPlace.h"
#include "listTransition.h"
#include "listArc.h"
#include "readLib.h"

#include"net.h"



ioptNodeSet * newNodeSet(){
    ioptNodeSet* nodeSet = malloc(sizeof(ioptNodeSet));
    if(nodeSet== NULL){
        printf("Error allocation nodeSet\n");
    }
    nodeSet->head = NULL;
    nodeSet->tail = NULL;
    nodeSet->size = 0;
    return nodeSet;
}

ioptNode * newNode(){
    ioptNode * node = malloc(sizeof(ioptNode));
    //node->visited = false;
    //node->checked = false;
    node->next = NULL;
    return node;
}

void addToNodeSet(ioptNode node, ioptNodeSet* nodeSet){
    ioptNode* newNode = malloc(sizeof(ioptNode));
    if(newNode == NULL){
        printf("Error allocating node\n");
    }
    *newNode = node;
    newNode->next = NULL;
    if(nodeSet->head == NULL){
        nodeSet->head = nodeSet->tail = newNode;
    }
    else{
        nodeSet->tail->next = newNode;
        nodeSet->tail = newNode;
    }
    nodeSet->size++;
}
ioptNodeSet * getNeighbourNodes(ioptArcSet * arcSet, ioptNode * origin){
    
    ioptNodeSet * nodeSet = newNodeSet();
    ioptArc * arc;
    
    arc = arcSet->head;
    while(arc != NULL){
        if (arc->source == origin->id){
            ioptNode * node = newNode();
            node->id = arc->target;
            addToNodeSet(*node, nodeSet);
            free(node);
        }
        if (arc->target == origin->id){
            ioptNode * node = newNode();
            node->id = arc->source;
            addToNodeSet(*node, nodeSet);
            free(node);
        }
        arc = arc->next;
    }
    return nodeSet;
}

void destroyNode (ioptNode * node){
    free(node);
}

void destroyNodeSet (ioptNodeSet * nodeSet){
    ioptNode * next = nodeSet->head;
    ioptNode * next2;
    while (next != NULL) {
        next2 = next->next;
        destroyNode (next);
        next = next2;
    }
    free(nodeSet);
}


void removeNode (ioptNodeSet * nodeSet, int id){
    ioptNode * ante = NULL;
    ioptNode * iterator = nodeSet->head;
    
    while (iterator) {
        if (iterator->id == id) {
            if (ante == NULL) {
                nodeSet->head = iterator->next;
                free(iterator);
                nodeSet->size--;
                break;
            }
            else {
                ante->next = iterator->next;
                free(iterator);
                nodeSet->size--;
                break;
            }
        }
        ante = iterator;
        iterator = iterator->next;
    }
}





void loadToNodeSet(ioptNodeSet * nodeSet, ioptPlaceSet * placeSet, ioptTransitionSet * trasintionSet){
    
    ioptPlace * place = placeSet->head;
    ioptTransition * transition = trasintionSet->head;
    ioptNode * node;
    
    while (place) {
        node = newNode();
        node->id = place->id;
        addToNodeSet(*node, nodeSet);
        free(node);
        place = place->next;
    }
    while (transition) {
        node = newNode();
        node->id = transition->id;
        addToNodeSet(*node, nodeSet);
        free(node);
        transition = transition->next;
    }
}

bool isConnected(ioptArcSet * arcSet, ioptNode * src, ioptNode * dest){
    ioptArc * arc = arcSet->head;
    while (arc) {
        if ((src->id == arc->source && dest->id == arc->target) || (src->id == arc->target && dest->id == arc->source)) {
            return true;
        }
        arc=arc->next;
    }
    return false;
}


bool isConnectedSet(ioptNodeSet * nodeSet, ioptArcSet * arcSet, ioptNode * node){
    
    ioptNode * iteratorNode = nodeSet->head;
    while (iteratorNode) {
        
        if (isConnected(arcSet, node, iteratorNode)) {
            return true;
        }
        iteratorNode = iteratorNode->next;
    }
    return false;
}


bool nodeExists(ioptNodeSet * nodeSet, ioptNode * node){
    ioptNode * iterator = nodeSet->head;
    while (iterator) {
        if (iterator->id == node->id) {
            return true;
        }
        iterator = iterator->next;
    }
    return false;
}

void search(ioptNode * origin, ioptNodeSet * nodeSet, ioptNodeSet * connected, ioptArcSet * arcSet){
    ioptNodeSet * neighbours = getNeighbourNodes(arcSet, origin);
    ioptNode * iterator = neighbours->head;
    
    if (neighbours->size > 0) {
        while (iterator) {
            if (!nodeExists(connected, iterator)) {
                addToNodeSet( * iterator, connected);
                search(iterator, nodeSet, connected, arcSet);
            }
            iterator = iterator->next;
        }
    } else{
        addToNodeSet( * origin, connected);
    }
    
}

bool connected(ioptNet * net){
    ioptNodeSet * connected = newNodeSet();
    ioptNodeSet * nodeSet = newNodeSet();
    ioptNode * origin;
    
    loadToNodeSet(nodeSet, net->placeSet, net->transitionSet);
    origin = nodeSet->head;
    
    search(origin, nodeSet, connected, net->arcSet);
    
    if (connected->size == (net->placeSet->size + net->transitionSet->size)) {
        return true;
    }
    return false;
}

void addUniqueToNodeSet(ioptNode node, ioptNodeSet * nodeSet){
    ioptNode * iterator = newNode();
    iterator = nodeSet->head;
    bool exists = false;
    
    while (iterator){
        if (node.id == iterator->id) {
            exists = true;
            break;
        }
        iterator = iterator->next;
    }
    if (exists == false) {
        addToNodeSet(node, nodeSet);
    }
    
    
}

void teste (ioptNet * net){
    ioptNodeSet * nodeSet = newNodeSet();
    
    loadToNodeSet(nodeSet, net->placeSet, net->transitionSet);
    removeNode(nodeSet, 3);
    

}


