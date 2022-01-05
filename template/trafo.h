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

UA_Nodestore* trafo_new(OV_INSTPTR_template_interface context);
void trafo_delete(UA_Nodestore * nodestore);

#endif /* trafo_H_ */