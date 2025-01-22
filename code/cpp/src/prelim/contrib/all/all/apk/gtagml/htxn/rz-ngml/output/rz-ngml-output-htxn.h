
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_NGML_OUTPUT_HTXN__H
#define RZ_NGML_OUTPUT_HTXN__H

#include <QStack>

#include "rz-ngml-output-base.h"
#include "rz-ngml-output-event-handler.h"

#include "htxn/graph/htxn-node-detail.h"
#include "htxn/htxn-document-8b.h"

#include "global-types.h"

#include "rzns.h"

KANS_CLASS_DECLARE(HTXN ,Glyph_Layer_8b)

USING_KANS(HTXN)

RZNS_(NGML)

class NGML_Graph;
class NGML_Document;
class NGML_Command_Callback;
class NGML_HTXN_Node;

class NGML_Output_HTXN : public NGML_Output_Base, private NGML_Output_Event_Handler
{
 QStack<caon_ptr<NGML_Node>> chiefs_;


 QMap<QString, caon_ptr<NGML_Command_Callback>> callbacks_;

 QString tag_command_layer_;
 QString tag_command_arg_layer_;

 QTextStream tag_command_arg_qts_;
 u4 tag_command_arg_index_;

 QString whitespace_scratch_;
 QTextStream whitespace_qts_;

 Glyph_Layer_8b* tag_command_gl_;
 Glyph_Layer_8b* tag_command_arg_gl_;
 Glyph_Layer_8b* main_gl_;

 QMap<QString, QPair<u4, u4>> tag_command_spans_;

 QMap<QString, QPair<Glyph_Layer_8b*, QPair<u4, u4>>> xml_save_spans_; 
 QStack<QString> xml_save_stack_; 

 QMap<u4, QPair<u4, u4>> range_starts_;

 QStack<QPair<u4, QList<caon_ptr<NGML_Tag_Command>>>> multi_parent_range_stack_;

 caon_ptr<NGML_Tag_Command> current_multi_arg_;

 u4 write_position_;

// HTXN_Document_8b* htxn_document_;

 QString htxne_path_;

 QString htxne_top_;

 u4 split_arg_layer_arguments(QString arg, QStringList& args);

 void init_callbacks();

 void check_post_callback(QTextStream& qts,
  caon_ptr<NGML_Command_Callback> cb, caon_ptr<tNode> node, u4 index);

 caon_ptr<NGML_Command_Callback> check_command_callback(caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;
 HTXN_Node_Detail::Wrap_Mode_Indicator_Codes get_wmic(NGML_Tag_Command& ntc);

 void tie_multi_optional_arg_layer(const NGML_Output_Bundle& b, NGML_Tag_Command& ntc);
 void tie_multi_mandatory_arg_layer(const NGML_Output_Bundle& b, NGML_Tag_Command& ntc);

 void tie_multi_optional_main_layer(const NGML_Output_Bundle& b, NGML_Tag_Command& ntc);
 void tie_multi_mandatory_main_layer(const NGML_Output_Bundle& b, NGML_Tag_Command& ntc);

 void check_whitespace_merge(NGML_Tag_Command& ntc);

 void enter_xml_save(caon_ptr<NGML_Node> node, u4 index);
 void leave_xml_save(u4 index);

public:

 NGML_Output_HTXN(NGML_Document& document, HTXN_Document_8b* htxn_document);

 ACCESSORS(Glyph_Layer_8b* ,main_gl)

 void export_htxne(QString path = "..htxne");
 void write_htxne_output(QString& html_output);

 void write_latex_out(QString path);

 void write_saved_xml(QString arg, QString& text);

 void generate(QTextStream& qts);

 void generate_root(const NGML_Output_Bundle& b, caon_ptr<NGML_Root> nr) Q_DECL_OVERRIDE;

 void check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;


// void generate_space_following_token(const NGML_Output_Bundle& b) Q_DECL_OVERRIDE;
// void generate_token(const NGML_Output_Bundle& b, caon_ptr<NGML_Token> token) Q_DECL_OVERRIDE;
// void generate_token_connection_descriptor(const NGML_Output_Bundle& b) Q_DECL_OVERRIDE;

 void generate_tag_command_entry(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;

 void generate_tag_command_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;

 void generate_tag_body_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;

 void generate_tag_command_auto_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;

 void generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Paralex_Tile> tile) Q_DECL_OVERRIDE;
 void generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Tile> tile) Q_DECL_OVERRIDE;

 void generic_generate_tile(QString text, u4 width, QTextStream& qts, u4& index, NGML_HTXN_Node& nhn);
 void generic_generate_tile(const NGML_Output_Bundle& b,
   QString text, u4 width, NGML_HTXN_Node& nhn);
};

_RZNS(NGML)


#endif // RZ_NGML_OUTPUT_HTXN__H


