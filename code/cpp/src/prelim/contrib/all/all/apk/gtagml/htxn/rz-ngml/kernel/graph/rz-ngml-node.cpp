
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-ngml-node.h"

#include "rzns.h"
USING_RZNS(NGML)

QString NGML_Node::debug_connectors() const
{
 QString result;

 static NGML_Frame* fr = NGML_Frame::instance_ptr();

 in_frame_targets_iterator_type it(targets_[fr]);

 while(it.hasNext())
 {
  it.next();
  QString connector = it.key()->label();
  caon_ptr<NGML_Node> target = it.value();
  CAON_PTR_DEBUG(NGML_Node ,target)
  result += QString("\n |%1| %2").arg(connector).arg(target->label());

 }
 return result;
}
