
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "dgh-sdi-sentence.h"

#include "ngml-sdi/ngml-sdi-sentence.h"



#include <QDebug>



USING_KANS(DGH)


DGH_SDI_Sentence::DGH_SDI_Sentence(u4 in_file_id, u4 file_id, u4 page)
  :  in_file_id_(in_file_id), file_id_(file_id), page_(page), ngml_(nullptr)
{

}

DGH_SDI_Sentence::DGH_SDI_Sentence(NGML_SDI_Sentence* ngml,
    u4 in_file_id, u4 file_id, u4 page)
  :  in_file_id_(in_file_id), file_id_(file_id), page_(page), ngml_(ngml)
{

}

QString DGH_SDI_Sentence::get_summary()
{
 if(ngml_)
   return QString("%1 = %2:%3,%4-%5,%6; %7-%8").arg(file_id_)
     .arg(ngml_->id())
     .arg(ngml_->start_x())
     .arg(ngml_->end_x())
     .arg(ngml_->start_y())
     .arg(ngml_->end_y())
     .arg(ngml_->start_index())
     .arg(ngml_->end_index())
   ;
 return {};
}

