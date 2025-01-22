
#include "chasm-rz-block-entry.h"

#include "rzns.h"


USING_RZNS(RZ_Core)

ChasmRZ_Block_Entry::ChasmRZ_Block_Entry(int block_id)
 : Flags(0), block_id_(block_id), nested_depth_(0),
   lexical_scope_(nullptr)
{

}

