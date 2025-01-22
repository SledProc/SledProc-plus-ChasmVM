
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-ngml-output-xml.h"

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

#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "rzns.h"

#define CHECK_SUPPRESS_NODE if(suppress_node_) return;


USING_RZNS(NGML)


NGML_Output_XML::NGML_Output_XML(NGML_Document& document)
 :  NGML_Output_Base(document), NGML_Output_Event_Handler(), 
    suppress_node_(nullptr), ngml_output_htxn_(nullptr)
{
 htxn_qts_.setString(&htxn_acc_);
 init_callbacks();
}

void NGML_Output_XML::init_callbacks()
{
 #include "rz-ngml-output-callbacks-common.h"

 #include "rz-ngml-output-xml.callbacks.h"
}

void NGML_Output_XML::write_xml_output(QString& xml_output)
{
 QTextStream qts(&xml_output);
 generate(qts);
 xml_output.replace("$OP$", "(");
 xml_output.replace("$CP$", ")");
 xml_output.replace("$EQ$", "=");
}

void NGML_Output_XML::generate(QTextStream& qts)
{
 NGML_Output_Event_Generator events(*this, *this);
 events.generate(qts);
}

void NGML_Output_XML::generate_root(const NGML_Output_Bundle& b, caon_ptr<NGML_Root> nr)
{
 nr->write_whitespace(b.qts);
}


void NGML_Output_XML::export_xml(QString path)
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
 QString xml_output;
 write_xml_output(xml_output);

 QFile outfile(path);
 if(outfile.open(QFile::WriteOnly | QIODevice::Text))
 {
  outfile.write(xml_output.toLatin1());
  outfile.close();
 }
}

caon_ptr<NGML_Command_Callback> NGML_Output_XML::check_command_callback(caon_ptr<NGML_Tag_Command> ntc)
{
 QString name = ntc->name();
 return callbacks_.value(name, caon_ptr<NGML_Command_Callback>( nullptr ));
}


//caon_ptr<NGML_Command_Callback> NGML_Output_XML::check_callback
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

void NGML_Output_XML::check_post_callback
 (QTextStream& qts, caon_ptr<NGML_Command_Callback> cb, caon_ptr<tNode> node, u4 index)
{
 if(cb->flags.has_post_callback)
 {
  cb->post_callback(qts, node, index, cb);
 }
}

void NGML_Output_XML::write_saved_xml(QTextStream& qts, caon_ptr<NGML_Node> node)
{
 CAON_PTR_DEBUG(NGML_Node ,node)
 if(caon_ptr<NGML_Tag_Command> ntc = node->ngml_tag_command())
 {
  CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)
  QString arg = ntc->argument();
  if(ngml_output_htxn_)
  {
   QString text;
   ngml_output_htxn_->write_saved_xml(arg, text);
   qts << text;
  }
 }
}


void NGML_Output_XML::generate_tag_command_auto_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{ 
 CHECK_SUPPRESS_NODE
 b.qts << "/>";
}

void NGML_Output_XML::generate_tag_command_argument(const NGML_Output_Bundle& b,
  NGML_HTXN_Node& nhn)
{
 CHECK_SUPPRESS_NODE
 HTXN_Node_Detail* nd = nhn.get_node_detail(htxn_document_);
 b.qts << (nd->flags.optional? "<opt>" : "<man>");
 htxn_document_->write_minimal_xml_out(nd->get_layer(), nd->enter, nd->leave, b.qts);
 b.qts << (nd->flags.optional? "<opt>" : "<man>");
}

void NGML_Output_XML::check_generate_tag_command_argument(const NGML_Output_Bundle& b,
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

void NGML_Output_XML::generate_tag_command_entry(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{ 
 CHECK_SUPPRESS_NODE
 HTXN_Node_Detail* nd = nhn.get_node_detail(htxn_document_);
 Glyph_Layer_8b* gl = nd->get_layer();

#ifdef HIDE
 if(nd->flags.pre_line_double_gap)
   check_reconcile_gap(b, 20);  //? b.qts << "\n\n";
 else if(nd->flags.pre_line_gap)
   check_reconcile_gap(b, 10);  //? b.qts << '\n';
 else if(nd->flags.pre_space_gap)
   check_reconcile_gap(b, 1);  //? b.qts << ' ';
#endif //def HIDE

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
  htxn_document_->write_minimal_xml_out(gl, nd->enter,
    nd->leave, htxn_qts_);

  b.qts << '<' << htxn_acc_;// << '}';
  end_names_[nd] = htxn_acc_;
  htxn_acc_.clear();
  htxn_qts_.reset();
 }
}


void NGML_Output_XML::generate_tag_command_entry(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{ 
 CAON_PTR_B_DEBUG(NGML_Node ,node)
 chiefs_.push(b.node);
 switch(b.connection_descriptor)
 {
 case NGML_Connection_Descriptor::Tag_Command_Cross_From_Blank:
 case NGML_Connection_Descriptor::Tag_Command_Cross:
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
  CHECK_SUPPRESS_NODE

  if(htxn_document_)
  {
   if(NGML_HTXN_Node* nhn = ntc->ngml_htxn_node())
   {
    generate_tag_command_entry(b, *nhn);
    check_generate_tag_command_argument(b, *ntc);
//?    reset_active_gap_code();
    break;
   }
  }

  QString command_print_name;
  if(cb && cb->flags.has_rename)
   command_print_name = cb->rename_tag();
  else
   command_print_name = ntc->name();

  QString style_class_name;
  if(cb && cb->flags.has_rename_style_class)
   style_class_name = cb->rename_style_class();

  b.qts << '\n' << '<' << command_print_name;
  if(!style_class_name.isEmpty())
   b.qts << " class='" << style_class_name << '\'';
  break;
 }
}

void NGML_Output_XML::generate_tag_command_leave(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
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
   b.qts << "</" << end_names_.take(nd) << ">";
//?   reset_active_gap_code();
  }
 }
 else if(nd->flags.region)
 {
  if(nd->flags.main_only_block_environment)
    ; //?    check_reconcile_gap(b, 10);  //? b.qts << '\n';
  b.qts << "</" << end_names_.take(nd) << '>';
//?  reset_active_gap_code();
 }
 else if(nd->flags.wmi_none)
   ; // nothing
 else
 {
  b.qts << "</" << end_names_.take(nd) << '>';
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



void NGML_Output_XML::generate_tag_command_leave(const NGML_Output_Bundle& b,
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

void NGML_Output_XML::generate_tag_body_leave(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
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

void NGML_Output_XML::generate_tag_body_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
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

void NGML_Output_XML::generate_tile_via_htxn(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{
 CHECK_SUPPRESS_NODE
 htxn_document_->write_minimal_xml_out(nhn.layer_code(), nhn.range(), b.qts);
 //? reset_active_gap_code();
}

