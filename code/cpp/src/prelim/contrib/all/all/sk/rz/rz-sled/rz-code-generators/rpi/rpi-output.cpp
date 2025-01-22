
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#include "rpi-output.h"
#include "rzns.h"

#include "rpi-block.h"

USING_RZNS(GVal)

RPI_Output::RPI_Output(RZ_Graph_Visitor_Phaon& visitor_phaon)
  :  visitor_phaon_(visitor_phaon), top_level_block_(nullptr)
{
}

void RPI_Output::init_function_def_syntax()
{
}

void RPI_Output::init_top_level_block(PGB_IR_Build& pgb)
{
 top_level_block_ = new RPI_Block(pgb);
}


void RPI_Output::build_phaon_graph(PGB_IR_Build& pgb)
{
 init_top_level_block(pgb);


 top_level_block_->scan_top_level(visitor_phaon_);

// QString qs;
// QTextStream qts(&qs);
 top_level_block_->write_top_level(step_forms_, nullptr);

 top_level_block_->build_phaon_graph();

}

void RPI_Output::write(QTextStream* qts)
{
 if(top_level_block_)
 {
  top_level_block_->scan_top_level(visitor_phaon_);
  top_level_block_->write(step_forms_, qts);
 }
}

