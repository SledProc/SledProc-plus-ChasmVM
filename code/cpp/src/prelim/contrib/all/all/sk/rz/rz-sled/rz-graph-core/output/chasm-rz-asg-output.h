
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef CHASM_RZ_ASG_OUTPUT__H
#define CHASM_RZ_ASG_OUTPUT__H

#include "accessors.h"
#include "flags.h"

#include "relae-graph/relae-node-ptr.h"

#include "global-types.h"

#include "rz-graph-core/kernel/query/chasm-rz-query.h"
#include "rz-graph-core/kernel/frame/chasm-rz-frame.h"

#include "rz-graph-core/traverser/chasm-rz-traverser.h"


#include <QString>
#include <QTextStream>
#include <QMap>

#include <functional>

#include "rzns.h"


RZNS_CLASS_DECLARE(GBuild ,RZ_ASG_Token)
USING_RZNS(GBuild)

RZNS_(RZ_Core)


class ChasmRZ_Document;
class ChasmRZ_Node;

class ChasmRZ_Node;
class ChasmRZ_Node_Factory;

class ChasmRZ_ASG_Output
{
protected:

 ChasmRZ_Traverser traverser_;

 caon_ptr<ChasmRZ_Document> document_;

 ChasmRZ_Node_Factory& node_factory_;


 ChasmRZ_Frame& Cf;
 ChasmRZ_Frame& Sf;
 ChasmRZ_Frame& Tf;
 const ChasmRZ_Query& Qy;

 virtual caon_ptr<ChasmRZ_Node> get_root_node();

 virtual caon_ptr<ChasmRZ_Node> get_root_node(QTextStream& qts)
 {
  return get_root_node();
 }

 virtual void output_from_individual_node(QTextStream& qts,
   const ChasmRZ_Node& node, caon_ptr<ChasmRZ_Call_Entry>& rce, u2 indent);

 virtual void output_from_node(QTextStream& qts,
  const ChasmRZ_Node& node, u2 indent = 0, caon_ptr<ChasmRZ_Call_Entry> rce_ptr = nullptr);


 virtual caon_ptr<ChasmRZ_Node> find_next_node_via_block_entry(caon_ptr<ChasmRZ_Node> node);
 virtual caon_ptr<ChasmRZ_Node> find_next_node_via_entry(caon_ptr<ChasmRZ_Node> node);
 virtual caon_ptr<ChasmRZ_Node> find_next_node_via_sequence(caon_ptr<ChasmRZ_Node> node);
 virtual caon_ptr<ChasmRZ_Node> find_next_node_via_cross(caon_ptr<ChasmRZ_Node> node);


 virtual QString get_asg_out(const ChasmRZ_Token& token);

 virtual void report_call_entry(QTextStream& qts,
  const ChasmRZ_Call_Entry& rce) = 0;

 virtual void report_call_leave(QTextStream& qts,
  const ChasmRZ_Call_Entry& rce) = 0;

 virtual void report_tuple_info_entry(QTextStream& qts,
  const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> = nullptr) = 0;

 virtual void report_tuple_info_leave(QTextStream& qts,
  const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> = nullptr) = 0;

 virtual void report_token(QTextStream& qts,
  const ChasmRZ_Token& token) = 0;

 virtual void init_token(ChasmRZ_Token& token) {}
 virtual void init_token(const ChasmRZ_Node& node, caon_ptr<ChasmRZ_Node>& rat_node) {}

public:

 ChasmRZ_ASG_Output(caon_ptr<ChasmRZ_Document> document);

 virtual void output(QString path);
 virtual void write_report(QTextStream& qts);

};



_RZNS(RZ_Core)

#endif //CHASM_RZ_ASG_OUTPUT__H
