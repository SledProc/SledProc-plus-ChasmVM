
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-document.h"

#include "relae-graph/relae-parser.templates.h"

#include "kernel/grammar/chasm-rz-parser.h"
#include "kernel/grammar/chasm-rz-grammar.h"
#include "kernel/grammar/chasm-rz-graph-build.h"

#include "kernel/chasm-rz-root.h"

#include "kernel/graph/chasm-rz-graph.h"

#include "textio.h"


#include "rzns.h"

#include <QFile>
#include <QFileInfo>
#include <QDir>

#include <QRegularExpression>

USING_RZNS(RZ_Core)
USING_KANS(TextIO)


ChasmRZ_Document::ChasmRZ_Document(QString path)
 : graph_(nullptr), grammar_(nullptr), number_of_lines_(0),
   nl_lexicon_(nullptr)
{
 if(!path.isEmpty())
  load_file(path);
}

void ChasmRZ_Document::finalize_raw_text()
{
 QMap<QPair<int, int>, QString> posmap;
 int index = 0;
 while( (index = raw_text_.indexOf("<#", index)) != -1 )
 {
  int end = raw_text_.indexOf(">", index);
  if(end == -1)
    break;
  QString fn = raw_text_.mid(index + 2, end - index - 2);
  if(fn.contains('\n'))
    break;
  posmap[{index, end - index + 1}] = fn;
  index = end + 1;
 }

 int diff = 0;
 QMapIterator<QPair<int, int>, QString> it(posmap);
 while(it.hasNext())
 {
  it.next();
  int index = it.key().first;
  int end = it.key().second;
  QString fn = it.value();
  if(fn.endsWith('#'))
  {
   fn.chop(1);
  }
  else if(!fn.contains('.'))
  {
   fn.append(".rz");
  }
  else
  {
   int i = fn.lastIndexOf('.');
   if(i < fn.size() - 1)
   {
    if(fn[i + 1] == '/')
    {
     fn.append(".rz");
    }
   }
  }

  if(!fn.contains('/'))
  {
   fn.prepend(local_directory_ + '/');
  }
  else if(fn.startsWith('.'))
  {
   fn.prepend(local_directory_ + '/');
  }

  QString nt;
  nt = ::load_file(fn);
  raw_text_.replace(index + diff, end, nt);
  diff += nt.size() - end;
 }

 if(posmap.isEmpty())
 {
  report_raw_text("..raw.txt", "_");
 }
 else
 {
  finalize_raw_text();
 }

 report_raw_text("..raw.txt", "_");
}

void ChasmRZ_Document::load_file(QString path)
{
 QFile file(path);
 if(file.open(QFile::ReadOnly | QIODevice::Text))
 {
  raw_text_ = file.readAll();
  local_path_ = path;
  QFileInfo qfi(local_path_);
  local_directory_ = qfi.absoluteDir().absolutePath();
  finalize_raw_text();
 }
}


void ChasmRZ_Document::resolve_report_path(QString& path)
{
 if(path.startsWith('.'))
 {
  path = path.mid(1);
  if(path.startsWith('.'))
  {
   path = local_path_ + path;
  }
  else
  {
  }
 }
}

void ChasmRZ_Document::report_raw_text(QString path, QString pre)
{
 resolve_report_path(path);
 int index = path.lastIndexOf('/');
 path.insert(index + 1, pre);
 ::save_file(path, raw_text_);
}

void ChasmRZ_Document::report_raw_text(QString path)
{
 resolve_report_path(path);
 ::save_file(path, raw_text_);
}

void ChasmRZ_Document::report_graph(QString path)
{
 resolve_report_path(path);
 QFile file(path);
 if(file.open(QFile::WriteOnly | QIODevice::Text))
 {
  QTextStream qts(&file);
  if(graph_)
   graph_->report(qts);
 }
}


void ChasmRZ_Document::set_grammar(ChasmRZ_Grammar* grammar)
{
 if(grammar)
  grammar_ = grammar;
 else
  grammar_ = new ChasmRZ_Grammar();
}

void ChasmRZ_Document::preprocess_raw_text()
{
 // // for \else ...
 raw_text_.replace("\\else", "\\t \\do");

 // // for n-to-s ...
 raw_text_.replace("\\$(", "(~n-to-s ");

 // // for \do ...
 raw_text_.replace("\\do", "do .() -> ");

 // // for \t ...
 raw_text_.replace("\\t", "\\(#?\\ 1)");
 // //?

 // // for \t ...
 raw_text_.replace("\\f", "\\(#?\\ 0)");

 // // for do blocks sharing signature ...
 {
  QRegularExpression rx("do(([^{;>]*\\s+)[@\\w]*->\\s+)\\{([^}]+)\\}");

  int pos = 0;
  while(true)
  {
   QRegularExpressionMatch rxm = rx.match(raw_text_, pos);
   if(rxm.hasMatch())
   {
    QString c1 = rxm.captured(1);

    QString c1a = rxm.captured(2);

    if(c1a.trimmed().isEmpty())
    {
     c1.prepend(" .() ");
    }

    QString c2 = rxm.captured(3);
    int pos1 = rxm.capturedStart(3);
    int pos2 = rxm.capturedEnd(3);
    QString replace = insert_block_map_signatures(c2, c1);
    raw_text_.replace(pos1, pos2 - pos1, replace);
    pos += replace.length();
    raw_text_.replace(rxm.capturedStart(1), rxm.capturedLength(1), " \n");
    pos -= rxm.capturedLength(1);
    pos += 2;
   }
   else
   {
    break;
   }
  }
 }

 QFile outf(local_path_ + ".pre");
 if(outf.open(QFile::WriteOnly | QIODevice::Text))
 {
  QTextStream out(&outf);
  out << raw_text_;
 }
}


QString ChasmRZ_Document::insert_block_map_signatures(QString original_source, QString sig)
{
 int sigl = sig.length();

 QList<int> positions;

 QRegularExpression rx("(-\\w+:)\\s+\\w");

 int pos = 0;
 while(true)
 {
  QRegularExpressionMatch rxm = rx.match(original_source, pos);
  if(rxm.hasMatch())
  {
   int ce1 = rxm.capturedEnd(1);
   positions.append(ce1);
   int ce = rxm.capturedEnd();
   pos += ce;
  }
  else
  {
   break;
  }
 }
 QString result = original_source;
 for(int i = 0; i < positions.length(); ++i)
 {
  int pos = positions[i] + i*sigl;
  result.replace(pos, 0, sig);
 }
 return result;
}

void ChasmRZ_Document::parse(int start_position, int end_position)
{
 preprocess_raw_text();

 graph_ = new ChasmRZ_Graph();
 parser_ = new ChasmRZ_Parser(graph_);
 parser_->set_raw_text(raw_text_);

 graph_build_ = new ChasmRZ_Graph_Build(this, *parser_, *graph_);
 graph_build_->init();

 grammar_ = new ChasmRZ_Grammar;

 grammar_->init(*parser_, *graph_, *graph_build_);

 grammar_->compile(*parser_, *graph_, raw_text_, start_position);
}

ChasmRZ_Document::~ChasmRZ_Document()
{

}

