#include <iostream>
#include <utility>

#include "../include/type_calculator.h"

void requirement_test()
{
  using namespace mpl;
  // 代表int要么是指针，但必须是平凡的，即int must be trivial or int is pointer
  auto req1 = type<int> | either<std::is_pointer> | must<std::is_trivial>;
  // 此处期待true_type
  static_assert(std::is_same_v<decltype(req1)::type, __true_type__<int>>);

  // 可交换验证
  auto req2 = type<int> | must<std::is_trivial> | either<std::is_pointer>;
  // 此处期待true_type
  static_assert(std::is_same_v<decltype(req2)::type, __true_type__<int>>);

  // 有一个either为真则表达式恒真
  struct A {};
  auto req3 = type<A> | either<std::is_pointer> | either<std::is_class> | must<std::is_integral>;
  // 此处期待always_true_type
  static_assert(std::is_same_v<decltype(req3)::type, __always_true_type__<A>>);

  // 可交换验证，验证either为0或非0时在末尾时的情况
  // 有一个either为真则表达式恒真
  auto req4 = type<A> | either<std::is_class> | must<std::is_integral> | either<std::is_pointer>;
  auto req5 = type<A> | must<std::is_integral> | either<std::is_pointer> | either<std::is_class>;
  // 此处期待always_true_type
  static_assert(std::is_same_v<decltype(req4)::type, __always_true_type__<A>>);
  static_assert(std::is_same_v<decltype(req5)::type, __always_true_type__<A>>);

  // 此处A必须满足抽象类，或者常量，或者指针
  auto req6 = type<A> | either<std::is_pointer> | either<std::is_const> | must<std::is_abstract>;
  // 此处期待false_type
  static_assert(std::is_same_v<decltype(req6)::type, __false_type__<A>>);

  // 可交换验证，不将must放在尾部
  auto req7 = type<A> | either<std::is_pointer> | must<std::is_abstract> | either<std::is_const>;
  // 此处期待false_type
  static_assert(std::is_same_v<decltype(req7)::type, __false_type__<A>>);
}

// todo: 添加type_requirement对type_t、__and__和__or__的重载运算符和pred的测试

void and_or_test()
{
  using namespace mpl;
  auto _and_type1_type2_ = type<int> && type<long>;
  auto _or_type1_type2_ = type<float> || type<short>;
  auto _and_and_or_ = _and_type1_type2_ && _or_type1_type2_;
  std::cout << pred<std::is_integral>(_and_and_or_) << std::endl;
  static_assert(pred<std::is_integral>(_and_and_or_) ==
    (std::is_integral_v<int> && std::is_integral_v<long>) && (std::is_integral_v<float> || std::is_integral_v<short>)
    , "not equal");
}

int main()
{
  and_or_test();
  requirement_test();
}