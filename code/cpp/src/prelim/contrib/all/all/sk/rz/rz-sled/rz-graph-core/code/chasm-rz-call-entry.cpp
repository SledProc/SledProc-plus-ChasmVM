
#include "chasm-rz-call-entry.h"

#include "kernel/graph/chasm-rz-node.h"

#include "token/chasm-rz-token.h"

#include "rzns.h"

#include <QDebug>



USING_RZNS(RZ_Core)

ChasmRZ_Call_Entry::ChasmRZ_Call_Entry(int call_id, QString prefix) //caon_ptr<ChasmRZ_Node> parent_entry_node)
 : Flags(0), call_depth_(0),
 line_number_(0),
 file_id_(0),
 call_id_(call_id),
 ref_node_(nullptr),
 parent_entry_node_(nullptr),
  self_node_(nullptr),
  block_entry_node_(nullptr),
  block_chief_node_(nullptr), block_continue_node_(nullptr),
  prefix_(prefix)
{
 if(!prefix.isEmpty())
 {
  if(prefix == "\\")
  {
   flags.is_deferred = true;
  }
  else if(prefix == "`")
  {
   flags.is_backquoted = true;
   flags.no_normalize = true;
   flags.no_anticipate = true;
  }
  else if(prefix == ",")
  {
   flags.is_unquoted = true;
   flags.no_normalize = true;
   flags.no_anticipate = true;
  }

 }
}

// // //   After completed first pass graph build:
// // //    If ref_node_ == parent_entry_node_, or nullptr, then this
//           is in tail position.
//          If ref_node_ as ChasmRZ_Call_Entry is this, then this is an
//           expression followed by a nonnested token.
//          Otherwise ref_node_ points to a "sibling" tuple entry.
//


caon_ptr<ChasmRZ_Call_Entry> ChasmRZ_Call_Entry::unwind()
{
 if(parent_entry_node_)
  return parent_entry_node_->chasm_rz_call_entry();
 return nullptr;
}

caon_ptr<ChasmRZ_Call_Entry> ChasmRZ_Call_Entry::unwind(int& unwind_count)
{
 if(parent_entry_node_)
 {
  ++unwind_count;
  return parent_entry_node_->chasm_rz_call_entry();
 }
 else
 {
  unwind_count = 0;
  return nullptr;
 }
}


void ChasmRZ_Call_Entry::debug_check_entry(const ChasmRZ_Connectors& conn)
{
 qDebug() << "call depth: " << call_depth_;
 if(ref_node_)
 {
  if(caon_ptr<ChasmRZ_Node> node = ref_node_->retrieve(conn))
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,node);
   if(caon_ptr<ChasmRZ_Token> token = node->chasm_rz_token())
   {
    ChasmRZ_Token& tok = *token;
    qDebug() << "ref: " << tok.raw_text();
   }

  }
 }

 if(parent_entry_node_)
 {
  if(caon_ptr<ChasmRZ_Node> node = parent_entry_node_->retrieve(conn))
  {
   CAON_PTR_DEBUG(ChasmRZ_Node ,node);
   if(caon_ptr<ChasmRZ_Token> token = node->chasm_rz_token())
   {
    ChasmRZ_Token& tok = *token;
    qDebug() << "parent: " << tok.raw_text();
   }
  }
 }

}


caon_ptr<ChasmRZ_Node> ChasmRZ_Call_Entry::get_next_entry_node()
{
 if(ref_node_ == parent_entry_node_)
  return nullptr;
 else
  return ref_node_;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Call_Entry::get_next_entry_node(
  caon_ptr<ChasmRZ_Call_Entry>& next_call_entry, int& unwind_count)
{
 CAON_PTR_DEBUG(ChasmRZ_Node ,ref_node_)
 CAON_PTR_DEBUG(ChasmRZ_Node ,parent_entry_node_)


 if(ref_node_ == parent_entry_node_)
 {
  unwind_count = 1;
  next_call_entry = unwind(unwind_count);
  return nullptr;
 }
 else if(ref_node_)
 {
  if(ref_node_->chasm_rz_call_entry() == this)
  {
   unwind_count = 1;
   next_call_entry = unwind(unwind_count);
   return ref_node_;
  }
  else
  {
   unwind_count = 0;
   next_call_entry = ref_node_->chasm_rz_call_entry();
   return ref_node_;
  }
 }
 else
 {
  unwind_count = 1;
  next_call_entry = unwind(unwind_count);
  return nullptr;
 }
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Call_Entry::get_parent_entry_node_unwind()
{
 if(!ref_node_)
  return parent_entry_node_;
 if(ref_node_ == parent_entry_node_)
  return ref_node_;
 return nullptr;
}

caon_ptr<ChasmRZ_Node> ChasmRZ_Call_Entry::get_node()
{
 return self_node_;
}

