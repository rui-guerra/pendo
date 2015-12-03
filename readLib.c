//
//  readLib.c
//  pendo
//
//  Created by Rui Guerra on 14/11/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ioptDataStructures.h"
#include "listPlace.h"
#include "listTransition.h"
#include "listArc.h"
#include "readLib.h"

#include "listIO.h"

void readPlace(xmlNode * node, ioptPlace *place){
    
    xmlNode *cur_node = NULL;
    
    for (cur_node = node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            //printf("node type: Element, name: %s\n", cur_node->name);
            if(strcmp((char*)cur_node->name, "place") == 0){
                place->id = atoi(xmlGetProp(cur_node, "id"));
            }
            
            if (strcmp((char*)cur_node->name, "text") == 0) {
                if (strcmp((char*)cur_node->parent->name, "name") == 0) {
                //    strcpy(place->name.name, (char*)cur_node->children->content);
                    place->name.name = (char*)cur_node->children->content;
                }
                if (strcmp((char*)cur_node->parent->name, "comment") == 0) {
                    //if (cur_node->next->content != NULL) {
                    if (cur_node->children != NULL && cur_node->children->content != NULL) {

                        //strcpy(place->comment.comment, "cde");//(char*)cur_node->children->content);
                        place->comment.comment = strdup(cur_node->children->content);
                        //place->comment.comment = cur_node->next->content;
                        //printf("%s\n", place->comment.comment);
                    }
                    else{
                        place->comment.comment = NULL;
                    }
                }
                if (strcmp((char*)cur_node->parent->name, "initialMarking") == 0) {
                    place->initialMarking.initialMarking = atoi(cur_node->children->content);
                }
                if (strcmp((char*)cur_node->parent->name, "bound") == 0) {
                    place->bound = atoi(cur_node->children->content);
                }
            }
            
            if (strcmp((char*)cur_node->name, "offset") == 0) {
                if (strcmp((char*)cur_node->parent->parent->name, "name") == 0) {
                    place->name.graphicsOffset.x = atoi(xmlGetProp(cur_node, "x"));
                    place->name.graphicsOffset.y = atoi(xmlGetProp(cur_node, "y"));
                }
                if (strcmp((char*)cur_node->parent->parent->name, "comment") == 0){
                    place->comment.graphicsOffset.x = atoi(xmlGetProp(cur_node, "x"));
                    place->comment.graphicsOffset.y = atoi(xmlGetProp(cur_node, "y"));
                }
                if (strcmp((char*)cur_node->parent->parent->name, "initialMarking") == 0) {
                    place->initialMarking.graphicsOffset.x = atoi(xmlGetProp(cur_node, "x"));
                    place->initialMarking.graphicsOffset.y = atoi(xmlGetProp(cur_node, "y"));
                }
            }

            if (strcmp((char*)cur_node->name, "position") == 0) {
                place->graphicsPosition.page = atoi(xmlGetProp(cur_node, "page"));
                place->graphicsPosition.x = atoi(xmlGetProp(cur_node, "x"));
                place->graphicsPosition.y = atoi(xmlGetProp(cur_node, "y"));

            }
            
        }
        readPlace(cur_node->children, place);
    }
}

void readAllPlaces2(ioptPlaceSet *placeSet, xmlDocPtr doc){
    int i;
    xmlXPathObjectPtr rawSet = getNodeSet(doc, "//place");
    for(i = 0; i < rawSet->nodesetval->nodeNr; i++){
        if (rawSet->nodesetval->nodeNr >1) {
            rawSet->nodesetval->nodeTab[i]->next->next = NULL;
        }
        ioptPlace * place = newPlace();
        readPlace(rawSet->nodesetval->nodeTab[i], place);
        addToPlaceSet(*place, placeSet);
        free(place);
    }
}
//COPY
void readAllPlaces(ioptPlaceSet *placeSet, xmlDocPtr doc){
    int i;
    xmlXPathObjectPtr rawSet = getNodeSet(doc, "//place");
    for(i = 0; i < rawSet->nodesetval->nodeNr; i++){
        
        xmlNodePtr placeCopy = xmlCopyNodeList(rawSet->nodesetval->nodeTab[i]);
        placeCopy->next->next = NULL;
        ioptPlace * place = newPlace();
        readPlace(placeCopy, place);
        addToPlaceSet(*place, placeSet);
        free(place);
    }
}
//ENDCOPY

xmlXPathObjectPtr getNodeSet(xmlDocPtr doc, xmlChar* xpath){
    xmlXPathContextPtr context;
    xmlXPathObjectPtr result;
    
    context = xmlXPathNewContext(doc);
    if(context == NULL)
    {
        printf("Error in xmlXpathNewContext\n");
        return NULL;
    }
    result = xmlXPathEvalExpression(xpath, context);
    if(result == NULL)
    {
        printf("Error in xmlXPathEvalExpression\n");
        return NULL;
    }
    if(xmlXPathNodeSetIsEmpty(result->nodesetval))
    {
        xmlXPathFreeObject(result);
        printf("No results\n");
        return NULL;
    }
    return result;
}


void print_element_names(xmlNode * a_node){
    xmlNode *cur_node = NULL;
    for (cur_node = a_node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            printf("node type: Element, name: %s\n", cur_node->name);
        }
        print_element_names(cur_node->children);
    }
}

void readAllTransitions2(ioptTransitionSet * transitionSet, xmlDocPtr doc){
    int i;
    xmlXPathObjectPtr rawSet = getNodeSet(doc, "//transition");
    for (i = 0; i < rawSet->nodesetval->nodeNr; i++) {
        if (rawSet->nodesetval->nodeNr > 1) {
            rawSet->nodesetval->nodeTab[i]->next->next = NULL;
        }
        ioptTransition * transition = newTransition();
        readTransition(rawSet->nodesetval->nodeTab[i], transition);
        addToTransitionSet(*transition, transitionSet);
        free(transition);
    }
}
//COPY
void readAllTransitions(ioptTransitionSet * transitionSet, xmlDocPtr doc){
    int i;
    xmlXPathObjectPtr rawSet = getNodeSet(doc, "//transition");
    for (i = 0; i < rawSet->nodesetval->nodeNr; i++) {
        
        
        xmlNodePtr transitionCopy = xmlCopyNodeList(rawSet->nodesetval->nodeTab[i]);
        transitionCopy->next->next = NULL;
        
        ioptTransition * transition = newTransition();
        //print_element_names(transitionCopy);
        //printf("\n");
        readTransition(transitionCopy, transition);
        addToTransitionSet(*transition, transitionSet);
        free(transition);
    }
}
//ENDCOPY

void readTransition(xmlNode * node, ioptTransition * transition){
    xmlNode * cur_node = NULL;
    ioptAssociatedEvent * event;
    for(cur_node = node; cur_node; cur_node = cur_node->next){
        if(cur_node->type == XML_ELEMENT_NODE){
            if(strcmp((char*)cur_node->name, "transition") == 0){
                transition->id = atoi(xmlGetProp(cur_node, "id"));
            }
            if (strcmp((char*)cur_node->name, "text") == 0) {
                if (strcmp((char*)cur_node->parent->name, "name") == 0) {
                    //    strcpy(place->name.name, (char*)cur_node->children->content);
                    transition->name.name = (char*)cur_node->children->content;
                }
                if (strcmp((char*)cur_node->parent->name, "comment") == 0) {
                    //if (cur_node->next->content != NULL) {
                    if (cur_node->children != NULL && cur_node->children->content != NULL) {
                        //strcpy(place->comment.comment, "cde");//(char*)cur_node->children->content);
                        
                        transition->comment.comment = strdup(cur_node->children->content);
                        //transition->comment.comment = cur_node->next->content;
                        //printf("%s\n", place->comment.comment);
                    }
                    else{
                        transition->comment.comment = NULL;
                    }
                }
            }
            if (strcmp((char*)cur_node->name, "offset") == 0) {
                if (strcmp((char*)cur_node->parent->parent->name, "name") == 0) {
                    transition->name.graphicsOffset.x = atoi(xmlGetProp(cur_node, "x"));
                    transition->name.graphicsOffset.y = atoi(xmlGetProp(cur_node, "y"));
                }
                if (strcmp((char*)cur_node->parent->parent->name, "comment") == 0){
                    transition->comment.graphicsOffset.x = atoi(xmlGetProp(cur_node, "x"));
                    transition->comment.graphicsOffset.y = atoi(xmlGetProp(cur_node, "y"));
                }
            }
            if (strcmp((char*)cur_node->name, "position") == 0) {
                transition->graphicsPosition.page = atoi(xmlGetProp(cur_node, "page"));
                transition->graphicsPosition.x = atoi(xmlGetProp(cur_node, "x"));
                transition->graphicsPosition.y = atoi(xmlGetProp(cur_node, "y"));
            }
            if (strcmp((char*)cur_node->name, "priority") == 0) {
                transition->priority = atoi(cur_node->children->content);
            }

            
            
            
            if (strcmp((char*)cur_node->name, "event") == 0) {
                if (strcmp((char*)cur_node->parent->name, "inputEvents") == 0) {
                    event = newAssociatedEvent();
                    event->eventID = strdup(xmlGetProp(cur_node, "idRef"));
                    addEventToTransition(event, INPUT, transition);
                }
                if (strcmp((char*)cur_node->parent->name, "outputEvents") == 0) {
                    event = newAssociatedEvent();
                    event->eventID = strdup(xmlGetProp(cur_node, "idRef"));
                    addEventToTransition(event, OUTPUT, transition);
                }
            }
            
            
            
        }
        readTransition(cur_node->children, transition);
    }
}

void readAllArcs2(ioptArcSet * arcSet, xmlDocPtr doc){
    int i;
    xmlXPathObjectPtr rawSet = getNodeSet(doc, "//arc");
    for (i=0; i < rawSet->nodesetval->nodeNr; i++) {
        if (rawSet->nodesetval->nodeNr > 1) {
            rawSet->nodesetval->nodeTab[i]->next->next = NULL;
        }
        ioptArc * arc = newArc();
        readArc(rawSet->nodesetval->nodeTab[i], arc);
        addToArcSet(*arc, arcSet);
        free(arc);
        
    }
}
//COPY
void readAllArcs(ioptArcSet * arcSet, xmlDocPtr doc){
    int i;
    xmlXPathObjectPtr rawSet = getNodeSet(doc, "//arc");
    for (i=0; i < rawSet->nodesetval->nodeNr; i++) {
        xmlNodePtr arcCopy = xmlCopyNodeList(rawSet->nodesetval->nodeTab[i]);
        arcCopy->next->next = NULL;
        ioptArc * arc = newArc();
        readArc(arcCopy, arc);
        addToArcSet(*arc, arcSet);
        free(arc);
        
    }
}
//ENDCOPY

void readArc (xmlNode * node, ioptArc * arc){
    xmlNode * cur_node = NULL;
    for (cur_node = node; cur_node; cur_node = cur_node->next) {
        if(cur_node->type == XML_ELEMENT_NODE){
            if(strcmp((char*)cur_node->name, "arc") == 0){
                arc->id = atoi(xmlGetProp(cur_node, "id"));
                arc->source = atoi(xmlGetProp(cur_node, "source"));
                arc->target = atoi(xmlGetProp(cur_node, "target"));
            }
            if (strcmp((char *)cur_node->name, "offset") == 0) {
                if (strcmp((char*)cur_node->parent->parent->name, "inscription") == 0) {
                    arc->inscription.graphicsPosition.page = atoi(xmlGetProp(cur_node, "page"));
                    arc->inscription.graphicsPosition.x = atoi(xmlGetProp(cur_node, "x"));
                    arc->inscription.graphicsPosition.x = atoi(xmlGetProp(cur_node, "y"));
                }
            }
            if (strcmp((char *)cur_node->name, "type") == 0) {
                arc->type = cur_node->children->content;
            }
            if (strcmp((char *)cur_node->name, "value") == 0) {
                arc->inscription.value = atoi(cur_node->children->content);
            }
            
        }        
        readArc(cur_node->children, arc);
    }
    
}
void readSignal(xmlNode * node, ioptSignal * signal){
    xmlNode * cur_node = NULL;
    for (cur_node = node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (strcmp((char*)cur_node->name, "signal") == 0) {
                signal->id = strdup((char*) xmlGetProp(cur_node, "id"));
                signal->type = strdup((char*) xmlGetProp(cur_node, "type"));
                signal->value = atoi(xmlGetProp(cur_node, "value"));
            }
            if (strcmp((char*)cur_node->name, "position") == 0) {
                signal->graphicsPosition.x = atoi(xmlGetProp(cur_node, "x"));
                signal->graphicsPosition.y = atoi(xmlGetProp(cur_node, "y"));
                signal->graphicsPosition.page = atoi(xmlGetProp(cur_node, "page"));
            }
        }
        readSignal(cur_node->children, signal);
    }
}

void readAllSignals (ioptIO * io, xmlDocPtr doc){
    int i;
    xmlXPathObjectPtr rawSet = getNodeSet(doc, "//signal");
    if(rawSet != NULL){
    for (i=0; i < rawSet->nodesetval->nodeNr; i++) {
        xmlNodePtr signalCopy = xmlCopyNodeList(rawSet->nodesetval->nodeTab[i]);
        signalCopy->next->next = NULL;
        ioptSignal * signal = newSignal();
        readSignal(signalCopy, signal);
        if (!strcmp(rawSet->nodesetval->nodeTab[i]->parent->name, "input")) {
            addSignalToIO(*signal, INPUT, io);
        } else if(!strcmp(rawSet->nodesetval->nodeTab[i]->parent->name, "output")){
            addSignalToIO(*signal, OUTPUT, io);
        }
        free(signal);
    }
    }
}

void readIO(xmlNode * node, ioptIO * io){
}

void readEvent(xmlNode * node, ioptEvent * event){
    xmlNode * cur_node = NULL;
    for (cur_node = node; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (strcmp((char*)cur_node->name, "event") == 0) {
                event->id = strdup((char*)xmlGetProp(cur_node, "id"));
                event->edge = strdup((char*)xmlGetProp(cur_node, "edge"));
                event->level = atoi(xmlGetProp(cur_node, "level"));
                event->signal = strdup((char*)xmlGetProp(cur_node, "signal"));
            }
            if (strcmp((char*)cur_node->name, "position") == 0){
                event->graphicsPosition.x = atoi(xmlGetProp(cur_node, "x"));
                event->graphicsPosition.y = atoi(xmlGetProp(cur_node, "y"));
                event->graphicsPosition.page = atoi(xmlGetProp(cur_node, "page"));
            }
        }
        readEvent(cur_node->children, event);
    }
}
void readAllEvents (ioptIO * io, xmlDocPtr doc){
    int i;
    xmlXPathObjectPtr rawSet = getNodeSet(doc, "//event");
    if(rawSet != NULL){
    for (i = 0; i < rawSet->nodesetval->nodeNr; i++) {
        xmlNodePtr eventCopy = xmlCopyNodeList(rawSet->nodesetval->nodeTab[i]);
        if (!strcmp(rawSet->nodesetval->nodeTab[i]->parent->name, "input") || !strcmp(rawSet->nodesetval->nodeTab[i]->parent->name, "output")) {
            eventCopy->next->next = NULL;
            ioptEvent * event = newEvent();
            readEvent(eventCopy, event);
            if (!strcmp(rawSet->nodesetval->nodeTab[i]->parent->name, "input")) {
                addEventToIO(*event, INPUT, io);
            } else if(!strcmp(rawSet->nodesetval->nodeTab[i]->parent->name, "output")){
                addEventToIO(*event, OUTPUT, io);
            }
            free(event);
        }
        }
    }
}





