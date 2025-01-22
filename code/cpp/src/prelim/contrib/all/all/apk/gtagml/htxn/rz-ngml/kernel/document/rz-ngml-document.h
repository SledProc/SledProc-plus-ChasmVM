
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef RZ_NGML_DOCUMENT__H
#define RZ_NGML_DOCUMENT__H

#include "relae-graph/relae-node-ptr.h"

#include "kernel/rz-ngml-dominion.h"

#include "kernel/query/rz-ngml-query.h"

#include "kernel/grammar/rz-ngml-parsing-mode.h"

#include "rz-ngml-document-info.h"

#include "accessors.h"

#include "rzns.h"

RZNS_(NGML)

class NGML_Parser;
class NGML_Grammar;
class NGML_Graph_Build;
class NGML_Word_Entry_List;

class NGML_Annotation_Tile;

class NGML_Document
{
 caon_ptr<NGML_Graph> graph_;
 caon_ptr<NGML_Parser> parser_;
 caon_ptr<NGML_Graph_Build> graph_build_;
 caon_ptr<NGML_Grammar> grammar_;

 QString local_path_;
 QString raw_text_;

 QString khi_info_;

 QMap<QString, caon_ptr<NGML_Annotation_Tile> >* annotations_;

 typedef QMap<QString, caon_ptr<NGML_Annotation_Tile> >* annotations_type;

 QMap<QString, caon_ptr<NGML_Annotation_Tile> > local_annotations_;



 NGML_Parsing_Modes parsing_mode_;

 NGML_Document_Info document_info_;

public:


 ACCESSORS(QString ,local_path)
 ACCESSORS(QString ,raw_text)
 ACCESSORS(caon_ptr<NGML_Graph> ,graph)
 ACCESSORS__GET(caon_ptr<NGML_Grammar> ,grammar)

 ACCESSORS(annotations_type ,annotations)

 ACCESSORS(QString ,khi_info)
 ACCESSORS(NGML_Parsing_Modes ,parsing_mode)

 NGML_Document();

 ~NGML_Document();

 void load_and_parse(QString path, caon_ptr<NGML_Grammar> grammar =
   caon_ptr<NGML_Grammar>(nullptr) );
 void load_file(QString path);
 void set_grammar(caon_ptr<NGML_Grammar> grammar =
   caon_ptr<NGML_Grammar>(nullptr) );
 void parse();

 void use_light_xml();

 QString file_name();

 QString save_word_count();
 QString save_word_stream();
 QString save_hrefs();
 QString save_light_xml();

 QString save_quotes();

 static void save_file(QString path, QString contents);

 static void clean_html(QString& str);
 static QString escape_unicode(QString contents);

 void tag_command_annotation(caon_ptr<NGML_Tile> nt, caon_ptr<NGML_Annotation_Tile> tile);
 void tag_command_annotation(caon_ptr<NGML_Annotation_Tile> tile);

 void write_local_annotations(QString path);
 static void write_annotations(QString path, QMap<QString, caon_ptr<NGML_Annotation_Tile>>& annotations);

};

_RZNS(NGML)


#endif
