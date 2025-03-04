
#include "m2m.h"

#include <QRegularExpression>
#include <QRegularExpressionMatch>


QString Page_Ref::to_code() const
{
 if(roman.isEmpty())
 {
  if(secondary_note)
    return "@%1/%2;%3"_qt.arg(number).arg(note).arg(secondary_note);
  if(note)
    return "@%1/%2"_qt.arg(number).arg(note);
  return "%%1"_qt.arg(number);
 }
 return "$%1"_qt.arg(roman);
}


Page_Ref Page_Ref::_from(QString qs)
{
 ushort number = 0;
 uint note = 0, secondary_note = 0;
 QString roman;

 QRegularExpression qre ("^(\\d+)n(\\d+)");
 QRegularExpressionMatch match = qre.match(qs);
 if(match.hasMatch())
 {
  number = match.captured(1).toInt();
  QString n = match.captured(2);
  note = match.captured(2).toInt();
 }
 else
 {
  QRegularExpression qre1 ("^[xvi]+$");
  QRegularExpressionMatch match1 = qre1.match(qs);
  if(match1.hasMatch())
    roman = qs;
  else
    number = qs.toInt();
 }

 if(note > 1000)
 {
  QString n = QString::number(note);
  int ix = n.lastIndexOf("000");
  if(ix != -1)
  {
   note = n.mid(0, ix).toInt();
   secondary_note = n.mid(ix + 3).toInt();
  }
 }

 return {number, note, secondary_note, roman};
}

Page_Ref_Pair Page_Ref::from(QString qs)
{
 QVector<Page_Ref> result;

 QStringList qsl = qs.split("--");

 for(QString qs : qsl)
 {
  Page_Ref pr = Page_Ref::_from(qs.simplified());
  if(pr.is_valid())
    result.push_back(pr);
 }
 return Page_Ref_Pair::from_vector(result);
}


void Index_Entry::add_redirect(QString text, QString ksupplement, QVector<Index_Entry>& entries)
{
 if(text.isEmpty())
   text = ksupplement;
 else
   text += " " + ksupplement;

 int ix = text.indexOf(".");
 if(ix != -1)
 {
  QString key = text.mid(0, ix);
  QString carried = text.mid(ix + 1).simplified();

  entries.push_back({nullptr, 0, 0, key, {}, carried, 0});
 }
 else
 {
  entries.push_back({nullptr, 0, 0, text, {}, {}, 0});
 }
}

void write_index_entries(const QVector<Index_Entry>& index_entries, QTextStream& qts)
{
 for(const Index_Entry& ie : index_entries)
 {
  qts << "\n ";

  if(ie.parent)
    qts << "[[  \"" << ie.key << "\"";
  else
    qts << "[[ \"" << ie.key << "\"";

  qts << " &" << ie.id;

  if(ie.parent)
    qts << " *" << ie.count_in_parent;

  if(ie.sub_count)
    qts << " #" << ie.sub_count;

  if(!ie.carried.isEmpty())
    qts << " +{" << ie.carried << "}";

  if(!ie.sub_carried.isEmpty())
    qts << " -{" << ie.sub_carried << "}";

  qts <<  " => ";// << ie.refs.count();

  int count = 0;
  for(Page_Ref_Pair pr: ie.refs)
  {
   if(count > 0)
     qts << " ->";
   ++count;
   qts << " " << pr.to_code();
  }

  qts << "\n ]] \n (( \n" << ie.supplement << "\n )) \n";

 }
}

void parse_ref(QString text, Page_Ref& ref)
{
 if(text.startsWith("%"))
   ref.number = text.mid(1).toInt();
 else if(text.startsWith("@"))
 {
  QRegularExpression qre("(\\d+)/(\\d+)(?:;(\\d+))?");
  QRegularExpressionMatch match = qre.match(text);
  if(match.hasMatch())
  {
   ref.number = match.captured(1).toInt();
   ref.note = match.captured(2).toInt();

   if(!match.captured(3).isEmpty())
     ref.secondary_note = match.captured(3).toInt();
  }
 }
 else if(text.startsWith("$"))
 {
  ref.roman = text.mid(1);
 }
}

void parse_ref_pair(QString text, Page_Ref_Pair& pr)
{
 if(text.startsWith("^^"))
 {
  text = text.mid(2);
  QStringList qsl = text.split("--");
  parse_ref(qsl.value(0), pr.first);
  parse_ref(qsl.value(1), pr.second);
  qDebug() << "CAON_DEBUG_NOOP";
 }
 else if(text.startsWith("^"))
 {
  parse_ref(text.mid(1), pr.first);
  qDebug() << "CAON_DEBUG_NOOP";
 }

}

void read_index_entry(QString text, Index_Entry& ie)
{
 int qspos = text.indexOf("\"") + 1;
 int qepos = text.indexOf("\"", qspos);

 QString key = text.mid(qspos, qepos - qspos);

 text = text.mid(qepos + 1).simplified();

 // count_in_parent, sub_count, carried, sub_carried

 QString id_token;
 QRegularExpression id_token_regex("^\\s*&(\\d+)");
 QRegularExpressionMatch id_token_regex_match = id_token_regex.match(text);
 if(id_token_regex_match.hasMatch())
 {
  id_token = id_token_regex_match.captured(1);
  text = text.mid(id_token_regex_match.capturedEnd());
 }

 QString count_in_parent_token;
 QRegularExpression count_in_parent_token_regex("^\\s*\\*(\\d+)");
 QRegularExpressionMatch count_in_parent_token_regex_match = count_in_parent_token_regex.match(text);
 if(count_in_parent_token_regex_match.hasMatch())
 {
  count_in_parent_token = count_in_parent_token_regex_match.captured(1);
  text = text.mid(count_in_parent_token_regex_match.capturedEnd());
 }

 QString sub_count_token;
 QRegularExpression sub_count_token_regex("^\\s*\\#(\\d+)");
 QRegularExpressionMatch sub_count_token_regex_match = sub_count_token_regex.match(text);
 if(sub_count_token_regex_match.hasMatch())
 {
  sub_count_token = sub_count_token_regex_match.captured(1);
  text = text.mid(sub_count_token_regex_match.capturedEnd());
 }



 QString carried;
 QRegularExpression carried_regex("^\\s*\\+\\{([^}]*)\\}");
 QRegularExpressionMatch carried_regex_match = carried_regex.match(text);
 if(carried_regex_match.hasMatch())
 {
  carried = carried_regex_match.captured(1);
  text = text.mid(carried_regex_match.capturedEnd());
 }

 QString sub_carried;
 QRegularExpression sub_carried_regex("^\\s*-\\{([^}]*)\\}");
 QRegularExpressionMatch sub_carried_regex_match = sub_carried_regex.match(text);
 if(sub_carried_regex_match.hasMatch())
 {
  sub_carried = sub_carried_regex_match.captured(1);
  text = text.mid(sub_carried_regex_match.capturedEnd());
 }

 ie.carried = carried;
 ie.sub_carried = sub_carried;
 ie.key = key;
 ie.count_in_parent = count_in_parent_token.toInt();
 ie.sub_count = sub_count_token.toInt();
 ie.id = id_token.toInt();

 if(ie.key.contains("Childhelp"))
   qDebug() << "CAON_DEBUG_NOOP";


 QRegularExpression follow_regex("^\\s*=>\\s*");
 QRegularExpressionMatch follow_regex_match = follow_regex.match(text);
 if(follow_regex_match.hasMatch())
 {
  text = text.mid(follow_regex_match.capturedEnd());
 }
 else
   return;

 text = text.simplified();
 if(text.isEmpty())
    return;

 QStringList qsl = text.split(" -> ");

 for(QString ref : qsl)
 {
  Page_Ref_Pair pr = Page_Ref_Pair::default_values();// {{0, 0, QString{}}, {0, 0, QString{}}};
  parse_ref_pair(ref, pr);
  ie.refs.push_back(pr);
 }

// if(text.startsWith(QRegularExpression))


 qDebug() << "CAON_DEBUG_NOOP";

}


void read_index_entries(QString text, QVector<Index_Entry>& ies)
{
 int spos = 0, epos = 0;
 int sspos = 0, sepos = 0;

 Index_Entry* current_parent = nullptr;

 while(true)
 {
  spos = text.indexOf("[[", epos);
  if(spos == -1)
    break;

  epos = text.indexOf("]]", spos);
  if(epos == -1)
    break;

  sspos = text.indexOf("((", epos);
  if(spos == -1)
    break;

  sepos = text.indexOf("))", sspos);
  if(sepos == -1)
    break;

  Index_Entry ie;
  ie.supplement =  text.mid(sspos + 2, sepos - sspos - 2).simplified();


  read_index_entry(text.mid(spos + 2, epos - spos - 2).simplified(), ie);
  ies.push_back(ie);

  if(ie.sub_count)
    current_parent = &ies.back();
  else if(ie.count_in_parent)
    ies.back().parent = current_parent;
  else
    ies.back().parent = nullptr;
 }
}
