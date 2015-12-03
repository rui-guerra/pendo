//
//  readLib.h
//  pendo
//
//  Created by Rui Guerra on 14/11/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>

#include "listIO.h"

#ifndef pendo_readLib_h
#define pendo_readLib_h


void readPlace(xmlNode * node, ioptPlace *place);
void readAllPlaces(ioptPlaceSet *placeSet, xmlDocPtr doc);
void readAllTransitions(ioptTransitionSet * transitionSet, xmlDocPtr doc);
xmlXPathObjectPtr getNodeSet(xmlDocPtr doc, xmlChar* xpath);
void print_element_names(xmlNode * a_node);
void readTransition(xmlNode * node, ioptTransition * transition);
void readAllArcs(ioptArcSet * arcSet, xmlDocPtr doc);
void readArc(xmlNode * node, ioptArc * arc);

void readAllSignals (ioptIO * io, xmlDocPtr doc);
void readAllEvents (ioptIO * io, xmlDocPtr doc);

void readIO(xmlNode * node, ioptIO * io);
#endif
