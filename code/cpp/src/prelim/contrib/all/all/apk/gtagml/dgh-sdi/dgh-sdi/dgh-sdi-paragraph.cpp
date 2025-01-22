
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "dgh-sdi-paragraph.h"

#include "ngml-sdi/ngml-sdi-paragraph.h"


#include <QDebug>



USING_KANS(DGH)


DGH_SDI_Paragraph::DGH_SDI_Paragraph(u4 id)
  :  in_database_id_(id), in_file_id_(0), file_id_(0),
     page_(0), order_in_page_(0), ngml_(nullptr)
{

}

DGH_SDI_Paragraph::DGH_SDI_Paragraph(NGML_SDI_Paragraph* ngml)
 :  in_database_id_(0), in_file_id_(0), file_id_(0),
    page_(0), order_in_page_(0), ngml_(ngml)
{

}


DGH_SDI_Paragraph::DGH_SDI_Paragraph(u4 file_id, u4 in_file_id)
  :  in_database_id_(0), in_file_id_(in_file_id), file_id_(file_id),
     page_(0), order_in_page_(0), ngml_(nullptr)
{

}


QString DGH_SDI_Paragraph::get_summary()
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
