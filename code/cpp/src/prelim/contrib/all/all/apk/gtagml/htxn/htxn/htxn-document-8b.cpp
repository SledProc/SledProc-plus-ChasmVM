
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "htxn-document-8b.h"

#include "standard-glyphdeck-8b.h"
#include "standard-diacritic-glyphdeck.h"

#include "glyph-vector-8b.h"
#include "glyph-layer-8b.h"

#include "glyph-argument-package.h"

#include "infoset/sdi-callback-8b.h"

#include <QDebug>

USING_KANS(HTXN)


HTXN_Document_8b::HTXN_Document_8b()
  :  current_deck_(nullptr), 
     current_diacritic_deck_(nullptr), 
     current_deck_code_(0),
     current_glyph_vector_(nullptr)
{
}

void HTXN_Document_8b::add_standard_deck()
{
 current_deck_ = new Standard_GlyphDeck_8b;
 decks_by_id_.push_back({current_deck_});
 id_by_deck_[current_deck_] = decks_by_id_.size();
}

void HTXN_Document_8b::add_standard_diacritic_deck()
{
 current_diacritic_deck_ = new Standard_Diacritic_GlyphDeck;
 generic_glyph_base ggb {.dia =
   (Diacritic_GlyphDeck_Base*) current_diacritic_deck_};
 decks_by_id_.push_back(ggb);
 dia_id_by_deck_[current_diacritic_deck_] = decks_by_id_.size();
}

// void HTXN_Document_8b::sss;

void HTXN_Document_8b::mark_last_as_environment_main_tile(u4 ref,
  Glyph_Layer_8b* target_layer, u4 ref_position, u4 ref_order, u4 leave)
{
 HTXN_Node_Detail& nd = node_details_[ref - 1];
 if(QVector<u4>* vec = nd.get_refs())
 {
  if(vec->isEmpty())
    return;
  u4 last = vec->last();
  HTXN_Node_Detail& nd1 = node_details_[last - 1];
  nd1.flags.region_main_preempts_wrap = true;
  // // maybe even elevate nd1 to a main tile ...
   //   but for now just try
  if(target_layer)
  {
   if(nd1.get_layer() == target_layer)
   {
    nd1.flags.is_ghosted = true;
    target_layer->set_range_leave(ref_position, ref_order, leave);
    return;
   }
  }
  nd.flags.needs_write_end = true;
 } 
}

void HTXN_Document_8b::check_precedent_ranges(const HTXN_Node_Detail& nd,
  u2 enter_order, QVector<QPair<HTXN_Node_Detail*, QString>>& result,
  Glyph_Layer_8b* calling_layer)
{
 if(QVector<u4>* vec = nd.get_refs())
 {
  result.resize(vec->size());

   // // use an extra list to view the results 
    //   while debugging; the important 
    //   operations are those modifying the 
    //   pairs in place ...  
  QStringList check;
  std::transform(vec->begin(), vec->end(), 
    result.begin(), std::back_inserter(check),
    [this, enter_order, calling_layer](u4 i, QPair<HTXN_Node_Detail*, QString>& pr)
  {
   HTXN_Node_Detail* nd = &node_details_[i - 1];

   if(nd->flags.is_ghosted)
   {
    pr.first = nullptr;
    return QString("<ghost>");
   }

   if(nd->get_layer() == calling_layer)
     calling_layer->add_insert_loop_guard(nd->enter);

   get_latex_insert(nd, nd->order,  pr.second);

   if(nd->get_layer() == calling_layer)
     calling_layer->update_insert_loop_guard(nd->enter, nd->leave);

   pr.first = nd;
   return pr.second;
  });

  qDebug() << "Check: " << check;

 }
}

void HTXN_Document_8b::get_latex_insert(HTXN_Node_Detail* nd,
  u2 enter_order, QString& result)
{
 Glyph_Layer_8b* gl = nd->get_layer();
 get_latex_out(gl, enter_order, nd->enter, nd->leave, result, nd);
} 

void HTXN_Document_8b::check_pre_space_append(QString& text, const HTXN_Node_Detail& nd)
{
 if(nd.flags.pre_line_double_gap)
 {
  if(text.endsWith("\n\n"))
    return;
  if(text.endsWith("\n"))
  {
   text.append("\n");
   return;
  }
  text.append("\n\n");
  return;
 }
 if(nd.flags.pre_line_gap)
 {
  if(text.endsWith("\n"))
    return;
  text.append("\n");
  return;
 }
 if(nd.flags.pre_space_gap)
 {
  if(text.endsWith(" "))
    return;
  text.append(" ");
  return;
 }
}

void HTXN_Document_8b::calculate_orders()
{
 QMap< QPair<Glyph_Layer_8b*, u4>, u2 > counts;
 for(HTXN_Node_Detail& nd : node_details_)
 {
  nd.order = ++counts[{nd.get_layer(), nd.enter}];
 }
}


QString HTXN_Document_8b::check_latex_insert(Glyph_Layer_8b& gl,
  u2 enter_order, u4 index, Glyph_Argument_Package& cmdgap, 
  QVector<QPair<HTXN_Node_Detail*, QString>>& precs,
  QStringList& succs, QString& result)
{
 // //  check enters ...
 u4 leave = 0;
 u2 count = 0;
 if(enter_order > 0)
   count = enter_order - 1;
 for (;;)
 {
  u4 node_code = gl.get_range_by_enter(index, leave, count);
  if(node_code == 0)
    break;
  ++count;
  const HTXN_Node_Detail& nd = node_details_[node_code - 1];
  Glyph_Layer_8b* ngl = nd.get_layer();
    //(Glyph_Layer_8b*) nd.node_ref;
  QString cmd;
  get_latex_command(*ngl, nd.enter, nd.leave, cmdgap, cmd);

  check_pre_space_append(result, nd);

//?
//  QString pre_space = nd.get_pre_space();
//  result.append(pre_space);

  if(nd.flags.ref_preempts_wrap)
  {
   if(nd.flags.region)
     result.append(QString("\\begin{%1}").arg(cmd));
   else
     result.append(QString("\\%1").arg(cmd));
  }
  else if(nd.flags.optional)
  {
   if(nd.flags.wmi_left)
   {
    if(nd.flags.wmi_with_space)
      result.append(QString("[\\%1 ").arg(cmd));
    else
      result.append(QString("[\\%1").arg(cmd));
   }
   else
     result.append(QString("\\%1[").arg(cmd));
  }
  else if(nd.flags.region)
    result.append(QString("\\begin{%1}").arg(cmd));
  else
  {
   if(nd.flags.wmi_left)
   {
    if(nd.flags.wmi_with_space)
      result.append(QString("{\\%1 ").arg(cmd));
    else
      result.append(QString("{\\%1").arg(cmd));
   }
   else if(nd.flags.wmi_none)
   {
    if(nd.flags.wmi_with_space)
      result.append(QString("\\%1 ").arg(cmd));
    else
      result.append(QString("\\%1").arg(cmd));
   }
   else
    result.append(QString("\\%1{").arg(cmd));
  }
  cmdgap.reset();
  if(leave == 0)
  {
   if(nd.flags.ref_preempts_wrap)
     ; // nothing
   else if(nd.flags.wmi_none)
     ; // nothing
   else if(nd.flags.optional)
     result.append("]");
   else if(nd.flags.region)
     result.append(QString("\\end{%1}").arg(cmd));
   else
     result.append("}");
   //?result.append(post_space);
  }  
  else
    gl.add_leave(leave, cmd, &nd, node_code);

   // // here ?
  check_precedent_ranges(nd, enter_order, precs, &gl);
  for(QPair<HTXN_Node_Detail*, QString>& pr : precs)
  {
   if(!pr.first)
     continue; // ghosted ...
   if(pr.first->flags.optional)
     result.append(QString("[%1]").arg(pr.second));
   else if(pr.first->flags.region_main_preempts_wrap)
     result.append(pr.second);
   else
     result.append(QString("{%1}").arg(pr.second));
  }
  if(nd.flags.ref_preempts_wrap)
  {
   //check_post_space_append(result, nd);
   QString post_space = nd.get_post_space();
   result.append(post_space);
  }
  precs.clear();
  if(nd.flags.needs_write_end)
    result.append(QString("\\end{%1}").arg(cmd));
 }

 QString end_result;
 // // and leaves ...
 QVector<QPair<QString, const HTXN_Node_Detail*>> prs = gl.check_leave(index);
 for(auto pr: prs)
 {
  const HTXN_Node_Detail* nd = pr.second;
  if(nd->flags.wmi_none)
    ; // nothing
  else if(nd->flags.optional)
    end_result.append("]");
  else if(nd->flags.region)
    end_result.append(QString("\\end{%1}").arg(pr.first));
  else
    end_result.append(QString("}"));
  QString post_space = nd->get_post_space();
  end_result.append(post_space);
 }
 return end_result;
}

void HTXN_Document_8b::get_latex_command(Glyph_Layer_8b& gl, u4 enter, u4 leave,
  Glyph_Argument_Package& gap, QString& result)
{
 for(u4 i = enter; i <= leave; ++i)
 {
  this->Glyph_Layers_8b::get_latex_command_out(gl, i, gap);
  if(gap.chr.isNull())
    result.append(gap.str);
  else
    result.append(gap.chr);
  gap.reset();
 }
}

void HTXN_Document_8b::write_latex_out(QTextStream& qts)
{
 QString text;
 get_latex_out(1, text);
 qts << text;
} 

void HTXN_Document_8b::get_latex_out(u4 layer, QString& result)
{
 Glyph_Layer_8b* gl = value(layer - 1);
 if(!gl)
   return;
 get_latex_out(gl, 0, 0, gl->size() - 1, result);
}

void HTXN_Document_8b::tie_detail_range_preempt(u4 rc1, u4 rc2)
{
 HTXN_Node_Detail* nd = tie_detail_range(rc1, rc2);
 nd->flags.ref_preempts_wrap = true;
}

void HTXN_Document_8b::write_minimal_latex_out(u4 layer_code,
  const QPair<u4, u4>& range, QTextStream& qts, SDI_Callback_8b* cb)
{
 Glyph_Layer_8b* gl = value(layer_code - 1);
 write_minimal_latex_out(gl, range.first, range.second, qts, cb);
}

void HTXN_Document_8b::write_minimal_latex_out(u4 layer_code,
  const QPair<u4, u4>& range, QTextStream& qts)
{
 Glyph_Layer_8b* gl = value(layer_code - 1);
 write_minimal_latex_out(gl, range.first, range.second, qts);
}

void HTXN_Document_8b::write_minimal_latex_out(Glyph_Layer_8b* gl, u4 enter,
  u4 leave, QTextStream& qts)
{
 Glyph_Argument_Package gap;
 gap.internal_deck = current_deck_;

 for(u4 i = enter; i <= leave; ++i)
 {
  this->Glyph_Layers_8b::get_latex_out(*gl, i, gap);
  if(gap.chr.isNull())
    qts << gap.str;
  else
    qts << gap.chr;
  gap.reset();
 }
}

void HTXN_Document_8b::write_minimal_latex_out(Glyph_Layer_8b* gl, u4 enter,
  u4 leave, QTextStream& qts, SDI_Callback_8b* cb)
{
 Glyph_Argument_Package gap;
 gap.internal_deck = current_deck_;
 u8 skip_flag = 0;
 QString pre_insert;
 QString post_insert;

 for(u4 i = enter; i <= leave; ++i)
 {
  cb->pre_write(*gl, i, gap, skip_flag, pre_insert, post_insert);
  if(!pre_insert.isEmpty())
  {
   qts << pre_insert;
   pre_insert.clear();
  }
  if(skip_flag)
  {
   skip_flag = 0;
   if(!post_insert.isEmpty())
   {
    qts << post_insert;
    post_insert.clear();
   }
  }
  else
  {
   this->Glyph_Layers_8b::get_latex_out(*gl, i, gap);
   if(gap.chr.isNull())
     qts << gap.str;
   else
     qts << gap.chr;
   if(!post_insert.isEmpty())
   {
    qts << post_insert;
    post_insert.clear();
   }
  }
  gap.reset();
 }
}

// // use a "private" struct in this case for the 
 //   enum ...
struct _csb
{
 u4 enter;
 u4 leave;
 Glyph_Layers_8b& layers;
 Glyph_Layer_8b& gl;
 GlyphDeck_Base_8b& deck; 
 QMap<u4, QString>& notes;

 enum States {
   N_A, Letter, Maybe_Sentence_End, 
   One_Space, Two_Space, 
   //?Confirmed_Sentence_End   
 };

 States state;
 u4 r1;
 u4 r2;

 QVector<u4> intermediates;
 QVector<QPair<u4, u4>>& gaps;
 
 void check_sentence_boundaries();
 void check_state(u1 gp, u4 i);
 void check_state_letter();
 void check_state_space(u4 i);  
 void check_state_newline(u4 i);  
 void check_state_maybe_sentence_end(u4 i);
};

void _csb::check_state_letter()
{
 switch(state)
 {
 case N_A:
 case Letter:  
 case Two_Space:
   state = Letter;
   break;
 case One_Space:
   deck.swap_false_sentence_end(gl[r1]);
   for(u4 u : intermediates)
     deck.swap_false_sentence_end(gl[u]);
   intermediates.clear();
   break;  
 case Maybe_Sentence_End:
   deck.check_swap_dot(gl[r1], true);
   break;
 }
 r1 = 0;
 r2 = 0;
}

void _csb::check_state_maybe_sentence_end(u4 i)
{
 switch(state)
 {
 case N_A:
 case Letter: 
   state = Maybe_Sentence_End;
   r1 = i;
   break;
 case Maybe_Sentence_End:
   if(!intermediates.isEmpty())
     intermediates.push_back(i);
    // // i.e., if this is true, the 
     //   period is interpreted as 
     //   non-punctuation (e.g., abbreviation)
     //   which means the current glyph 
     //   is the true sentence end ...
   else if(deck.check_swap_dot(gl[r1], true))
     r1 = i;
   else
     intermediates = {i};     
   break;
 case One_Space:
   deck.swap_false_sentence_end(gl[i]);
   break;
 case Two_Space:
   deck.swap_false_sentence_end(gl[i]);
   goto r_reset;
 }
 return;
r_reset:
 r1 = 0;
 r2 = 0;
}

void _csb::check_state_space(u4 i)
{
 switch(state)
 {
 case N_A:
 case Letter: 
   state = Letter;
   goto r_reset;
 case One_Space:
   if(r2)
   {
    if(r1)
    {
     if(r2 > (r1 + 1))
       gaps.push_back({r1, r2});
    }
    deck.swap_sentence_end_space(gl[r2]);
   }
   for(u4 u : intermediates)
     deck.swap_false_sentence_end(gl[u]);
   intermediates.clear();
   state = Two_Space;
   goto r_reset;
 case Maybe_Sentence_End:
   state = One_Space;
   r2 = i;
   break;
 case Two_Space:
   break;
 }
 return;
r_reset:
 r1 = 0;
 r2 = 0;
}

void _csb::check_state_newline(u4 i)
{
  // //  newline is like two spaces ...
 if(state == Maybe_Sentence_End)
   state = One_Space;
 check_state_space(i);
}

void _csb::check_state(u1 gp, u4 i)
{
 if(gp == 100) // //  null 
 {
  // // actually should really check if 
   //   there's an extension ...
  check_state_letter();
  return;
 }
 else if(gp < 63)
 {
  check_state_letter();
  return;
 }
 else if(gp == 63)
 {
  check_state_space(i);
  return;
 }

 gp &= 63;
   
 switch(gp)
 {
 case 0: case 1: case 9:
  check_state_maybe_sentence_end(i);
  break;
 case 3: // // )
 case 21: // // ]
  break; // // no change in state ...
 case 63:
  check_state_newline(i);
  break;
 default:
  check_state_letter();
  break;  
 }
}

void _csb::check_sentence_boundaries()
{
 Glyph_Argument_Package gap;
 gap.internal_deck = &deck;

 for(u4 i = enter; i <= leave; ++i)
 {
  layers.get_screened_code(gl, i, gap);
  u1 gp = gap.internal_deck->get_standard_equivalent(gap.screened_code);
  check_state(gp, i);
 } 
}

//void HTXN_Document_8b::check_sentence_boundaries(Glyph_Layer_8b* gl, 
//  u4 enter, u4 leave, QMap<u4, QString>& notes, 
//  GlyphDeck_Base_8b* deck)
//{
// SDI_Callback cb;
// check_sentence_boundaries(gl, enter, leave, 
//   notes, &cb, deck);
//}

void HTXN_Document_8b::check_sentence_boundaries(Glyph_Layer_8b* gl, 
  u4 enter, u4 leave, QMap<u4, QString>& notes, 
  //SDI_Callback* cb, 
  GlyphDeck_Base_8b* deck)
{
 if(!deck)
   deck = current_deck_;

 QVector<QPair<u4, u4>> gaps;

 _csb({enter, leave, *this, *gl, *deck, notes, 
   _csb::N_A, 0, 0, {}, gaps}).check_sentence_boundaries();

 for(QPair<u4, u4> pr : gaps)
 {
  sentence_end_gaps_[gl][pr.first] = pr.second; 
 }
}

bool HTXN_Document_8b::scan_for_sentence_start(Glyph_Layer_8b* gl, u4 start, u4 end, u4& result, GlyphDeck_Base_8b* deck)
{
 if(!deck)
  deck = current_deck_;

 Glyph_Argument_Package gap;
 gap.internal_deck = deck;

 for(u4 i = start; i <= end; ++i)
 {
  if(check_letter(*gl, i, gap))
  {
   result = i;
   return i;
  }
 }
 return false; 
}

u4 HTXN_Document_8b::check_advance_to_sentence_end_space(Glyph_Layer_8b* gl, 
  u4 pos, GlyphDeck_Base_8b* deck)
{
 if(!deck)
   deck = current_deck_;

 Glyph_Argument_Package gap;
 gap.internal_deck = deck;

 if(sentence_end_gaps_.contains(gl) && 
   sentence_end_gaps_[gl].contains(pos))
   return sentence_end_gaps_[gl][pos];

 return check_sentence_end_space(*gl, pos + 1, gap)?
   pos + 1 : pos;
}

bool HTXN_Document_8b::scan_for_sentence_end(Glyph_Layer_8b* gl, u4 start, u4 end, u4& result, GlyphDeck_Base_8b* deck)
{
 if(!deck)
  deck = current_deck_;

 Glyph_Argument_Package gap;
 gap.internal_deck = deck;

 for(u4 i = start; i <= end; ++i)
 {
  if(check_sentence_end_marker(*gl, i, gap))
  {
   result = i;
   return true;
  }
 }
}


void HTXN_Document_8b::write_minimal_xml_out(u4 layer_code,
  const QPair<u4, u4>& range, QTextStream& qts)
{
 Glyph_Layer_8b* gl = value(layer_code - 1);
 write_minimal_xml_out(gl, range.first, range.second, qts);
}

void HTXN_Document_8b::write_minimal_xml_out(Glyph_Layer_8b* gl, u4 enter, u4 leave, QTextStream& qts)
{
 Glyph_Argument_Package gap;
 Glyph_Argument_Package cmdgap;
 gap.internal_deck = current_deck_;
 cmdgap.internal_deck = current_deck_;

 for(u4 i = enter; i <= leave; ++i)
 {
  this->Glyph_Layers_8b::get_xml_out(*gl, i, gap);
  if(gap.chr.isNull())
    qts << gap.str;
  else
    qts << gap.chr;
  gap.reset();
 }
}

void HTXN_Document_8b::get_latex_out(Glyph_Layer_8b* gl, u2 enter_order,
  u4 enter, u4 leave, QString& result, HTXN_Node_Detail* nd)
{
 //parse_layer(gv)l
 Glyph_Argument_Package gap;
 Glyph_Argument_Package cmdgap;
 gap.internal_deck = current_deck_;
 cmdgap.internal_deck = current_deck_;
 //?gap.internal_diacritic_deck = current_diacritic_deck_;
 QString end_result;
 QVector<QPair<HTXN_Node_Detail*, QString>> precs;
 QStringList succs;

  // //  this is only relevant at the start of the loop; 
   //    then set it to 0 ...
 u2 insert_enter_order = enter_order;

  // //  need to run at least once if leave < enter ...
   //    
 for(u4 i = enter; (leave < enter) || (i <= leave); ++i, insert_enter_order = 0)
 {
//?
//  if(nd)
//  {
//   // // nd is only non-null if this is a partial
//    //   generation going in to an argument ...
//   // // anything? ...
//  }
//  else
  {
   u4 lg = gl->check_insert_loop_guard(i);
   if(lg > 1)
   {
     // //  in this case everything
      //    is already generated 
    i = lg;
    continue;
   }
   if(lg == 0)
     end_result = check_latex_insert(*gl, insert_enter_order, i, cmdgap,
     precs, succs, result); 

    // //  the insert might have changed the guard...
   lg = gl->check_insert_loop_guard(i);
   if(lg > 1)
   {
     // //  in this case everything
      //    is already generated 
    i = lg;
    continue;
   }
   if(leave < enter)
     break;
  }
  this->Glyph_Layers_8b::get_latex_out(*gl, i, gap);
  if(gap.chr.isNull())
    result.append(gap.str);
  else
    result.append(gap.chr);
  if(!end_result.isEmpty())
  {
   result.append(end_result);
   end_result.clear();
  }
  gap.reset();
 }
}

Glyph_Layer_8b* HTXN_Document_8b::get_layer(u4 layer)
{
 return value(layer);
}

void HTXN_Document_8b::get_qstring_out(u4 layer, QString& result)
{
 Glyph_Vector_8b* gv = value(layer);
 if(!gv)
   return;
 //parse_layer(gv)
 Glyph_Argument_Package gap;
 gap.internal_deck = current_deck_;
 //?gap.internal_diacritic_deck = current_diacritic_deck_;
 for(u4 i = 0; i < gv->size(); ++i)
 {
  Glyph_Layers_8b::get_qstring_out(*gv, i, gap);
  if(gap.chr.isNull())
    result.append(gap.str);
  else
    result.append(gap.chr);
  gap.reset();
 } 
}

void HTXN_Document_8b::write_htxne_out(QIODevice& qiod)
{
 qiod.write("## htxne\n#\n");
 QByteArray qba;
 for(Glyph_Layer_8b* gl : *this)
 {
  get_htxne_out(gl, qba);
  qiod.write(qba, qba.size());
  qiod.write("\n\n"); 
  qba.clear();
 }
// qiod.write("@@ ranges\n");

 QTextStream qts(&qiod);
 qts << "@@ ranges\n";
 qts << (QVector<Glyph_Layer_8b*>&)*this;

 qts << " *\n@@ details\n";
 //qiod.write("@@ details\n");
 qts << node_details_;

 qts << " *\n%% ties\n";
 write_ties(qts);
 //result.append(QString::fromLatin1(qba)); 
}

void HTXN_Document_8b::get_htxne_out(u4 layer, QString& result)
{
 QByteArray qba;
 get_htxne_out(layer, qba);
 result.append(QString::fromLatin1(qba));
}

void HTXN_Document_8b::get_htxne_out(u4 layer, QByteArray& result)
{
 Glyph_Vector_8b* gv = value(layer);
 if(!gv)
   return;
 get_htxne_out(gv, result);
}

void HTXN_Document_8b::get_htxne_out(Glyph_Vector_8b* gv, QByteArray& result)
{
 result.reserve(gv->size());
 Glyph_Argument_Package gap;
 gap.internal_deck = current_deck_;
 u1 run_code = 0;
 u1 run_length = 0;
 u1 tilde_check = 0;

 static auto handle_alt = [&result, &run_code, 
   &run_length, &tilde_check, &gap]
   (u1 alt, char enter, char leave)
 {
  tilde_check = 0;
  if(run_length == 0) 
  {
   result.append('`');
   result.append(enter);
   result.append(gap.chr);
   run_length = 1;
   run_code = alt;
   return;
  }
  // //  now we know there's a run on
  if(run_code == alt)
  {
   result.append(gap.chr);
   if(gap.chr == leave)
   {
    // // always break the run here
    result.append(leave);
    run_length = 0;
    run_code = 0;
   }
   else
     ++run_length;
   return;
  }
  // // now we know the prior run has to break;
  switch(run_code)
  {
   // // the trailing ` is for the next run ...
  case 1: result.append(")`"); break;
  case 2: result.append("]`"); break;
  case 3: result.append("}`"); break;
  case 4: result.append(">`"); break;
  default: break;
  }
  result.append(enter);
  result.append(gap.chr);
  run_length = 1;
  run_code = alt;
 };

 for(u4 i = 0; i < gv->size(); ++i)
 {
  Glyph_Layers_8b::get_htxne_out(*gv, i, gap);

  if(gap.flags.has_alt)
  {
   if(gap.flags.alt_1)
   {
    handle_alt(1, '(', ')');
   }
   else if(gap.flags.alt_2)
   {
    handle_alt(2, '{', '}');
   }
   else if(gap.flags.alt_3)
   {
    handle_alt(3, '[', ']');
   }
   else if(gap.flags.alt_4)
   {
    handle_alt(4, '<', '>');
   }
  }
  else if(run_length > 0)
  {
   // // have to break a run ...
   switch(run_code)
   {
   case 1: result.append(')'); break;
   case 2: result.append('}'); break;
   case 3: result.append(']'); break;
   case 4: result.append('>'); break;
   default: break;
   }
   run_code = 0;
   run_length = 0;
   tilde_check = 1;
   if(gap.chr.isNull())
     result.append(gap.str);
   else
     result.append(gap.chr);
  }
  else 
  {
   if(tilde_check > 0)
   {
    if(tilde_check == 1)
    {
     if(gap.chr == '~')
       tilde_check = 2;
     else 
       tilde_check = 0;
    }
    else if(tilde_check == 2)
    {
     if(gap.chr == '(')
       result.append('~');
     tilde_check = 0;
    }  
   }
   if(gap.chr.isNull())
     result.append(gap.str);
   else
     result.append(gap.chr);
  }
  gap.reset();
 }

 if(run_length > 0)
 {
  // // have to break a run ...
  switch(run_code)
  {
  case 1: result.append(')'); break;
  case 2: result.append('}'); break;
  case 3: result.append(']'); break;
  case 4: result.append('>'); break;
  default: break;
  }
 }

}

void HTXN_Document_8b::read_glyph_point(Glyph_Argument_Package& gap, 
  u4 index, Glyph_Vector_8b& gv)
{
// gv.check_exterenal(index, *current_deck_, gap);
// if(gap.flags.

}


u4 HTXN_Document_8b::add_detail_range_region(Glyph_Layer_8b* layer, u4 enter, u4 leave,
  HTXN_Node_Detail::Wrap_Mode_Indicator_Codes wmic, u2 whitespace_code)
{
 u4 result = 0;
 HTXN_Node_Detail* nd = this->HTXN_Node_Details::add_detail_range(enter, leave, result);
 nd->set_layer(layer);
 nd->flags.region = true;
 nd->incorporate_wrap_mode_indicator(wmic);
 nd->note_whitespace_code(whitespace_code);
 return result;
}

u4 HTXN_Document_8b::add_detail_range(Glyph_Layer_8b* layer, u4 enter, u4 leave,
  HTXN_Node_Detail::Wrap_Mode_Indicator_Codes wmic, u2 whitespace_code)
{
 u4 result = 0;
 HTXN_Node_Detail* nd = this->HTXN_Node_Details::add_detail_range(enter, leave, result);
 nd->set_layer(layer);
 nd->incorporate_wrap_mode_indicator(wmic);
 nd->note_whitespace_code(whitespace_code);
 return result;
}

u4 HTXN_Document_8b::add_detail_range_optional(Glyph_Layer_8b* layer, u4 enter, u4 leave,
  HTXN_Node_Detail::Wrap_Mode_Indicator_Codes wmic, u2 whitespace_code)
{
 u4 result = 0;
 HTXN_Node_Detail* nd = this->HTXN_Node_Details::add_detail_range(enter, leave, result);
 nd->set_layer(layer);
 nd->flags.optional = true;
 nd->incorporate_wrap_mode_indicator(wmic);
 nd->note_whitespace_code(whitespace_code);
 return result;
}

Glyph_Layer_8b* HTXN_Document_8b::add_layer()
{
 Glyph_Layer_8b* result = new Glyph_Layer_8b(size() + 1);
 push_back(result);
 return result;
}

Glyph_Layer_8b* HTXN_Document_8b::read_layer(QString text, u2 offset)
{
 Glyph_Layer_8b* result = add_layer();
 read_layer(result, text, offset);
 return result;
}

void HTXN_Document_8b::read_layer(Glyph_Layer_8b* gl, QString text, u2 offset)
{
 current_glyph_vector_ = gl;
 encode_latin1(text.toLatin1(), *gl, offset);
}

u2 HTXN_Document_8b::get_diacritic_cue_code(char cue)
{
 return (u2) current_diacritic_deck_->get_cue_code(cue);
}


u2 HTXN_Document_8b::get_diacritic_code(char cue, u1 scope)
{
 return (u2) current_diacritic_deck_->get_code(cue, scope);
}

u2 HTXN_Document_8b::get_diacritic_code_inh(u1 pos, u1 length)
{
 return (u2) 
   current_diacritic_deck_->get_diacritic_code_inh(pos, length);
}

u1 HTXN_Document_8b::get_diacritic_length_or_code(char cue, u2& code)
{
 // // non-zero return means the cue character is a length 
  //   indicator.  If the result is also non-zero 
  //   the character signals both length and cue.
 QPair<u1, u1> pr = current_diacritic_deck_->get_length(cue);
 if(pr.first > 0)
 {
  code = pr.second;
  return pr.first;
 }
 code = current_diacritic_deck_->get_code(cue, 1);
 return 0;
}

void HTXN_Document_8b::read_htxne_in(QIODevice& qiod)
{
 QByteArray scratch_qba; 
 while(!qiod.atEnd())
 {
  scratch_qba = qiod.peek(1);
  if(scratch_qba[0] == '#')
  {
   qiod.readLine();
   scratch_qba.clear();
   continue;
  }
  else if(scratch_qba[0] == '@')
  {
   qiod.readLine();
   scratch_qba.clear();
   break;
  }

  Glyph_Layer_8b* result = new Glyph_Layer_8b(size());
  push_back(result);
  current_glyph_vector_ = result;
  u4 last_index = 0;
  QByteArray src;
  Glyph_Vector_8b offset_acc;
  encode_latin1(src, *result, 0, last_index, 
    &offset_acc, &qiod);
  (*current_glyph_vector_)[last_index] = Standard_GlyphDeck_8b::Boundary;
  current_glyph_vector_->resize(last_index + 1);
 }

 QByteArray* ranges_qba = new QByteArray;
 QByteArray* details_qba = nullptr;
 QByteArray* ties_qba = nullptr;

 QByteArray* current = ranges_qba;
 while(!qiod.atEnd())
 {
  scratch_qba = qiod.readLine();
  if(scratch_qba.startsWith('#'))
    continue;
  if(scratch_qba.startsWith(" *"))
    continue;

  if(scratch_qba.startsWith('@'))
  {
   QTextStream rqts(*ranges_qba);
   rqts >> (QVector<Glyph_Layer_8b*>&)*this;
   delete ranges_qba;
   ranges_qba = nullptr;
   details_qba = new QByteArray;
   current = details_qba;
  }
  else if(scratch_qba.startsWith('%'))
  {
   QTextStream dqts(*details_qba);
   dqts >> node_details_;
   delete details_qba;
   details_qba = nullptr; 
   ties_qba = new QByteArray;
   current = ties_qba;
  }
  else
    current->append(scratch_qba);
 }

 QTextStream tqts(*ties_qba); 
 read_ties(tqts, (const QVector<Glyph_Layer_8b*>&)*this);
 delete ties_qba;
 ties_qba = nullptr;

// QTextStream tqts(ties_qba);
// tqts >> 
}


void HTXN_Document_8b::encode_latin1(QByteArray& src, 
  Glyph_Vector_8b& target, u4 index, u4& last_index, 
  Glyph_Vector_8b* offset_acc,  
  QIODevice* qiod, u4 buffer_length) //, u4 layer_size_estimate)
{
 u4 boundary_count = (offset_acc)? 3 : 1; 

 u4 reread_index, oneread_index, index_landmark;
 s8 seek_landmark;
 if(qiod)
 {
  seek_landmark = qiod->pos();
  if(buffer_length == 0)
    buffer_length = DEFAULT_HTXN_READ_BUFFER_LENGTH;
  src = qiod->read(buffer_length);
  reread_index = src.length();
  oneread_index = 0;
  index_landmark = index;
 }
 else
 {
  reread_index = 0;
  oneread_index = src.length();
  index_landmark = 0;
  seek_landmark = 0;
 }
// if(layer_size_estimate == 0)
//    layer_size_estimate = src.length();

 u4 l_index = 0;
 u4 total_read = 0;

 static QMap<QPair<char, u1>, quint8> static_47 {

   { { '\n', 0 }, 127 },

   { { ' ', 0 }, 63 },

   { { '.', 0 }, 64 },
   { { '.', 1 }, Standard_GlyphDeck_8b::NpDot },
   { { '.', 2 }, Standard_GlyphDeck_8b::NsPer },

   { { '!', 0 }, 65 },
   { { '!', 1 }, Standard_GlyphDeck_8b::NpExcX },

   { { '(', 0 }, 66 },
   { { '(', 1 }, Standard_GlyphDeck_8b::NpOParX },

   { { ')', 0 }, 67 },
   { { '(', 1 }, Standard_GlyphDeck_8b::NpCParX },

   { { '-', 0 }, 68 },
   { { '-', 1 }, Standard_GlyphDeck_8b::DashSML },
   { { '-', 2 }, Standard_GlyphDeck_8b::SnDash },
   { { '-', 3 }, Standard_GlyphDeck_8b::NpMinus },
   { { '-', 4 }, Standard_GlyphDeck_8b::DashX },

   { { ',', 0 }, 69 },
   { { ',', 1 }, Standard_GlyphDeck_8b::NmComX },

   { { '\'', 0 }, 70 },
   { { '\'', 1 }, Standard_GlyphDeck_8b::SqSqX },

   { { '#', 0 }, 77 },
   { { '#', 1 }, Standard_GlyphDeck_8b::TxtNumX },

   { { '$', 0 }, 78 },
   { { '$', 1 }, Standard_GlyphDeck_8b::TxtDolX },

   { { '%', 0 }, 79 },
   { { '%', 1 }, Standard_GlyphDeck_8b::TxtPerX },

   { { '&', 0 }, 80 },
   { { '&', 1 }, Standard_GlyphDeck_8b::TxtAmpX },

   { { '"', 0 }, Standard_GlyphDeck_8b::SqDqX },
   { { '"', 1 }, Standard_GlyphDeck_8b::SqDqX },

   { { '*', 0 }, Standard_GlyphDeck_8b::TxtStarX },
   { { '*', 1 }, Standard_GlyphDeck_8b::TxtStarX },

   { { '+', 0 }, 75 },
   { { '+', 1 }, Standard_GlyphDeck_8b::TxtPlusX },

   { { '/', 0 }, 86 },
   { { '/', 1 }, Standard_GlyphDeck_8b::FslX },
   { { '/', 2 }, Standard_GlyphDeck_8b::SpaceX },
   { { '/', 3 }, Standard_GlyphDeck_8b::Boundary },
   { { '/', 4 }, Standard_GlyphDeck_8b::Null },
 };


 static QMap<QPair<char, u1>, quint8> static_64 {
   { { ':', 0 }, 71 },
   { { ':', 1 }, Standard_GlyphDeck_8b::NpColX },

   { { ';', 0 }, 72 },
   { { ';', 1 }, Standard_GlyphDeck_8b::NpSemiX },

   { { '?', 0 }, 73 },
   { { '?', 1 }, Standard_GlyphDeck_8b::NpQmX },
//?   { { '?', 1 }, Standard_GlyphDeck_8b::IndQm },

   { { '<', 0 }, Standard_GlyphDeck_8b::NpLtX },
   { { '<', 1 }, Standard_GlyphDeck_8b::NpLtX },

   { { '>', 0 }, Standard_GlyphDeck_8b::NpGtX },
   { { '>', 1 }, Standard_GlyphDeck_8b::NpGtX },

   { { '=', 0 }, Standard_GlyphDeck_8b::TxtEqX },
   { { '=', 1 }, Standard_GlyphDeck_8b::TxtEqX },

   { { '@', 0 }, Standard_GlyphDeck_8b::TxtAtX },
   { { '@', 1 }, Standard_GlyphDeck_8b::TxtAtX },

 };

 static QMap<QPair<char, u1>, quint8> static_96 {
   { { '[', 0 }, 84 },
   { { '[', 1 }, Standard_GlyphDeck_8b::OSqBrX },

   { { ']', 0 }, 85 },
   { { ']', 1 }, Standard_GlyphDeck_8b::CSqBrX },

   { { '\\', 0 }, 87 },
   { { '\\', 1 }, Standard_GlyphDeck_8b::BslX },

   { { '^', 0 }, Standard_GlyphDeck_8b::TxtHatX },
   { { '^', 1 }, Standard_GlyphDeck_8b::TxtHatX },

   { { '_', 0 }, 36 },
   //?{ { '_', 1 }, Standard_GlyphDeck_8b::NullX },

   { { '`', 0 }, Standard_GlyphDeck_8b::BqX },
   { { '`', 1 }, Standard_GlyphDeck_8b::BqX },
 };

 static QMap<QPair<char, u1>, quint8> static_127 {
   { { '{', 0 }, Standard_GlyphDeck_8b::OCyBrX },
   { { '{', 1 }, Standard_GlyphDeck_8b::OCyBrX },

   { { '|', 0 }, Standard_GlyphDeck_8b::PipeX },
   { { '|', 1 }, Standard_GlyphDeck_8b::PipeX },

   { { '}', 0 }, Standard_GlyphDeck_8b::CCyBrX },
   { { '}', 1 }, Standard_GlyphDeck_8b::CCyBrX },

   { { '~', 0 }, Standard_GlyphDeck_8b::TildeX },
   { { '~', 0 }, Standard_GlyphDeck_8b::TildeX },
 };

 

 u1 held_state = 0;

 u2 length_with_held_state = 0;
 u2 diacritic_code = 0;

 QString flags_acc;

 QMap<u4, QString> flags_strings;

 // //  held state codes: 
  //    0 -- nothing
  //    1-4 -- alt interpretation 1-4
  //    10 -- read alt interpretation
  //    11 -- done alt interpretation; maybe flags
  //    12 -- read flags
  //    13 -- flags acc
  //    20 -- diacritic start
  //    21 -- diacritic scope +
  //    22-4 -- diacritic scope 2-4
  //    201 -- 0 of 1
  //    202 -- 0 of 2
  //    203 -- 0 of 3
  //    212 -- 1 of 2
  //    213 -- 1 of 3
  //    223 -- 2 of 3

 while(reread_index || oneread_index)
 {
  if(reread_index)
  {
   // // need to make sure target has enough space
   if(target.size() < index + reread_index)
    target.resize(index + reread_index);
  }
  for(char chr : src)
  {
   ++total_read;

   quint8 code = 0;

   if(held_state == 11)
   {
    if(chr == '~')
    {
     // //  we'll keep length_with_held_state
     //    for future reference
     held_state = 12;
     continue;
    }
    length_with_held_state = 0;
    held_state = 0;
   }
   else if(held_state == 12)
   {
    if(chr == '(')
    {
     // //  Again, we'll keep length_with_held_state
     //    for future reference
     held_state = 13;
     continue;
    }
    length_with_held_state = 0;
    held_state = 0;
   }
   else if(held_state == 13)
   {
    if(chr == ')')
    {
     // // store the acc;
     for(u2 i = 0; i < length_with_held_state; ++i)
     {
      flags_strings[index - i] = flags_acc;
     }
     flags_acc.clear();
     length_with_held_state = 0;
     held_state = 0;
    }
    else
     flags_acc.append(QChar::fromLatin1(chr));
    continue;
   }
   else if(chr == '|')
   {
    if(held_state == 20)
     held_state = 0; // // || escape; no diacritic
    // //  inside the offset_acc | is just a normal character
    else if( (held_state == 0) && !(boundary_count > 1) )
    {
     held_state = 20;
     continue;
    }
   }

   if(held_state >= 20)
   {
    // //  part of a diacritic ...

    //    20 -- diacritic start
    //    22-4 -- diacritic scope 2-4
    //    201 -- 0 of 1
    //    202 -- 0 of 2
    //    203 -- 0 of 3
    //    212 -- 1 of 2
    //    213 -- 1 of 3
    //    223 -- 2 of 3

    switch(held_state)
    {
    case 20:
    {
     u2 dcode;
     u1 length = get_diacritic_length_or_code(chr, dcode);
     if(length > 0)
     {
      if(code > 0)
      {
       diacritic_code = dcode;
       held_state = 210 + length;
      }
      else
      {
       held_state = 20 + length;
      }
     }
     else
     {
      diacritic_code = dcode;
      held_state = 201;
     }
    }
     continue; // overall loop
    case 22:
     diacritic_code = get_diacritic_code(chr, 2);
     held_state = 202;
     continue; // overall loop
    case 23:
     diacritic_code = get_diacritic_code(chr, 3);
     held_state = 203;
     continue; // overall loop
    case 24:
     // //  to be determined ...
     exit(0);
     break;

    default:
     code = get_diacritic_cue_code(chr);
     code |= 64; // set dia bit ...
     break;
    }
   }
   else if(chr < 48)
   {
    if( (held_state == 10) && (chr == '(') )
    {
     held_state = 1;
     continue;
    }
    if( (held_state == 1) && (chr == ')')
      && (length_with_held_state > 0) )
    {
     held_state = 11;
     continue;
    }

    code = static_47.value( {chr, held_state} );
    if(code == 0)
     continue;
   }
   else if(chr < 58)
   {
    // // (48 .. 58):  a digit
    code = chr - 48;
   }
   else if(chr < 65)
   {
    if( (held_state == 10) && (chr == '<') )
    {
     held_state = 4;
     continue;
    }
    if( (held_state == 4) &&
        (chr == '>') && (length_with_held_state > 0) )
    {
     held_state = 11;
     continue;
    }
    code = static_64.value( {chr, held_state} );
    if(code == 0)
     continue;
   }
   else if(chr < 91)
   {
    // //   (A .. Z)
    code = chr - 28;
   }
   else if(chr < 97)
   {
    if(chr == '`')
    {
     held_state = 10;
     continue;
    }
    if( (held_state == 10) && (chr == '[') )
    {
     held_state = 3;
     continue;
    }
    if( (held_state == 3) && (chr == ']')
        && (length_with_held_state > 0) )
    {
     if(l_index > 0)
     {
      // // this means that we've seen a boundary ...
      last_index = l_index;
      s8 pos = seek_landmark + total_read + 2;
      qiod->seek(pos);
      return;
     }
     else
     {
      held_state = 11;
      continue;
     }
    }
    code = static_96.value( {chr, held_state} );
    if(code == 0)
     continue;
   }
   else if(chr < 123)
   {
    // //   (a .. z)
    code = chr - 87;
   }
   else if(chr == '{')
   {
    if(held_state == 10)
    {
     held_state = 2;
     continue;
    }
    code = static_127.value( {chr, held_state} );
    if(code == 0)
     continue;
   }
   else if(chr == '}')
   {
    if( (held_state == 2) && (length_with_held_state > 0) )
    {
     held_state = 11;
     continue;
    }
    code = static_127.value( {chr, held_state} );
    if(code == 0)
     continue;
   }
   else
   {
    code = static_127.value( {chr, held_state} );
    if(code == 0)
     continue;
   }

   // // got code ...

   if(held_state >= 20)
   {
    // // we need to mark the diacritic
    //   also set the diacritic_code
    //   anticipating next char in 2 or 3 length
    switch(held_state)
    {
    case 201:
     mark_diacritic_code(target, index, diacritic_code);
     diacritic_code = 0;
     held_state = 0;
     break;
    case 202:
     mark_diacritic_code(target, index, diacritic_code);
     diacritic_code = get_diacritic_code_inh(2, 2);
     held_state = 212;
     break;
    case 212:
     mark_diacritic_code(target, index, diacritic_code);
     diacritic_code = 0;
     held_state = 0;
     break;
    case 203:
     mark_diacritic_code(target, index, diacritic_code);
     diacritic_code = get_diacritic_code_inh(2, 3);
     held_state = 213;
     break;
    case 213:
     mark_diacritic_code(target, index, diacritic_code);
     diacritic_code = get_diacritic_code_inh(3, 3);
     held_state = 223;
     break;
    case 223:
     mark_diacritic_code(target, index, diacritic_code);
     diacritic_code = 0;
     held_state = 0;
     break;
    default: break;
    }
   }
   else if(held_state != 0)
    ++length_with_held_state;

   //current_deck_->encode()
   if(code == Standard_GlyphDeck_8b::Boundary)
   {
    --boundary_count;
    if(boundary_count == 0)
    {
     // // return after closing ')' ...
     l_index = index;
     continue;
    }
   }
   else if(boundary_count > 1)
   {
    offset_acc->push_back(code);
    continue;
   }
   
   target[index] = code;
   ++index;
  }

  if(qiod)
  {
   // // keep track of this index for rewind ...
   index_landmark = index;
   seek_landmark = qiod->pos();
   total_read = 0;

   src = qiod->read(buffer_length);
   reread_index = src.length();
  }
  else
  {
   oneread_index = 0;
  }
  // end while
 }
 last_index = index;
}

void HTXN_Document_8b::mark_diacritic_code(Glyph_Vector_8b& target, u4 index, u2 diacritic_code)
{
 u2 deck_code = dia_id_by_deck_[current_diacritic_deck_];
 target.add_diacritic(index, deck_code, diacritic_code);
}


void HTXN_Document_8b::encode_latin1(const QByteArray& src, 
  Glyph_Vector_8b& target, u2 offset)
{
 target.resize(src.size() + offset + 3);

 target[0] = Standard_GlyphDeck_8b::Boundary;

 u4 index = 1;
 while(index < offset + 1)
 {
  target[index] = Standard_GlyphDeck_8b::Space;
  ++index;
 } 
 target[index] = Standard_GlyphDeck_8b::Boundary;
 ++index;

 u4 last_index = index;

 // // when called from here src will not be changed ...
 encode_latin1(const_cast<QByteArray&>(src), target, 
   index, last_index);
 target[last_index] = Standard_GlyphDeck_8b::Boundary;
 target.resize(last_index + 1);
}

