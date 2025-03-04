#include "textio.h"

struct Page_Ref_Pair;



struct Page_Ref {
 ushort number;
 uint note;
 uint secondary_note;
 QString roman;

 QString to_code() const;

 static Page_Ref_Pair from(QString qs);
 static Page_Ref _from(QString qs);


 bool is_valid()
 {
  return number || !roman.isEmpty();
 }
};

struct Page_Ref_Pair : QPair<Page_Ref, Page_Ref> {

 static Page_Ref_Pair default_values()
 {
  Page_Ref_Pair result;
  result.first = {0, 0, 0};
  result.second = {0, 0, 0};
  return result;
 }

 static Page_Ref_Pair from_vector(const QVector<Page_Ref>& vec)
 {
  Page_Ref_Pair result;
  result.first = vec.value(0);
  result.second = vec.value(1);
  return result;
 }

 QVector<Page_Ref> to_vector()
 {
  if(first.is_valid())
  {
   if(second.is_valid())
     return {first, second};
   return {first};
  }
  return {};
 }

 QString to_code()
 {
  if(first.is_valid())
  {
   if(second.is_valid())
     return "^^%1--%2"_qt.arg(first.to_code()).arg(second.to_code());
   return "^%1"_qt.arg(first.to_code());
  }
  return "??";
 }

};



struct Index_Entry {

 Index_Entry* parent;
 int count_in_parent;
 int id;
 QString key;
 QVector<Page_Ref_Pair> refs;
 QString carried;
 int sub_count;
 QString sub_carried;
 QString supplement;

 static void add_redirect(QString text, QString supplement, QVector<Index_Entry>& entries);

};

void write_index_entries(const QVector<Index_Entry>& index_entries, QTextStream& qts);

void parse_ref(QString text, Page_Ref& ref);

void parse_ref_pair(QString text, Page_Ref_Pair& pr);

void read_index_entry(QString text, Index_Entry& ie);

void read_index_entries(QString text, QVector<Index_Entry>& ies);
