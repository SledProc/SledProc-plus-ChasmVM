
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_PRERUN_SETUP__H
#define RZ_PRERUN_SETUP__H

#include "accessors.h"
#include "flags.h"

#include "relae-graph/relae-node-ptr.h"

#include "rz-graph-core/output/chasm-rz-asg-output.h"

#include "rz-graph-core/kernel/graph/chasm-rz-graph.h"

#include "rz-graph-core/output/chasm-rz-asg-output.h"

#include "rz-graph-core/kernel/grammar/chasm-rz-node-factory.h"


#include <QString>
#include <QTextStream>
#include <QMap>

#include <functional>

#include "rzns.h"


RZNS_(RZ_Core)


class ChasmRZ_Document;
class ChasmRZ_Node;
class ChasmRZ_Graph;

class ChasmRZ_Node_Factory;


class RZ_Prerun_Setup : public ChasmRZ_ASG_Output
{
// ChasmRZ_Node_Factory& node_factory_;


 void add_observer_function(caon_ptr<ChasmRZ_Node> start_node,
   caon_ptr<ChasmRZ_Node> next_node, const ChasmRZ_Connectors& connector);

public:

 RZ_Prerun_Setup(caon_ptr<ChasmRZ_Document> document);

 virtual caon_ptr<ChasmRZ_Node> get_root_node(QTextStream& qts)  Q_DECL_OVERRIDE;

 virtual void output_from_individual_node(QTextStream& qts,
   const ChasmRZ_Node& node, caon_ptr<ChasmRZ_Call_Entry>& rce,
   u2 indent = 0)  Q_DECL_OVERRIDE;

 virtual void report_token(QTextStream& qts,
  const ChasmRZ_Token& token)  Q_DECL_OVERRIDE;

 virtual void report_tuple_info_entry(QTextStream& qts,
  const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr = nullptr)  Q_DECL_OVERRIDE;

 virtual void report_tuple_info_leave(QTextStream& qts,
  const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr = nullptr)  Q_DECL_OVERRIDE;

 virtual void report_call_entry(QTextStream& qts,
  const ChasmRZ_Call_Entry& rce) Q_DECL_OVERRIDE;

 virtual void report_call_leave(QTextStream& qts,
  const ChasmRZ_Call_Entry& rce)  Q_DECL_OVERRIDE;

 virtual caon_ptr<ChasmRZ_Node> find_next_node_via_block_entry(caon_ptr<ChasmRZ_Node> node)  Q_DECL_OVERRIDE;
 virtual caon_ptr<ChasmRZ_Node> find_next_node_via_entry(caon_ptr<ChasmRZ_Node> node)  Q_DECL_OVERRIDE;
 virtual caon_ptr<ChasmRZ_Node> find_next_node_via_sequence(caon_ptr<ChasmRZ_Node> node)  Q_DECL_OVERRIDE;
 virtual caon_ptr<ChasmRZ_Node> find_next_node_via_cross(caon_ptr<ChasmRZ_Node> node)  Q_DECL_OVERRIDE;



// virtual void write_report(QTextStream& qts) Q_DECL_OVERRIDE;


};

_RZNS(RZ_Core)

#endif //RZ_PRERUN_SETUP__H
