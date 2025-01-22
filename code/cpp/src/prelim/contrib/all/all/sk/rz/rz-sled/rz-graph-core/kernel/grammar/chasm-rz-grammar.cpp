
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-grammar.h"

#include "kernel/grammar/chasm-rz-parse-context.h"

#include "kernel/grammar/chasm-rz-graph-build.h"

#include "kernel/grammar/chasm-rz-parser.h"

#include "relae-graph/relae-parser.templates.h"

USING_RZNS(RZ_Core)

ChasmRZ_Grammar::ChasmRZ_Grammar()
{
}

void ChasmRZ_Grammar::init(ChasmRZ_Parser& p, ChasmRZ_Graph& g, ChasmRZ_Graph_Build& graph_build)
{

 pre_rule( "script-word", "(?:[^{}()\\[\\]\\s`;,:]|(?:\\w::?\\w))+" );
 pre_rule( "ns-word", "(?: [^{}()\\[\\]\\s`;,:]+ )" );

 pre_rule( "space-to-end-of-line", "[__\\t]* (?: ;- [^\\n]+ )? (?= \\n )" );
 pre_rule( "end-of-line", "[__\\t\\S]* \\n" );
 pre_rule( "single-space", "[__\\t]" );
 pre_rule( "namespaced-word", "[\\w-:]+" );

 Context run_context = add_context("run-context");

 Context data_context = add_context("data-context");

 Context asg_context = add_context("asg-context");

 track_context({&run_context,
                &data_context,
                &asg_context});

 activate(run_context);

 ChasmRZ_Parse_Context& parse_context = graph_build.parse_context();

 parse_context.flags.pchasm_rz_markup = true;

 add_rule(flags_all_(parse_context ,take_a_space), run_context, "take-a-space",
   " .single-space. ",
   [&]
  {
   graph_build.finalize_take_a_space();
  } );

 add_rule( flags_all_(parse_context ,inside_string_literal), run_context,
  "string-literal-character",
    // // note ++ prevents backtracking inside \\"-like strings,
     //   which avoids rroneously matching the \'s
     //   before the last \; the point here is to
     //   exclude any run of \'s before " as an
     //   atomic unit (which needs ++ rather than +).
  " (?: [^\"\\\\]+ ) | (?: (?:\\\\++)(?!\") ) ",
  [&]
 {
  QString str = p.match_text();
// //?  if(str == "\\\"")
//   str = "\"";
  graph_build.add_to_string_literal(str);
 });


 add_rule( flags_all_(parse_context ,inside_string_literal), run_context,
  "string-literal-maybe-end",
  " \\\\*\" ",
  [&]
 {
  // depends on how many back-slahes ...
  QString str = p.match_text();
  if(str.size() % 2) // odd mans even backslashes ...
  {
   if(str.size() > 1)
   {
    str.chop(1);
    graph_build.add_to_string_literal(str);
   }
   graph_build.process_string_literal();
   parse_context.flags.inside_string_literal = false;
  }
  else
  {
   graph_build.add_to_string_literal(str);
  }

 });


 add_rule( run_context, "raw-asg-start",
  " asg_ ",
   [&]
 {
  graph_build.raw_asg_start();
 });

 add_rule( run_context, "raw-asg-end",
  " _asg ",
   [&]
 {
  graph_build.process_raw_asg();
  parse_context.flags.inside_raw_asg = false;
 });


 add_rule(flags_all_(parse_context ,inside_raw_asg), run_context, "raw-asg-text",
          " . "
          ,[&]
 {
  graph_build.add_to_raw_asg(p.match_text());
 });


 add_rule( run_context, "string-literal-start",
  " \" ",
   [&]
 {
  graph_build.string_literal_start();
 });



 add_rule(flags_all_(parse_context ,inside_string_plex), run_context, "string-plex-switch",
          " : (?<key> \\S* ) : "
          ,[&]
 {
  QString key = p.matched("key");
  graph_build.string_plex_switch(key);
 });


 add_rule(flags_all_(parse_context ,inside_string_plex), run_context, "end-string-plex",
          " (?<entry> \\] ) (?<suffix> -- ) "
          ,[&]
 {
  QString name = QString();
  QString prefix = QString();
  QString entry = p.matched("entry");
  QString suffix = p.matched("suffix");
  graph_build.enter_tuple(name, prefix, entry, suffix);
 });

 add_rule(flags_all_(parse_context ,inside_string_plex), run_context, "string-plex-text",
          " (?: [^:] | (?: : \\S* \\s+) ) "
          ,[&]
 {
  graph_build.string_plex_acc(p.match_text());
 });


 add_rule( run_context, "lexical-symbol-declaration",
  " , (?<token>.script-word.) ",
   [&]
 {
  QString token = p.matched("token");
  graph_build.declare_lexical_symbol(token);
 });



 add_rule( run_context, "initialization-via-constructor",
  " == ",
   [&]
 {
  QString match_text = p.match_text();
  graph_build.add_equalizer_to_type(match_text);
 });


 add_rule( run_context, "run-token-type-related-function",
  " (?<word> ::: ) "
  " (?<eol> .space-to-end-of-line. \\n)?",
   [&]
 {
  QString m = p.matched("word");
  QString s = p.matched("eol");
  graph_build.add_run_token("", m, "", ChasmRZ_Graph_Build::Token_Formations::Normal, s);
 });

 add_rule( run_context, "run-token-with-eol",
  " (?<prefix> [:,;`']* )"
  " (?<word> .script-word.) "
  " (?<suffix> :? ) "
  " (?<eol> .space-to-end-of-line. \\n)?",
   [&]
 {
  QString prefix = p.matched("prefix");
  QString m = p.matched("word");
  QString suffix = p.matched("suffix");
  QString s = p.matched("eol");
  graph_build.add_run_token(prefix, m, suffix, ChasmRZ_Graph_Build::Token_Formations::Normal, s);
 });


 add_rule( run_context, "run-token-semis",
  " (?<word> ;+ )"
  " (?<suffix> [,/.]? )"
  " (?<eol> .space-to-end-of-line. \\n)?",
   [&]
 {
  QString m = p.matched("word");
  QString s = p.matched("suffix");
  QString e = p.matched("eol");
  graph_build.add_semis(m, s, e);
 });

}

