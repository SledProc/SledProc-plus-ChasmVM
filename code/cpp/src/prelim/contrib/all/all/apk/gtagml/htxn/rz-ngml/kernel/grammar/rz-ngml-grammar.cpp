
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-ngml-grammar.h"

#include "kernel/grammar/rz-ngml-parse-context.h"

#include "kernel/grammar/rz-ngml-graph-build.h"

#include "kernel/grammar/rz-ngml-parser.h"

#include "relae-graph/relae-parser.templates.h"

USING_RZNS(NGML)

NGML_Grammar::NGML_Grammar()
{
}

void NGML_Grammar::init(NGML_Parser& p, NGML_Graph& g, NGML_Graph_Build& graph_build)
{
 pre_rule( "script-word", "(?:[^{}()\\[\\]\\s`;,:]|(?:\\w::?\\w))+" );
 pre_rule( "valid-tag-command-name", "\\w(?:[^{}()\\[\\]<>/\\s`;,.:]|(?:\\w+::?\\w))*" );
 pre_rule( "space-to-end-of-line", "[__\\t]* \\n" );
 pre_rule( "end-of-line", "[__\\t\\S]* \\n" );
 pre_rule( "single-space", "[__\\t]" );

 pre_rule( "tag-command-wrap-mode-indicator", ": :? \\.?" );

 Context ngml_context = add_context("ngml-context");
 Context html_context = add_context("html-context");

 Context ngml_or_html_context = add_context("ngml-or-html",
  {ngml_context, html_context});

 Context cxml_context = add_context("cxml-context");

 track_context({&ngml_context, &html_context,
   &ngml_or_html_context, &cxml_context });

 switch(graph_build.current_parsing_mode())
 {
 case NGML_Parsing_Modes::HTML:
  activate(ngml_or_html_context);
  break;

 case NGML_Parsing_Modes::CXML:
  activate(cxml_context);
  break;

 case NGML_Parsing_Modes::NGML:
  activate(ngml_context);
  break;
 }


 NGML_Parse_Context& parse_context = graph_build.parse_context();

 size_t test;

#ifdef HIDE

 add_rule( khif_context, "tag-command-with-predicate-vector",
  " \\(  (?<tag-command> .valid-tag-command-name. ) "
  "  \\s+  (?<connector-prefix> [-+] \\[ ) (?<connectors> [^\\]] ) \\] "
   ,[&]
 {
  QString tag_command = p.matched("tag-command");
  QString connector_prefix = p.matched("connector-prefix");
  QString connectors = p.matched("connectors");
  graph_build.enter_tag_command_with_predicate_vector(tag_command, connector_prefix, connectors);
 });

 add_rule( khif_context, "tag-command-with-adjunct-predicate-vector",
  " \\(  (?<tag-command> .valid-tag-command-name. ) "
  "  \\s+  (?<connector-prefix> [-+] \\{ ) (?<vec> [^\\]]) \\} "
   ,[&]
 {
  QString tag_command = p.matched("tag-command");
  QString connector_prefix = p.matched("connector-prefix");
  QString connectors = p.matched("connector-prefix");
  graph_build.enter_tag_command_with_predicate_vector(tag_command, connector_prefix, connectors);
 });

 add_rule( flags_all_(parse_context ,inside_khif_tile), "attach-predicate-vector",
  " (?<connector-prefix> [-+] \\[ ) (?<vec> [^\\]]+) \\] "
   ,[&]
 {
  QString connector_prefix = p.matched("connector-prefix");
  QString connectors = p.matched("vec");
  graph_build.attach_predicate_vector(connector_prefix, connectors);
 });


  add_rule( flags_all_(parse_context ,inside_khif_tile), //? khif_context,
  "khif-tile-acc",
  " [^()-+]+ "
   ,[&]
 {
  QString m = p.match_text();
  graph_build.khif_tile_acc(m);
 });

  add_rule( flags_all_(parse_context ,inside_khif_tile), //? khif_context,
  "khif-tile-acc-special-chars",
  " [+-]+ "
   ,[&]
 {
  QString m = p.match_text();
  graph_build.khif_tile_acc(m);
 });

 add_rule( flags_all_(parse_context ,inside_khif_tile), //? khif_context,
  "khif-tile-acc-end-with-predicate-vector",
  " \\) \\s+ (?<connector-prefix> [+-] \\[ ) (?<connectors> [^\\]]+ ) \\] "
   ,[&]
  {
   QString connector_prefix = p.matched("connector-prefix");
   QString m = p.matched("connectors");
   graph_build.end_khif_tile(connector_prefix, m);
  });

 add_rule( flags_all_(parse_context ,inside_khif_tile), //? khif_context,
 "khif-tile-acc-end",
 " \\) "
  ,[&]
{
 graph_build.end_khif_tile();
});


 add_rule( html_context, "html-tag-instruction",
  " < (?<prefix> [!?] ) (?<tag-command> .valid-tag-command-name. ) "
  "  (?<argument> [^>]+ ) > "
   ,[&]
 {

  QString prefix = p.matched("prefix");
  QString tag_command = p.matched("tag-command");
  QString argument = p.matched("argument");
  graph_build.html_tag_instruction(prefix, tag_command, argument);

 });

 add_rule( html_context, "html-script",
  " <script "
   ,[&]
 {
  parse_context.flags.inside_html_script_tag = true;
 });

 add_rule( flags_all_(parse_context ,inside_html_script_tag), "html-end-script",
  " </script> "
   ,[&]
 {
  parse_context.flags.inside_html_script_tag = false;
 });

 add_rule( flags_all_(parse_context ,inside_html_script_tag), "html-inside-script",
  " . "
   ,[&]
 {
 });

 add_rule( html_context, "html-style",
  " <style "
   ,[&]
 {
  parse_context.flags.inside_html_style_tag = true;
 });

 add_rule( flags_all_(parse_context ,inside_html_style_tag), "html-end-style",
  " </style> "
   ,[&]
 {
  parse_context.flags.inside_html_style_tag = false;
 });

 add_rule( flags_all_(parse_context ,inside_html_style_tag), "html-inside-style",
  " . "
   ,[&]
 {
 });

  add_rule( html_context, "html-tag-command-entry",
   " < (?<prefix> [!?] )? (?<tag-command> .valid-tag-command-name. ) "
    ,[&]
  {

   QString prefix = p.matched("prefix");
   QString tag_command = p.matched("tag-command");
   graph_build.html_tag_command_entry(prefix, tag_command);

   if(parse_context.flags_all(parse_context.flags.inside_html_tag_body_))
   {
    void* pv = &parse_context.flags;
    size_t st = (size_t) pv;
    test = st;
   }

  });

   add_rule( flags_all_(parse_context ,inside_html_tag_body), "html-tag-command-attribute-entry",
    " (?<pre-space> \\s* ) (?<attribute> \\S+ ) \\s* = \\s* (?<s-or-d> ['\"] ) "
     ,[&]
   {
    QString pre_space = p.matched("pre-space");
    QString attribute = p.matched("attribute");
    QString s_or_d = p.matched("s-or-d");
    graph_build.html_tag_command_attribute_entry(pre_space, attribute, s_or_d);
   });
#endif //def HIDE


 add_rule( flags_none_(parse_context ,inside_multiline_comment),
           ngml_context, "ngml-multiline-comment",
           " (?: (?<semis> ;; ;* ) (?<tildes> ~~ ~* ) )"
           ,[&]
 {
  QString semis = p.matched("semis");
  QString tildes = p.matched("tildes");
  graph_build.enter_multiline_comment(semis, tildes);
 });

 add_rule( flags_all_(parse_context ,inside_multiline_comment),
           ngml_context, "ngml-multiline-comment-leave",
          " (?: (?<tildes> ~~ ~~* ) (?<semis> ;; ;* ) )"
          ,[&]
 {
  QString semis = p.matched("semis");
  QString tildes = p.matched("tildes");
  graph_build.check_leave_multiline_comment(semis, tildes);
 });

 add_rule( flags_all_(parse_context ,inside_multiline_comment),
           ngml_context, "ngml-multiline-comment-characters",
          " [^~]+ | ~ "
          ,[&]
 {
 });

 add_rule( ngml_context, "ngml-comment",
           " (?: (?: ;; ) ;* ~  ) .end-of-line. "
           ,[&]
 {
 });

 add_rule( ngml_context, "annotation-entry",
  " ` (?: (?<flag> \\*) )?  \\[ (?<text>[^]]+) \\] (?: < (?<rel> [^>]+ ) > )? "
  " (?: \\{ (?<tile> [^}]+ ) \\}  )? (?<follow> [,;.] | :+ )"
   ,[&]
  {
   QString flag = p.matched("flag");
   QString text = p.matched("text");
   QString rel = p.matched("rel");
   if(!rel.isEmpty())
   {
    rel.replace(0, 1, rel[0].toUpper());
   }
   QString tile = p.matched("tile");
   QString follow = p.matched("follow");
   graph_build.annotation_entry(flag, text, rel, tile, follow);
  });


#ifdef HIDE


 //?
 add_rule( ngml_context, "tag-command-entry",
  "  `< (?<prefix> / )? "
  " (?<wmi> .tag-command-wrap-mode-indicator.? ) "
  " (?<tag-command> .valid-tag-command-name. ) "
  " (?: \\[ (?<parent-tag-type> \\w+ ) \\] )? "
           ,[&]
 {
  QString wmi = p.matched("wmi");
  QString prefix = p.matched("prefix");
  QString tag_command = p.matched("tag-command");
  QString parent_tag_type = p.matched("parent_tag_type");
  graph_build.tag_command_entry(wmi, prefix, tag_command, parent_tag_type);
 });



 add_rule( flags_all_(parse_context ,inside_html_tag_body), "html-tag-body-close",
  " (?<pre-space> \\s* ) (?<prefix> /? ) > "
   ,[&]
 {
  QString pre_space = p.matched("pre-space");
  QString prefix = p.matched("prefix");
  QString m = p.match_text();
  graph_build.html_tag_body_leave(prefix);
 });

 add_rule( flags_all_(parse_context ,inside_html_tag_attribute_single_quote),
  "html-tag-command-attribute-single-acc",
  " (?: \\') | [^'] "
   ,[&]
 {
  QString pm = p.match_text();
  if(pm.startsWith('\\'))
  {
   if(pm.length() > 1)
    pm.remove(0, 1);
  }
  graph_build.html_tag_command_attribute_acc(pm);
 });


 add_rule( flags_all_(parse_context ,inside_html_tag_attribute_double_quote),
  "complete-html-tag-command-attribute-double",
  " \" "
   ,[&]
 {
  graph_build.complete_html_tag_command_attribute();
 });


 add_rule( flags_all_(parse_context ,inside_html_tag_attribute_double_quote),
  "html-tag-command-attribute-double-acc",
  " (?: \\\") | (?: [^\"]+ ) "
   ,[&]
 {
  QString pm = p.match_text();
  if(pm.startsWith('\\'))
  {
   if(pm.length() > 1)
    pm.remove(0, 1);
  }
  graph_build.html_tag_command_attribute_acc(pm);
 });

 add_rule( flags_all_(parse_context ,inside_html_tag_attribute_single_quote),
  "complete-html-tag-command-attribute-single",
  " ' "
   ,[&]
 {
  graph_build.complete_html_tag_command_attribute();
 });
#endif

 add_rule( flags_all_(parse_context ,inside_attribute_sequence),
   ngml_context, "attribute-sequence-leave",
   " \\s+ => \\s+ "
   //" [/-]? "
   //" > "
   //" [,;.]? "
   ,[&]
 {
  //QString m = p.match_text();
  graph_build.attribute_sequence_leave();
 });

 add_rule( flags_all_(parse_context ,inside_attribute_sequence),
   ngml_context, "mark-attribute-tile",
   " \\s+ @ \\s+ "
   ,[&]
 {
  graph_build.mark_attribute_tile();
 });



 // //  these should be for graph_build the equivalent
  //    of ->> (etc.) then `::some_cmd;
 add_rule( flags_all_(parse_context ,inside_multi_generic),
   ngml_context,
   "cmd-multi-arg-transition",
   " (?<fiat-or-cmd> (?: :: ) | = ) (?<main> -{1,2}>{1,2}) "
   " \\s+ (?<cmd> .valid-tag-command-name. ) "
    ,[&]
 {
  QString m = p.matched("main");
  QString cmd = p.matched("cmd");
  QString fiat = p.matched("fiat-or-cmd");
  graph_build.multi_arg_transition({}, {}, m);
  if(fiat == "=")
    graph_build.tag_command_entry_inline("::", fiat, cmd, ";", {});
  else
    graph_build.tag_command_entry_inline(fiat, {}, cmd, ";", {});
 });

 add_rule( flags_all_(parse_context ,inside_multi_generic),
   ngml_context,
   "multi-arg-transition-to-main-tile",
   " \\s+ => \\s+ "
   ,[&]
 {
  graph_build.multi_arg_transition_to_main_tile();
 });

 add_rule( flags_all_(parse_context ,inside_multi_generic),
   ngml_context, 
   "multi-arg-transition",
   " \\s+ (?<wmi> .tag-command-wrap-mode-indicator.? ) "
   " (?<fiat> =?)  "
   " (?<main> -{1,2}>{1,2} ) \\s+ "
   ,[&]
 {
  QString wmi = p.matched("wmi");  
  QString fiat = p.matched("fiat");  
  QString m = p.matched("main");
  graph_build.multi_arg_transition(wmi, fiat, m);
 });

 add_rule( flags_all_(parse_context ,inside_multi_parent_semis),
   ngml_context, "tag-command-leave-multi",
   "  \\s+ ;; "
   ,[&]
 {
  graph_build.tag_command_leave_multi({});
 //?  parse_context.flags.inside_multi_parent_semis = false;
 //? parse_context.flags.inside_multi_parent = false;
 });

 add_rule( flags_all_(parse_context ,inside_multi_parent),
   ngml_context, "tag-command-leave-multi",
   "  ` (?<tag-command> .valid-tag-command-name.? ) ` "
   ,[&]
 {
  QString tag_command = p.matched("tag-command");
  graph_build.tag_command_leave_multi(tag_command);
//?  parse_context.flags.inside_multi_parent = false;
 });


 add_rule( ngml_context, "cmd-tag-command-entry-multi",
   " ` (?<wmi> .tag-command-wrap-mode-indicator.? ) "
   " (?<tag-command> .valid-tag-command-name. ) "
   " (?<tag-body-follow> [,.]?) "
   " \\s+ (?<fiat-or-cmd> (?: :: ) | = ) "
   " (?<first-arg-marker> -{1,2} >{1,2} ) \\s+ (?<cmd> .valid-tag-command-name. )"
   ,[&]
 {
  QString wmi = p.matched("wmi");
  QString tag_command = p.matched("tag-command");
  QString tag_body_follow = p.matched("tag-body-follow");
  QString fiat = p.matched("fiat-or-cmd");
  QString first_arg_marker = p.matched("first-arg-marker");
  graph_build.tag_command_entry_multi(wmi, tag_command,
    tag_body_follow, {}, {}, first_arg_marker);
  QString cmd = p.matched("cmd");

  if(fiat == "=")
    graph_build.tag_command_entry_inline("::", fiat, cmd, ";", {});
  else
    graph_build.tag_command_entry_inline(fiat, {}, cmd, ";", {});

  //graph_build.tag_body_leave();
 });


 add_rule( ngml_context, "tag-command-entry-multi",
  " ` (?<wmi> .tag-command-wrap-mode-indicator.? ) " 
  " (?<tag-command> .valid-tag-command-name. ) "
  " (?<tag-body-follow> [,.]?) \\s+  "
  " (?<fwmi> .tag-command-wrap-mode-indicator.? ) "
  " (?<first-arg-marker> @ | (?: -{1,2} >{1,2} ) ) \\s+ "
           ,[&]
 {
  QString wmi = p.matched("wmi");
  QString fwmi = p.matched("fwmi");
  QString tag_command = p.matched("tag-command");
  QString tag_body_follow = p.matched("tag-body-follow");
  QString first_arg_marker = p.matched("first-arg-marker");
  graph_build.tag_command_entry_multi(wmi, tag_command, 
    tag_body_follow, {}, fwmi, first_arg_marker);
    //graph_build.tag_body_leave();
 });

 add_rule( ngml_context, "tag-command-entry-inline",
   " ` (?<wmi> .tag-command-wrap-mode-indicator.? ) "
   " (?<tag-command> .valid-tag-command-name. ) "
   " (?: < (?<argument> [^>]+ ) >)?  (?<tag-body-follow> [,;.] ) "
   ,[&]
 {
  QString wmi = p.matched("wmi");
  QString tag_command = p.matched("tag-command");
  QString tag_body_follow = p.matched("tag-body-follow");
  QString argument = p.matched("argument");
  graph_build.tag_command_entry_inline(wmi, {}, tag_command, tag_body_follow, argument);
  //graph_build.tag_body_leave();
 });

 add_rule( ngml_context, "alt-tag-command-entry-inline",
   " ` (?<wmi> .tag-command-wrap-mode-indicator.? ) "
   " (?<tag-command> .valid-tag-command-name. ) "
   " <> (?<argument> [^;]* ) ;"
   ,[&]
 {
  QString wmi = p.matched("wmi");
  QString tag_command = p.matched("tag-command");
  QString argument = p.matched("argument");

  argument.replace('{', "! ");
  argument.replace('[', "? ");
  argument.replace('}', " ");
  argument.replace(']', " ");

  graph_build.tag_command_entry_inline(wmi, {}, tag_command, ";", argument);
  //graph_build.tag_body_leave();
 });


#ifdef HIDE
 add_rule( html_context, "html-tag-command-leave",
  " </ (?<tag-command> .valid-tag-command-name. ) "
  " > "
  ,[&]
 {
  QString tag_command = p.matched("tag-command");
  graph_build.check_html_tag_command_leave(tag_command, p.match_text());
 });

 add_rule( ngml_context, "tag-command-leave",
  " < (?<load-connector> .valid-tag-command-name. )? "
  "  < (?<tag-command> .valid-tag-command-name. ) "
  " /? "  // this really needs to be set to check for matching env's
  " > "
           ,[&]
 {
  QString load_connector = p.matched("load-connector");
  QString tag_command = p.matched("tag-command");
  if(load_connector.isEmpty())
   graph_build.check_tag_command_leave(tag_command, p.match_text());
  else
   graph_build.tag_command_leave(load_connector, tag_command);
 });
#endif //def HIDE

 add_rule( ngml_context, "tag-block-command-leave",
  "  ` (?<tag-command> .valid-tag-command-name. ) ` "
  ,[&]
 {
  QString tag_command = p.matched("tag-command");
  graph_build.check_tag_command_leave(tag_command, p.match_text());
 });

 add_rule( ngml_context, "inline-tag-command-leave",
  " ` (?: / | (?= \\s))  "
           ,[&]
 {
  graph_build.inline_tag_command_leave();
 });


#ifdef HIDE
 add_rule( ngml_context, "generic-tag-command-leave",
  " </>  "
           ,[&]
 {
  graph_build.tag_command_leave();
 });
#endif //def HIDE

//?
// add_rule( ngml_context, "special-character-sequence",
//  " (?: %-- ) | (?: %[<>$'] ) | (&-\\S>\\w) | (&#\\w+;) | (&:\\w+;) "
//           ,[&]
// {
//  QString m = p.match_text();
//  graph_build.special_character_sequence(m);
// });


  add_rule( ngml_context, "special-character-sequence",
   " (?: %-- ) | (?: ->- ) "
   "  | (?: ` \\( (?<bq-esc1> [^]]+ ) \\) ) "
   "  | (?: ` { (?<bq-esc2> [^}]+ ) } ) "
   "  | (?: ` \\[ (?<bq-esc3> [^]]+ ) \\] ) "
   "  | (?: ` < (?<bq-esc4> [^>]+ ) \\> ) "
   "  | (?: \\| (?<pipe-esc> [^|]+) \\| )"
 
          //? "| (?: %[<>$'] ) | (&-\\S>\\w) | (&#\\w+;) | (&:\\w+;) "
            ,[&]
  {
   int which = 1;
   QString esc = p.matched("bq-esc1");
   if(esc.isEmpty())
   {
    esc = p.matched("bq-esc2");
    ++which;
   }
   if(esc.isEmpty())
   {
    esc = p.matched("bq-esc3");
    ++which;
   }
   if(esc.isEmpty())
   {
    esc = p.matched("bq-esc4");
    ++which;
   }
   if(esc.isEmpty())
   {
    esc = p.matched("pipe-esc");
    ++which;
   }
   if(esc.isEmpty())
   {
    which = 0;
   }

   QString m = p.match_text();

   graph_build.special_character_sequence(m, esc, which);
  });


 add_rule( ngml_context, "tile-acc",
  " . "
           ,[&]
 {
  graph_build.tile_acc(p.match_text());
 });


}
