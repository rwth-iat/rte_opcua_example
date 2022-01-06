/*
 * trafo.h
 *
 *  Created on: 05.01.2022
 *      Author: Torben Miny, Julian Grothoff
 *
 */

#ifndef trafo_H_
#define trafo_H_

#include "demo.h"

UA_Nodestore* trafoTypes_new(OV_INSTPTR_demo_interfaceTypes context);
void trafoTypes_delete(UA_Nodestore * nodestore);

#endif /* trafo_H_ */