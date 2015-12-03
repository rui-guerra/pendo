//
//  listIO.c
//  pendo
//
//  Created by Rui Guerra on 22/08/14.
//  Copyright (c) 2014 Rui Guerra. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "listIO.h"
#include <string.h>

ioptEvent * newEvent(){
    ioptEvent * event = malloc(sizeof(ioptEvent));
    event->graphicsPosition.page = 1;
    event->graphicsPosition.x = 200;
    event->graphicsPosition.y = 200;
    
    event->level = 1;
    event->edge = "up";
    event->next = NULL;
    return event;
}

ioptSignal * newSignal(){
    ioptSignal * signal = malloc(sizeof(ioptSignal));
    signal->graphicsPosition.page = 1;
        signal->graphicsPosition.x = 100;
        signal->graphicsPosition.y = 100;
    signal->type = "boolean";
    signal->value = 0;
    signal->next = NULL;
    return signal;
}

ioptEventSet * newEventSet(){
    ioptEventSet * eventSet = malloc(sizeof(ioptEventSet));
    eventSet->head = NULL;
    eventSet->tail = NULL;
    eventSet->size = 0;
    return eventSet;
}

ioptSignalSet * newSignalSet(){
    ioptSignalSet * signalSet = malloc(sizeof(ioptSignalSet));
    signalSet->head = NULL;
    signalSet->tail = NULL;
    signalSet->size = 0;
    return signalSet;
}

ioptInput * newInput(){
    ioptInput * input = malloc(sizeof(ioptInput));
    input->eventSet = newEventSet();
    input->signalSet = newSignalSet();
    return input;
}

ioptOutput * newOutput(){
    ioptOutput * output = malloc(sizeof(ioptOutput));
    output->eventSet = newEventSet();
    output->signalSet = newSignalSet();
    return output;
}

ioptIO * newIO(){
    ioptIO * io = malloc(sizeof(ioptIO));
    io->input = newInput();
    io->output = newOutput();
    return io;
}

void addEventToIO(ioptEvent event, int ioType, ioptIO * io){
    ioptEvent * auxEvent = newEvent();
    
    *auxEvent = event;
    auxEvent->next = NULL;
    if (ioType == INPUT) {
        if (io->input->eventSet->head == NULL) {
            io->input->eventSet->head = io->input->eventSet->tail = auxEvent;
        }else{
            io->input->eventSet->tail->next = auxEvent;
            io->input->eventSet->tail = auxEvent;
        }
        io->input->eventSet->size++;
    }else if (ioType == OUTPUT){
        if (io->output->eventSet->head == NULL) {
            io->output->eventSet->head = io->output->eventSet->tail = auxEvent;
        }else{
            io->output->eventSet->tail->next = auxEvent;
            io->output->eventSet->tail = auxEvent;
        }
        io->output->eventSet->size++;
    }
}
void addSignalToIO(ioptSignal signal, int ioType, ioptIO * io){
    ioptSignal * auxSignal = newSignal();
    
    *auxSignal = signal;
    auxSignal->next = NULL;
    if (ioType == INPUT) {
        if (io->input->signalSet->head == NULL) {
            io->input->signalSet->head = io->input->signalSet->tail = auxSignal;
        }else{
            io->input->signalSet->tail->next = auxSignal;
            io->input->signalSet->tail = auxSignal;
        }
        io->input->signalSet->size++;
    }else if (ioType == OUTPUT){
        if (io->output->signalSet->head == NULL) {
            io->output->signalSet->head = io->output->signalSet->tail = auxSignal;
        }else{
            io->output->signalSet->tail->next = auxSignal;
            io->output->signalSet->tail = auxSignal;
        }
        io->output->signalSet->size++;
    }
}

