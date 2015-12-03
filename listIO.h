
//
//  listIO.h
//  pendo
//
//  Created by Rui Guerra on 22/08/14.
//  Copyright (c) 2014 Rui Guerra. All rights reserved.
//

#include "ioptDataStructures.h"

#ifndef pendo_listIO_h
#define pendo_listIO_h

typedef struct ioptIO ioptIO;
typedef struct ioptInput ioptInput;
typedef struct ioptOutput ioptOutput;
typedef struct ioptSignalSet ioptSignalSet;
typedef struct ioptEventSet ioptEventSet;
typedef struct ioptSignal ioptSignal;
typedef struct ioptEvent ioptEvent;

enum ioType {INPUT, OUTPUT};

struct ioptIO{
    ioptInput * input;
    ioptOutput * output;
};

struct ioptInput{
    ioptSignalSet * signalSet;
    ioptEventSet * eventSet;
};

struct ioptOutput{
    ioptSignalSet * signalSet;
    ioptEventSet * eventSet;
};

struct ioptSignalSet{
    ioptSignal * head;
    ioptSignal * tail;
    int size;
};

struct ioptEventSet{
    ioptEvent * head;
    ioptEvent * tail;
    int size;
};

struct ioptSignal{
    char * id;
    char * type;
    int value;
    int min;
    int max;
    int wrap;
    ioptGraphicsPosition  graphicsPosition;
    ioptSignal * next;
};

struct ioptEvent{
    char * id;
    char * edge;
    int level;
    char * signal;
    ioptGraphicsPosition graphicsPosition;
    ioptEvent * next;
};

ioptIO * newIO();
ioptEvent * newEvent();
ioptSignal * newSignal();
void addEventToIO(ioptEvent event, int ioType, ioptIO * io);
void addSignalToIO(ioptSignal signal, int ioType, ioptIO * io);

#endif
