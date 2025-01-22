
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef DGH_SDI_SENTENCE__H
#define DGH_SDI_SENTENCE__H

#include "kans.h"
#include "accessors.h"

#include <QVector>
#include <QString>

#include "global-types.h"


class NGML_SDI_Sentence;


KANS_(DGH)

class DGH_SDI_Paragraph;

class DGH_SDI_Sentence
{
 u4 in_file_id_;
 u4 file_id_;

 u4 page_;

 NGML_SDI_Sentence* ngml_;

public:

 DGH_SDI_Sentence(u4 in_file_id, u4 file_id, u4 page = 0);

 DGH_SDI_Sentence(NGML_SDI_Sentence* ngml, u4 in_file_id = 0, u4 file_id = 0, u4 page = 0);

 ACCESSORS(u4 ,in_file_id)
 ACCESSORS(u4 ,file_id)
 ACCESSORS(u4 ,page)
 ACCESSORS(NGML_SDI_Sentence* ,ngml)

 QString get_summary();

};


_KANS(DGH)

#endif // DGH_SDI_SENTENCE__H
