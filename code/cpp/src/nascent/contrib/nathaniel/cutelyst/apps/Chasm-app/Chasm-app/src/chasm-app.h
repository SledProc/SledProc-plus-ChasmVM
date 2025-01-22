
#ifndef CHASM_APP__H
#define CHASM_APP__H


         #define _including_from_app_
#include <Cutelyst/Application>
         #undef _including_from_app_


using namespace Cutelyst;


namespace_class(Cutelyst ,GrantleeView)
namespace_class(Chasm::TSI ,Chasm_View)


#include "Cutelyst/tsi/chasm-macros.h"
USING_CHASM_NAMESPACE


class Chasm_app : public Application
{
 Q_OBJECT
 CUTELYST_APPLICATION(IID "Chasm_app")

 Chasm_View* chasm_view_;
 GrantleeView* grantlee_view_;


public:


 Q_INVOKABLE explicit Chasm_app(QObject *parent = nullptr);
 ~Chasm_app();

 bool init();
};

#endif //CHASM_APP__H

