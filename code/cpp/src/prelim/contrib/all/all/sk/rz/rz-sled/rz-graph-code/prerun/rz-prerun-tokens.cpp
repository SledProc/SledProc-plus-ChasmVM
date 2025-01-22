
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)


#include "rz-prerun-tokens.h"

#include "rz-graph-token/token/rz-asg-token.h"

#include "rz-graph-core/token/chasm-rz-token.h"

USING_RZNS(RZ_Core)
USING_RZNS(GBuild)


RZ_Prerun_Tokens::RZ_Prerun_Tokens(caon_ptr<ChasmRZ_Document> document)
 : ChasmRZ_ASG_Output(document)
{

}


void RZ_Prerun_Tokens::init_token(ChasmRZ_Token& token)
{
 RZ_ASG_Token::init_asg_token(token);
}

void RZ_Prerun_Tokens::report_token(QTextStream& qts,
 const ChasmRZ_Token& token)
{
 qts << QString("<<%1>>").arg(token.raw_text());
}


void RZ_Prerun_Tokens::report_tuple_info_entry(QTextStream& qts,
 const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr)
{

}

void RZ_Prerun_Tokens::report_tuple_info_leave(QTextStream& qts,
 const ChasmRZ_Tuple_Info& rti, caon_ptr<ChasmRZ_Call_Entry> rce_ptr)
{

}

void RZ_Prerun_Tokens::report_call_entry(QTextStream& qts,
 const ChasmRZ_Call_Entry& rce)
{

}

void RZ_Prerun_Tokens::report_call_leave(QTextStream& qts,
 const ChasmRZ_Call_Entry& rce)
{

}

