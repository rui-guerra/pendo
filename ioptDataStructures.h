//
//  ioptDataStructures.h
//  pendo
//
//  Created by Rui Guerra on 11/5/13.
//  Copyright (c) 2013 Rui Guerra. All rights reserved.
//

#ifndef pendo_ioptDataStructures_h
#define pendo_ioptDataStructures_h


enum nodeType {PLACE, TRANSITION}; // DEFFINES PLACE AND TRANSITION TYPE FOR CHECKS
enum copyType {ORIGINAL, COPY};

typedef struct{
    int x;
    int y;
}ioptGraphicsOffset;

typedef struct{
    int page;
    int x;
    int y;
}ioptGraphicsPosition;

typedef struct{
    char * name;
    ioptGraphicsOffset graphicsOffset;
}ioptName;

typedef struct{
    int initialMarking;
    ioptGraphicsOffset graphicsOffset;
}ioptInitialMarking;

typedef struct{
    char * comment;
    ioptGraphicsOffset graphicsOffset;
}ioptComment;

typedef struct{
    ioptGraphicsPosition graphicsPosition;
    int value;
}ioptInscription;

#endif
