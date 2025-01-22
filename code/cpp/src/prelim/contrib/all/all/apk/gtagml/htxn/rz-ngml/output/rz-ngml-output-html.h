
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_NGML_OUTPUT_HTML__H
#define RZ_NGML_OUTPUT_HTML__H

#include <QStack>

#include "rz-ngml-output-base.h"
#include "rz-ngml-output-event-handler.h"

#include "global-types.h"

#include "rzns.h"

RZNS_(NGML)

class NGML_Graph;
class NGML_Document;
class NGML_Command_Callback;

class NGML_Output_Html : public NGML_Output_Base, private NGML_Output_Event_Handler
{
 QStack<caon_ptr<NGML_Node>> chiefs_;


 QMap<QString, caon_ptr<NGML_Command_Callback>> callbacks_;
 void init_callbacks();

 void check_post_callback(QTextStream& qts,
  caon_ptr<NGML_Command_Callback> cb, 
  caon_ptr<tNode> node, u4 index);

 caon_ptr<NGML_Command_Callback> check_command_callback(caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;


public:

 NGML_Output_Html(NGML_Document& document);

 void export_html(QString path = "..htm");
 void write_html_output(QString& html_output);

 void generate(QTextStream& qts);

 void generate_root(const NGML_Output_Bundle& b, caon_ptr<NGML_Root> nr) Q_DECL_OVERRIDE;

 void generate_tag_command_entry(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;

 void generate_tag_command_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;

 void generate_tag_body_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;

 void insert_khif_link(QTextStream& qts);
};

_RZNS(NGML)


#endif
