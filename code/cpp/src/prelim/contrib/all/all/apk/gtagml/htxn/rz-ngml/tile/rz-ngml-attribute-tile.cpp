
#include "rz-ngml-attribute-tile.h"

#include "rzns.h"

USING_RZNS(NGML)


NGML_Attribute_Tile::NGML_Attribute_Tile(QString key, QString value)
 : key_(key), value_(value)
{

}


QString NGML_Attribute_Tile::thumbnail()
{
 return key_ + "=...";
}


