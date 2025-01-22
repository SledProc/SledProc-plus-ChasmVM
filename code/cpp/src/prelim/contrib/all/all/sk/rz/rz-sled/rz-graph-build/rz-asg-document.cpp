
#include "rz-asg-document.h"


#include "rz-graph-token/token/rz-asg-token.h"


//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rzns.h"
USING_RZNS(GBuild)


RZ_ASG_Document::RZ_ASG_Document(ChasmRZ_Document& rz_document)
 : rz_document_(rz_document)
{

}

