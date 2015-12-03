//
//  outputConstructor.h
//  pendo
//
//  Created by Rui Guerra on 19/09/14.
//  Copyright (c) 2014 Rui Guerra. All rights reserved.
//

#ifndef pendo_outputConstructor_h
#define pendo_outputConstructor_h
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

void createXML (char * path, ioptNetSet * netSet);
void createXmlPlaces (xmlNodePtr node, ioptPlaceSet * placeSet);
void createXmlTransitions (xmlNodePtr node, ioptTransitionSet * transitionSet);
void writeXml (char * path, ioptNetSet * netSet);
void writeXmlTransition (xmlTextWriterPtr writer, ioptTransition * transition);
void writeXmlPlace (xmlTextWriterPtr writer, ioptPlace * place);
void writeXmlArc (xmlTextWriterPtr writer, ioptArc * arc);
void writeXmlOutput(xmlTextWriterPtr writer, ioptOutput output);
void writeXmlInput(xmlTextWriterPtr writer, ioptInput input);
#endif
