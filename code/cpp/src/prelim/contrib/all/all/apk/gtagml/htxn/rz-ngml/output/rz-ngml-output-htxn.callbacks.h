

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
  enter_xml_save(node, index);
  //qDebug() << "xmlsave_";
  //suppress_node_ = node;
 }
 _WHEN_(post)
 {
 }
 _WHEN
_NGML_CALLBACK


NGML_CALLBACK_(_xml-save)
 WHEN_(pre)
 {
  leave_xml_save(index - 1);
  //qDebug() << "xmlsave_";
  //suppress_node_ = node;
 }
 _WHEN_(post)
 {
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


 NGML_CALLBACK_(khi-file-info)
  WHEN_(around)
  {
//   process_khi_file_info(qts, node);
   //?cb->flags.pre_fallthrough = true;
  }
  //_WHEN_(post)
  //{
  //?qts << "&rdquo;";
  //?check_post_whitespace(qts, node);
  //}
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
//  insert_khif_link(qts);
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

