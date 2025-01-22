
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-ngml-output-latex.h"

#include "rz-ngml-output-event-generator.h"
#include "kernel/document/rz-ngml-document.h"
#include "kernel/output/rz-ngml-output-bundle.h"
#include "tag-command/rz-ngml-tag-command.h"
#include "tile/rz-ngml-paralex-tile.h"

#include "tag-command/rz-ngml-command-callback.h"

#include "kernel/graph/rz-ngml-node.h"

#include "ngml-htxn/ngml-htxn-node.h"


#include "htxn/htxn-document-8b.h"
#include "htxn/glyph-layer-8b.h"

#include "htxn/infoset/sdi-callback-8b.h"

#include "rz-ngml-output-infoset.h"
#include "rz-ngml-output-htxn.h"

#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>

#include "rzns.h"
USING_RZNS(NGML)

struct _SDI_Callback : public SDI_Callback_8b
{
 NGML_Output_Infoset& infoset;
 u4 target_layer_id;
 _SDI_Callback(NGML_Output_Infoset& noi, u4 u);
 void pre_write(Glyph_Layer_8b& gl, u4 pos,
   Glyph_Argument_Package& gap, u8& skip_flag, 
   QString& pre_insert, QString& post_insert) Q_DECL_OVERRIDE;
};

_SDI_Callback::_SDI_Callback(NGML_Output_Infoset& noi, u4 u)
  :  infoset(noi), target_layer_id(u)
{

}


NGML_Output_Latex::NGML_Output_Latex(NGML_Document& document)
  :  NGML_Output_Base(document), active_gap_code_(0), 
     suppress_node_(nullptr), infoset_(nullptr)
{
 htxn_qts_.setString(&htxn_acc_);
 ws_qts_.setString(&ws_acc_);
 init_callbacks();
}

void NGML_Output_Latex::init_callbacks()
{
 #include "rz-ngml-output-callbacks-common.h"

 #include "rz-ngml-output-latex.callbacks.h"
}

#define inframe NGML_Frame::instance(),


void NGML_Output_Latex::generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Attribute_Tile> tile)
{
}


void NGML_Output_Latex::write_latex_output(QString& latex_output)
{
 QTextStream qts(&latex_output);
 generate(qts);
 latex_output.replace(QRegularExpression("\\n{2,}"), "\n\n");
}

void NGML_Output_Latex::generate(QTextStream& qts)
{
 NGML_Output_Event_Generator events(*this, *this);
 events.generate(qts);
}

void NGML_Output_Latex::export_latex(QString path)
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
 QString latex_output;
 write_latex_output(latex_output);

 // //temp
 latex_output.replace("@@", "%%");
 latex_output.replace("#{", "\\scaps{");
 latex_output.replace("}#", "}");

 QFile outfile(path);
 if(outfile.open(QFile::WriteOnly | QIODevice::Text))
 {
  outfile.write(latex_output.toLatin1());
  outfile.close();
 }
}

void NGML_Output_Latex::handle_annotation(caon_ptr<NGML_Tile> tile, caon_ptr<NGML_Annotation_Tile> atile)
{
 document_.tag_command_annotation(tile, atile);
}

void NGML_Output_Latex::check_reconcile_gap(const NGML_Output_Bundle& b, u1 new_code)
{
 check_reconcile_gap(b, new_code, active_gap_code_);
}

void NGML_Output_Latex::check_reconcile_gap(const NGML_Output_Bundle& b, u1 new_code, u1& result)
{
 u2 code = (active_gap_code_ * 100) + new_code;

 switch(code)
 {
 case 2020:
  result = 20; break;
 case 2121: 
  result = 21; break;
 case 2120:
  result = 21; break;
 case 2010:
  result = 20; break;
 case 2111:
  result = 21; break;
 case 2110:
  result = 21; break;
 case 1010:
  result = 10; break;
 case 1111:
  result = 11; break;
 case 1110:
  result = 11; break;
 case 2101:
  result = 21; break;
 case 1101:
  result = 11; break;
 case 1011:
  b.qts << ' '; result = 11; break;
 case 2021:
  b.qts << ' '; result = 21; break;
 case 1001:
  b.qts << ' '; result = 11; break;
 case 2001:
  b.qts << ' '; result = 21; break;
 case 1:
  b.qts << ' '; result = 1; break;
 case 1020:
  b.qts << '\n'; result = 20; break;
 case 1120:
  b.qts << '\n'; result = 20; break;
 case 10:
  b.qts << '\n'; result = 10; break;
 case 1021:
  b.qts << "\n "; result = 21; break;
 case 1121:
  b.qts << "\n "; result = 21; break;
 case 11:
  b.qts << "\n "; result = 11; break;
 case 21:
  b.qts << "\n\n "; result = 21; break;
 case 20:
  b.qts << "\n\n"; result = 20; break;
 default: break;
 }
}


void NGML_Output_Latex::generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Paralex_Tile> tile)
{
 tile->write_latex(b.qts);
 check_generate_whitespace(b, tile);
 reset_active_gap_code();
}

void NGML_Output_Latex::generate_tag_command_entry(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{
 HTXN_Node_Detail* nd = nhn.get_node_detail(htxn_document_);
 Glyph_Layer_8b* gl = nd->get_layer();

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
     check_reconcile_gap(b, 10);  //? b.qts << '\n';
  }
  else if(nd->flags.optional)
  {
   b.qts << '[';
   reset_active_gap_code();
  }
  else
  {
   b.qts << '{';
   reset_active_gap_code();
  }
 }
 else
 {
  htxn_document_->write_minimal_latex_out(gl, nd->enter,
    nd->leave, htxn_qts_);

//  if(nd->flags.ref_preempts_wrap)
//    b.qts << '\\' << htxn_acc_;
  if(nd->flags.region)
  {
   b.qts << "\\begin{" << htxn_acc_;// << '}';
   region_end_names_[nd] = htxn_acc_;
  }
  else if(nd->flags.wmi_left)
  {
   if(nd->flags.wmi_with_space)
     b.qts << "{\\" << htxn_acc_ << ' ';
   else
     b.qts << "{\\" << htxn_acc_;
  }
  else
    b.qts << '\\' << htxn_acc_;// << '{';
  htxn_acc_.clear();
  htxn_qts_.reset();
 }
}

void _SDI_Callback::pre_write(Glyph_Layer_8b& gl, u4 pos,
  Glyph_Argument_Package& gap, u8& skip_flag, 
  QString& pre_insert, QString& post_insert)
{
 if(gl.id() != target_layer_id)
   return;
 QString pre_result;
 QString post_result;
 u8 inserts = infoset.check_sdi_latex_insert(&gl, pos, 
   pre_result, post_result);

 if(inserts)
 {
  pre_insert = pre_result;
  post_insert = post_result;
 }
}

void NGML_Output_Latex::generate_tile_via_htxn(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{
 if(infoset_)
 {
  u4 id = infoset_->ngml_output_htxn()->main_gl()->id();
  _SDI_Callback cb(*infoset_, id);
  htxn_document_->write_minimal_latex_out(nhn.layer_code(), nhn.range(), b.qts, &cb);
 }
 else
   htxn_document_->write_minimal_latex_out(nhn.layer_code(), nhn.range(), b.qts);
 reset_active_gap_code();
}

void NGML_Output_Latex::generate_tag_command_argument(const NGML_Output_Bundle& b,
  NGML_HTXN_Node& nhn)
{
 HTXN_Node_Detail* nd = nhn.get_node_detail(htxn_document_);
 b.qts << (nd->flags.optional? '[' : '{');
 htxn_document_->write_minimal_latex_out(nd->get_layer(), nd->enter, nd->leave, b.qts);
 b.qts << (nd->flags.optional? ']' : '}');
}

void NGML_Output_Latex::check_generate_tag_command_argument(const NGML_Output_Bundle& b,
  NGML_Tag_Command& ntc)
{
 if(NGML_HTXN_Node* nhn = ntc.arg_ngml_htxn_node())
   generate_tag_command_argument(b, *nhn);

 ntc.each_arg_ngml_htxn_node([&b, this](NGML_HTXN_Node* nhn)
 {
  generate_tag_command_argument(b, *nhn);
 });
}

void NGML_Output_Latex::generate_tag_command_entry(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
 CAON_PTR_B_DEBUG(NGML_Node ,node)
 CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)

 caon_ptr<NGML_Command_Callback> cb = b.cb;


 switch(b.connection_descriptor)
 {
 case NGML_Connection_Descriptor::Tag_Command_Cross_From_Blank:
 case NGML_Connection_Descriptor::Tag_Command_Cross:
 case NGML_Connection_Descriptor::Tag_Command_Entry:
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

  if(htxn_document_)
  {
   if(NGML_HTXN_Node* nhn = ntc->ngml_htxn_node())
   {
    generate_tag_command_entry(b, *nhn);
    check_generate_tag_command_argument(b, *ntc);
    reset_active_gap_code();
    break;
   }
  }

  if(ntc->flags.is_region)
    b.qts << "\\begin{" << ntc->latex_name();
  else
    b.qts << '\\' << ntc->latex_name();
  reset_active_gap_code();
  break;
 }

 if(caon_ptr<tNode> n = qry_.Tag_Command_Annotation(inframe b.node))
 {
  if(caon_ptr<tNode> mt = qry_.Tag_Command_Main_Tile(inframe b.node))
  {
   if(caon_ptr<NGML_Tile> nt = mt->ngml_tile())
    tag_command_annotation(nt, n);
  }
 }
}

void NGML_Output_Latex::generate_tag_command_leave(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{
 HTXN_Node_Detail* nd = nhn.get_node_detail(htxn_document_);

 if(nd->flags.is_ghosted)
 {
  if(nd->flags.region_main_preempts_wrap)
    ; // nothing ...
  else if(nd->flags.optional)
  {
   b.qts << ']';
   reset_active_gap_code();
  }
  else
  {
   b.qts << '}';
   reset_active_gap_code();
  }
 }
 else if(nd->flags.ref_preempts_wrap)
 {
  if(nd->flags.region)
  {
    // // should be some sort of whitespace flag ...
   check_reconcile_gap(b, 10);  //? b.qts << '\n';
   b.qts << "\\end{" << region_end_names_.take(nd) << '}';
   reset_active_gap_code();
  }
 }
 else if(nd->flags.region)
 {
  if(nd->flags.main_only_block_environment)
    check_reconcile_gap(b, 10);  //? b.qts << '\n';
  b.qts << "\\end{" << region_end_names_.take(nd) << '}';
  reset_active_gap_code();
 }
 else if(nd->flags.wmi_none)
   ; // nothing
 else
 {
  b.qts << '}';
  reset_active_gap_code();
 }

 if(nd->flags.post_line_double_gap)
   check_reconcile_gap(b, 20);  //? b.qts << "\n\n";
 else if(nd->flags.post_line_gap)
   check_reconcile_gap(b, 10);  //? b.qts << '\n';
 else if(nd->flags.post_space_gap)
   check_reconcile_gap(b, 1);  //? b.qts << ' ';
}


void NGML_Output_Latex::generate_tag_command_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
 CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)

 if(b.cb)
 {
  if(b.cb->flags.has_post_callback)
  {
   b.cb->post_callback(b.qts, b.node, b.index, b.cb);
  }
  if(!b.cb->flags.post_fallthrough)
   return;
 }

 if(htxn_document_)
 {
  if(NGML_HTXN_Node* nhn = ntc->ngml_htxn_node())
  {
   generate_tag_command_leave(b, *nhn);
   return;
  }
 }

 if(ntc->flags.is_region)
   b.qts << "\\end{" << ntc->latex_name() << '}';
 else
   b.qts << '}';

 reset_active_gap_code();
}

void NGML_Output_Latex::check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Tile> tile)
{
 NGML_Output_Event_Handler::check_generate_whitespace(b, tile);
}

void NGML_Output_Latex::check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Paralex_Tile> tile)
{
 NGML_Output_Event_Handler::check_generate_whitespace(b, tile);
}

u1 NGML_Output_Latex::get_ws_gap_code()
{
 return get_ws_gap_code(ws_acc_);
}

u1 NGML_Output_Latex::get_ws_gap_code(const QString& str)
{
 if(str.endsWith("\n\n"))
   return 20;
 if(str.endsWith("\n"))
   return 10;
 if(str.endsWith("\n\n "))
   return 21;
 if(str.endsWith("\n "))
   return 11;
 if(str.endsWith(' '))
   return 1;
 return 0;
}


void NGML_Output_Latex::check_generate_whitespace(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{
 HTXN_Node_Detail* nd = nhn.get_node_detail(htxn_document_);
 if(!nd)
   return;
 if(nd->flags.wmi_none)
   return;
  // // or take ws from the node detail?

 nhn.write_whitespace(ws_qts_); //b.qts);

 if(ws_acc_.isEmpty())
 {
  ws_qts_.reset();
  return;
 }

 u1 wsgc = get_ws_gap_code();

 if(active_gap_code_ == 0)
 {
  active_gap_code_ = wsgc;
  b.qts << ws_acc_;
  ws_acc_.clear();
  ws_qts_.reset();
  return;
 }
 if(wsgc == 0)
 {
  b.qts << ws_acc_;
  ws_acc_.clear();
  ws_qts_.reset();
  reset_active_gap_code();
  return;
 }

 u2 code = (active_gap_code_ * 100) + wsgc;

 switch (code)
 {
 case 2121:
   ws_acc_.chop(3); break;
 case 2020:
 case 1111:
   ws_acc_.chop(2); break;
 case 1010:
 case 101:
   ws_acc_.chop(1); break;
 case 2010:
   ws_acc_.chop(1); break;
 case 1020:
   ws_acc_.chop(1); active_gap_code_ = 20; break;
 case 2110:
   active_gap_code_ = 10; break;
 case 2120:
   active_gap_code_ = 20; break;
 case 2111:
   ws_acc_.chop(2); break;
 case 2101:
   ws_acc_.chop(1); break;
 case 110:
  active_gap_code_ = 10; break;
 case 111:
  active_gap_code_ = 11; break;
 case 120:
  active_gap_code_ = 20; break;
 case 121:
  active_gap_code_ = 21; break;
 default:
  reset_active_gap_code();
 }

 b.qts << ws_acc_;
 ws_acc_.clear();
 ws_qts_.reset();
}

void NGML_Output_Latex::check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
 CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)
 if(htxn_document_)
 {
  if(NGML_HTXN_Node* nhn = ntc->ngml_htxn_node())
  {
   check_generate_whitespace(b, *nhn);
   return;
  }
 }
 NGML_Output_Event_Handler::check_generate_whitespace(b, ntc);
 //ntc->write_whitespace(b.qts);
}

void NGML_Output_Latex::generate_tag_body_leave(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn)
{
 HTXN_Node_Detail* nd = nhn.get_node_detail(htxn_document_);

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
}

void NGML_Output_Latex::generate_tag_body_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
 CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)

 if(htxn_document_)
 {
  if(NGML_HTXN_Node* nhn = ntc->ngml_htxn_node())
  {
   generate_tag_body_leave(b, *nhn);
   return;
  }
 }

 if(ntc->flags.is_region)
   b.qts << '}';
 else
  b.qts << '{';

 reset_active_gap_code();
}

caon_ptr<NGML_Command_Callback> NGML_Output_Latex::check_command_callback(caon_ptr<NGML_Tag_Command> ntc)
{
 QString name = ntc->name();
 return callbacks_.value(name, caon_ptr<NGML_Command_Callback> ( nullptr ));
}

