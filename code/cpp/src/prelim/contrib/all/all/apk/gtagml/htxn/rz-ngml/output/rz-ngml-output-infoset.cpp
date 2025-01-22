
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-ngml-output-infoset.h"

#include "rz-ngml-output-htxn.h"

#include "rz-ngml-output-event-generator.h"
#include "kernel/document/rz-ngml-document.h"
#include "kernel/output/rz-ngml-output-bundle.h"
#include "tile/rz-ngml-token.h"
#include "kernel/graph/rz-ngml-node.h"
#include "tag-command/rz-ngml-tag-command.h"
#include "kernel/rz-ngml-root.h"
#include "tag-command/rz-ngml-command-callback.h"

#include "ngml-htxn/ngml-htxn-node.h"
#include "htxn/htxn-document-8b.h"
#include "htxn/glyph-layer-8b.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "rzns.h"

#define CHECK_SUPPRESS_NODE if(suppress_node_) return;


USING_RZNS(NGML)


NGML_Output_Infoset::NGML_Output_Infoset(NGML_Document& document, HTXN_Infoset_8b* infoset)
 :  NGML_Output_Base(document), NGML_Output_Event_Handler(), 
    infoset_(infoset), suppress_node_(nullptr), 
    ngml_output_htxn_(nullptr), 
    held_sdi_sentence_end_index_(0), 
    canceled_sdi_sentence_start_index_(0),
    held_sdi_sentence_end_node_(nullptr)
{
 //?htxn_qts_.setString(&htxn_acc_);
 init_callbacks();
}

void NGML_Output_Infoset::init_callbacks()
{
 #include "rz-ngml-output-callbacks-common.h"

 #include "rz-ngml-output-infoset.callbacks.h"
}

// // common functionality for both insert methods ...

void _insert_start(u8* u, QVector<caon_ptr<NGML_Node>>* vec, u4 index, QString& pre_result, caon_ptr<NGML_Node> n, 
  u4& canceled_sdi_sentence_start_index)
{
 if(index == canceled_sdi_sentence_start_index)
 {
  // //  the \> supplants the \+ ...
  canceled_sdi_sentence_start_index = 0;
 }
 else
 {
  pre_result.append("\\+");
  if(u)
    ++*u;
  if(vec)
    vec->push_back(n);
 }
}

void _insert_end(u8* u, QVector<caon_ptr<NGML_Node>>* vec, u4 index,
  u4 new_index, QString& post_result, 
  QMap<u4, caon_ptr<NGML_Node>>& marked_sentence_starts,
  caon_ptr<NGML_Node> n, 
  u4& held_sdi_sentence_end_index,
  u4& canceled_sdi_sentence_start_index, 
  caon_ptr<NGML_Node>* held_sdi_sentence_end_node)
{
 if(new_index == index)
 {
  post_result.append("\\;");
  if(u)
    ++*u;
  if(vec)
    vec->push_back(n);
 }
 else
 {
  // // this assumes the post-space will always be 
   //   separated from a new sentence by one space or newline ...
  if(marked_sentence_starts.contains(new_index + 2))
  {
   held_sdi_sentence_end_index = new_index;
   if(held_sdi_sentence_end_node)
     *held_sdi_sentence_end_node = n;
   canceled_sdi_sentence_start_index = new_index + 2;
  }
  else
  {
   post_result.append("\\;");
   if(u)
     ++*u;
   if(vec)
     vec->push_back(n);
  }
 }
}

void _insert_start(u8* u, u4 index, QString& pre_result, u4& canceled_sdi_sentence_start_index)
{
 _insert_start(u, nullptr, index, pre_result, nullptr, canceled_sdi_sentence_start_index);
}

void _insert_start(QVector<caon_ptr<NGML_Node>>* vec, u4 index, QString& pre_result, caon_ptr<NGML_Node> n, u4& canceled_sdi_sentence_start_index)
{
 _insert_start(nullptr, vec, index, pre_result, n, canceled_sdi_sentence_start_index);
}

void _insert_end(u8* u, u4 index, u4 new_index, 
  QString& post_result, QMap<u4, caon_ptr<NGML_Node>>& marked_sentence_starts,
  u4& held_sdi_sentence_end_index,  
  u4& canceled_sdi_sentence_start_index)
{
 _insert_end(u, nullptr, index, new_index, post_result, 
   marked_sentence_starts, nullptr, 
   held_sdi_sentence_end_index, canceled_sdi_sentence_start_index, 
   nullptr);
}

void _insert_end(QVector<caon_ptr<NGML_Node>>* vec, 
  u4 index, u4 new_index, 
  QString& post_result, 
  QMap<u4, caon_ptr<NGML_Node>>& marked_sentence_starts, caon_ptr<NGML_Node> n, 
  u4& held_sdi_sentence_end_index,
  u4& canceled_sdi_sentence_start_index, 
  caon_ptr<NGML_Node>* held_sdi_sentence_end_node)
{
 _insert_end(nullptr, vec, index, new_index, post_result, 
   marked_sentence_starts, n, 
   held_sdi_sentence_end_index, canceled_sdi_sentence_start_index,
   held_sdi_sentence_end_node);
}


u8 NGML_Output_Infoset::check_sdi_latex_insert(Glyph_Layer_8b* gl, u4 index, QString& pre_result, QString& post_result)
{
 u8 count = 0;
 if(index == held_sdi_sentence_end_index_)
 {
  ++count;
  held_sdi_sentence_end_index_ = 0;
  post_result.append("\\>");
 }
 {
  auto it = marked_paragraph_starts_.find(index);
  if(it != marked_paragraph_starts_.end())
  {
   pre_result.append("\\:");
   ++count;
  }
 }
 {
  auto it = marked_sentence_starts_.find(index);
  if(it != marked_sentence_starts_.end())
  {
   _insert_start(&count, index, pre_result, canceled_sdi_sentence_start_index_);
//   if(index == canceled_sdi_sentence_start_index_)
//   {
//    // //  the \> supplants the \+ ...
//    canceled_sdi_sentence_start_index_ = 0;
//   }
//   else
//   {
//    pre_result.append("\\+");
//    ++count;
//   }
  }
 }
 {
  auto it = sdi_marks_.find(index);
  if(it != sdi_marks_.end())
  {
   if(it.value().first.isEmpty())
     pre_result.append("\\?");
   else
     pre_result.append(QString("\\?[%1]").arg(it.value().first));
   ++count;
  }
 }
 {
  auto it = sdi_secondary_marks_.find(index);
  if(it != sdi_secondary_marks_.end())
  {
   if(it.value().first.isEmpty())
     post_result.append("\\,");
   else
     post_result.append(QString("\\,[%1]").arg(it.value().first));
   ++count;
  }
 }
 {
  auto it = marked_sentence_ends_.find(index);
  if(it != marked_sentence_ends_.end())
  {
   u4 new_index = htxn_document_->
     check_advance_to_sentence_end_space(gl, index);

   _insert_end(&count, index, new_index, post_result, 
     marked_sentence_starts_,
     held_sdi_sentence_end_index_,
     canceled_sdi_sentence_start_index_);
   #ifdef HIDE
   u4 new_index = htxn_document_->
     check_advance_to_sentence_end_space(gl, index);

   if(new_index == index)
   {
    post_result.append("\\;");
    ++count;
   }
   else
   {
    // // this assumes the post-space will always be 
     //   separated from a new sentence by one space or newline ...
    if(marked_sentence_starts_.contains(new_index + 2))
    {
     held_sdi_sentence_end_index_ = new_index;
     canceled_sdi_sentence_start_index_ = new_index + 2;
    }
    else
    {
     post_result.append("\\;");
     ++count;
    }
   }
   #endif HIDE
  }
 }
 {
  auto it = marked_paragraph_ends_.find(index);
  if(it != marked_paragraph_ends_.end())
  {
   post_result.append("\\<");
   ++count;
  }
 }
}

QVector<caon_ptr<NGML_Node>> NGML_Output_Infoset::get_sdi_latex_insert_nodes(Glyph_Layer_8b* gl, u4 index, QString& pre_result, QString& post_result)
{
 QVector<caon_ptr<tNode>> rvec;
 if(index == held_sdi_sentence_end_index_)
 {
  rvec.push_back(held_sdi_sentence_end_node_);
  held_sdi_sentence_end_index_ = 0;
  post_result.append("\\>");
 }
 {
  auto it = marked_paragraph_starts_.find(index);
  if(it != marked_paragraph_starts_.end())
  {
   pre_result.append("\\:");
   rvec.push_back(it.value());
  }
 }
 {
  auto it = marked_sentence_starts_.find(index);
  if(it != marked_sentence_starts_.end())
  {
   _insert_start(&rvec, index, pre_result, it.value(), canceled_sdi_sentence_start_index_);
//   pre_result.append("\\+");
//   rvec.push_back(it.value());
  }
 }
 {
  auto it = sdi_marks_.find(index);
  if(it != sdi_marks_.end())
  {
   if(it.value().first.isEmpty())
     pre_result.append("\\?");
   else
     pre_result.append(QString("\\?[%1]").arg(it.value().first));
   rvec.push_back(it.value().second);
  }
 }
 {
  auto it = sdi_secondary_marks_.find(index);
  if(it != sdi_secondary_marks_.end())
  {
   if(it.value().first.isEmpty())
     post_result.append("\\,");
   else
     post_result.append(QString("\\,[%1]").arg(it.value().first));
   rvec.push_back(it.value().second);
  }
 }
 {
  auto it = marked_sentence_ends_.find(index);
  if(it != marked_sentence_ends_.end())
  {
   u4 new_index = htxn_document_->
     check_advance_to_sentence_end_space(gl, index);

   _insert_end(&rvec, index, new_index, post_result, 
      marked_sentence_starts_,
      it.value(),   
      held_sdi_sentence_end_index_,
      canceled_sdi_sentence_start_index_,
      &held_sdi_sentence_end_node_);

//   u4 new_index = htxn_document_->
//     check_advance_to_sentence_end_space(gl, index);
//   if(new_index == index)
//     post_result.append("\\;");
//   else
//     post_result.append("\\>");

//   rvec.push_back(it.value());
  }
 }
 {
  auto it = marked_paragraph_ends_.find(index);
  if(it != marked_paragraph_ends_.end())
  {
   pre_result.append("\\<");
   rvec.push_back(it.value());
  }
 }
 return rvec;
}


void NGML_Output_Infoset::write_infoset_output(QString& output)
{
 QTextStream qts(&output);
 generate(qts);
}

void NGML_Output_Infoset::generate(QTextStream& qts)
{
 NGML_Output_Event_Generator events(*this, *this);
 events.generate(qts);
}

void NGML_Output_Infoset::generate_root(const NGML_Output_Bundle& b, caon_ptr<NGML_Root> nr)
{
 nr->write_whitespace(b.qts);
}


void NGML_Output_Infoset::export_infoset(QString path)
{
 if(path.startsWith(".."))
 {
  path.remove(0, 1);
  path.prepend(document_.local_path());
 }
 else if(path.startsWith('.'))
 {
  QFileInfo qfi(document_.local_path());
  path.prepend(qfi.absolutePath() + '/' + qfi.completeBaseName());
 }
 QString output;
 write_infoset_output(output);

 QFile outfile(path);
 if(outfile.open(QFile::WriteOnly | QIODevice::Text))
 {
  outfile.write(output.toLatin1());
  outfile.close();
 }
}

caon_ptr<NGML_Command_Callback> NGML_Output_Infoset::check_command_callback(caon_ptr<NGML_Tag_Command> ntc)
{
 QString name = ntc->name();
 return callbacks_.value(name, caon_ptr<NGML_Command_Callback>( nullptr ));
}


//caon_ptr<NGML_Command_Callback> NGML_Output_Infoset::check_callback
// (QTextStream& qts, caon_ptr<NGML_Tag_Command> ntc, caon_ptr<tNode> node)
//{
// caon_ptr<NGML_Command_Callback> result = nullptr;
// QString name = ntc->name();
// if(callbacks_.contains(name))
// {
//  result = callbacks_[name];
//  if(result->flags.has_pre_callback)
//  {
//   result->pre_callback(qts, node, result);
//  }
// }
// return result;
//}

void NGML_Output_Infoset::check_post_callback
 (QTextStream& qts, caon_ptr<NGML_Command_Callback> cb, caon_ptr<tNode> node, u4 index)
{
 if(cb->flags.has_post_callback)
 {
  cb->post_callback(qts, node, index, cb);
 }
}

void NGML_Output_Infoset::generate_tag_command_auto_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{ 
 CHECK_SUPPRESS_NODE
//? b.qts << "/>";
}

void NGML_Output_Infoset::generate_tag_command_argument(const NGML_Output_Bundle& b,
  NGML_HTXN_Node& nhn)
{
 CHECK_SUPPRESS_NODE
 HTXN_Node_Detail* nd = nhn.get_node_detail(htxn_document_);
 b.qts << (nd->flags.optional? "<opt>" : "<man>");
 //?htxn_document_->write_minimal_xml_out(nd->get_layer(), nd->enter, nd->leave, b.qts);
 b.qts << (nd->flags.optional? "<opt>" : "<man>");
}

void NGML_Output_Infoset::check_generate_tag_command_argument(const NGML_Output_Bundle& b,
  NGML_Tag_Command& ntc)
{
 CHECK_SUPPRESS_NODE
 if(NGML_HTXN_Node* nhn = ntc.arg_ngml_htxn_node())
   generate_tag_command_argument(b, *nhn);

 ntc.each_arg_ngml_htxn_node([&b, this](NGML_HTXN_Node* nhn)
 {
  generate_tag_command_argument(b, *nhn);
 });
}

void NGML_Output_Infoset::generate_tag_command_entry(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{ 
// CHECK_SUPPRESS_NODE
 HTXN_Node_Detail* nd = nhn.get_node_detail(htxn_document_);
 Glyph_Layer_8b* gl = nd->get_layer();

#ifdef HIDE
 if(nd->flags.pre_line_double_gap)
   check_reconcile_gap(b, 20);  //? b.qts << "\n\n";
 else if(nd->flags.pre_line_gap)
   check_reconcile_gap(b, 10);  //? b.qts << '\n';
 else if(nd->flags.pre_space_gap)
   check_reconcile_gap(b, 1);  //? b.qts << ' ';

 if(nd->flags.is_ghosted)
 {
  if(nd->flags.region_main_preempts_wrap)
  {
   if(nd->flags.block_environment_marked_main)
     ; //? check_reconcile_gap(b, 10);  //? b.qts << '\n';
  }
  else if(nd->flags.optional)
  {
   b.qts << "<opt>";
 //?  reset_active_gap_code();
  }
  else
  {
   b.qts << "<man>";
//?   reset_active_gap_code();
  }
 }
 else
 {
//?  htxn_document_->write_minimal_xml_out(gl, nd->enter,
//?    nd->leave, htxn_qts_);

  b.qts << '<' << htxn_acc_;// << '}';
  end_names_[nd] = htxn_acc_;
  htxn_acc_.clear();
  htxn_qts_.reset();
 }
#endif //def HIDE
}


void NGML_Output_Infoset::generate_tag_command_entry(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{ 
 CAON_PTR_B_DEBUG(NGML_Node ,node)
 chiefs_.push(b.node);
 switch(b.connection_descriptor)
 {
 case NGML_Connection_Descriptor::Tag_Command_Cross:
 case NGML_Connection_Descriptor::Tag_Command_Cross_From_Blank:
  //?
 case NGML_Connection_Descriptor::Tag_Command_Entry:

  caon_ptr<NGML_Command_Callback> cb = b.cb;

  if(cb)
  {
   CAON_PTR_DEBUG(NGML_Command_Callback ,cb)

   if(cb->flags.has_around_callback)
   {
    cb->around_callback(b.qts, b.node, b.index, b.cb);
    break;
   }

   if(cb->flags.has_pre_callback)
    cb->pre_callback(b.qts, b.node, b.index, b.cb);
   if(!cb->flags.pre_fallthrough)
    break;
  }
//?  CHECK_SUPPRESS_NODE

  if(htxn_document_)
  {
   if(NGML_HTXN_Node* nhn = ntc->ngml_htxn_node())
   {
    generate_tag_command_entry(b, *nhn);
    check_generate_tag_command_argument(b, *ntc);
    break;
   }
  }
 }
}

void NGML_Output_Infoset::check_sentence_boundaries(QTextStream& qts, caon_ptr<tNode> node)
{
 CAON_PTR_DEBUG(tNode ,node)
 if(caon_ptr<NGML_Tag_Command> ntc = node->ngml_tag_command())
 {
  CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)
  if(NGML_HTXN_Node* nhn = ntc->ngml_htxn_node())
  {
   u4 enter = nhn->get_ref_enter();
   if(enter > 1)
   {
    u4 leave = nhn->get_ref_leave();
    if(leave >= enter)
    {
     // //  this assumes that ntc is a paragraph ...
     marked_paragraph_starts_[enter] = node;
     marked_paragraph_ends_[leave] = node;
     check_sentence_boundaries(qts, node, *nhn, enter, leave);
    } 
   }
  }
 } 
}

void NGML_Output_Infoset::mark_sentence(QTextStream& qts, caon_ptr<tNode> node, u4 enter, u4 leave)//, u4 pleave)
{
 marked_sentence_starts_[enter] = node;
 marked_sentence_ends_[leave] = node;
 //qts << "\n$ " << enter << " " << leave;
}

void NGML_Output_Infoset::check_sentence_boundaries(QTextStream& qts,
  caon_ptr<tNode> node, NGML_HTXN_Node& nhn, u4 enter, u4 leave)
{ 
 QMap<u4, QString> notes;
 Glyph_Layer_8b* gl = ngml_output_htxn_->main_gl();
 htxn_document_->check_sentence_boundaries(gl, 
   enter, leave, notes);
 u4 i = enter;
 while(i <= leave)
 {
  u4 ss = 0;
  if(!htxn_document_->scan_for_sentence_start(gl, i, leave, ss))
    break;
  u4 se = 0;
  if(!htxn_document_->scan_for_sentence_end(gl, ss, leave, se))
    break;
  
  mark_sentence(qts, node, ss, se);
  i = se + 1;
 }
}
// QSet<u4> marked_sentence_starts_;
// QSet<u4> marked_non_pivot_sentence_ends_;

void NGML_Output_Infoset::generate_tag_command_leave(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{ 
 HTXN_Node_Detail* nd = nhn.get_node_detail(htxn_document_);

 if(nd->flags.is_ghosted)
 {
  if(nd->flags.region_main_preempts_wrap)
    ; // nothing ...
  else if(nd->flags.optional)
  {
   b.qts << "</opt>";
//?   reset_active_gap_code();
  }
  else
  {
   b.qts << "</man>";
//?   reset_active_gap_code();
  }
 }
 else if(nd->flags.ref_preempts_wrap)
 {
  if(nd->flags.region)
  {
    // // should be some sort of whitespace flag ...
//?   check_reconcile_gap(b, 10);  //? b.qts << '\n';
   //?b.qts << "</" << end_names_.take(nd) << ">";
//?   reset_active_gap_code();
  }
 }
 else if(nd->flags.region)
 {
  if(nd->flags.main_only_block_environment)
    ; //?    check_reconcile_gap(b, 10);  //? b.qts << '\n';
  //?b.qts << "</" << end_names_.take(nd) << '>';
//?  reset_active_gap_code();
 }
 else if(nd->flags.wmi_none)
   ; // nothing
 else
 {
  //?b.qts << "</" << end_names_.take(nd) << '>';
//?  reset_active_gap_code();
 }

#ifdef HIDE
 if(nd->flags.post_line_double_gap)
   check_reconcile_gap(b, 20);  //? b.qts << "\n\n";
 else if(nd->flags.post_line_gap)
   check_reconcile_gap(b, 10);  //? b.qts << '\n';
 else if(nd->flags.post_space_gap)
   check_reconcile_gap(b, 1);  //? b.qts << ' ';
#endif // HIDE
}



void NGML_Output_Infoset::generate_tag_command_leave(const NGML_Output_Bundle& b,
 caon_ptr<NGML_Tag_Command> ntc)
{
 if(b.cb)
 {
  if(b.cb->flags.has_post_callback)
  {
   b.cb->post_callback(b.qts, b.node, b.index, b.cb);
  }
  if(!b.cb->flags.post_fallthrough)
   return;
 }

 CHECK_SUPPRESS_NODE
 if(htxn_document_)
 {
  if(NGML_HTXN_Node* nhn = ntc->ngml_htxn_node())
  {
   generate_tag_command_leave(b, *nhn);
   return;
  }
 }

 QString command_print_name;
 if(b.cb && b.cb->flags.has_rename)
  command_print_name = b.cb->rename_tag();
 else
  command_print_name = ntc->name();
 b.qts << "</" << ntc->name() << '>';
}

void NGML_Output_Infoset::generate_tag_body_leave(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{
 CHECK_SUPPRESS_NODE
 HTXN_Node_Detail* nd = nhn.get_node_detail(htxn_document_);

#ifdef HIDE
 if(nd->flags.is_ghosted)
   ; // nothing
 else if(nd->flags.wmi_left)
   ; // nothing
 else if(nd->flags.wmi_none)
 {
   ; // nothing
 }
 else if(nd->flags.ref_preempts_wrap)
 {
  if(nd->flags.region)
  {
   b.qts << '}';
   reset_active_gap_code();
  }
 }
 else if(nd->flags.region)
 {
  b.qts << '}';
  if(nd->flags.main_only_block_environment)
  {
    //?check_reconcile_gap(b, 10);  //? 
   b.qts << '\n';
   active_gap_code_ = 10;
  }
  reset_active_gap_code();
 }
 else
 {
  b.qts << '{';
  reset_active_gap_code();
 }
#endif // HIDE
 
 b.qts << '>';
}

void NGML_Output_Infoset::generate_tag_body_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{ 
 CHECK_SUPPRESS_NODE
 CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)

 if(htxn_document_)
 {
  if(NGML_HTXN_Node* nhn = ntc->ngml_htxn_node())
  {
   generate_tag_body_leave(b, *nhn);
   return;
  }
 }

 b.qts << '>';
 ntc->write_whitespace(b.qts);
}

void NGML_Output_Infoset::generate_tile_via_htxn(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{
//? CHECK_SUPPRESS_NODE
//? htxn_document_->write_minimal_xml_out(nhn.layer_code(), nhn.range(), b.qts);
 //? reset_active_gap_code();
}

