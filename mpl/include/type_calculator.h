//
// Created by Xymul on 2025/5/10.
//

#ifndef TYPE_CALCULATOR_H
#define TYPE_CALCULATOR_H

namespace mpl {
  template<typename T1, typename T2>
  struct __and__;
  template<typename T1, typename T2>
  struct __or__;

  template<typename T>
  struct type_t;

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

  template<typename T1, typename T2, template <typename> class Pred>
  struct __pred<__and__<T1, T2>, Pred>
  {
    static constexpr bool value = __pred<T1, Pred>::value && __pred<T2, Pred>::value;
  };

  template<typename T1, typename T2, template <typename> class Pred>
  struct __pred<__and__<type_t<T1>, type_t<T2>>, Pred>
  {
    static constexpr bool value = Pred<T1>::value && Pred<T2>::value;
  };

  template<typename T1, typename T2, template <typename> class Pred>
  struct __pred<__or__<T1, T2>, Pred>
  {
    static constexpr bool value = __pred<T1, Pred>::value || __pred<T2, Pred>::value;
  };

  template<typename T1, typename T2, template <typename> class Pred>
  struct __pred<__or__<type_t<T1>, type_t<T2>>, Pred>
  {
    static constexpr bool value = Pred<T1>::value || Pred<T2>::value;
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
  };

  template<typename T>
  type_t<T> type;
}

#endif //TYPE_CALCULATOR_H
