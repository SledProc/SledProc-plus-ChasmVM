#ifndef color_hsv_trait_bound_uint
#define color_hsv_trait_bound_uint

#include "../../category.hpp"

#include "../../../_internal/utility/bound/general.hpp"

#include "../../../generic/trait/bound.hpp"



namespace color
 {
  namespace trait
   {

    template< >
     struct bound< ::color::category::hsv_uint8  >
      : public ::color::_internal::utility::bound::general< std::uint8_t, typename ::color::trait::index< ::color::category::hsv_uint8 >::instance_type >
      {
      };

    template< >
     struct bound< ::color::category::hsv_uint16 >
      : public ::color::_internal::utility::bound::general< std::uint16_t, typename ::color::trait::index< ::color::category::hsv_uint16 >::instance_type >
      {
      };

    template< >
     struct bound< ::color::category::hsv_uint32 >
      : public ::color::_internal::utility::bound::general< std::uint32_t, typename ::color::trait::index< ::color::category::hsv_uint32 >::instance_type >
      {
      };

    template< >
     struct bound< ::color::category::hsv_uint64 >
      : public ::color::_internal::utility::bound::general< std::uint64_t, typename ::color::trait::index< ::color::category::hsv_uint64 >::instance_type >
      {
      };


   }
 }

#endif
