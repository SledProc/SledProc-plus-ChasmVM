
#include "rz-ngml-token.h"

#include "rzns.h"
USING_RZNS(NGML)


NGML_Token::NGML_Token(QString raw_text)
 : Flags(0), raw_text_(raw_text)
{

}


QString NGML_Token::thumbnail(int max_characters)
{
 QString result;
 if(flags.is_keyword)
 {
  result += ':';
  --max_characters;
 }
 if(flags.is_string_literal)
 {
  result += '"';
  max_characters -= 2;
 }
 if(raw_text_.size() > max_characters)
 {
  result += raw_text_.left(max_characters - 3) + "...";
 }
 else
 {
  result += raw_text_;
 }
 if(flags.is_string_literal)
 {
  result += '"';
 }
 return result;
}

QString NGML_Token::to_string()
{
 QString result = raw_text_;
 if(flags.is_string_literal)
 {
  result.replace('"', "\\\"");
  result.prepend('"');
  result.append('"');
 }
 if(flags.is_keyword)
  result.prepend(':');
 return result;
}


