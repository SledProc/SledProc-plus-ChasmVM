
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#ifndef CHASM_RZ_SOURCE_FILE__H
#define CHASM_RZ_SOURCE_FILE__H

#include "accessors.h"
#include "flags.h"


#include <QString>
#include <QTextStream>
#include <QMap>

#include <functional>

#include "rzns.h"


RZNS_(RZ_Core)


class ChasmRZ_Source_File
{
 QString local_path_;


public:

 ChasmRZ_Source_File(QString local_path);


};

_RZNS(RZ_Core)

#endif //CHASM_RZ_SOURCE_FILE__H
