
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


//?#define QSTRING_STRIP_HYPHEN(x) QString(x).remove('-')

//?#define QSTRING_STRIP_HYPHEN(x) QString(x)

#define RENAME_(name, tag, style_class) \
 callbacks_[#name] = caon_ptr<NGML_Command_Callback>( new NGML_Command_Callback(#name, #tag, #style_class) ); \


#define RENAME_TAG(name, tag) \
 callbacks_[#name] = caon_ptr<NGML_Command_Callback>( new NGML_Command_Callback(#name, #tag) ); \


#define NGML_CALLBACK_(name) \
 callbacks_[#name] = caon_ptr<NGML_Command_Callback>( new NGML_Command_Callback(#name, \
  NGML_Command_Callback::Callback_Map_type{{ \


// //  Should b be captured, or an argument?

#define WHEN_(kind) \
 {#kind, [this](QTextStream& qts, caon_ptr<tNode> node, \
 u4 index, caon_ptr<NGML_Command_Callback> cb)

#define _WHEN },

#define _WHEN_(kind) _WHEN WHEN_(kind)

#define _NGML_CALLBACK }}) );

#define NGML_CALLBACK_SUPPRESS(name) \
 NGML_CALLBACK_(name) \
 WHEN_(pre) \
 { \
  suppress_node_ = node; \
 } \
 _WHEN_(post) \
 { \
  suppress_node_ = nullptr; \
 } \
 _WHEN \
_NGML_CALLBACK \

#define NGML_CALLBACK_NOACTION(name) \
 NGML_CALLBACK_(name) \
  WHEN_(around) \
  { \
  } \
  _WHEN \
_NGML_CALLBACK \


