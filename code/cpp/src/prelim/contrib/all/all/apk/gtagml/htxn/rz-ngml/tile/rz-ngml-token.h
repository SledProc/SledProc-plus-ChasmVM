
#ifndef RZ_NGML_TOKEN__H
#define RZ_NGML_TOKEN__H

#include "rzns.h"
#include "accessors.h"

#include "flags.h"

#include <QString>


RZNS_(NGML)

class NGML_Token
{
public:

 flags_(1)
  bool is_string_literal:1;
  bool is_keyword:1;
 _flags

private:

 QString raw_text_;

public:

 ACCESSORS(QString ,raw_text)

 NGML_Token(QString raw_text);

 QString to_string();

 QString thumbnail(int max_characters = 6);

};

_RZNS(NGML)

#endif
