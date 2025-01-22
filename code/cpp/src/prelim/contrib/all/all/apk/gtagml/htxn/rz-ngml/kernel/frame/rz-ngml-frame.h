
#ifndef RZ_NGML_FRAME__H
#define RZ_NGML_FRAME__H

#include "relae-graph/relae-node-ptr.h"

#include "kernel/rz-ngml-dominion.h"

#include "rzns.h"

RZNS_(NGML)


class NGML_Frame : public node_frame<NGML_Dominion>
{
 NGML_Frame();

public:

 static NGML_Frame& instance();
 static NGML_Frame* instance_ptr() { return &instance(); }

};

_RZNS(NGML)

#endif
