#pragma once

#include "Core.h"
#include <cmath>

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4514)
// enable /Wall
// C4514: remove unused inline functions
#endif

namespace euclid::math {

template<floating_point Ty>
inline constexpr Ty pi = static_cast<Ty>(3.1415926535897932384626433832795028841972);

template<floating_point Ty>
inline constexpr Ty radian = pi<Ty> / 180;

// https://en.wikipedia.org/wiki/Fast_inverse_square_root
constexpr float sqrt(float number) noexcept {
#if defined(__GLIBCXX__) && !defined(__clang__)
  // libstdc++ provides constexpr math function
  return std::sqrt(number);
#else
  if (__builtin_is_constant_evaluated()) {
    auto value = __builtin_bit_cast(unsigned, number);
    auto magic = __builtin_bit_cast(float, 0x5f1ffff9 - value);
    return magic *= (0.703952253f * (2.38924456f - number * magic * magic)) * number;
  }
  return std::sqrt(number);
#endif
}

constexpr float cos(float angle) noexcept {
#if defined(__GLIBCXX__) && !defined(__clang__)
  return std::cos(angle);
#else
  if (__builtin_is_constant_evaluated()) {
    const float first  = angle * radian<float> * angle * radian<float>;
    const float second = first * first;
    const float third  = first * second;
    return 1.f - 0.5f * first + second * 0.04166666791f - third * 0.001361971023f;
  }
  return std::cos(angle * radian<float>);
#endif
}

constexpr float sin(float angle) noexcept {
#if defined(__GLIBCXX__) && !defined(__clang__)
  return std::sin(angle);
#else
  if (__builtin_is_constant_evaluated()) {
    const float first  = angle  * radian<float>;
    const float square = first  * first;
    const float second = first  * square;
    const float third  = second * square;
    const float forth  = third  * square;
    return first - second * 0.1666666716f + third * 0.008333333768f - forth * 0.0001984127011f;
  }
  return std::sin(angle * radian<float>);
#endif
}

constexpr float tan(float angle) noexcept {
#if defined(__GLIBCXX__) && !defined(__clang__)
  return std::tan(angle);
#else
  if (__builtin_is_constant_evaluated()) {
    const float first  = (90 - angle) * radian<float>;
    const float square = first  * first;
    const float second = first  * square;
    const float third  = second * square;
    const float forth  = third  * square;
    return -(-1.f / first + first / 3.f + second / 45.f + third * 2.f / 945.f + forth / 4725.f);
  }
  return std::tan(angle * radian<float>);
#endif
}

} // namespace euclid::math

#ifdef _MSC_VER
#pragma warning(pop)
#endif