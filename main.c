//
//  main.c
//  pendo
//
//  Created by Rui Guerra on 10/28/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//


#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>

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

xmlDocPtr parseDoc(char *docname){
    xmlDocPtr doc;
    
    FILE * file;
    if((file = fopen(docname, "r"))){
        fclose(file);
        doc = xmlParseFile(docname);
        if (doc == NULL) {
            printf("Document not parsed\n");
            return NULL;
        }
        return doc;
    }
    return NULL;
}

xmlDoc * getFile(char * path){  //change
    char docname [200];
    char * dir;
    xmlDoc * doc;
    printf("Filename:");
    fgets(docname, 200, stdin);
    sscanf(docname, "%s\n", docname);

    dir = dirname(path);
        strcat(dir, "/");
    strcat(dir, docname);
    strcat(dir, ".pnml");
    doc = parseDoc((char*)dir);  //change
    if (doc == NULL) {
        return NULL;
    }else{
        return doc;
    }
}


int main(int argc, char ** argv){
    //xmlChar * docname;
    xmlDoc *doc = NULL;
    //xmlNode *root = NULL;
    doc = getFile(argv[0]);
    if (doc == NULL) {
        printf("\nThere is no file with that name.");
        exit(0);
    }
    ioptNet * net= newNet();
    ioptNetSet * netSet = NULL;
    //conditionSet * placementSet = newConditionSet();
    readAllArcs(net->arcSet, doc);
    readAllTransitions(net->transitionSet,doc);
    readAllPlaces(net->placeSet, doc);
    ioptNodeSet * cutSet;
    //conditionSet * conditions = newConditionSet();
    readAllSignals(net->io, doc);
    readAllEvents(net->io, doc);
    printNet(net);
    cutSet = getCuttingSet();
    if (cutSet != NULL){
        if (connected(net)) {
            printf("\nNet fully connected\n.");
            if (checkCutSet(net, cutSet)) {
                printf("\nCutSet OK\n.");
                netSet = divideNet(net, cutSet);
                if (netSet->size > 1) {
                    printf("\n2 or more subnets.\n");
                    printNets(netSet);
                    mergeSubNets(netSet, net);
                    printNets(netSet);
                    rules(netSet, net, cutSet);
                    reasignIO(net, netSet);
                    printNets(netSet);
                    writeXml(dirname(argv[0]), netSet);
                } else{
                    printf("\nNo subnets after splitting.");
                }
            } else{
                printf("\nCutting set nodes are connected. Aborted.");
            }
        }else{
            printf("\nNet not fully connected. Aborted.");
        }
    }else{
        printf("\nEmpty or invalid cutting set.");
    }
    printf("\n\n");
    return 0;
}





