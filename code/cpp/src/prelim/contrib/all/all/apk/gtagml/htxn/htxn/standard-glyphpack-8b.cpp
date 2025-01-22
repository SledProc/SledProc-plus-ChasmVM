
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "standard-glyphpack-8b.h"

#include "glyph-argument-package.h"

USING_KANS(HTXN)

Standard_GlyphPack_8b::Standard_GlyphPack_8b(u1 code)
 :  code_(code)
{

}


std::pair<u1, u1> Standard_GlyphPack_8b::get_latex_cue()
{
 // // ?
 if( (code_ & 128) > 0 )
 {
  return {2, code_ & 127 };
 }
 if( (code_ & 64) > 0 )
 {
  u1 c = code_ & 63;
//  if(c == 0)
//    return {3, 0};
//  if(c == 1)
//    return {4, 0};
  return {1, c};
 }
 return {0, code_};
}

std::pair<u1, u1> Standard_GlyphPack_8b::get_xml_cue()
{
 if( (code_ & 128) > 0 )
 {
  return {2, code_ & 127 };
 }
 if( (code_ & 64) > 0 )
 {
  u1 c = code_ & 63;
  return {1, c};
 }
 return {0, code_};
}

u1 Standard_GlyphPack_8b::get_diacritic_code_cue()
{

}





