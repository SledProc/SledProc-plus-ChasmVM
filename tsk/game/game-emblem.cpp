
//           Copyright Nathaniel Christen 2020.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)




#include "game-emblem.h"


Game_Emblem::Game_Emblem(u2 code)
  :  details_(code)
{

}

Game_Emblem& Game_Emblem::green()
{
 details_ |= (u2) Game_Emblem::Color::Green;
 return *this;
}

Game_Emblem& Game_Emblem::orange()
{
 details_ |= (u2) Game_Emblem::Color::Orange;
 return *this;
}

Game_Emblem& Game_Emblem::purple()
{
 details_ |= (u2) Game_Emblem::Color::Purple;
 return *this;
}

Game_Emblem& Game_Emblem::closed()
{
 details_ |= (u2) Game_Emblem::Open_or_Closed::Closed;
 return *this;
}

Game_Emblem& Game_Emblem::open()
{
 details_ |= (u2) Game_Emblem::Open_or_Closed::Open;
 return *this;
}

Game_Emblem& Game_Emblem::s()
{
 details_ |= (u2) Game_Emblem::Range::Short;
 return *this;
}

Game_Emblem& Game_Emblem::m()
{
 details_ |= (u2) Game_Emblem::Range::Medium;
 return *this;
}

Game_Emblem& Game_Emblem::l()
{
 details_ |= (u2) Game_Emblem::Range::Long;
 return *this;
}

Game_Emblem& Game_Emblem::canon()
{
 details_ |= (u2) Game_Emblem::Special::Canon;
 return *this;
}

Game_Emblem& Game_Emblem::knight()
{
 details_ |= (u2) Game_Emblem::Special::Knight;
 return *this;
}

Game_Emblem& Game_Emblem::alt()
{
 details_ |= (u2) Game_Emblem::Special::Alternating_Mirror;
 return *this;
}






