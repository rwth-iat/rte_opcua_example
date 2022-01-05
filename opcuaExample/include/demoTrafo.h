/*
 * demoTrafo.h
 *
 *  Created on: 23.02.2019
 *      Author: Julian Grothoff
 *
 */

#ifndef demoTrafo_H_
#define demoTrafo_H_

#include "opcuaExample.h"

UA_Nodestore* demoTrafo_new(OV_INSTPTR_opcuaExample_customInterface context);
void demoTrafo_delete(UA_Nodestore * nodestoreInterface);

#endif /* demoTrafo_H_ */