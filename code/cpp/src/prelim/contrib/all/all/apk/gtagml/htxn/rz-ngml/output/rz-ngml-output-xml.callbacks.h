
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


NGML_CALLBACK_(analysis-package)
 WHEN_(around)
 {
  //report_analysis_package(qts, node);
 }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(xml-save_)
 WHEN_(pre)
 {
  suppress_node_ = node;
 }
 _WHEN_(post)
 {
 }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(_xml-save)
 WHEN_(pre)
 {
  suppress_node_ = nullptr;
 }
 _WHEN_(post)
 {
 }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(saved-xml)
 WHEN_(around)
 {
  write_saved_xml(qts, node);
 }
 _WHEN
_NGML_CALLBACK



NGML_CALLBACK_SUPPRESS(documentclass)
NGML_CALLBACK_NOACTION(include)

NGML_CALLBACK_(noxml_)
 WHEN_(around)
 {
  suppress_node_ = node;
 }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(_noxml)
 WHEN_(around)
 {
  suppress_node_ = nullptr;
 }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(dclass)
 WHEN_(pre)
 {
  suppress_node_ = node;
 }
 _WHEN_(post)
 {
  suppress_node_ = nullptr;
 }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(document)
 WHEN_(pre)
 {
 }
 _WHEN_(post)
 {
 }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(inc)
 WHEN_(around)
 {
 }
 _WHEN
_NGML_CALLBACK



 NGML_CALLBACK_(fnl)
  WHEN_(around)
  {
   qts << '\n';
  }
  _WHEN
 _NGML_CALLBACK



//RENAME_TAG(cp, p)
RENAME_(cp, p, comment)
RENAME_(hide, span, hide)
RENAME_(ulead, u, leader)
RENAME_(iag, i, autogen)
RENAME_TAG(pdf-ref, span)
RENAME_(latex-only, span, latex-only)


NGML_CALLBACK_(q)
 WHEN_(pre)
 {
  qts << "&ldquo;";
 }
 _WHEN_(post)
 {
  qts << "&rdquo;";
  //?check_post_whitespace(qts, node);
 }
 _WHEN
_NGML_CALLBACK



 NGML_CALLBACK_(meetup-add)
  WHEN_(pre)
  {
//   start_meetup_add(qts, node);
  }
  _WHEN_(post)
  {
//   complete_meetup_add(qts, node);
  }
  _WHEN
 _NGML_CALLBACK



NGML_CALLBACK_(code)
 WHEN_(pre)
  {
//   flags.code_display_mode = true;
   cb->flags.pre_fallthrough = true;
  }
  _WHEN_(post)
  {
//   flags.code_display_mode = false;
   cb->flags.post_fallthrough = true;
  }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(bq)
 WHEN_(around)
 {
  qts << "`";
 }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(sq)
 WHEN_(pre)
 {
  qts << "&lsquo;";
 }
 _WHEN_(post)
 {
  qts << "&rsquo;";
 }
 _WHEN
_NGML_CALLBACK




NGML_CALLBACK_(kf-link)
 WHEN_(around)
 {
//?  insert_kf_link(qts);
//  qts << "<a href='/kf' class='kf-link'>.kf</a>";
 }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(show-annotations)
 WHEN_(around)
 {
//  html_show_annotations(qts);
 }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(show-local-annotations)
 WHEN_(around)
 {
//  html_show_local_annotations(qts, node);
 }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(srow)
 WHEN_(around)
 {
//  insert_semantic_row(qts, node);
 }
 _WHEN
_NGML_CALLBACK

