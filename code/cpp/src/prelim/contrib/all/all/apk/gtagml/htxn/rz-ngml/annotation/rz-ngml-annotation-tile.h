
#ifndef RZ_NGML_ANNOTATION_TILE__H
#define RZ_NGML_ANNOTATION_TILE__H

#include "accessors.h"
#include "flags.h"


#include <QString>

#include <functional>

#include "rzns.h"
RZNS_(NGML)


class NGML_Annotation_Tile
{
 QString connector_;
 QString tile_;
 QString subject_;

 int starting_line_number_;
 int ending_line_number_;

public:

 ACCESSORS(QString ,connector)
 ACCESSORS(QString ,tile)
 ACCESSORS(QString ,subject)

 ACCESSORS(int ,starting_line_number)
 ACCESSORS(int ,ending_line_number)

 NGML_Annotation_Tile();

 void parse(QString str);

 QString thumbnail(int length = 4, int ellipsis = 3) const;

};

_RZNS(NGML)

#endif
