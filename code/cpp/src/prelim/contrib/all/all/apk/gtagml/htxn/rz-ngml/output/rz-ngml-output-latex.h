
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_NGML_OUTPUT_LATEX__H
#define RZ_NGML_OUTPUT_LATEX__H

#include <QTextStream>


#include "rz-ngml-output-base.h"
#include "rz-ngml-output-event-handler.h"

#include "global-types.h"

#include "kans.h"

KANS_CLASS_DECLARE(HTXN, HTXN_Node_Detail)
USING_KANS(HTXN)

#include "rzns.h"

RZNS_(NGML)

class NGML_Graph;
class NGML_Document;

class NGML_HTXN_Node;

class NGML_Attribute_Tile;
class NGML_Tile;

class NGML_Output_Infoset;


class NGML_Output_Latex : public NGML_Output_Base, private NGML_Output_Event_Handler
{
 QMap<QString, caon_ptr<NGML_Command_Callback>> callbacks_;
 void init_callbacks();

 QString htxn_acc_;
 QTextStream htxn_qts_;

 QString ws_acc_;
 QTextStream ws_qts_;

 QMap<HTXN_Node_Detail*, QString> region_end_names_;

 caon_ptr<tNode> suppress_node_;

 u1 active_gap_code_;

 NGML_Output_Infoset* infoset_;

public:


 NGML_Output_Latex(NGML_Document& document);

 ACCESSORS(NGML_Output_Infoset* ,infoset)

 void export_latex(QString path = "..tex");
 void write_latex_output(QString& latex_output);

 void generate(QTextStream& qts);

 void reset_active_gap_code()
 {
  active_gap_code_ = 0;
 }

 u1 get_ws_gap_code();
 u1 get_ws_gap_code(const QString& str);

 void check_reconcile_gap(const NGML_Output_Bundle& b, u1 new_code);
 void check_reconcile_gap(const NGML_Output_Bundle& b, 
   u1 new_code, u1& result);

 void check_generate_tag_command_argument(const NGML_Output_Bundle& b,
   NGML_Tag_Command& ntc);

 void generate_tag_command_argument(const NGML_Output_Bundle& b,
   NGML_HTXN_Node& nhn);

 void check_generate_whitespace(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn);

 void check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;
 void check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Tile> tile) Q_DECL_OVERRIDE;
 void check_generate_whitespace(const NGML_Output_Bundle& b, caon_ptr<NGML_Paralex_Tile> tile) Q_DECL_OVERRIDE;

 void generate_tag_command_entry(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;

 void generate_tag_command_entry(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn);

 void generate_tag_command_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;
 void generate_tag_command_leave(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn);

 void generate_tag_body_leave(const NGML_Output_Bundle& b, caon_ptr<NGML_Tag_Command> ntc) Q_DECL_OVERRIDE;
 void generate_tag_body_leave(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn);

 void generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Paralex_Tile> tile) Q_DECL_OVERRIDE;
 void generate_tile(const NGML_Output_Bundle& b, caon_ptr<NGML_Attribute_Tile> tile) Q_DECL_OVERRIDE;
 void generate_tile_via_htxn(const NGML_Output_Bundle& b, NGML_HTXN_Node& nhn) Q_DECL_OVERRIDE;

 void handle_annotation(caon_ptr<NGML_Tile> tile, caon_ptr<NGML_Annotation_Tile> atile) Q_DECL_OVERRIDE;

 caon_ptr<NGML_Command_Callback> check_command_callback(caon_ptr<NGML_Tag_Command> ntc)  Q_DECL_OVERRIDE;

 
};

_RZNS(NGML)


#endif
