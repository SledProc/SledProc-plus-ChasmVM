
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef RZ_LISP_GRAPH_RUNNER__H
#define RZ_LISP_GRAPH_RUNNER__H

#include "accessors.h"


#include "rz-graph-token/types/run-types.h"

#include "rz-graph-core/kernel/query/chasm-rz-query.h"

#include "rz-graph-core/kernel/chasm-rz-dominion.h"

#include "functions/rz-asg-function-map.h"

#include "rz-graph-valuer/valuer/rz-asg-valuer.h"

#include <stack>

#include <map>

#include "rzns.h"

RZNS_(RZ_Core)

class ChasmRZ_Node;
class RE_Graph;

_RZNS(RZ_Core)

USING_RZNS(RZ_Core)


RZNS_(GRun)

class RZ_Graph_Run_Token;

_RZNS(GRun)


RZNS_(GBuild)

class RZ_Lisp_Core_Runner;
class RZ_ASG_Visitor;
class RZ_ASG_Result_Holder;

_RZNS(GBuild)

USING_RZNS(GBuild)


//RZNS_(GVal)

//class RZ_ASG_Valuer;

//_RZNS(GVal)

USING_RZNS(GVal)

RZNS_(GRun)

class RZ_ASG_Runner
{

 caon_ptr<RZ_ASG_Valuer> valuer_;

 const ChasmRZ_Query& Qy;


 typedef ChasmRZ_Node tNode;

public:

 RZ_ASG_Runner(caon_ptr<RZ_ASG_Valuer> valuer);

 void check_run_info(int generation, RZ_ASG_Result_Holder& rh,
  RZ_ASG_Core_Casement_Function& ccf, tNode& start_node);

 void check_core_function_info(RZ_ASG_Core_Casement_Function& ccf);

 void valuer_redirect(RZ_ASG_Result_Holder& rh,
  RZ_ASG_Core_Casement_Function& ccf, tNode& start_node);

 template<int Arity>
 void check_run_from_node(int generation, RZ_ASG_Result_Holder& rh,
   RZ_ASG_Core_Casement_Function& ccf, tNode& start_node);

 template<int Arity>
 void prepare_run_from_node(int generation, RZ_ASG_Result_Holder& rh,
   RZ_ASG_Core_Casement_Function& ccf,
   tNode& start_node,
                            caon_ptr<tNode> lhs_node,
   caon_ptr<tNode> left_new_node,
   caon_ptr<tNode> rhs_node, caon_ptr<tNode> right_new_node
                            );

// template<int Arity>
// void proceed_run_from_node(int generation, RZ_ASG_Result_Holder& rh,
//   RZ_ASG_Core_Casement_Function& ccf,
//   tNode& start_node, caon_ptr<tNode> lhs_node,
//   caon_ptr<tNode> left_new_node,
//   caon_ptr<tNode> rhs_node, caon_ptr<tNode> right_new_node

//                            );

template<int Arity>
void proceed_run_from_node(RZ_ASG_Result_Holder& rh,
   RZ_ASG_Valuer_Core_Pair& cp);


// template<int Arity>
// void proceed_run_from_node(RZ_ASG_Result_Holder& rh,
//   RZ_ASG_Core_Casement_Function& ccf,
//   caon_ptr<tNode> lhs, caon_ptr<tNode> rhs);


 void check_continue(RZ_ASG_Result_Holder& rh);

 caon_ptr<tNode> check_run_sequence(RZ_ASG_Result_Holder& rh,
  caon_ptr<ChasmRZ_Connectors>& premise, tNode& lhs_node);

 caon_ptr<tNode> run_token_as_first_argument(RZ_ASG_Result_Holder& rh);

 void init_run_sequence_pair(RZ_ASG_Result_Holder& rh,
  tNode& start_node, caon_ptr<tNode>& first_node,
  caon_ptr<tNode>& second_node,  caon_ptr<tNode>& left_new_node,
                        caon_ptr<tNode>& right_new_node);


};

_RZNS(GRun)


#endif //RZ_LISP_GRAPH_RUNNER__H
