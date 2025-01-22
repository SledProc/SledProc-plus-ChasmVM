
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rz-asg-runner.h"

#include "rz-graph-token/rz-asg-core-casement-function.h"



#include "rz-graph-valuer/valuer/rz-asg-valuer.h"
#include "rz-graph-token/rz-asg-core-casement-function.h"

#include "rz-graph-build/rz-asg-result-holder.h"

#include "rz-asg-core-runner.h"

#include "rz-graph-token/token/rz-asg-token.h"


#include "rz-graph-core/kernel/graph/chasm-rz-node.h"


USING_RZNS(GRun)


RZNS_(GRun)

template<>
void RZ_ASG_Runner::proceed_run_from_node<0>(RZ_ASG_Result_Holder& rh,
  RZ_ASG_Valuer_Core_Pair& cp)
{
 RZ_ASG_Core_Casement_Function& ccf = *cp.ccf;
 tNode& start_node = *cp.fnode;
 rh.hold(&start_node);
 RZ_ASG_Function_Code code = ccf.info().Core_Function_Code;
 switch(ccf.info().Core_Function_Family)
 {
 case RZ_Graph_Call_S:
  RZ_ASG_Core_Runner::run<RZ_Graph_Call_S>
    (rh, code,
     *start_node.asg_token(), &start_node);
  break;
 }
 check_continue(rh);
 rh.unhold();
}


template<>
void RZ_ASG_Runner::prepare_run_from_node<0>(int generation,
  RZ_ASG_Result_Holder& rh,
  RZ_ASG_Core_Casement_Function& ccf,
  tNode& start_node, caon_ptr<tNode> next_node,
  caon_ptr<tNode> left_new_node,
  caon_ptr<tNode> second_node, caon_ptr<tNode> right_new_node)
{
 //?
}

template<>
void RZ_ASG_Runner::check_run_from_node<0>(int generation,
  RZ_ASG_Result_Holder& rh,
  RZ_ASG_Core_Casement_Function& ccf, tNode& start_node)
{
 valuer_->mark_core_function_call_entry(generation, ccf, &start_node, nullptr, nullptr, nullptr, nullptr, nullptr);
 prepare_run_from_node<0>(generation, rh, ccf, start_node, nullptr, nullptr, nullptr, nullptr);
}

template<>
void RZ_ASG_Runner::proceed_run_from_node<1>(RZ_ASG_Result_Holder& rh,
  RZ_ASG_Valuer_Core_Pair& cp)
{

 RZ_ASG_Core_Casement_Function& ccf = *cp.ccf;
 tNode& start_node = *cp.fnode;

 caon_ptr<tNode> next_node = cp.lhs_node;
 caon_ptr<tNode> left_new_node = cp.left_new_node;
 caon_ptr<tNode> second_node = cp.rhs_node;
 caon_ptr<tNode> right_new_node = cp.right_new_node;
 caon_ptr<tNode> arity_value_node = cp.arity_value_node;

 caon_ptr<RZ_ASG_Token> lhs_token;

 if(left_new_node)
 {
  // //  indicate that the call entry is a core function ...
  //?valuer_->mark_core_function_call_entry(&ccf, start_node, *left_new_node);
  lhs_token = valuer_->get_token_from(*left_new_node);
 }
 else
 {
  //?valuer_->mark_core_function_call_entry(start_node, *next_node);
  lhs_token = valuer_->get_token_from(*next_node);
 }

 rh.hold(&start_node);

 static RZ_ASG_Value_Holder null_vh;

 switch(ccf.info().Core_Function_Family)
 {
 case RZ_Graph_Call_C:
  if(rh.flags.has_held_value)
  {
   rh.flags.has_held_value = false;
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_C>
    (rh, ccf.info().Core_Function_Code,
     rh.value_holder(), null_vh);
  }
  else if(arity_value_node)
  {
   RZ_ASG_Value_Holder vh;
   caon_ptr<RZ_Type_Object> tobj =
    valuer_->get_node_type_object(*arity_value_node);
   // //   Should this be (as it once was) a function in ChasmRZ_Node?
   vh.set_type_object(tobj);
   vh.set_value(arity_value_node->vertex());
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_C>
    (rh, ccf.info().Core_Function_Code,
     vh, lhs_token->vh() );
  }
  else
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_C>
    (rh, ccf.info().Core_Function_Code,
     lhs_token->vh(), null_vh );
  break;

 case RZ_Graph_Call_Tc:
  if(rh.flags.has_held_value)
  {
   rh.flags.has_held_value = false;
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_Tc>
    (rh, ccf.info().Core_Function_Code,
     *lhs_token, rh.value_holder() );
  }
  else
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_Tc>
    (rh, ccf.info().Core_Function_Code,
     *lhs_token, lhs_token->vh() );
  break;

 case RZ_Graph_Call_Tv:
  if(rh.flags.has_held_value)
  {
   rh.flags.has_held_value = false;
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_Tv>
    (rh, ccf.info().Core_Function_Code,
     *lhs_token, rh.value_holder() );
  }
  else
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_Tv>
    (rh, ccf.info().Core_Function_Code,
     *lhs_token, lhs_token->vh() );
  break;

 case RZ_Graph_Call_TC:
  if(rh.flags.has_held_value)
  {
   rh.flags.has_held_value = false;
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_TC>
    (rh, ccf.info().Core_Function_Code,
     *lhs_token, second_node->asg_token()->vh());
  }
  else
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_TC>
    (rh, ccf.info().Core_Function_Code,
     *lhs_token, second_node->asg_token()->vh());
  break;

 case RZ_Graph_Call_T:
  RZ_ASG_Core_Runner::run<RZ_Graph_Call_T>
    (rh, ccf.info().Core_Function_Code,
     *lhs_token );
  break;
 }
 check_continue(rh);
 rh.unhold();
}

template<>
void RZ_ASG_Runner::prepare_run_from_node<1>(int generation,
  RZ_ASG_Result_Holder& rh,
  RZ_ASG_Core_Casement_Function& ccf,
  tNode& start_node, caon_ptr<tNode> next_node,
  caon_ptr<tNode> left_new_node,
  caon_ptr<tNode> second_node, caon_ptr<tNode> right_new_node)
{
 CAON_PTR_DEBUG(tNode ,next_node)
 CAON_PTR_DEBUG(tNode ,second_node)

 CAON_PTR_DEBUG(tNode ,left_new_node)
 CAON_PTR_DEBUG(tNode ,right_new_node)

 valuer_->mark_core_function_call_entry(generation, ccf, &start_node,
   next_node, left_new_node, second_node, right_new_node, rh.arity_value_node());

}


template<>
void RZ_ASG_Runner::check_run_from_node<1>(int generation,
  RZ_ASG_Result_Holder& rh,
  RZ_ASG_Core_Casement_Function& ccf, tNode& start_node)
{
 caon_ptr<tNode> next_node;
 caon_ptr<tNode> second_node;

 caon_ptr<tNode> left_new_node;
 caon_ptr<tNode> right_new_node;


 init_run_sequence_pair(rh, start_node, next_node, second_node, left_new_node, right_new_node);
 prepare_run_from_node<1>(generation, rh, ccf, start_node, next_node, left_new_node, second_node, right_new_node);
}


//template<>
//void RZ_ASG_Runner::proceed_run_from_node<2>(int generation,
//  RZ_ASG_Result_Holder& rh,
//  RZ_ASG_Core_Function& ccf,
//  tNode& start_node, caon_ptr<tNode> lhs_node,
//  caon_ptr<tNode> left_new_node,
//  caon_ptr<tNode> rhs_node, caon_ptr<tNode> right_new_node)

template<>
void RZ_ASG_Runner::proceed_run_from_node<2>(RZ_ASG_Result_Holder& rh,
  RZ_ASG_Valuer_Core_Pair& cp)
{

 RZ_ASG_Core_Casement_Function& ccf = *cp.ccf;
 tNode& start_node = *cp.fnode;

 caon_ptr<tNode> lhs_node = cp.lhs_node;
 caon_ptr<tNode> left_new_node = cp.left_new_node;
 caon_ptr<tNode> rhs_node = cp.rhs_node;
 caon_ptr<tNode> right_new_node = cp.right_new_node;
 caon_ptr<tNode> arity_value_node = cp.arity_value_node;

 int generation = cp.generation;

 caon_ptr<RZ_ASG_Token> lhs_token;

 if(left_new_node)
 {
//  if(right_new_node)
//    valuer_->mark_core_function_call_entry(start_node,
//      *left_new_node, *right_new_node);
//  else
//    valuer_->mark_core_function_call_entry(start_node,
//      *left_new_node, *rhs_node);
  lhs_token = valuer_->get_token_from(*left_new_node);
 }
 else
 {
//  if(right_new_node)
//    valuer_->mark_core_function_call_entry(start_node,
//      *lhs_node, *right_new_node);
//  else
//    valuer_->mark_core_function_call_entry(start_node,
//      *lhs_node, *rhs_node);
  lhs_token = valuer_->get_token_from(*lhs_node);
 }

 CAON_PTR_DEBUG(RZ_ASG_Token ,lhs_token)

 CAON_EVALUATE_DEBUG(RZ_Type_Object, t, lhs_token->vh().type_object())


 RZ_ASG_Value_Holder rhs_vh;

 CAON_PTR_DEBUG(tNode ,rhs_node)

 caon_ptr<RZ_ASG_Token> rhs_token = rhs_node->asg_token();
 CAON_PTR_DEBUG(RZ_ASG_Token ,rhs_token)

 RZ_ASG_Value_Holder lhs_vh;

 CAON_PTR_DEBUG(tNode ,lhs_node)

 caon_ptr<RZ_Type_Object> rto;
 if(rhs_token)
 {
  rto = rhs_token->vh().type_object();
 }
 if(rto)
 {
  CAON_PTR_DEBUG(RZ_Type_Object ,rto)

  caon_ptr<GBuild::RZ_Lisp_Core_Function> rhs_lcf =
       rhs_token->pRestore<GBuild::RZ_Lisp_Core_Function>(valuer_->type_variety());

  CAON_PTR_DEBUG(GBuild::RZ_Lisp_Core_Function ,rhs_lcf)
  CAON_DEBUG_NOOP
 }

 if(right_new_node)
 {
  caon_ptr<RZ_Type_Object> tobj =
   valuer_->get_node_type_object(*right_new_node);

  // //   Should this be (as it once was) a function in ChasmRZ_Node?
  rhs_vh.set_type_object(tobj);

  rhs_vh.set_value(right_new_node->vertex());
 }
 else if(caon_ptr<RZ_ASG_Core_Casement_Function> rhs_ccf =
   rhs_token->pRestore<RZ_ASG_Core_Casement_Function>(valuer_->type_variety()))
 {
  RZ_ASG_Result_Holder rh1(*valuer_);
  //?
  check_run_info(generation + 1, rh1, *rhs_ccf, *rhs_node);
  rhs_vh = rh1.value_holder();
 }
 else if(caon_ptr<GBuild::RZ_Lisp_Core_Function> rhs_lcf =
   rhs_token->pRestore<GBuild::RZ_Lisp_Core_Function>(valuer_->type_variety()))
 {
  rhs_vh = rhs_token->vh();
 }
 else
  rhs_vh = rhs_token->vh();

 rh.hold(&start_node);


 //lhs_vh = lhs_token->vh();
//}

//template<>
//void RZ_ASG_Runner::proceed_run_from_node<2>(RZ_ASG_Result_Holder& rh,
// RZ_ASG_Core_Function& ccf, tNode& start_node)
//{
 switch(ccf.info().Core_Function_Family)
 {
 case RZ_Graph_Call_VV:
  {
   RZ_ASG_Value_Holder lhs_vh;
   if(left_new_node)
   {
    caon_ptr<RZ_Type_Object> tobj =
     valuer_->get_node_type_object(*left_new_node);

    // //   Should this be (as it once was) a function in ChasmRZ_Node?
    lhs_vh.set_type_object(tobj);

    lhs_vh.set_value(right_new_node->vertex());
   }
   else
    lhs_vh = lhs_token->vh();

   RZ_ASG_Core_Runner::run<RZ_Graph_Call_VV>
     (rh, ccf.info().Core_Function_Code,
      lhs_vh,
      rhs_vh);
  }
  break;

 case RZ_Graph_Call_TV:
  RZ_ASG_Core_Runner::run<RZ_Graph_Call_TV>
    (rh, ccf.info().Core_Function_Code,
     *lhs_node->asg_token(),
     rhs_vh);
  break;

 case RZ_Graph_Call_TC:
  RZ_ASG_Core_Runner::run<RZ_Graph_Call_TC>
    (rh, ccf.info().Core_Function_Code,
     *lhs_node->asg_token(),
     rhs_vh);
  break;

 case RZ_Graph_Call_CT:
  if(rh.flags.has_held_value)
  {
   rh.flags.has_held_value = false;
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_CT>
    (rh, ccf.info().Core_Function_Code,
     rh.value_holder(), *lhs_node->asg_token());
  }
  else
  {
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_CT>
     (rh, ccf.info().Core_Function_Code,
      lhs_vh,
      *rhs_node->asg_token());
  }
  break;

 }
 rh.unhold();
}

template<>
void RZ_ASG_Runner::prepare_run_from_node<2>(int generation,
  RZ_ASG_Result_Holder& rh,
  RZ_ASG_Core_Casement_Function& ccf,
  tNode& start_node, caon_ptr<tNode> lhs_node,
  caon_ptr<tNode> left_new_node,
  caon_ptr<tNode> rhs_node, caon_ptr<tNode> right_new_node)
{
 if(lhs_node)
 {
  caon_ptr<RZ_ASG_Token> lhst = lhs_node->asg_token();
  CAON_PTR_DEBUG(RZ_ASG_Token ,lhst)
  CAON_DEBUG_NOOP
 }

// CAON_PTR_DEBUG(tNode ,rhs_node)

// caon_ptr<RZ_ASG_Token> rhs_token = rhs_node->asg_token();
// CAON_PTR_DEBUG(RZ_ASG_Token ,rhs_token)

// RZ_ASG_Value_Holder rhs_vh;
// RZ_ASG_Value_Holder lhs_vh;

// CAON_PTR_DEBUG(tNode ,lhs_node)

// caon_ptr<RZ_Type_Object> rto;
// if(rhs_token)
// {
//  rto = rhs_token->vh().type_object();
// }
// if(rto)
// {
//  CAON_PTR_DEBUG(RZ_Type_Object ,rto)

//  caon_ptr<GBuild::RZ_Lisp_Core_Function> rhs_lcf =
//       rhs_token->pRestore<GBuild::RZ_Lisp_Core_Function>(valuer_->type_variety());

//  CAON_PTR_DEBUG(GBuild::RZ_Lisp_Core_Function ,rhs_lcf)
//  CAON_DEBUG_NOOP
// }


// if(right_new_node)
// {
//  caon_ptr<RZ_Type_Object> tobj =
//   valuer_->get_node_type_object(*right_new_node);

//  // //   Should this be (as it once was) a function in ChasmRZ_Node?
//  rhs_vh.set_type_object(tobj);

//  rhs_vh.set_value(right_new_node->vertex());
// }
// else if(caon_ptr<RZ_ASG_Core_Function> rhs_ccf =
//   rhs_token->pRestore<RZ_ASG_Core_Function>(valuer_->type_variety()))
// {
//  RZ_ASG_Result_Holder rh1(*valuer_);
//  check_run_info(rh1, *rhs_ccf, *rhs_node);
//  rhs_vh = rh1.value_holder();
// }
// else if(caon_ptr<GBuild::RZ_Lisp_Core_Function> rhs_lcf =
//   rhs_token->pRestore<GBuild::RZ_Lisp_Core_Function>(valuer_->type_variety()))
// {
//  rhs_vh = rhs_token->vh();
// }
// else
//  rhs_vh = rhs_token->vh();

// rh.hold(&start_node);

 caon_ptr<RZ_ASG_Token> lhs_token;

 valuer_->mark_core_function_call_entry(generation, ccf, &start_node,
   lhs_node, left_new_node, rhs_node, right_new_node, rh.arity_value_node());

 //!
// proceed_run_from_node<2>(generation, rh, ccf, start_node,
//   lhs_node, left_new_node, rhs_node, right_new_node);
// caon_ptr<RZ_ASG_Token> lhs_token;

#ifdef HIDE

 if(left_new_node)
 {
//  if(right_new_node)
//    valuer_->mark_core_function_call_entry(start_node,
//      *left_new_node, *right_new_node);
//  else
//    valuer_->mark_core_function_call_entry(start_node,
//      *left_new_node, *rhs_node);
  lhs_token = valuer_->get_token_from(*left_new_node);
 }
 else
 {
//  if(right_new_node)
//    valuer_->mark_core_function_call_entry(start_node,
//      *lhs_node, *right_new_node);
//  else
//    valuer_->mark_core_function_call_entry(start_node,
//      *lhs_node, *rhs_node);
  lhs_token = valuer_->get_token_from(*lhs_node);
 }

 CAON_PTR_DEBUG(RZ_ASG_Token ,lhs_token)

 CAON_EVALUATE_DEBUG(RZ_Type_Object, t, lhs_token->vh().type_object())


 RZ_ASG_Value_Holder rhs_vh;

 CAON_PTR_DEBUG(tNode ,rhs_node)

 caon_ptr<RZ_ASG_Token> rhs_token = rhs_node->asg_token();
 CAON_PTR_DEBUG(RZ_ASG_Token ,rhs_token)

 RZ_ASG_Value_Holder lhs_vh;

 CAON_PTR_DEBUG(tNode ,lhs_node)

 caon_ptr<RZ_Type_Object> rto;
 if(rhs_token)
 {
  rto = rhs_token->vh().type_object();
 }
 if(rto)
 {
  CAON_PTR_DEBUG(RZ_Type_Object ,rto)

  caon_ptr<GBuild::RZ_Lisp_Core_Function> rhs_lcf =
       rhs_token->pRestore<GBuild::RZ_Lisp_Core_Function>(valuer_->type_variety());

  CAON_PTR_DEBUG(GBuild::RZ_Lisp_Core_Function ,rhs_lcf)
  CAON_DEBUG_NOOP
 }

 if(right_new_node)
 {
  caon_ptr<RZ_Type_Object> tobj =
   valuer_->get_node_type_object(*right_new_node);

  // //   Should this be (as it once was) a function in ChasmRZ_Node?
  rhs_vh.set_type_object(tobj);

  rhs_vh.set_value(right_new_node->vertex());
 }
 else if(caon_ptr<RZ_ASG_Core_Function> rhs_ccf =
   rhs_token->pRestore<RZ_ASG_Core_Function>(valuer_->type_variety()))
 {
  RZ_ASG_Result_Holder rh1(*valuer_);
  check_run_info(rh1, *rhs_ccf, *rhs_node);
  rhs_vh = rh1.value_holder();
 }
 else if(caon_ptr<GBuild::RZ_Lisp_Core_Function> rhs_lcf =
   rhs_token->pRestore<GBuild::RZ_Lisp_Core_Function>(valuer_->type_variety()))
 {
  rhs_vh = rhs_token->vh();
 }
 else
  rhs_vh = rhs_token->vh();

 rh.hold(&start_node);


 //lhs_vh = lhs_token->vh();
//}

//template<>
//void RZ_ASG_Runner::proceed_run_from_node<2>(RZ_ASG_Result_Holder& rh,
// RZ_ASG_Core_Function& ccf, tNode& start_node)
//{
 switch(ccf.info().Core_Function_Family)
 {
 case RZ_Graph_Call_VV:
  {
   RZ_ASG_Value_Holder lhs_vh;
   if(left_new_node)
   {
    caon_ptr<RZ_Type_Object> tobj =
     valuer_->get_node_type_object(*left_new_node);

    // //   Should this be (as it once was) a function in ChasmRZ_Node?
    lhs_vh.set_type_object(tobj);

    lhs_vh.set_value(right_new_node->vertex());
   }
   else
    lhs_vh = lhs_token->vh();

   RZ_ASG_Core_Runner::run<RZ_Graph_Call_VV>
     (rh, ccf.info().Core_Function_Code,
      lhs_vh,
      rhs_vh);
  }
  break;

 case RZ_Graph_Call_TV:
  RZ_ASG_Core_Runner::run<RZ_Graph_Call_TV>
    (rh, ccf.info().Core_Function_Code,
     *lhs_node->asg_token(),
     rhs_vh);
  break;

 case RZ_Graph_Call_TC:
  RZ_ASG_Core_Runner::run<RZ_Graph_Call_TC>
    (rh, ccf.info().Core_Function_Code,
     *lhs_node->asg_token(),
     rhs_vh);
  break;

 case RZ_Graph_Call_CT:
  if(rh.flags.has_held_value)
  {
   rh.flags.has_held_value = false;
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_CT>
    (rh, ccf.info().Core_Function_Code,
     rh.value_holder(), *lhs_node->asg_token());
  }
  else
  {
   RZ_ASG_Core_Runner::run<RZ_Graph_Call_CT>
     (rh, ccf.info().Core_Function_Code,
      lhs_vh,
      *rhs_node->asg_token());
  }
  break;

 }
 rh.unhold();
#endif // HIDE
}

template<>
void RZ_ASG_Runner::check_run_from_node<2>(int generation,
  RZ_ASG_Result_Holder& rh,
  RZ_ASG_Core_Casement_Function& ccf, tNode& start_node)
{
 caon_ptr<tNode> rhs_node;
 caon_ptr<tNode> lhs_node;

 // //  New node might hold a representation of a data collection
 caon_ptr<tNode> left_new_node;
 caon_ptr<tNode> right_new_node;

 init_run_sequence_pair(rh, start_node, lhs_node, rhs_node, left_new_node, right_new_node);

 prepare_run_from_node<2>(generation, rh, ccf, start_node, lhs_node, left_new_node, rhs_node, right_new_node);

}


_RZNS(GRun)

//#endif // HIDE
