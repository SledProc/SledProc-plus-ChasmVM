
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-frame.h"

#include "kernel/graph/chasm-rz-node.h"

#include "rzns.h"

#include <QDebug>

#include <QMap>



USING_RZNS(RZ_Core)

ChasmRZ_Frame::ChasmRZ_Frame(QString label)
 : node_frame<ChasmRZ_Dominion>(), label_(label)
{


}

ChasmRZ_Frame& ChasmRZ_Frame::instance(QString label)
{
 static QMap<QString, ChasmRZ_Frame*> instances;

 auto it = instances.find(label);

 if(it == instances.end())
   it = instances.insert(label, new ChasmRZ_Frame(label));

 return **it;
}
