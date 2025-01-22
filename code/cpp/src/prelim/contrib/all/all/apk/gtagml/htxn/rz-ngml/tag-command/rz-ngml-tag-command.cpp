
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-ngml-tag-command.h"

#include "rzns.h"
USING_RZNS(NGML)


NGML_Tag_Command::NGML_Tag_Command(QString name, 
  QString argument, QString parent_tag_type)
 : Flags(0), name_(name), argument_(argument),
   parent_tag_type_(parent_tag_type),
   ref_position_(0), ref_order_(0),
   ngml_htxn_node_(nullptr), arg_ngml_htxn_node_(nullptr)
{

}

void NGML_Tag_Command::each_arg_ngml_htxn_node(std::function<void(NGML_HTXN_Node*)> fn)
{
 for(NGML_HTXN_Node* nhn : arg_ngml_htxn_nodes_)
   fn(nhn);
}

void NGML_Tag_Command::add_arg_ngml_htxn_node(NGML_HTXN_Node* nhn)
{
 arg_ngml_htxn_nodes_.append(nhn);
}

u2 NGML_Tag_Command::get_whitespace_code()
{
 if(flags.nonstandard_space)
   return 9999;
 u2 result = 0;
 if(flags.left_line_double_gap)
   result += 2000;
 if(flags.left_line_gap)
   result += 1000;
 if(flags.left_space_gap)
   result += 100;
 if(flags.right_space_gap)
   result += 1;
 if(flags.right_line_double_gap)
   result += 20;
 if(flags.right_line_gap)
   result += 10;
 return result;
}


void NGML_Tag_Command::normalize_whitespace()
{
 u1 counts [4];
 ws().get_counts(counts);
 get_whitespace_counts_as_inherited(counts);
 for(int i = 0; i < 4; ++i)
   if(counts[i] == 255)
   {
    flags.nonstandard_space = true;
    return;
   }

 if(counts[0] > 1)
   flags.right_line_double_gap = true;
 else if(counts[0] == 1)
   flags.right_line_gap = true;
 if(counts[1] >= 1)
   flags.right_space_gap = true;

 if(counts[2] > 1)
   flags.left_line_double_gap = true;
 else if(counts[2] == 1)
   flags.left_line_gap = true;
 if(counts[3] >= 1)
   flags.left_space_gap = true;
}


QString NGML_Tag_Command::latex_name()
{
 QString result = name_;
 result.remove('-');
 return result;
}

