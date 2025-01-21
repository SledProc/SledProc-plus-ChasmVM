
//           Copyright Nathaniel Christen 2019.
//  Distributed under the Boost Software License, Version 1.0.
//     (See accompanying file LICENSE_1_0.txt or copy at
//           http://www.boost.org/LICENSE_1_0.txt)

#ifndef ACCESSORS__H
#define ACCESSORS__H

#include <functional>

#ifndef MACRO_PASTE
#define MACRO_PASTE(...) __VA_ARGS__
#endif


#ifndef ACCESSORS__DECLARATIVE__SET
#define ACCESSORS__DECLARATIVE__SET(cl, type, name) \
 cl& name(const type& _val) { name##_ = _val; return *this; }
#endif


#ifndef ACCESSORS__DECLARATIVE
#define ACCESSORS__DECLARATIVE(cl, type, name) \
ACCESSORS__DECLARATIVE__SET(cl, type, name) \
ACCESSORS__GET(type, name)
#endif


#ifndef ACCESSORS__GET_2
#define ACCESSORS__GET_2(type, name) \
 type name() const { return name##_; }
#endif

#ifndef ACCESSORS__IMPL_GET
#define ACCESSORS__IMPL_GET(type, name) \
 type name() const { return impl_.name(); }
#endif

#ifndef ACCESSORS__IMPL_SET
#define ACCESSORS__IMPL_SET(type, name) \
 void set_##name(type t) { impl_.set_##name(t); }
#endif


#ifndef ACCESSORS__IMPL
#define ACCESSORS__IMPL(type, name) \
 ACCESSORS__IMPL_GET(type, name) \
 ACCESSORS__IMPL_SET(type, name)
#endif


#ifndef ACCESSORS_CALL_IMPL
#define ACCESSORS_CALL_IMPL(type, name) \
 type name() const { return impl_.name(); }
#endif

#ifndef ACCESSORS_CALL_IMPL_VOID
#define ACCESSORS_CALL_IMPL_VOID(type, name) \
 void name(type arg) { impl_.name(arg); }
#endif


#ifndef ACCESSORS__CGET
#define ACCESSORS__CGET(type, name) \
 const type name() const { return name##_; }
#endif


#ifndef ACCESSORS__STATIC_GET
#define ACCESSORS__STATIC_GET(type, name) \
 static type name() { return name##_; }
#endif


#ifndef ACCESSORS__RGET_2
#define ACCESSORS__RGET_2(type, name) \
 type& name() { return name##_; }
#endif


#ifndef ACCESSORS__RGET_CONST
#define ACCESSORS__RGET_CONST(type, name) \
 type& name() const { return (type&) name##_; }
#endif

#ifndef ACCESSORS__CONST_RGET
#define ACCESSORS__CONST_RGET(type, name) \
 const type& name() const { return name##_; }
#endif

#ifndef ACCESSORS__CONST_RGET_NONCONST
#define ACCESSORS__CONST_RGET_NONCONST(type, name) \
 const type& name() { return name##_; }
#endif

#ifndef ACCESSORS__NONCONST_RGET_CONST
#define ACCESSORS__NONCONST_RGET_CONST(type, name) \
 type& name() const { return (type&) name##_; }
#endif

#ifndef ACCESSORS__RGET_DEREF
#define ACCESSORS__RGET_DEREF(type, name) \
 type& name() { return *name##_; }
#endif


#ifndef ACCESSORS__STATIC_RGET
#define ACCESSORS__STATIC_RGET(type, name) \
 static type& name() { return name##_; }
#endif


#ifndef ACCESSORS__SET_2
#define ACCESSORS__SET_2(type, name) \
 void set_##name(type _arg_) { name##_ = _arg_; }
#endif


#ifndef ACCESSORS__SET_FNP
#define ACCESSORS__SET_FNP(ret, type, name) \
 void set_##name(ret (*_arg_) (type)) { name##_ = _arg_; }
#endif

#ifndef ACCESSORS__GET_FNP
#define ACCESSORS__GET_FNP(ret, type, name) \
 ret (*get_##name())(type) { return name##_; }
#endif


#ifndef ACCESSORS__FNP
#define ACCESSORS__FNP(ret, type, name) \
 ACCESSORS__SET_FNP(ret, type, name) \
 ACCESSORS__GET_FNP(ret, type, name)
#endif



#ifndef ACCESSORS__SET_FNP__VOID
#define ACCESSORS__SET_FNP__VOID(type, name) \
 ACCESSORS__SET_FNP(void, type, name)
#endif

#ifndef ACCESSORS__GET_FNP__VOID
#define ACCESSORS__GET_FNP__VOID(type, name) \
 ACCESSORS__GET_FNP(void, type, name)
#endif


#ifndef ACCESSORS__FNP__VOID
#define ACCESSORS__FNP__VOID(type, name) \
 ACCESSORS__SET_FNP__VOID(type, name) \
 ACCESSORS__GET_FNP__VOID(type, name)
#endif




#ifndef ACCESSORS__SET_FNP__STATIC
#define ACCESSORS__SET_FNP__STATIC(ret, type, name) \
 static void set_##name(ret (*_arg_) (type)) { name##_ = _arg_; }
#endif

#ifndef ACCESSORS__GET_FNP__STATIC
#define ACCESSORS__GET_FNP__STATIC(ret, type, name) \
 static ret (*get_##name())(type) { return name##_; }
#endif

#ifndef ACCESSORS__FNP__STATIC
#define ACCESSORS__FNP__STATIC(ret, type, name) \
 ACCESSORS__SET_FNP__STATIC(ret, type, name) \
 ACCESSORS__GET_FNP__STATIC(ret, type, name)
#endif

#ifndef ACCESSORS__SETR
#define ACCESSORS__SETR(type, name) \
 auto& set_##name(type _arg_) { name##_ = _arg_; return *this; }
#endif

#ifndef ACCESSORS__SETP
#define ACCESSORS__SETP(type, name) \
 auto set_##name(type _arg_) { name##_ = _arg_; return this; }
#endif

#ifndef Q_INVOKABLE__ACCESSORS__SET
#define Q_INVOKABLE__ACCESSORS__SET(type, name) \
 void Q_INVOKABLE set_##name(type _arg_) { name##_ = _arg_; }
#endif

#ifndef ACCESSORS__SDECLARE
#define ACCESSORS__SDECLARE(type, name) \
 void set_##name(type name);
#endif

#ifndef ACCESSORS__SET__IMPL
#define ACCESSORS__SET__IMPL(type, name) \
 void set_##name(type name)
#endif


#ifndef ACCESSORS__GDECLARE
#define ACCESSORS__GDECLARE(type, name) \
 type name() const;
#endif


#ifndef ACCESSORS__DECLARE
#define ACCESSORS__DECLARE(type, name) \
 ACCESSORS__GDECLARE(type, name) \
 ACCESSORS__SDECLARE(type, name)
#endif


#ifndef ACCESSORS__GSDECLARE
#define ACCESSORS__GSDECLARE(type, name) \
 type get_##name() const; \
 ACCESSORS__SDECLARE(type, name)
#endif


#ifndef ACCESSORS_2
#define ACCESSORS_2(type, name) \
 ACCESSORS__GET_2(MACRO_PASTE(type), name) \
 ACCESSORS__SET_2(MACRO_PASTE(type), name)
#endif


#ifndef ACCESSORSR
#define ACCESSORSR(type, name) \
 ACCESSORS__GET(MACRO_PASTE(type), name) \
 ACCESSORS__SETR(MACRO_PASTE(type), name)
#endif


#ifndef ACCESSORSP
#define ACCESSORSP(type, name) \
 ACCESSORS__GET(MACRO_PASTE(type), name) \
 ACCESSORS__SETP(MACRO_PASTE(type), name)
#endif


// // this all implements the set_fn() << ... syntax
template<typename OBJECT_Type, typename FN_Type>
struct fn_setter
{
 OBJECT_Type& _this;
 void(*setter)(OBJECT_Type&, FN_Type);
 void operator << (FN_Type fn)
 {
  setter(_this, fn);
 }
};


#ifndef FN_VIA_OP
#define FN_VIA_OP(typ ,x) \
  set_##x##_{*this, &typ::static_set_##x}
#endif


#ifndef ACCESSORS_FN_SETTER_VIA_OP_DECLARE
#define ACCESSORS_FN_SETTER_VIA_OP_DECLARE(typ, ret_args, fname) \
 private: static void static_set_##fname(typ& _this, std::function<ret_args> fn) \
 { \
  _this.set_##fname(fn); \
 } \
 fn_setter<typ, std::function<ret_args>> set_##fname##_; public:
#endif


#ifndef ACCESSORS_FN_SET
#define ACCESSORS_FN_SET(ret_args ,x) \
  ACCESSORS__SET(std::function<ret_args> ,x)
#endif



#ifndef ACCESSORS_FN_SET_VIA_OP
#define ACCESSORS_FN_SET_VIA_OP(typ, ret_args ,x) \
 ACCESSORS_FN_SET(ret_args ,x) \
 ACCESSORS_FN_SETTER_VIA_OP_DECLARE(typ, ret_args ,x) \
 fn_setter<typ, std::function<ret_args>> set_##x() { return set_##x##_; }
#endif



#ifndef ACCESSORS_FN_VIA_OP
#define ACCESSORS_FN_VIA_OP(typ, ret_args ,x) \
  ACCESSORS__GET(std::function<ret_args> ,x) \
  ACCESSORS_FN_SET_VIA_OP(typ, ret_args ,x)
#endif



#ifndef Q_INVOKABLE__ACCESSORS
#define Q_INVOKABLE__ACCESSORS(type, name) \
 ACCESSORS__GET(type, name) \
 Q_INVOKABLE__ACCESSORS__SET(type, name)
#endif


#ifndef ACCESSORS__SET__CONST
#define ACCESSORS__SET__CONST(type, name) \
 void set_##name(type _arg_) const { name##_ = _arg_; }
#endif

#ifndef ACCESSORS__CONST
#define ACCESSORS__CONST(type, name) \
 ACCESSORS__GET(type, name) \
 ACCESSORS__SET__CONST(type, name)
#endif


#ifndef ACCESSORS__DO
#define ACCESSORS__DO(type, name) \
 void name##__do(std::function<void(type)> fn) const { fn(name()); }
#endif


#ifndef ACCESSORS__GDECLARE__DO
#define ACCESSORS__GDECLARE__DO(type, name) \
  ACCESSORS__GDECLARE(type, name) \
  ACCESSORS__DO(type, name)
#endif


#ifndef ACCESSORS__DECLARE__DO
#define ACCESSORS__DECLARE__DO(type, name) \
  ACCESSORS__DECLARE(type, name) \
  ACCESSORS__DO(type, name)
#endif

// // now setup macros for various lengths of arguments -- up to 9 for now ...



#ifndef ACCESSORS_3
#define ACCESSORS_3(arg1, arg2, arg3) \
ACCESSORS_2(MACRO_PASTE(arg1, arg2) ,arg3)
#endif

#ifndef ACCESSORS__GET_3
#define ACCESSORS__GET_3(arg1, arg2, arg3) \
ACCESSORS__GET_2(MACRO_PASTE(arg1, arg2) ,arg3)
#endif

#ifndef ACCESSORS__SET_3
#define ACCESSORS__SET_3(arg1, arg2, arg3) \
ACCESSORS__SET_2(MACRO_PASTE(arg1, arg2) ,arg3)
#endif


#ifndef ACCESSORS__RGET_3
#define ACCESSORS__RGET_3(arg1, arg2, arg3) \
ACCESSORS__RGET_2(MACRO_PASTE(arg1, arg2) ,arg3)
#endif


#ifndef ACCESSORS__RGET_4
#define ACCESSORS__RGET_4(arg1, arg2, arg3, arg4) \
ACCESSORS__RGET_2(MACRO_PASTE(arg1, arg2, arg3) ,arg4)
#endif



#include "preproc-concat.h"

#ifndef ACCESSORS
#define ACCESSORS(...) _preproc_CONCAT(ACCESSORS_, _preproc_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#endif

#ifndef ACCESSORS__GET
#define ACCESSORS__GET(...) _preproc_CONCAT(ACCESSORS__GET_, _preproc_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#endif

#ifndef ACCESSORS__SET
#define ACCESSORS__SET(...) _preproc_CONCAT(ACCESSORS__SET_, _preproc_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#endif

#ifndef ACCESSORS__RGET
#define ACCESSORS__RGET(...) _preproc_CONCAT(ACCESSORS__RGET_, _preproc_NUM_ARGS(__VA_ARGS__))(__VA_ARGS__)
#endif



//#ifndef SET_STR_ADAPTER_
//#define SET_STR_ADAPTER_(fn, from_) void set_##fn(QString arg) { set_##fn(arg.to##to_()); }
//#endif


#ifndef SET_ADAPTER_
#define SET_ADAPTER_(fn, to_) void set_##fn(QString arg) { set_##fn(arg.to##to_()); }
#endif

#ifndef SET_ADAPTER_DBL
#define SET_ADAPTER_DBL(fn) SET_ADAPTER_(fn, Double)
#endif

#ifndef SET_ADAPTER_INT
#define SET_ADAPTER_INT(fn) SET_ADAPTER_(fn, Int)
#endif


#ifndef SET_ADAPTER_N8
#define SET_ADAPTER_N8(fn) SET_ADAPTER_(fn, LongLong)
#endif


#ifndef STR_ADAPTER
#define STR_ADAPTER(fn) QString str_##fn() const { return  QString::number(fn()); }
#endif


#ifndef SET_and_STR_ADAPTER_DBL
#define SET_and_STR_ADAPTER_DBL(fn) SET_ADAPTER_(fn, Double) STR_ADAPTER(fn)
#endif

#ifndef SET_and_STR_ADAPTER_INT
#define SET_and_STR_ADAPTER_INT(fn) SET_ADAPTER_(fn, Int) STR_ADAPTER(fn)
#endif

#ifndef SET_and_STR_ADAPTER_N8
#define SET_and_STR_ADAPTER_N8(fn) SET_ADAPTER_(fn, LongLong) STR_ADAPTER(fn)
#endif




#ifndef SET_ADAPTER_neg1_
#define SET_ADAPTER_neg1_(fn, to_) \
  void set_##fn(QString arg, u1 col = -1) { set_##fn(arg.to##to_()); }
#endif

#ifndef SET_ADAPTER_DBL_neg1
#define SET_ADAPTER_DBL_neg1(fn) SET_ADAPTER_neg1_(fn, Double)
#endif

#ifndef SET_ADAPTER_INT_neg1
#define SET_ADAPTER_INT_neg1(fn) SET_ADAPTER_neg1_(fn, Int)
#endif


#ifndef SET_ADAPTER_N8_neg1
#define SET_ADAPTER_N8_neg1(fn) SET_ADAPTER_neg1_(fn, LongLong)
#endif


#ifndef STR_ADAPTER_neg1
#define STR_ADAPTER_neg1(fn) QString str_##fn(u1 col = -1) const { return QString::number(fn(col)); }
#endif


#ifndef SET_and_STR_ADAPTER_DBL_neg1
#define SET_and_STR_ADAPTER_DBL_neg1(fn) SET_ADAPTER_neg1(fn, Double) STR_ADAPTER(fn)
#endif

#ifndef SET_and_STR_ADAPTER_INT_neg1
#define SET_and_STR_ADAPTER_INT_neg1(fn) SET_ADAPTER_neg1(fn, Int) STR_ADAPTER(fn)
#endif

#ifndef SET_and_STR_ADAPTER_N8_neg1
#define SET_and_STR_ADAPTER_N8_neg1(fn) SET_ADAPTER_neg1(fn, LongLong) STR_ADAPTER(fn)
#endif



#endif //ACCESSORS__H
