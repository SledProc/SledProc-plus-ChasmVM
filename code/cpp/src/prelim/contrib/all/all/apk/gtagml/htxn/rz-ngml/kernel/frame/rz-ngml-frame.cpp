
#include "rz-ngml-frame.h"

#include "kernel/graph/rz-ngml-node.h"

#include "rzns.h"

#include <QDebug>


USING_RZNS(NGML)

NGML_Frame::NGML_Frame()
 : node_frame<NGML_Dominion>()
{


}

NGML_Frame& NGML_Frame::instance()
{
 static NGML_Frame* the_instance = nullptr;
 if(!the_instance)
  the_instance = new NGML_Frame;
 return *the_instance;
}
