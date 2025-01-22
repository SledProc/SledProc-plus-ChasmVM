
#ifndef RZ_NGML_PARSER__H
#define RZ_NGML_PARSER__H

#include "relae-graph/relae-node-ptr.h"
#include "relae-graph/relae-parser.h"

#include "kernel/rz-ngml-dominion.h"

#include "rzns.h"

RZNS_(NGML)


class NGML_Parser : public Relae_Parser<NGML_Galaxy>
{
public:
 NGML_Parser(caon_ptr<NGML_Graph> g);
};

_RZNS(NGML)

#endif
