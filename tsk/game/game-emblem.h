
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef GAME_EMBLEM__H
#define GAME_EMBLEM__H

#include "accessors.h"

#include "flags.h"

#include "global-types.h"

#include <QString>
#include <QQueue>


class Game_Emblem
{
 enum class Color : u2 {
   N_A = 0, Orange = 1,
    Purple = 2, Green = 4
 };

 enum class Range : u2 {
   N_A = 0, Short = 16,
    Medium = 32, Long = 64
 };

 enum class Open_or_Closed : u2 {
   N_A = 0, Open = 256, Closed = 512
 };

 enum class Special : u2 {
   N_A = 0, Canon = 1024, Knight = 2048,
   Alternating_Mirror = 4096,
 };

 enum class Alternating_Status : u2 {
   N_A = 0, Canon = 1024, Knight = 2048,
   Mirror = 4096,
 };

 enum class Alternating_State : u2 {
   N_A = 0, Canon_1 = 8192, Knight_2_plus = 16384
 };

 u2 details_;


public:

 Game_Emblem(u2 code = 0);

 Game_Emblem& green();
 Game_Emblem& orange();
 Game_Emblem& purple();

 Game_Emblem& closed();
 Game_Emblem& open();

 Game_Emblem& s();
 Game_Emblem& m();
 Game_Emblem& l();

 Game_Emblem& canon();
 Game_Emblem& knight();
 Game_Emblem& alt();



};



#endif // GAME_EMBLEM__H
