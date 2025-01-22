
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


// //  This is a list of relevant types.
//    The DOMINION_TYPE has different
//    definitions depending on where
//    this file is included.


#undef DOMINION_OUTER_NAMESPACE
#undef DOMINION_INNER_NAMESPACE

#define DOMINION_NODE_TYPE NGML_Node

#include "relae-graph/dominion-macros.h"
DOMINION_TYPE(string, QString, String)

#define DOMINION_OUTER_NAMESPACE RZ
#define DOMINION_INNER_NAMESPACE NGML
#include "relae-graph/dominion-macros.h"

DOMINION_TYPE(ngml_root, NGML_Root, NGML_Root)
DOMINION_TYPE(ngml_token, NGML_Token, NGML_Token)
DOMINION_TYPE(ngml_tile, NGML_Tile, NGML_Tile)
DOMINION_TYPE(ngml_annotation_tile, NGML_Annotation_Tile, NGML_Annotation_Tile)
DOMINION_TYPE(ngml_tag_command, NGML_Tag_Command, NGML_Tag_Command)
DOMINION_TYPE(ngml_attribute_tile, NGML_Attribute_Tile, NGML_Attribute_Tile)
DOMINION_TYPE(ngml_paralex_tile, NGML_Paralex_Tile, NGML_Paralex_Tile)
