
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "chasm-rz-root.h"

#include "kernel/document/chasm-rz-document.h"

#include "rzns.h"

USING_RZNS(RZ_Core)


ChasmRZ_Root::ChasmRZ_Root(ChasmRZ_Document* document)
  : document_(document)
{

}

QString ChasmRZ_Root::document_path()
{
 return document_->local_path();
}
