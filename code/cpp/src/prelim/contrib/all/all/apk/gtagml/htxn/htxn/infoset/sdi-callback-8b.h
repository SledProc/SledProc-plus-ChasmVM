
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef SDI_CALLBACK_8B__H
#define SDI_CALLBACK_8B__H

#include "kans.h"

#include "global-types.h"

KANS_CLASS_DECLARE(HTXN, Glyph_Layer_8b)
KANS_CLASS_DECLARE(HTXN, Glyph_Argument_Package)

USING_KANS(HTXN)

class SDI_Callback_8b
{
public:
 SDI_Callback_8b();
 virtual void pre_write(Glyph_Layer_8b& gl, u4 pos,
   Glyph_Argument_Package& gap, n8& skip_flag,
   QString& pre_insert, QString& post_insert);
// virtual void marked_sentence_start(Glyph_Layer_8b& gl, u4 pos);
// virtual void marked_non_pivot_sentence_end(Glyph_Layer_8b& gl, u4 pos);
};




#endif // SDI_CALLBACK_8B__H
