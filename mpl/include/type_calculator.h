//
// Created by Xymul on 2025/5/10.
//

#ifndef TYPE_CALCULATOR_H
#define TYPE_CALCULATOR_H
#include <type_traits>

namespace mpl {
  template<typename T1, typename T2>
  struct __and__;
  template<typename T1, typename T2>
  struct __or__;

  template<typename T>
    struct __always_true_type__
  {
    using type = T;
    static constexpr bool value = true;
  };

  // todo: 添加文档和注释
  template<typename T>
  struct __true_type__
  {
    using type = T;
    static constexpr bool value = true;
  };

  template<typename T>
  struct __false_type__
  {
    using type = T;
    static constexpr bool value = false;
  };

  template<template <typename> class P>
  struct __must;

  template<template <typename> class P>
  struct __either;

  template<template <typename> class P>
  __must<P> must;

  template<template <typename> class P>
  __either<P> either;

  template<typename T>
  struct type_t;

  template<template <typename> class P>
  struct __must
  {
  };

  template<template <typename> class P>
  struct __either
  {
  };

  template<typename T, class ...Rs>
  struct type_requirement;

  /**
   *
   * 存在either满足->always_true_type
   * 所有either不满足，所有must满足->true_type
   * 所有either不满足，存在must不满足->false_type
   */
  template<typename T, template <typename> class P, class ...Rs>
  struct type_requirement<T, __must<P>, Rs...> : type_requirement<T, Rs...>
  {
    using type = std::conditional_t<
      std::is_same_v<typename type_requirement<T, Rs...>::type, __always_true_type__<T>>,
      __always_true_type__<T>,
      std::conditional_t<P<T>::value && type_requirement<T, Rs...>::type::value, __true_type__<T>, __false_type__<T>>
      >;
  };

  template<typename T, template <typename> class P, class ...Rs>
  struct type_requirement<T, __either<P>, Rs...> : type_requirement<T, Rs...>
  {
    using type = std::conditional_t<
      std::is_same_v<typename type_requirement<T, Rs...>::type, __always_true_type__<T>>,
      __always_true_type__<T>,
      std::conditional_t<P<T>::value, __always_true_type__<T>, typename type_requirement<T, Rs...>::type>
      >;
    /*
     *
     * 对于either的情况，如果上一级为always_true，直接传递
     * 如果上一级不为always_true，则:
     *   \ | 1 | 0
     *  ----------
     *  0 | 1 | 0
     *  ----------
     *  1 | 1 | 1
     *  可以看出，这里如果为1，则就可以传递always_true，如果为0，则不改变上一级传下来的类型
     */
  };

  template<typename T, template <typename> class P>
  struct type_requirement<T, __must<P>>
  {
    using type = std::conditional_t<P<T>::value, __true_type__<T>, __false_type__<T>>;
  };

  template<typename T, template <typename> class P>
  struct type_requirement<T, __either<P>>
  {
    using type = std::conditional_t<P<T>::value, __always_true_type__<T>, __true_type__<T>>;
    // 由于过程中的either只传递类型，所以初始时刻的either只需要设置真即可，换而言之，either只决定表达式是否恒真
    // 而不决定表达式是否恒假，因此只需要设置true即可，这样将表达式的真假将由must决定
    // 而且，either的语义本来就表明是除了指定的限定条件以外的其他可能限定，即使这些其他可能限定都不满足，语句仍然是为true的
    // 换而言之，就是将最终的结果交由指定的限定条件决定
  };

  template<typename T, template <typename> class P, class ...Rs>
  constexpr type_requirement<T,  __must<P>, Rs...>
  operator|(type_requirement<T, Rs...>, __must<P>)
  { return {}; }

  template<typename T, template <typename> class P, class ...Rs>
  constexpr type_requirement<T,  __either<P>, Rs...>
  operator|(type_requirement<T, Rs...>, __either<P>)
  { return {}; }

  // todo: 添加更全面的重载函数

  template<typename T, typename U, class ...Rs>
  constexpr __and__<type_t<U>, type_t<typename type_requirement<T, Rs...>::type>>
  operator&&(type_requirement<T, Rs...>, type_t<U>)
  { return {}; }

  template<typename T, typename T1, typename T2, class ...Rs>
  constexpr __and__<__and__<T1, T2>, type_t<typename type_requirement<T, Rs...>::type>>
  operator&&(type_requirement<T, Rs...>, __and__<T1, T2>)
  { return {}; }

  template<typename T, typename T1, typename T2, class ...Rs>
  constexpr __and__<__or__<T1, T2>, type_t<typename type_requirement<T, Rs...>::type>>
  operator&&(type_requirement<T, Rs...>, __or__<T1, T2>)
  { return {}; }

  template<typename T, typename T1, class ...Rs, class ...Us>
  constexpr __and__<type_t<typename type_requirement<T1, Us...>::type>, type_t<typename type_requirement<T, Rs...>::type>>
  operator&&(type_requirement<T, Rs...>, type_requirement<T1, Us...>)
  { return {}; }

  template<typename T, typename U, class ...Rs>
  constexpr __or__<type_t<U>, type_t<typename type_requirement<T, Rs...>::type>>
  operator||(type_requirement<T, Rs...>, type_t<U>)
  { return {}; }

  template<typename T, typename T1, typename T2, class ...Rs>
  constexpr __or__<__and__<T1, T2>, type_t<typename type_requirement<T, Rs...>::type>>
  operator||(type_requirement<T, Rs...>, __and__<T1, T2>)
  { return {}; }

  template<typename T, typename T1, typename T2, class ...Rs>
  __or__<__or__<T1, T2>, type_t<typename type_requirement<T, Rs...>::type>>
  operator||(type_requirement<T, Rs...>, __or__<T1, T2>)
  { return {}; }

  template<typename T, typename T1, class ...Rs, class ...Us>
  constexpr __or__<type_t<typename type_requirement<T1, Us...>::type>, type_t<typename type_requirement<T, Rs...>::type>>
  operator||(type_requirement<T, Rs...>, type_requirement<T1, Us...>)
  { return {}; }

  template<typename T1, typename T2>
  struct __or__
  {
    using type1 = T1;
    using type2 = T2;

    template<typename T>
    constexpr __and__<type_t<T>, __or__<T1, T2>>
    operator&&(type_t<T>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __and__<__and__<U1, U2>, __or__<T1, T2>>
    operator&&(__and__<U1, U2>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __and__<__and__<type_t<U1>, type_t<U2>>, __or__<T1, T2>>
    operator&&(__and__<type_t<U1>, type_t<U2>>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __and__<__or__<U1, U2>, __or__<T1, T2>>
    operator&&(__or__<U1, U2>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __and__<__or__<type_t<U1>, type_t<U2>>, __or__<T1, T2>>
    operator&&(__or__<type_t<U1>, type_t<U2>>)
    { return {}; }

    template<typename T>
    constexpr __or__<type_t<T>, __or__<T1, T2>>
    operator||(type_t<T>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __or__<__or__<U1,U2>, __or__<T1, T2>>
    operator||(__or__<U1, U2>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __or__<__or__<type_t<U1>, type_t<U2>>, __or__<T1, T2>>
    operator||(__or__<type_t<U1>, type_t<U2>>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __or__<__and__<U1,U2>, __or__<T1, T2>>
    operator||(__and__<U1, U2>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __or__<__and__<type_t<U1>, type_t<U2>>, __or__<T1, T2>>
    operator||(__and__<type_t<U1>, type_t<U2>>)
    { return {}; }
  };

  template<typename T1, typename T2>
  struct __and__
  {
    using type1 = T1;
    using type2 = T2;

    template<typename T>
    constexpr __and__<type_t<T>, __and__<T1, T2>>
    operator&&(type_t<T>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __and__<__and__<U1, U2>, __and__<T1, T2>>
    operator&&(__and__<U1, U2>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __and__<__and__<type_t<U1>, type_t<U2>>, __and__<T1, T2>>
    operator&&(__and__<type_t<U1>, type_t<U2>>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __and__<__or__<U1, U2>, __and__<T1, T2>>
    operator&&(__or__<U1, U2>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __and__<__or__<type_t<U1>, type_t<U2>>, __and__<T1, T2>>
    operator&&(__or__<type_t<U1>, type_t<U2>>)
    { return {}; }

    template<typename T>
    constexpr __or__<type_t<T>, __and__<T1, T2>>
    operator||(type_t<T>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __or__<__or__<U1, U2>, __and__<T1, T2>>
    operator||(__or__<U1, U2>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __or__<__or__<type_t<U1>, type_t<U2>>, __and__<T1, T2>>
    operator||(__or__<type_t<U1>, type_t<U2>>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __or__<__and__<U1, U2>, __and__<T1, T2>>
    operator||(__and__<U1, U2>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __or__<__and__<type_t<U1>, type_t<U2>>, __and__<T1, T2>>
    operator||(__and__<type_t<U1>, type_t<U2>>)
    { return {}; }

  };

  template<typename T, template <typename> class Pred>
  struct __pred;

  template<typename U, template <typename> class Pred>
  struct __pred<type_t<U>, Pred>
  {
    static constexpr bool value = Pred<U>::value;
  };

  template<typename T, template <typename> class Pred>
  struct __pred<type_t<__true_type__<T>>, Pred>
  {
    static constexpr bool value = Pred<T>::value;
  };

  template<typename T, template <typename> class Pred>
  struct __pred<type_t<__false_type__<T>>, Pred>
  {
    static constexpr bool value = false;
  };

  template<typename T, template <typename> class Pred>
  struct __pred<type_t<__always_true_type__<T>>, Pred>
  {
    static constexpr bool value = true;
  };

  template<typename T1, typename T2, template <typename> class Pred>
  struct __pred<__and__<T1, T2>, Pred>
  {
    static constexpr bool value = __pred<T1, Pred>::value && __pred<T2, Pred>::value;
  };

  template<typename T1, typename T2, template <typename> class Pred>
  struct __pred<__and__<type_t<T1>, type_t<T2>>, Pred>
  {
    static constexpr bool value = __pred<type_t<T1>, Pred>::value && __pred<type_t<T2>, Pred>::value;
  };

  template<typename T1, typename T2, template <typename> class Pred>
  struct __pred<__or__<T1, T2>, Pred>
  {
    static constexpr bool value = __pred<T1, Pred>::value || __pred<T2, Pred>::value;
  };

  template<typename T1, typename T2, template <typename> class Pred>
  struct __pred<__or__<type_t<T1>, type_t<T2>>, Pred>
  {
    static constexpr bool value = __pred<type_t<T1>, Pred>::value || __pred<type_t<T2>, Pred>::value;
  };

  template<template<typename> class Pred, typename T>
  constexpr bool
  pred(type_t<T>)
  { return __pred<T, Pred>::value; }

  /**
   * if tparam T1 and tparam T2 are not type_t, __and__ or __or__, then the behaviour is undefined
   *
   */
  template<template<typename> class Pred, typename T1, typename T2>
  constexpr bool
  pred(__and__<T1, T2>)
  { return __pred<__and__<T1, T2>, Pred>::value; }

  /**
   * if tparam T1 and tparam T2 are not type_t, __and__ or __or__, then the behaviour is undefined
   *
   */
  template<template<typename> class Pred, typename T1, typename T2>
  constexpr bool
  pred(__or__<T1, T2>)
  { return __pred<__or__<T1, T2>, Pred>::value; }

  template<template<typename> class Pred, typename T, class... Rs>
  constexpr bool
  pred(type_requirement<T, Rs...>)
  { return __pred<type_t<typename type_requirement<T, Rs...>::type>, Pred>::value; }

  template<typename T>
  struct type_t
  {
    using type = T;

    template<typename U>
    constexpr __and__<type_t<U>, type_t<T>>
    operator &&(type_t<U>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __and__<__and__<U1, U2>, type_t<T>>
    operator &&(__and__<U1, U2>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __and__<__and__<type_t<U1>, type_t<U2>>, type_t<T>>
    operator &&(__and__<type_t<U1>, type_t<U2>>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __and__<__or__<U1, U2>, type_t<T>>
    operator &&(__or__<U1, U2>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __and__<__and__<type_t<U1>, type_t<U2>>, type_t<T>>
    operator &&(__or__<type_t<U1>, type_t<U2>>)
    { return {}; }

    template<typename U>
    constexpr __or__<type_t<U>, type_t<T>>
    operator ||(type_t<U>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __or__<__or__<U1, U2>, type_t<T>>
    operator ||(__or__<U1, U2>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __or__<__or__<type_t<U1>, type_t<U2>>, type_t<T>>
    operator ||(__or__<type_t<U1>, type_t<U2>>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __or__<__and__<U1, U2>, type_t<T>>
    operator ||(__and__<U1, U2>)
    { return {}; }

    template<typename U1, typename U2>
    constexpr __or__<__and__<type_t<U1>, type_t<U2>>, type_t<T>>
    operator ||(__and__<type_t<U1>, type_t<U2>>)
    { return {}; }

    template<template <typename> class P>
    constexpr type_requirement<T, __must<P>>
    operator|(__must<P>)
    { return {}; }

    template<template <typename> class P>
    constexpr type_requirement<T, __either<P>>
    operator|(__either<P>)
    { return {}; }

    template<typename U>
    constexpr bool
    operator==(type_t<U>)
    { return std::is_same_v<T, U>; }
  };

  template<typename T>
  type_t<T> type;
}

#endif //TYPE_CALCULATOR_H
