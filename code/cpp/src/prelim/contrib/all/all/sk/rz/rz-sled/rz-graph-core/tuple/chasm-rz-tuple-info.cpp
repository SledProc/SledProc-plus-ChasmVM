
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-tuple-info.h"

#include "code/chasm-rz-call-entry.h"
#include "kernel/graph/chasm-rz-node.h"

#include "rzns.h"

USING_RZNS(RZ_Core)

ChasmRZ_Tuple_Info::ChasmRZ_Tuple_Info(Tuple_Formations tf, Tuple_Indicators ti, int data_id)
 : formation_(tf), indicator_(ti), data_id_(data_id), call_entry_node_(nullptr)
{

}

QString ChasmRZ_Tuple_Info::asg_out_with_id(bool with_prefix) const
{
 return asg_out(with_prefix) + QString("<%1>").arg(data_id_);
}


QString ChasmRZ_Tuple_Info::token_representation() const
{
 // //   Return a string representation of an
 //      empty example of this tuple, which
 //      could act as a token
 QString result = base_token_representation();
 switch(formation_)
 {
 case Tuple_Formations::Indicates_Declarations:
  result.prepend(',');
  break;
 case Tuple_Formations::Indicates_Input:
  result.prepend('.');
  break;
 case Tuple_Formations::Indicates_Final:
  result.prepend("..");
  break;
 case Tuple_Formations::Indicates_String:
 case Tuple_Formations::Indicates_Multi:
 case Tuple_Formations::Indicates_Functional:
 case Tuple_Formations::Indicates_Text_Map:
 case Tuple_Formations::Indicates_Text_Vector:
 case Tuple_Formations::Indicates_Pattern:
 case Tuple_Formations::Indicates_Assignment:
 default:
  break;
 }
 return result;
}


QString ChasmRZ_Tuple_Info::base_token_representation() const
{
 // //   Returns the base for a token_representation
 switch(indicator_)
 {
 case Tuple_Indicators::Enter_Array:
  return "()";
 case Tuple_Indicators::Enter_Vector:
  return "[]";
 case Tuple_Indicators::Enter_Map:
  return "{}";
 case Tuple_Indicators::Enter_Set:
  return "<>";
 }
}


void ChasmRZ_Tuple_Info::mark_as_empty()
{
 if(call_entry_node_)
 {
  if(caon_ptr<ChasmRZ_Call_Entry> rce = call_entry_node_->chasm_rz_call_entry())
  {
   rce->flags.is_empty_tuple = true;
  }
 }
}

bool ChasmRZ_Tuple_Info::is_empty() const
{
 if(call_entry_node_)
 {
  if(caon_ptr<ChasmRZ_Call_Entry> rce = call_entry_node_->chasm_rz_call_entry())
  {
   return rce->flags.is_empty_tuple;
  }
 }
 return false;
}


QString ChasmRZ_Tuple_Info::asg_out(bool with_prefix) const
{
 QString result = "core::q-create :";

 if(with_prefix)
 {
  switch(formation_)
  {
  case Tuple_Formations::Indicates_Declarations:
   result = "decl-"; break;
  case Tuple_Formations::Indicates_Input:
   result = "input-"; break;
  case Tuple_Formations::Indicates_Final:
   result = "final-"; break;
  case Tuple_Formations::Indicates_String:
   result = "string-"; break;
  case Tuple_Formations::Indicates_Multi:
   result = "multi-"; break;
  case Tuple_Formations::Indicates_Functional:
   result = "funcational-"; break;
  case Tuple_Formations::Indicates_Text_Map:
   result = "text-map-"; break;
  case Tuple_Formations::Indicates_Text_Vector:
   result = "text-vector-"; break;
  case Tuple_Formations::Indicates_Pattern:
   result = "pattern-"; break;
  case Tuple_Formations::Indicates_Assignment:
   result = "assignment-"; break;
  default:
   result = "rz-";
   break;
  }
 }

 switch(indicator_)
 {
 case Tuple_Indicators::Enter_Array:
  result += "make-array"; break;
 case Tuple_Indicators::Enter_Vector:
  result += "make-vector"; break;
 case Tuple_Indicators::Enter_Map:
  result += "make-vmap"; break;
 case Tuple_Indicators::Enter_Set:
  result += "make-set"; break;
 default: break;
 }

 return result;

}
