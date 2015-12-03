//
//  outputConstructor.c
//  pendo
//
//  Created by Rui Guerra on 19/09/14.
//  Copyright (c) 2014 Rui Guerra. All rights reserved.
//

#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ioptDataStructures.h"
#include "listPlace.h"
#include "listTransition.h"
#include "listArc.h"
#include "listIO.h"

#include "readLib.h"


#include "connectivity.h"

#include "split.h"

#include "interface.h"
#include "conditions.h"

#include"net.h"
#include "rules.h"

#include "outputConstructor.h"



#define MY_ENCODING "ISO-8859-1"




void writeXml (char * path, ioptNetSet * netSet){
    xmlTextWriterPtr writer;
    ioptNet * net = netSet->head;
    ioptTransition * transition;
    ioptPlace * place;
    ioptArc * arc;
    xmlDocPtr doc;
    int c;
    char xmlName [200];
    int i = 1;
    while (net) {
        sprintf(xmlName, "%s/pnml%d", path, i);
        writer = xmlNewTextWriterDoc(&doc, 0);
        if (writer == NULL) {
            printf("Does not create PNML\n");
        }
        c = xmlTextWriterSetIndent(writer, 1);
        c = xmlTextWriterStartDocument(writer, NULL, MY_ENCODING, NULL);
        
        c = xmlTextWriterStartElement(writer, (xmlChar*) "Snoopy");
        c = xmlTextWriterStartElement(writer, (xmlChar*) "pnml");
        c = xmlTextWriterStartElement(writer, (xmlChar*) "net");

        c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "id", "1");
        c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "name", "pnml%d", i);
        c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "type", "IOPT");
        
        
        writeXmlInput(writer, *net->io->input);
        writeXmlOutput(writer, *net->io->output);

        
        transition = net->transitionSet->head;
        while (transition) {
        
            writeXmlTransition(writer, transition);
            transition = transition->next;
        }
        place = net->placeSet->head;
        while (place) {
            writeXmlPlace(writer, place);
            place = place->next;
        }
        arc = net->arcSet->head;
        while (arc) {
            writeXmlArc(writer, arc);
            arc = arc->next;
        }

        
        c = xmlTextWriterEndElement(writer);
        c = xmlTextWriterEndElement(writer);
        c = xmlTextWriterEndElement(writer);

        
        c = xmlTextWriterEndDocument(writer);
        xmlFreeTextWriter(writer);
        xmlSaveFormatFileEnc(xmlName, doc, MY_ENCODING, 1);
        xmlFreeDoc(doc);
        net = net->next;
        i++;
    }
}

void writeXmlTransition (xmlTextWriterPtr writer, ioptTransition * transition){
    
    int c;
    ioptAssociatedEvent * inputEvent = transition->inputEvents->head;
    ioptAssociatedEvent * outputEvent = transition->outputEvents->head;
    
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "transition");

    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "id", "%d", transition->id);
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "name");

    c = xmlTextWriterStartElement(writer, (xmlChar*) "text");
    c = xmlTextWriterWriteString(writer, (xmlChar*) transition->name.name);
    c = xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "graphics");
    c = xmlTextWriterStartElement(writer, (xmlChar*) "offset");
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "x", "%d", transition->name.graphicsOffset.x);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "y", "%d", transition->name.graphicsOffset.y);
    c = xmlTextWriterEndElement(writer);
    c = xmlTextWriterEndElement(writer);

    c = xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "comment");

    c = xmlTextWriterStartElement(writer, (xmlChar*) "text");
    if(transition->comment.comment != NULL){
    	c = xmlTextWriterWriteString(writer, (xmlChar*) transition->comment.comment);
    }
    c = xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "graphics");
    c = xmlTextWriterStartElement(writer, (xmlChar*) "offset");
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "x", "%d", transition->comment.graphicsOffset.x);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "y", "%d", transition->comment.graphicsOffset.y);
    c = xmlTextWriterEndElement(writer);
    c = xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterWriteFormatElement(writer, (xmlChar*) "priority", "%d", transition->priority);
    
    xmlTextWriterStartElement(writer, (xmlChar*) "inputEvents");
    while (inputEvent) {
        xmlTextWriterStartElement(writer, (xmlChar*) "event");
        xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "idRef", "%s", inputEvent->eventID);
        xmlTextWriterEndElement(writer);
        inputEvent = inputEvent->next;
    }
    xmlTextWriterEndElement(writer);
    
    xmlTextWriterStartElement(writer, (xmlChar*) "outputEvents");
    while (outputEvent) {
        xmlTextWriterStartElement(writer, (xmlChar*) "event");
        xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "idRef", "%s", outputEvent->eventID);
        xmlTextWriterEndElement(writer);
        outputEvent = outputEvent->next;
    }
    xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "graphics");
    c = xmlTextWriterStartElement(writer, (xmlChar*) "position");
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "page", "%d", transition->graphicsPosition.page);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "x", "%d", transition->graphicsPosition.x);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "y", "%d", transition->graphicsPosition.y);
    c = xmlTextWriterEndElement(writer);
    c = xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterEndElement(writer);
}

void writeXmlPlace (xmlTextWriterPtr writer, ioptPlace * place){
    int c;
    c = xmlTextWriterStartElement(writer, (xmlChar*) "place");

    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "id", "%d", place->id);

    c = xmlTextWriterStartElement(writer, (xmlChar*) "name");
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "text");
    c = xmlTextWriterWriteString(writer, (xmlChar*) place->name.name);
    c = xmlTextWriterEndElement(writer);

    c = xmlTextWriterStartElement(writer, (xmlChar*) "graphics");
    c = xmlTextWriterStartElement(writer, (xmlChar*) "offset");
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "x", "%d", place->name.graphicsOffset.x);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "y", "%d", place->name.graphicsOffset.y);
    c = xmlTextWriterEndElement(writer);
    c = xmlTextWriterEndElement(writer);

    c = xmlTextWriterEndElement(writer);
    
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "comment");
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "text");

    if(place->comment.comment != NULL){
    	 c = xmlTextWriterWriteString(writer, (xmlChar*) place->comment.comment);
    }
    c = xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "graphics");
    c = xmlTextWriterStartElement(writer, (xmlChar*) "offset");
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "x", "%d", place->comment.graphicsOffset.x);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "y", "%d", place->comment.graphicsOffset.y);
    c = xmlTextWriterEndElement(writer);
    c = xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterEndElement(writer);
    
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "initialMarking");
    
    c = xmlTextWriterWriteFormatElement(writer, (xmlChar*) "text", "%d", place->initialMarking.initialMarking);


    c = xmlTextWriterStartElement(writer, (xmlChar*) "graphics");
    c = xmlTextWriterStartElement(writer, (xmlChar*) "offset");
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "x", "%d", place->initialMarking.graphicsOffset.x);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "y", "%d", place->initialMarking.graphicsOffset.y);
    c = xmlTextWriterEndElement(writer);
    c = xmlTextWriterEndElement(writer);

    c = xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "bound");
    c = xmlTextWriterWriteFormatElement(writer, (xmlChar*) "text", "%d", place->bound);
    c = xmlTextWriterEndElement(writer);
    
    xmlTextWriterStartElement(writer, (xmlChar*) "graphics");
    xmlTextWriterStartElement(writer, (xmlChar*) "position");
    xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "page", "%d", place->graphicsPosition.page);
    xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "x", "%d", place->graphicsPosition.x);
    xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "y", "%d", place->graphicsPosition.y);
    xmlTextWriterEndElement(writer);
    xmlTextWriterEndElement(writer);
    
    xmlTextWriterEndElement(writer);
}

void writeXmlArc (xmlTextWriterPtr writer, ioptArc * arc){
    int c;
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "arc");

    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "id", "%d", arc->id);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "source", "%d", arc->source);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "target", "%d", arc->target);


    c = xmlTextWriterStartElement(writer, (xmlChar*) "type");
    c = xmlTextWriterWriteString(writer, (xmlChar*) arc->type);
    c = xmlTextWriterEndElement(writer);


    c = xmlTextWriterWriteElement(writer, (xmlChar*) "graphics", NULL);
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "inscription");
    c = xmlTextWriterStartElement(writer, (xmlChar*) "graphics");
    c = xmlTextWriterStartElement(writer, (xmlChar*) "offset");
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "page", "%d", arc->inscription.graphicsPosition.page);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "x", "%d", arc->inscription.graphicsPosition.x);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "y", "%d", arc->inscription.graphicsPosition.y);
    c = xmlTextWriterEndElement(writer);
    c = xmlTextWriterEndElement(writer);
    c = xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterEndElement(writer);
}


void writeXmlEvent(xmlTextWriterPtr writer, ioptEvent * event){
	int c;
    c = xmlTextWriterStartElement(writer, (xmlChar*) "event");

    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "id", "%s", event->id);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "edge", "%s", event->edge);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "level", "%d", event->level);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "signal", "%s", event->signal);
    
    c = xmlTextWriterStartElement(writer, (xmlChar*) "graphics");
    c = xmlTextWriterStartElement(writer, (xmlChar*) "position");
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "page", "%d", event->graphicsPosition.page);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "x", "%d", event->graphicsPosition.x);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "y", "%d", event->graphicsPosition.y);
    c = xmlTextWriterEndElement(writer);
    c = xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterEndElement(writer);
}
void writeXmlSignal(xmlTextWriterPtr writer, ioptSignal * signal){
    int c;
    c = xmlTextWriterStartElement(writer, (xmlChar*) "signal");
    
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "id", "%s", signal->id);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "type", "%s", signal->type);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "value", "%d", signal->value);

    c = xmlTextWriterStartElement(writer, (xmlChar*) "graphics");
    c = xmlTextWriterStartElement(writer, (xmlChar*) "position");
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "page", "%d", signal->graphicsPosition.page);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "x", "%d", signal->graphicsPosition.x);
    c = xmlTextWriterWriteFormatAttribute(writer, (xmlChar*) "y", "%d", signal->graphicsPosition.y);
    
    c = xmlTextWriterEndElement(writer);
    c = xmlTextWriterEndElement(writer);
    
    c = xmlTextWriterEndElement(writer);
}
void writeXmlInput(xmlTextWriterPtr writer, ioptInput input){
    ioptSignal * signal = input.signalSet->head;
    ioptEvent * event = input.eventSet->head;
    int c;
    c = xmlTextWriterStartElement(writer, (xmlChar*) "input");

    while (signal) {
        writeXmlSignal(writer, signal);
        signal = signal->next;
    }
    while (event) {
        writeXmlEvent(writer, event);
        event = event->next;
    }
    c = xmlTextWriterEndElement(writer);
}

void writeXmlOutput(xmlTextWriterPtr writer, ioptOutput output){
    ioptSignal * signal = output.signalSet->head;
    ioptEvent * event = output.eventSet->head;
    int c;
    c = xmlTextWriterStartElement(writer, (xmlChar*) "output");
    
    while (signal) {
        writeXmlSignal(writer, signal);
        signal = signal->next;
    }
    while (event) {
        writeXmlEvent(writer, event);
        event = event->next;
    }
    c = xmlTextWriterEndElement(writer);
}


