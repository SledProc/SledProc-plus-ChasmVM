
#include "rz-ngml-output-base.h"

#include "kernel/document/rz-ngml-document.h"

#include <QFile>
#include <QFileInfo>

#include "rzns.h"
USING_RZNS(NGML)


NGML_Output_Base::NGML_Output_Base(NGML_Document& document)
 : document_(document), graph_(*document.graph()), qry_(NGML_Query::instance())
{

}

