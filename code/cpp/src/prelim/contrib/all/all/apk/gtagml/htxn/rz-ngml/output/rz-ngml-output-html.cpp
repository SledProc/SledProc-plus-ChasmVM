
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-ngml-output-html.h"

#include "rz-ngml-output-event-generator.h"
#include "kernel/document/rz-ngml-document.h"
#include "kernel/output/rz-ngml-output-bundle.h"
#include "tile/rz-ngml-token.h"
#include "kernel/graph/rz-ngml-node.h"
#include "tag-command/rz-ngml-tag-command.h"
#include "kernel/rz-ngml-root.h"
#include "tag-command/rz-ngml-command-callback.h"

#include <QFile>
#include <QFileInfo>

#include "rzns.h"

USING_RZNS(NGML)


NGML_Output_Html::NGML_Output_Html(NGML_Document& document)
 : NGML_Output_Base(document), NGML_Output_Event_Handler()
{
 init_callbacks();
}

#define inframe NGML_Frame::instance(),


void NGML_Output_Html::init_callbacks()
{
 #include "rz-ngml-output-callbacks-common.h"

 #include "rz-ngml-output-html.callbacks.h"
}

void NGML_Output_Html::insert_khif_link(QTextStream& qts)
{
 QString file_name = document_.file_name();
 QString ngml_link = QString("/sr__ngml/%1.ngml").arg(file_name);
 QString khif_link = QString("/kf__n2k/%1.khif").arg(file_name);

 //div +[@] class kf-link +[e] (span +[e] (span +[m] "
 //  "View as:) +[x] (a +[@] href %1 +[m] ngml) +[x] "
 //                "(a +[@] href %2 +[m] khif)) +[x] (hr +[s]))"
 // ).arg(ngml_link).arg(khif_link);
 qts << QString(
                               "<div class='kf-link'><span><span>View as:</span><a href='%1'>ngml</a>"
                               "<a href='%2'>khif</a></span><hr/></div>"

                ).arg(ngml_link).arg(khif_link);
}

void NGML_Output_Html::write_html_output(QString& html_output)
{
 QTextStream qts(&html_output);
 generate(qts);
}

void NGML_Output_Html::generate(QTextStream& qts)
{
 NGML_Output_Event_Generator events(*this, *this);
 events.generate(qts);
}

void NGML_Output_Html::generate_root(const NGML_Output_Bundle& b, caon_ptr<NGML_Root> nr)
{
 nr->write_whitespace(b.qts);
}


void NGML_Output_Html::export_html(QString path)
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
 QString html_output;
 write_html_output(html_output);

 QFile outfile(path);
 if(outfile.open(QFile::WriteOnly | QIODevice::Text))
 {
  outfile.write(html_output.toLatin1());
  outfile.close();
 }
}

caon_ptr<NGML_Command_Callback> NGML_Output_Html::check_command_callback(caon_ptr<NGML_Tag_Command> ntc)
{
 QString name = ntc->name();
 return callbacks_.value(name, caon_ptr<NGML_Command_Callback> ( nullptr ));
}


void NGML_Output_Html::check_post_callback
 (QTextStream& qts, caon_ptr<NGML_Command_Callback> cb, caon_ptr<tNode> node, u4 index)
{
 if(cb->flags.has_post_callback)
 {
  cb->post_callback(qts, node, index, cb);
 }
}


void NGML_Output_Html::generate_tag_command_entry(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
 CAON_PTR_B_DEBUG(NGML_Node ,node)
 chiefs_.push(b.node);
 bool node_was_blank = false;
 switch(b.connection_descriptor)
 {
 case NGML_Connection_Descriptor::Tag_Command_Cross_From_Blank:
  node_was_blank = true;
  // fallthrough
 case NGML_Connection_Descriptor::Tag_Command_Entry:
 case NGML_Connection_Descriptor::Tag_Command_Cross:

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

  QString command_print_name;
  if(cb && cb->flags.has_rename)
   command_print_name = cb->rename_tag();
  else
   command_print_name = ntc->name();

  QString style_class_name;
  if(cb && cb->flags.has_rename_style_class)
   style_class_name = cb->rename_style_class();

  if(caon_ptr<tNode> n = qry_.Tag_Command_Annotation(inframe b.node))
  {
   if(caon_ptr<tNode> mt = qry_.Tag_Command_Main_Tile(inframe b.node))
   {
    if(caon_ptr<NGML_Tile> nt = mt->ngml_tile())
     tag_command_annotation(nt, n);
   }
  }


  b.qts << '<' << command_print_name;
  if(!style_class_name.isEmpty())
   b.qts << " class='" << style_class_name << '\'';
  break;
 }
}


void NGML_Output_Html::generate_tag_command_leave(const NGML_Output_Bundle& b,
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

 QString command_print_name;
 if(b.cb && b.cb->flags.has_rename)
  command_print_name = b.cb->rename_tag();
 else
  command_print_name = ntc->name();
 b.qts << "</" << ntc->name() << '>';
}

void NGML_Output_Html::generate_tag_body_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc)
{
 b.qts << '>';
 ntc->write_whitespace(b.qts);
}

