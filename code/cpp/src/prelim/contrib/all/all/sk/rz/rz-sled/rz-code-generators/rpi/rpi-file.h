
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RPI_FILE__H
#define RPI_FILE__H

#include "accessors.h"
#include "flags.h"

#include <QString>
#include <QMap>

#include <QVector>

#include <QTextStream>

#include "rzns.h"

#include "relae-graph/relae-caon-ptr.h"

#include "multistep-token.h"

#include "phr-graph-core/kernel/graph/pgb-ir-build.h"


//RZNS_CLASS_DECLARE(PhrGraphCore ,PGB_IR_Build)

USING_RZNS(PhrGraphCore)

RZNS_CLASS_DECLARE(GBuild ,RZ_Graph_Visitor_Phaon)
USING_RZNS(GBuild)


RZNS_CLASS_DECLARE(RZ_Core ,ChasmRZ_Node)
USING_RZNS(RZ_Core)

//RZNS_(GBuild)
// class RZ_ASG_Visitor;
// class RZ_Graph_Visitor_Phaon;
// class RZ_ASG_Token;
//_RZNS(GBuild)


RZNS_(GVal)

class RPI_Stage_Form;
class RZ_ASG_Lexical_Scope;

class RZ_Function_Def_Syntax;
class RZ_ASG_Block_Info;
class RZ_Code_Statement;
class RZ_Function_Def_Info;
class RPI_Block;


class RPI_File
{
 QString path_;
 caon_ptr<RPI_Block> implicit_block_;
 PGB_IR_Build& pgb();
 _PGB_IR_Build pgb(QList<PGB_IR_Build::Text_With_Purpose>&);

public:

 RPI_File(QString path, caon_ptr<RPI_Block> implicit_block);

 void scan_top_level(RZ_Graph_Visitor_Phaon& visitor_phaon);
 void write_top_level(QList<PGB_IR_Build::Text_With_Purpose>& tps, QTextStream* qts);
 void scan(RZ_Graph_Visitor_Phaon& visitor_phaon, ChasmRZ_Node& start_node);


};

_RZNS(GVal)

#endif //RPI_FILE__H
