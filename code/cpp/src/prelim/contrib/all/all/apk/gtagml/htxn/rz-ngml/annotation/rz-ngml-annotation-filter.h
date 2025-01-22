
#ifndef RZ_NGML_ANNOTATION_FILTER__H
#define RZ_NGML_ANNOTATION_FILTER__H

#include "accessors.h"
#include "flags.h"


#include <QString>
#include <QStringList>

#include <functional>

#include "rzns.h"
RZNS_(NGML)


class NGML_Annotation_Filter
{
 QStringList positive_keys_;
 QStringList negative_keys_;

public:

 NGML_Annotation_Filter(QString raw_text);

 void parse(QString raw_text);
 static void parse(QString raw_text, QStringList& target);

 bool check(QString& key);
};

_RZNS(NGML)

#endif
