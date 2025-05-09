#include <iostream>
#include <utility>

#include "../include/type_calculator.h"

void and_or_test()
{
  using namespace mpl;
  auto _and_type1_type2_ = type<int> && type<long>;
  auto _or_type1_type2_ = type<float> || type<short>;
  auto _and_and_or_ = _and_type1_type2_ && _or_type1_type2_;
  static_assert(pred<std::is_integral>(_and_type1_type2_) ==
    (std::is_integral_v<int> && std::is_integral_v<long>) && (std::is_integral_v<float> || std::is_integral_v<short>)
    , "not equal");
}

int main()
{
  and_or_test();
}