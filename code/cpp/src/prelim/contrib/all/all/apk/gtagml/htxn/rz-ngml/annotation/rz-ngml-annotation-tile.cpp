
#include "rz-ngml-annotation-tile.h"

#include "rzns.h"

#include <QRegExp>

USING_RZNS(NGML)

NGML_Annotation_Tile::NGML_Annotation_Tile()
{
}

QString NGML_Annotation_Tile::thumbnail(int length, int ellipsis) const
{
 QString result = subject_.trimmed().left(length);
 if(ellipsis > 0 && subject_.size() > length)
  result += QString(ellipsis, '.');
 return result;
}

void NGML_Annotation_Tile::parse(QString str)
{
 // //  Superceded by initial grammar rules?
}


