
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_NGML_OUTPUT_BASE__H
#define RZ_NGML_OUTPUT_BASE__H

#include "relae-graph/relae-node-ptr.h"

#include "kernel/rz-ngml-dominion.h"

#include "kernel/query/rz-ngml-query.h"

#include <QString>
#include <QTextStream>

#include "accessors.h"


#include "kans.h"
KANS_CLASS_DECLARE(HTXN ,HTXN_Document_8b)
USING_KANS(HTXN)


#include "rzns.h"

RZNS_(NGML)

class NGML_Graph;
class NGML_Document;


class NGML_Output_Base
{
protected:

 NGML_Graph& graph_;
 NGML_Document& document_;

 const NGML_Query& qry_;
 
 HTXN_Document_8b* htxn_document_;

public:


 NGML_Output_Base(NGML_Document& document);
 virtual void generate(QTextStream& qts) = 0;

 ACCESSORS(HTXN_Document_8b* ,htxn_document)


};

_RZNS(NGML)


#endif
