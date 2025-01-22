
#include "rz-ngml-tile.h"

#include "rzns.h"

USING_RZNS(NGML)


NGML_Tile::NGML_Tile(QString raw_text)
  :  raw_text_(raw_text), 
  ngml_htxn_node_(nullptr), length_adjustment_(0)
{

}

u4 NGML_Tile::effective_length()
{
 return raw_text_.size() - length_adjustment_;
}

QString NGML_Tile::thumbnail(int max_characters)
{
 QString result;
 if(raw_text_.size() > max_characters)
 {
  result += raw_text_.left(max_characters - 3) + "...";
 }
 else
 {
  result += raw_text_;
 }
 return result;
}

QString NGML_Tile::to_string()
{
 QString result = raw_text_;
 return result;
}


