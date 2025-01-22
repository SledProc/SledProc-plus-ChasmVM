
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rz-graph-embed-run-valuer.h"

#include "rz-graph-valuer/valuer/rz-asg-valuer.h"

#include "rz-graph-valuer/vector/rz-asg-vector.h"


#include <QDebug>

#include <QStringList>


USING_RZNS(GEmbed)




RZ_Graph_Embed_Run_Valuer::RZ_Graph_Embed_Run_Valuer(RZ_ASG_Valuer& valuer)
 : valuer_(valuer), current_user_package_(nullptr), default_user_package_(nullptr),
   current_user_class_(nullptr), default_user_class_(nullptr)
{
}


void RZ_Graph_Embed_Run_Valuer::valuer_deferred_callback(QString qs)
{
 valuer_.run_deferred_callback(qs);
}


