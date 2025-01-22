
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef DGH_SDI_PARAGRAPH__H
#define DGH_SDI_PARAGRAPH__H

#include "kans.h"
#include "accessors.h"

#include <QVector>
#include <QString>

#include "global-types.h"

#include "ntxh-parser/ntxh-document.h"


class NGML_SDI_Paragraph;

//KANS_CLASS_DECLARE(HGDMCore ,NTXH_Graph)
//USING_KANS(HGDMCore)


KANS_(DGH)

class DGH_SDI_Sentence;


class DGH_SDI_Paragraph
{
 u4 in_database_id_;

 u4 in_file_id_;
 u4 file_id_;

 u2 page_;
 u2 order_in_page_;

// u4 start_;
// u4 end_;
// DGH_SDI_Sentence* first_sentence_;
// DGH_SDI_Sentence* last_sentence_;

 NGML_SDI_Paragraph* ngml_;


public:

 DGH_SDI_Paragraph(u4 id);

 DGH_SDI_Paragraph(NGML_SDI_Paragraph* ngml);

 DGH_SDI_Paragraph(u4 file_id, u4 in_file_id);


 ACCESSORS(u4 ,in_database_id)
 ACCESSORS(u4 ,file_id)
 ACCESSORS(u4 ,in_file_id)
 ACCESSORS(u2 ,page)
 ACCESSORS(u2 ,order_in_page)

// ACCESSORS(u4 ,start)
// ACCESSORS(u4 ,end)
// ACCESSORS(DGH_SDI_Sentence* ,first_sentence)
// ACCESSORS(DGH_SDI_Sentence* ,last_sentence)

 ACCESSORS(NGML_SDI_Paragraph* ,ngml)

 QString get_summary();

};


_KANS(DGH)

#endif // DGH_SDI_PARAGRAPH__H
