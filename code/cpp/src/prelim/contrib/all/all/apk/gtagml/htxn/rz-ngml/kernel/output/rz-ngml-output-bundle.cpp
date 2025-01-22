
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-ngml-output-bundle.h"

#include "rzns.h"
USING_RZNS(NGML)

NGML_Output_Bundle NGML_Output_Bundle::with(QTextStream& new_qts) const
{
 return {new_qts, index, node, connection_descriptor, cb};
}

NGML_Output_Bundle NGML_Output_Bundle::with(NGML_Connection_Descriptor descriptor) const
{
 return {qts, index, node, descriptor, cb};
}

NGML_Output_Bundle NGML_Output_Bundle::with(caon_ptr<tNode> n, NGML_Connection_Descriptor descriptor) const
{
 return {qts, index, n, descriptor, cb};
}

NGML_Output_Bundle NGML_Output_Bundle::with_node(caon_ptr<tNode> n) const
{
 return {qts, index, n, connection_descriptor, cb};
}

NGML_Output_Bundle NGML_Output_Bundle::with_callback(caon_ptr<NGML_Command_Callback> c) const
{
 return {qts, index, node, connection_descriptor, c};
}



