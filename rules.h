//
//  rules.h
//  pendo
//
//  Created by Rui Guerra on 19/05/14.
//  Copyright (c) 2014 Rui Guerra. All rights reserved.
//

#ifndef pendo_rules_h
#define pendo_rules_h

//void ruleOne(ioptNetSet * netSet, ioptNet * originalNet, conditionSet * conditions, ioptNode * cutNode);
void rules(ioptNetSet * netSet, ioptNet * originalNet, ioptNodeSet * cutSet);
void ruleOne(ioptNetSet * netSet, ioptNet * originalNet, ioptPlace * place);
void ruleTwo(ioptNetSet * netSet, ioptNet * originalNet, ioptTransition * transition);
void ruleThree (ioptNetSet * netSet, ioptNet * originalNet, ioptTransition * transition);

int getPlaceDestinySubnet(ioptNetSet * netSet, ioptNet * originalNet, ioptPlace * place);
int getTransitionDestinySubNet(ioptNetSet * netSet, ioptNet * originalNet, ioptTransition * transition);

ioptTransitionSet * getAnteTrantitions (ioptNetSet * netSet, ioptNet * originalNet, int placeID);
ioptPlaceSet * getPostPlaces(ioptNetSet * netSet, ioptNet * originalNet, int transitionID);
ioptPlaceSet * getAntePlaces(ioptNetSet * netSet, ioptNet * originalNet, int transitionID);

int getNodeType (ioptNode * node, ioptNet * originalNet);
int getTransitionSubNet(ioptNetSet * netSet, ioptTransition * transition);
int getPlaceSubNet(ioptNetSet * netSet, ioptPlace * place);
int getNetSetHighID(ioptNetSet * netSet);
#endif
