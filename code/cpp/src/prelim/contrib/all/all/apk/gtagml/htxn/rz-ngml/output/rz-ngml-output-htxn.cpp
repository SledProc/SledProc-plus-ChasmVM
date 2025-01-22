
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-ngml-output-htxn.h"

#include "rz-ngml-output-event-generator.h"
#include "kernel/document/rz-ngml-document.h"
#include "kernel/output/rz-ngml-output-bundle.h"
#include "tile/rz-ngml-token.h"
#include "tile/rz-ngml-tile.h"
#include "kernel/graph/rz-ngml-node.h"
#include "tag-command/rz-ngml-tag-command.h"
#include "kernel/rz-ngml-root.h"
#include "tag-command/rz-ngml-command-callback.h"

#include "tile/rz-ngml-paralex-tile.h"

#include "htxn/glyph-layer-8b.h"

#include "ngml-htxn/ngml-htxn-node.h"


#include <QFile>
#include <QFileInfo>
#include <QDebug>

#include "rzns.h"

USING_RZNS(NGML)


NGML_Output_HTXN::NGML_Output_HTXN(NGML_Document& document, 
HTXN_Document_8b* htxn_document)
 : NGML_Output_Base(document), NGML_Output_Event_Handler(),
   current_multi_arg_(nullptr), tag_command_arg_index_(2)
{
 this->NGML_Output_Base::htxn_document_ = htxn_document;

 init_callbacks();

 htxn_document_->add_standard_deck();
 htxn_document_->add_standard_diacritic_deck();
// Glyph_Layer_8b* 

 //discard_gl_ = htxn_document_->add_layer();

 main_gl_ = htxn_document_->add_layer();
 tag_command_gl_ = htxn_document_->add_layer();
 tag_command_arg_gl_ = htxn_document_->add_layer();

 Glyph_Layer_8b__SET_DESCRIPTION(main_gl_)
 Glyph_Layer_8b__SET_DESCRIPTION(tag_command_gl_)
 Glyph_Layer_8b__SET_DESCRIPTION(tag_command_arg_gl_)

// main_gl_->set_description("main_gl_");
// tag_command_gl_->set_description("tag_command_gl_");
// tag_command_arg_gl_->set_description("tag_command_arg_gl_");

 tag_command_arg_qts_.setString(&tag_command_arg_layer_);
 whitespace_qts_.setString(&whitespace_scratch_);

}

void NGML_Output_HTXN::init_callbacks()
{
 #include "rz-ngml-output-callbacks-common.h"

 #include "rz-ngml-output-htxn.callbacks.h"
}

void NGML_Output_HTXN::leave_xml_save(u4 index)
{
 QString key = xml_save_stack_.pop();
 QPair<Glyph_Layer_8b*, QPair<u4, u4>>& pr = xml_save_spans_[key];
 if(pr.first == main_gl_)
   pr.second.second = index;
 else if(pr.first == tag_command_arg_gl_)
   pr.second.second = tag_command_arg_index_;
}

void NGML_Output_HTXN::write_saved_xml(QString arg, QString& text)
{
 auto it = xml_save_spans_.find(arg);
 if(it == xml_save_spans_.end())
   return;
 Glyph_Layer_8b* gl = it->first;
 if(!gl)
   return;
 QTextStream qts(&text);
 htxn_document_->write_minimal_xml_out(gl, it->second.first, 
   it->second.second, qts);
}

void NGML_Output_HTXN::enter_xml_save(caon_ptr<NGML_Node> node, u4 index)
{
 CAON_PTR_DEBUG(NGML_Node ,node)
 QStringList args;
 if(caon_ptr<NGML_Tag_Command> ntc = node->ngml_tag_command())
 {
  CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)
  QString arg = ntc->argument();
  args = arg.split(':');  
 }
 if(args.size() >= 2)
 {
  if(args[0] == "main")
    xml_save_spans_[args[1]] = {main_gl_, {index, 0}};
  else if(args[0] == "arg")
    xml_save_spans_[args[1]] = {tag_command_arg_gl_, {tag_command_arg_index_, 0}};
  xml_save_stack_.push(args[1]);
 }
 else if(args.size() == 1)
 {
  xml_save_spans_[args[0]] = {main_gl_, {index, 0}};
  xml_save_stack_.push(args[0]);
 }
}


void NGML_Output_HTXN::write_htxne_output(QString& htxne_output)
{
 QTextStream qts(&htxne_output);
 generate(qts);
// htxne_output.replace("$OP$", "(");
// htxne_output.replace("$CP$", ")");
// htxne_output.replace("$EQ$", "=");
}

void NGML_Output_HTXN::generate(QTextStream& qts)
{
 NGML_Output_Event_Generator events(*this, *this);
 events.generate(qts);
}

void NGML_Output_HTXN::generate_root(const NGML_Output_Bundle& b, caon_ptr<NGML_Root> nr)
{
 htxne_top_ += "## htxne generated...\n";
 QStringList lines = nr->ws().to_string().split('\n');
 QStringListIterator it(lines);
 while(it.hasNext())
 {
  QString l = it.next();
  if( (!it.hasNext()) && l.isEmpty())
    break;
  htxne_top_ += QString("# %1 \n").arg(l);
 }
}

void NGML_Output_HTXN::write_latex_out(QString path)
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
 
 QFile file(path);
 if(file.open(QIODevice::WriteOnly | QIODevice::Text))
 {
  QTextStream qts(&file);
  qts << htxne_top_;
  htxn_document_->write_latex_out(qts); 
 }
}


void NGML_Output_HTXN::export_htxne(QString path)
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
 QString htxne_output;
 write_htxne_output(htxne_output);

 htxn_document_->read_layer(main_gl_, htxne_output);
 htxn_document_->read_layer(tag_command_gl_, tag_command_layer_);
 htxn_document_->read_layer(tag_command_arg_gl_, tag_command_arg_layer_);

 htxn_document_->calculate_orders();

 QFile outfile(path);
 if(outfile.open(QFile::WriteOnly | QIODevice::Text))
 {
  htxne_path_ = path;
  htxn_document_->write_htxne_out(outfile);
//  outfile.write(htxne_output.toLatin1());
  outfile.close();
 }


// QFile outfile(path);
// if(outfile.open(QFile::WriteOnly | QIODevice::Text))
// {
//  outfile.write(htxne_output.toLatin1());
//  outfile.close();
// }

}

caon_ptr<NGML_Command_Callback> NGML_Output_HTXN::check_command_callback(caon_ptr<NGML_Tag_Command> ntc)
{
 QString name = ntc->name();
 return callbacks_.value(name, caon_ptr<NGML_Command_Callback>( nullptr ));
}

void NGML_Output_HTXN::check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
 CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)

   // // this is just for debugging to see the ws ...
 ntc->write_whitespace(whitespace_qts_);

#ifdef HIDE
 u2 space_count = 0;
 u2 line_count = 0;
 char other = 0;
 for(QChar c : whitespace_scratch_)
 {
  if(c == ' ')
    ++space_count;
  else if(c == '\n')
    ++line_count;
  else
  {
   other = c.toLatin1();
   break;
  }
 }

 if(other)
 {
  ntc->write_whitespace(b.qts);
  check_update_index_after_whitespace(b, *ntc);
 }
 else if(line_count == 1)
   ntc->flags.right_line_gap = true;
 else if(line_count > 1)
   ntc->flags.right_line_double_gap = true;
 else if(space_count >= 1)
   ntc->flags.right_space_gap = true;

#endif // HIDE

 check_whitespace_merge(*ntc);

 whitespace_scratch_.clear();
 whitespace_qts_.reset();
}

void NGML_Output_HTXN::check_whitespace_merge(NGML_Tag_Command& ntc)
{
 if(NGML_HTXN_Node* node = ntc.ngml_htxn_node())
 {
  if(HTXN_Node_Detail* nd = node->get_node_detail(htxn_document_))
  {
   nd->flags.pre_line_double_gap = ntc.flags.left_line_double_gap;
   nd->flags.pre_line_gap = ntc.flags.left_line_gap;
   nd->flags.pre_space_gap = ntc.flags.left_space_gap;

   nd->flags.post_space_gap = ntc.flags.right_space_gap;
   nd->flags.post_line_gap = ntc.flags.right_line_gap;
   nd->flags.post_line_double_gap = ntc.flags.right_line_double_gap;
  }
 }
}


void NGML_Output_HTXN::check_post_callback
 (QTextStream& qts, caon_ptr<NGML_Command_Callback> cb, caon_ptr<tNode> node, u4 index)
{
 if(cb->flags.has_post_callback)
 {
  cb->post_callback(qts, node, index, cb);
 }
}

void NGML_Output_HTXN::generate_tag_command_auto_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
 // b.qts << "/>";
}

u4 NGML_Output_HTXN::split_arg_layer_arguments(QString arg,
  QStringList& args)
{
 if(arg.isEmpty())
   return 0;
 QString trim = arg.trimmed();
 if(trim.isEmpty())
 {
  args = QStringList {" "};
  return 1;
 }
 QChar c = trim[0];
 if( (c != '?') && (c != '!') )
 {
  args = QStringList {arg};
  return 1;
 }
 trim.replace(" ? ", " >? ");
 trim.replace(" ! ", " >! ");
 args = trim.split('>');
 return args.size();
}

//void NGML_Output_HTXN::update_current_multi_arg()
//{
//}

HTXN_Node_Detail::Wrap_Mode_Indicator_Codes NGML_Output_HTXN::get_wmic(
  NGML_Tag_Command& ntc)
{
 HTXN_Node_Detail::Wrap_Mode_Indicator_Codes result = HTXN_Node_Detail::Wrap_Mode_Indicator_Codes::Normal;

 if(ntc.flags.is_non_wrapped)
 {
  if(ntc.flags.has_non_wrapped_space)
    result = HTXN_Node_Detail::Wrap_Mode_Indicator_Codes::None_With_Space;
  else
    result = HTXN_Node_Detail::Wrap_Mode_Indicator_Codes::None;
 }
 else if(ntc.flags.is_left_wrapped)
 {
  if(ntc.flags.has_non_wrapped_space)
    result = HTXN_Node_Detail::Wrap_Mode_Indicator_Codes::Left_With_Space;
  else
    result = HTXN_Node_Detail::Wrap_Mode_Indicator_Codes::Left;
 }

 return result;
}


void NGML_Output_HTXN::tie_multi_optional_main_layer(const NGML_Output_Bundle& b,
  NGML_Tag_Command& ntc)
{
// main_gl_->set_range_leave(ntc->ref_position(), ntc->ref_order(), b.index);
 HTXN_Node_Detail::Wrap_Mode_Indicator_Codes wmic = get_wmic(ntc);

 u4 nc1 = multi_parent_range_stack_.top().first;
 u4 enter = ntc.ref_position();
 u4 leave = b.index - 1;
 u4 nc2 = htxn_document_->add_detail_range_optional(main_gl_, enter, leave, wmic);
 htxn_document_->mark_ghosted(nc2);

 NGML_HTXN_Node* nhn = new NGML_HTXN_Node(nc2);
 ntc.set_ngml_htxn_node(nhn);

 htxn_document_->tie_detail_range_preempt(nc1, nc2);
 current_multi_arg_ = nullptr;
}

void NGML_Output_HTXN::tie_multi_mandatory_main_layer(const NGML_Output_Bundle& b,
  NGML_Tag_Command& ntc)
{
 HTXN_Node_Detail::Wrap_Mode_Indicator_Codes wmic = get_wmic(ntc);

 u4 nc1 = multi_parent_range_stack_.top().first;
 u4 enter = ntc.ref_position();
 u4 leave = b.index - 1;
 u4 nc2 = htxn_document_->add_detail_range(main_gl_, enter, leave, wmic);

 htxn_document_->tie_detail_range_preempt(nc1, nc2);
 htxn_document_->mark_ghosted(nc2);

 NGML_HTXN_Node* nhn = new NGML_HTXN_Node(nc2);
 ntc.set_ngml_htxn_node(nhn);

 if(ntc.flags.marked_main)
 {
  HTXN_Node_Detail* nd = nhn->get_node_detail(htxn_document_);
  nd->flags.block_environment_marked_main = true;
 }

 current_multi_arg_ = nullptr;
}

void NGML_Output_HTXN::tie_multi_optional_arg_layer(const NGML_Output_Bundle& b,
  NGML_Tag_Command& ntc)
{
 HTXN_Node_Detail::Wrap_Mode_Indicator_Codes wmic = get_wmic(ntc);
// tag_command_argument_gl_->set_range_leave(ntc->ref_position(), ntc->ref_order(), b.index);

 u4 nc1 = multi_parent_range_stack_.top().first;
 u4 enter = ntc.ref_position();
 u4 leave = tag_command_arg_index_ - 1;
 u4 nc2 = htxn_document_->add_detail_range_optional(tag_command_arg_gl_, enter, leave, wmic);
 htxn_document_->tie_detail_range_preempt(nc1, nc2);
 htxn_document_->mark_ghosted(nc2);

 NGML_HTXN_Node* nhn = new NGML_HTXN_Node(nc2);
 ntc.set_ngml_htxn_node(nhn);

 current_multi_arg_ = nullptr;
}

void NGML_Output_HTXN::tie_multi_mandatory_arg_layer(const NGML_Output_Bundle& b,
  NGML_Tag_Command& ntc)
{
 HTXN_Node_Detail::Wrap_Mode_Indicator_Codes wmic = get_wmic(ntc);

 u4 nc1 = multi_parent_range_stack_.top().first;
 u4 enter = ntc.ref_position();
 u4 leave = tag_command_arg_index_ - 1;
 u4 nc2 = htxn_document_->add_detail_range(tag_command_arg_gl_, enter, leave, wmic);
 htxn_document_->tie_detail_range_preempt(nc1, nc2);
 htxn_document_->mark_ghosted(nc2);

 NGML_HTXN_Node* nhn = new NGML_HTXN_Node(nc2);
 ntc.set_ngml_htxn_node(nhn);

 current_multi_arg_ = nullptr;
}

void NGML_Output_HTXN::generate_tag_command_entry(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
 CAON_PTR_B_DEBUG(NGML_Node ,node)
 chiefs_.push(b.node);

 ntc->normalize_whitespace();

 CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)

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

  if( (ntc->flags.is_multi_optional) || (ntc->flags.is_multi_mandatory) )
  {
   current_multi_arg_ = ntc;
   multi_parent_range_stack_.top().second.push_back(ntc);
   if(ntc->flags.multi_arg_layer)
     ntc->set_ref_position(tag_command_arg_index_);
   else if(ntc->flags.multi_main_layer)
     ntc->set_ref_position(b.index);

#ifdef HIDE
   u4 nc1 = multi_parent_range_stack_.top().first;
   if(ntc->flags.multi_arg_layer)
   {
    u4 order = tag_command_arg_gl_->add_range(tag_command_arg_index_, 0, nc1);
    ntc->set_ref_position(tag_command_arg_index_);
    ntc->set_ref_order(order);
   }
   else if(ntc->flags.multi_main_layer)
   {
    u4 order = main_gl_->add_range(b.index, 0, nc1);
    ntc->set_ref_position(b.index);
    ntc->set_ref_order(order);
   }
#endif HIDE
   return;
  }

  QString command_print_name;
  if(cb && cb->flags.has_rename)
    command_print_name = cb->rename_tag();
  else
    command_print_name = ntc->name();

  QString style_class_name;
  if(cb && cb->flags.has_rename_style_class)
   style_class_name = cb->rename_style_class();

  //u4 pos = (u4) b.qts.pos();

  u4 span_start, span_end;

  auto it = tag_command_spans_.find(command_print_name);
  if(it == tag_command_spans_.end())
  {
   span_start = tag_command_layer_.size() + 2;
   span_end = span_start + command_print_name.size() - 1;

   tag_command_spans_.insert(command_print_name,
     {span_start, span_end});
   tag_command_layer_ += command_print_name;
  }
  else
  {
   span_start = (*it).first;
   span_end = (*it).second;
  }

  u4 nc1;

  u2 wsc = ntc->get_whitespace_code();

  HTXN_Node_Detail::Wrap_Mode_Indicator_Codes wmic = get_wmic(*ntc);

  if(ntc->flags.is_region)
    nc1 = htxn_document_->add_detail_range_region(tag_command_gl_, span_start, span_end, wmic, wsc);
  else
    nc1 = htxn_document_->add_detail_range(tag_command_gl_, span_start, span_end, wmic, wsc);

  if(ntc->flags.is_fiat)
    htxn_document_->mark_fiat(nc1);

  if(ntc->flags.is_multi_parent)
    multi_parent_range_stack_.push({nc1, {}});

  NGML_HTXN_Node* nhn = new NGML_HTXN_Node(nc1);
  ntc->set_ngml_htxn_node(nhn);

  if(ntc->flags.is_region && !(ntc->flags.is_multi_parent))
  {
   if(HTXN_Node_Detail* nd = nhn->get_node_detail(htxn_document_))
     nd->flags.main_only_block_environment = true;
  }

  QStringList args;
  u4 sz = split_arg_layer_arguments(ntc->argument(), args);
  if(sz > 0)
  {
   if(sz == 1)
   {
    u4 enter = tag_command_arg_index_;// tag_command_arg_layer_.size() + 2;
    tag_command_arg_qts_ << args[0]; //tag_command_arg_layer_ += args[0];
    tag_command_arg_index_ += args[0].size(); //qts_
    u4 leave = tag_command_arg_index_ - 1;

    u4 nc2 = htxn_document_->add_detail_range(tag_command_arg_gl_, enter, leave,
      HTXN_Node_Detail::Wrap_Mode_Indicator_Codes::Normal);

    NGML_HTXN_Node* nhn = new NGML_HTXN_Node(nc2);
    ntc->set_arg_ngml_htxn_node(nhn);

    htxn_document_->tie_detail_range_preempt(nc1, nc2);
   }
   else for(QString arg : args)
   {
    QChar c = arg[0];
    QString a = arg.mid(1).trimmed();
    u4 enter = tag_command_arg_index_; //tag_command_arg_layer_.size() + 2;
    tag_command_arg_qts_ << a;
    tag_command_arg_index_ += a.size();
    u4 leave = tag_command_arg_index_ - 1;
    u4 nc2;
    if(c == '?')
      nc2 = htxn_document_->add_detail_range_optional(tag_command_arg_gl_, enter, leave, 
      HTXN_Node_Detail::Wrap_Mode_Indicator_Codes::Normal);
    else if(c == '!')
      nc2 = htxn_document_->add_detail_range(tag_command_arg_gl_, enter, leave, 
      HTXN_Node_Detail::Wrap_Mode_Indicator_Codes::Normal);

    NGML_HTXN_Node* nhn = new NGML_HTXN_Node(nc2);
    ntc->add_arg_ngml_htxn_node(nhn);

    htxn_document_->tie_detail_range_preempt(nc1, nc2);
   }
   //tag_command_gl_->
  }

  if(ntc->flags.multi_arg_layer || ntc->flags.multi_arg_layer_inherited)
  {
   u4 order = tag_command_arg_gl_->add_range(tag_command_arg_index_, 0, nc1);
   ntc->set_ref_position(tag_command_arg_index_);
   ntc->set_ref_order(order);
  }
  else
  {
   u4 order = main_gl_->add_range(b.index, 0, nc1);
   ntc->flags.is_main_layer = true;
   ntc->set_ref_position(b.index);
   ntc->set_ref_order(order);
  }


//  b.qts << '\n' << '[' << command_print_name;

//?  if(!style_class_name.isEmpty())
//?   b.qts << " class='" << style_class_name << '\'';

  break;
 }
}

void NGML_Output_HTXN::generate_tag_command_leave(const NGML_Output_Bundle& b,
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

 CAON_PTR_DEBUG(NGML_Tag_Command ,ntc)

 QString command_print_name;
 if(b.cb && b.cb->flags.has_rename)
   command_print_name = b.cb->rename_tag();
 else
   command_print_name = ntc->name();

 if(ntc->flags.is_multi_parent)
 {
  u4 nc1 = multi_parent_range_stack_.pop().first;

  if(ntc->flags.is_region)
  {
   // //  we have to mark the last ref as a main tile ...
   if(ntc->flags.is_main_layer)
     htxn_document_->mark_last_as_environment_main_tile(nc1, main_gl_,
     ntc->ref_position(), ntc->ref_order(), b.index - 1);
   else
     htxn_document_->mark_last_as_environment_main_tile(nc1, nullptr, 0, 0, 0);
  }
 }

 if(ntc->flags.is_multi_optional)
 {
  if(ntc->flags.multi_arg_layer || ntc->flags.multi_arg_layer_inherited)
    tie_multi_optional_arg_layer(b, *ntc);
  else if(ntc->flags.multi_main_layer || ntc->flags.multi_main_layer_inherited)
    tie_multi_optional_main_layer(b, *ntc);
 }
 else if(ntc->flags.is_multi_mandatory)
 {
  if(ntc->flags.multi_arg_layer || ntc->flags.multi_arg_layer_inherited)
    tie_multi_mandatory_arg_layer(b, *ntc);
  else if(ntc->flags.multi_main_layer || ntc->flags.multi_main_layer_inherited)
    tie_multi_mandatory_main_layer(b, *ntc);
 }
 else if(ntc->flags.is_multi_parent)
   ; // nothing ...
 else if(ntc->flags.is_self_closed)
 {
  // // maybe need to add a one-character filler ...
  if(ntc->flags.multi_arg_layer || ntc->flags.multi_arg_layer_inherited)
  {
//?   tag_command_arg_layer_ += "`{/}";
//?   tag_command_arg_gl_->set_range_leave(ntc->ref_position(), ntc->ref_order(), tag_command_arg_index_);
//?   ++tag_command_arg_index_;
  }
 }
 else
 {
  if(ntc->flags.multi_arg_layer || ntc->flags.multi_arg_layer_inherited)
  {
   tag_command_arg_gl_->set_range_leave(ntc->ref_position(), ntc->ref_order(), tag_command_arg_index_ - 1);
   if(NGML_HTXN_Node* nhn = ntc->ngml_htxn_node())
     nhn->set_ref_positions(ntc->ref_position(), ntc->ref_order() + 1, b.index - 1);
  }
  else
  {
   main_gl_->set_range_leave(ntc->ref_position(), ntc->ref_order(), b.index - 1);
   if(NGML_HTXN_Node* nhn = ntc->ngml_htxn_node())
     nhn->set_ref_positions(ntc->ref_position(), ntc->ref_order() + 1, b.index - 1);
  }
 }
}

void NGML_Output_HTXN::generic_generate_tile(QString text, u4 width,
  QTextStream& qts, u4& index, NGML_HTXN_Node& nhn)
{
 qts << text;
 nhn.set_range_enter(index);
 index += width;
 nhn.set_range_leave(index - 1);
}

void NGML_Output_HTXN::generic_generate_tile(const NGML_Output_Bundle& b,
  QString text, u4 width, NGML_HTXN_Node& nhn)
{
 if(current_multi_arg_)
 {
  if(current_multi_arg_->flags.multi_arg_layer)
  {
   generic_generate_tile(text, width, tag_command_arg_qts_, tag_command_arg_index_, nhn);
   nhn.set_layer_code(tag_command_arg_gl_->id());
   return;
  }
 }
 generic_generate_tile(text, width, b.qts, b.index, nhn);
 nhn.set_layer_code(main_gl_->id());
}


void NGML_Output_HTXN::generate_tile(const NGML_Output_Bundle& b,
  caon_ptr<NGML_Tile> tile)
{
 CAON_PTR_DEBUG(NGML_Tile ,tile)
//
 QString rt = tile->raw_text();
 NGML_HTXN_Node* nhn = new NGML_HTXN_Node();

 u4 len = tile->effective_length();

// b.qts << tile->raw_text();
 generic_generate_tile(b, rt, len, *nhn);
 tile->set_ngml_htxn_node(nhn);
 nhn->set_ws(tile->ws());

 //check_update_index(b, *tile);

 //check_generate_whitespace(b, tile);
}

void NGML_Output_HTXN::generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Paralex_Tile> tile)
{   //    || (current_multi_arg_->flags.is_multi_mandatory) )
 NGML_HTXN_Node* nhn = new NGML_HTXN_Node();
 generic_generate_tile(b, tile->to_string(), tile->get_width(), *nhn);
 tile->set_ngml_htxn_node(nhn);
}

void NGML_Output_HTXN::generate_tag_body_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
//? b.qts << '>';
//? ntc->write_whitespace(b.qts);
}


