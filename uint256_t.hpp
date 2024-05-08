/*
uint256_t.h
An unsigned 256 bit integer library for C++

Copyright (c) 2013 - 2017 Jason Lee @ calccrypto at gmail.com

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

With much help from Auston Sterling

Thanks to François Dessenne for convincing me
to do a general rewrite of this class.
*/

#ifndef __UINT256_T__
#define __UINT256_T__

#include <vector>
#include <cstring>
#include <cstdint>
#include <ostream>
#include <stdexcept>
#include <type_traits>
#include <utility>

#include "uint128_t.hpp"

class uint256_t
{
  private:
#ifdef __BIG_ENDIAN__
    uint128_t UPPER, LOWER;
#endif
#ifdef __LITTLE_ENDIAN__
    uint128_t LOWER, UPPER;
#endif

  public:
    // Constructors
    constexpr uint256_t() : LOWER{uint128_t{0}}, UPPER{uint128_t{0}} {}
    constexpr uint256_t(const uint256_t & rhs) = default;
    constexpr uint256_t(uint256_t && rhs) = default;
    uint256_t(const std::string & s, uint8_t base = 10)
    {
        init_from_base(s.c_str(), base);
    }
    uint256_t(const char * val, uint8_t base = 10)
    {
        init_from_base(val, base);
    }
    constexpr uint256_t(const bool & b)
      : uint256_t((uint8_t) b)
    { }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t(const T & rhs)
#ifdef __BIG_ENDIAN__
        : UPPER(uint128_t{0}), LOWER(rhs)
#endif
#ifdef __LITTLE_ENDIAN__
        : LOWER(rhs), UPPER(uint128_t{0})
#endif
    {
        if (std::is_signed<T>::value)
        {
            if (rhs < 0)
            {
                UPPER = uint128_t(-1, -1);
            }
        }
    }

    template <typename S, typename T, typename = typename std::enable_if <std::is_integral<S>::value && std::is_integral<T>::value, void>::type>
    constexpr uint256_t(const S & upper_rhs, const T & lower_rhs)
#ifdef __BIG_ENDIAN__
        : UPPER(upper_rhs), LOWER(lower_rhs)
#endif
#ifdef __LITTLE_ENDIAN__
        : LOWER(lower_rhs), UPPER(upper_rhs)
#endif
    {}

    constexpr uint256_t(const uint128_t & upper_rhs, const uint128_t & lower_rhs)
#ifdef __BIG_ENDIAN__
        : UPPER(upper_rhs), LOWER(lower_rhs)
#endif
#ifdef __LITTLE_ENDIAN__
        : LOWER(lower_rhs), UPPER(upper_rhs)
#endif
    {}
    constexpr uint256_t(const uint128_t & lower_rhs)
#ifdef __BIG_ENDIAN__
        : UPPER(uint128_t{0}), LOWER(lower_rhs)
#endif
#ifdef __LITTLE_ENDIAN__
        : LOWER(lower_rhs), UPPER(uint128_t{0})
#endif
    {}

    template <typename R, typename S, typename T, typename U,
            typename = typename std::enable_if<std::is_integral<R>::value &&
            std::is_integral<S>::value &&
            std::is_integral<T>::value &&
            std::is_integral<U>::value, void>::type>
    constexpr uint256_t(const R & upper_lhs, const S & lower_lhs, const T & upper_rhs, const U & lower_rhs)
#ifdef __BIG_ENDIAN__
        : UPPER(upper_lhs, lower_lhs), LOWER(upper_rhs, lower_rhs)
#endif
#ifdef __LITTLE_ENDIAN__
        : LOWER(upper_rhs, lower_rhs), UPPER(upper_lhs, lower_lhs)
#endif
    {}

    // Assignment Operator
    constexpr uint256_t & operator=(const uint256_t & rhs) = default;
    constexpr uint256_t & operator=(uint256_t && rhs) = default;
    
    template <typename T, typename = typename std::enable_if <std::is_integral<T>::value, T>::type>
    constexpr uint256_t & operator=(const T & rhs)
    {
        UPPER = uint128_t{0};

        if (std::is_signed<T>::value)
        {
            if (rhs < 0)
            {
                UPPER = uint128_t(-1, -1);
            }
        }

        LOWER = rhs;
        return *this;
    }

    constexpr uint256_t & operator=(const bool & rhs)
    {
        UPPER = 0;
        LOWER = rhs;
        return *this;
    }

    // Typecast Operators
    constexpr explicit operator bool() const
    {
        return static_cast<bool>(UPPER | LOWER);
    }

    constexpr explicit operator uint8_t() const
    {
        return static_cast<uint8_t>(LOWER);
    }

    constexpr explicit operator uint16_t() const
    {
        return static_cast<uint16_t>(LOWER);
    }

    constexpr explicit operator uint32_t() const
    {
        return static_cast<uint32_t>(LOWER);
    }

    constexpr explicit operator uint64_t() const
    {
        return static_cast<uint64_t>(LOWER);
    }

    constexpr explicit operator uint128_t() const
    {
        return LOWER;
    }

    // Bitwise Operators
    constexpr uint256_t operator&(const uint128_t & rhs) const
    {
        return uint256_t(uint128_t{0}, LOWER & rhs);
    }

    constexpr uint256_t operator&(const uint256_t & rhs) const
    {
        return uint256_t(UPPER & rhs.UPPER, LOWER & rhs.LOWER);
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t operator&(const T & rhs) const
    {
        return uint256_t(uint128_t{0}, LOWER & (uint128_t) rhs);
    }

    constexpr uint256_t & operator&=(const uint128_t & rhs)
    {
        UPPER  = uint128_t{0};
        LOWER &= rhs;
        return *this;
    }

    constexpr uint256_t & operator&=(const uint256_t & rhs)
    {
        UPPER &= rhs.UPPER;
        LOWER &= rhs.LOWER;
        return *this;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t & operator&=(const T & rhs)
    {
        UPPER = uint128_t{0};
        LOWER &= rhs;
        return *this;
    }

    constexpr uint256_t operator|(const uint128_t & rhs) const
    {
        return uint256_t(UPPER , LOWER | rhs);
    }

    constexpr uint256_t operator|(const uint256_t & rhs) const
    {
        return uint256_t(UPPER | rhs.UPPER, LOWER | rhs.LOWER);
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t operator|(const T & rhs) const
    {
        return uint256_t(UPPER, LOWER | uint128_t(rhs));
    }

    constexpr uint256_t & operator|=(const uint128_t & rhs)
    {
        LOWER |= rhs;
        return *this;
    }

    constexpr uint256_t & operator|=(const uint256_t & rhs)
    {
        UPPER |= rhs.UPPER;
        LOWER |= rhs.LOWER;
        return *this;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t & operator|=(const T & rhs)
    {
        LOWER |= (uint128_t) rhs;
        return *this;
    }

    constexpr uint256_t operator^(const uint128_t & rhs) const
    {
        return uint256_t(UPPER, LOWER ^ rhs);
    }

    constexpr uint256_t operator^(const uint256_t & rhs) const
    {
        return uint256_t(UPPER ^ rhs.UPPER, LOWER ^ rhs.LOWER);
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t operator^(const T & rhs) const
    {
        return uint256_t(UPPER, LOWER ^ (uint128_t) rhs);
    }

    constexpr uint256_t & operator^=(const uint128_t & rhs)
    {
        LOWER ^= rhs;
        return *this;
    }

    constexpr uint256_t & operator^=(const uint256_t & rhs)
    {
        UPPER ^= rhs.UPPER;
        LOWER ^= rhs.LOWER;
        return *this;
    }

   template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    uint256_t & operator^=(const T & rhs)
    {
        LOWER ^= (uint128_t) rhs;
        return *this;
    }

    constexpr uint256_t operator~() const
    {
        return uint256_t(~UPPER, ~LOWER);
    }

    // Bit Shift Operators
    constexpr uint256_t operator<<(const uint128_t & rhs) const
    {
        return *this << uint256_t(rhs);
    }

    constexpr uint256_t operator<<(const uint256_t & rhs) const
    {
        const uint128_t shift = rhs.LOWER;
        if (((bool) rhs.UPPER) || (shift >= uint128_t{256}))
        {
            return uint256_t{0};
        }
        else if (shift == uint128_t{128})
        {
            return uint256_t(LOWER, uint128_t{0});
        }
        else if (shift == uint128_t{0})
        {
            return *this;
        }
        else if (shift < uint128_t{128})
        {
            return uint256_t((UPPER << shift) + (LOWER >> (uint128_t{128} - shift)), LOWER << shift);
        }
        else if ((uint128_t{256} > shift) && (shift > uint128_t{128}))
        {
            return uint256_t(LOWER << (shift - uint128_t{128}), uint128_t{0});
        }
        else{
            return uint256_t{0};
        }
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t operator<<(const T & rhs) const
    {
        return *this << uint256_t(rhs);
    }

    constexpr uint256_t & operator<<=(const uint128_t & shift)
    {
        return *this <<= uint256_t(shift);
    }

    constexpr uint256_t & operator<<=(const uint256_t & shift)
    {
        *this = *this << shift;
        return *this;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t & operator<<=(const T & rhs)
    {
        *this = *this << uint256_t(rhs);
        return *this;
    }

    constexpr uint256_t operator>>(const uint128_t & rhs) const
    {
        return *this >> uint256_t(rhs);
    }

    constexpr uint256_t operator>>(const uint256_t & rhs) const
    {
        const uint128_t shift = rhs.LOWER;
        if (((bool) rhs.UPPER) | (shift >= uint128_t{256}))
        {
            return uint256_t{0};
        }
        else if (shift == uint128_t{128})
        {
            return uint256_t(UPPER);
        }
        else if (shift == uint128_t{0})
        {
            return *this;
        }
        else if (shift < uint128_t{128})
        {
            return uint256_t(UPPER >> shift, (UPPER << (uint128_t{128} - shift)) + (LOWER >> shift));
        }
        else if ((uint128_t{256} > shift) && (shift > uint128_t{128}))
        {
            return uint256_t(UPPER >> (shift - uint128_t{128}));
        }
        else{
            return uint256_t{0};
        }
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t operator>>(const T & rhs) const
    {
        return *this >> uint256_t(rhs);
    }

    constexpr uint256_t & operator>>=(const uint128_t & shift)
    {
        return *this >>= uint256_t(shift);
    }

    constexpr uint256_t & operator>>=(const uint256_t & shift)
    {
        *this = *this >> shift;
        return *this;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t & operator>>=(const T & rhs)
    {
        *this = *this >> uint256_t(rhs);
        return *this;
    }

    // Logical Operators
    constexpr bool operator!() const
    {
        return ! (bool) *this;
    }

    constexpr bool operator&&(const uint128_t & rhs) const
    {
        return (*this && uint256_t(rhs));
    }

    constexpr bool operator&&(const uint256_t & rhs) const
    {
        return ((bool) *this && (bool) rhs);
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr bool operator&&(const T & rhs) const
    {
        return ((bool) *this && rhs);
    }

    constexpr bool operator||(const uint128_t & rhs) const
    {
        return (*this || uint256_t(rhs));
    }

    constexpr bool operator||(const uint256_t & rhs) const
    {
        return ((bool) *this || (bool) rhs);
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr bool operator||(const T & rhs) const
    {
        return ((bool) *this || rhs);
    }

    // Comparison Operators
    constexpr bool operator==(const uint128_t & rhs) const
    {
        return (*this == uint256_t(rhs));
    }

    constexpr bool operator==(const uint256_t & rhs) const
    {
        return ((UPPER == rhs.UPPER) && (LOWER == rhs.LOWER));
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr bool operator==(const T & rhs) const
    {
        return (!UPPER && (LOWER == uint128_t(rhs)));
    }

    constexpr bool operator!=(const uint128_t & rhs) const
    {
        return (*this != uint256_t(rhs));
    }

    constexpr bool operator!=(const uint256_t & rhs) const
    {
        return ((UPPER != rhs.UPPER) | (LOWER != rhs.LOWER));
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr bool operator!=(const T & rhs) const
    {
        return ((bool) UPPER | (LOWER != uint128_t(rhs)));
    }


    constexpr bool operator>(const uint128_t & rhs) const
    {
        return (*this > uint256_t(rhs));
    }

    constexpr bool operator>(const uint256_t & rhs) const
    {
        if (UPPER == rhs.UPPER)
        {
            return (LOWER > rhs.LOWER);
        }
        if (UPPER > rhs.UPPER)
        {
            return true;
        }
        return false;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr bool operator>(const T & rhs) const
    {
        return ((bool) UPPER | (LOWER > uint128_t(rhs)));
    }

    constexpr bool operator<(const uint128_t & rhs) const
    {
        return (*this < uint256_t(rhs));
    }

    constexpr bool operator<(const uint256_t & rhs) const
    {
        if (UPPER == rhs.UPPER)
        {
            return (LOWER < rhs.LOWER);
        }
        if (UPPER < rhs.UPPER)
        {
            return true;
        }
        return false;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr bool operator<(const T & rhs) const
    {
        return (!UPPER)?(LOWER < uint128_t(rhs)):false;
    }

    constexpr bool operator>=(const uint128_t & rhs) const
    {
        return (*this >= uint256_t(rhs));
    }

    constexpr bool operator>=(const uint256_t & rhs) const
    {
        return ((*this > rhs) | (*this == rhs));
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr bool operator>=(const T & rhs) const
    {
        return ((*this > rhs) | (*this == rhs));
    }

    constexpr bool operator<=(const uint128_t & rhs) const
    {
        return (*this <= uint256_t(rhs));
    }

    constexpr bool operator<=(const uint256_t & rhs) const
    {
        return ((*this < rhs) | (*this == rhs));
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    bool operator<=(const T & rhs) const
    {
        return ((*this < rhs) | (*this == rhs));
    }

    // Arithmetic Operators
    constexpr uint256_t operator+(const uint128_t & rhs) const
    {
        return *this + uint256_t(rhs);
    }

    constexpr uint256_t operator+(const uint256_t & rhs) const
    {
        return uint256_t(UPPER + rhs.UPPER + (((LOWER + rhs.LOWER) < LOWER)?uint128_t{1}:uint128_t{0}), LOWER + rhs.LOWER);
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t operator+(const T & rhs) const
    {
        return uint256_t(UPPER + ((LOWER + (uint128_t) rhs) < LOWER), LOWER + (uint128_t) rhs);
    }

    constexpr uint256_t & operator+=(const uint128_t & rhs)
    {
        return *this += uint256_t(rhs);
    }

    constexpr uint256_t & operator+=(const uint256_t & rhs)
    {
        UPPER = rhs.UPPER + UPPER + ((LOWER + rhs.LOWER) < LOWER);
        LOWER = LOWER + rhs.LOWER;
        return *this;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t & operator+=(const T & rhs)
    {
        return *this += uint256_t(rhs);
    }


    constexpr uint256_t operator-(const uint128_t & rhs) const
    {
        return *this - uint256_t(rhs);
    }

    constexpr uint256_t operator-(const uint256_t & rhs) const
    {
        return uint256_t(UPPER - rhs.UPPER - ((LOWER - rhs.LOWER) > LOWER), LOWER - rhs.LOWER);
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t operator-(const T & rhs) const
    {
        return uint256_t(UPPER - ((LOWER - rhs) > LOWER), LOWER - rhs);
    }

    constexpr uint256_t & operator-=(const uint128_t & rhs)
    {
        return *this -= uint256_t(rhs);
    }

    constexpr uint256_t & operator-=(const uint256_t & rhs)
    {
        *this = *this - rhs;
        return *this;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t & operator-=(const T & rhs)
    {
        return *this = *this - uint256_t(rhs);
    }


    constexpr uint256_t operator*(const uint128_t & rhs) const
    {
        return *this * uint256_t(rhs);
    }

    constexpr uint256_t operator*(const uint256_t & rhs) const
    {
        // split values into 4 64-bit parts
        uint128_t top[4] = {UPPER.upper(), UPPER.lower(), LOWER.upper(), LOWER.lower()};
        uint128_t bottom[4] = {rhs.upper().upper(), rhs.upper().lower(), rhs.lower().upper(), rhs.lower().lower()};
        uint128_t products[4][4]{};

        // multiply each component of the values
        for (int y = 3; y > -1; y--)
        {
            for (int x = 3; x > -1; x--)
            {
                products[3 - y][x] = top[x] * bottom[y];
            }
        }

        // first row
        uint128_t fourth64 = uint128_t(products[0][3].lower());
        uint128_t third64  = uint128_t(products[0][2].lower()) + uint128_t(products[0][3].upper());
        uint128_t second64 = uint128_t(products[0][1].lower()) + uint128_t(products[0][2].upper());
        uint128_t first64  = uint128_t(products[0][0].lower()) + uint128_t(products[0][1].upper());

        // second row
        third64  += uint128_t(products[1][3].lower());
        second64 += uint128_t(products[1][2].lower()) + uint128_t(products[1][3].upper());
        first64  += uint128_t(products[1][1].lower()) + uint128_t(products[1][2].upper());

        // third row
        second64 += uint128_t(products[2][3].lower());
        first64  += uint128_t(products[2][2].lower()) + uint128_t(products[2][3].upper());

        // fourth row
        first64  += uint128_t(products[3][3].lower());

        // combines the values, taking care of carry over
        return uint256_t(first64 << uint128_t{64}, uint128_t{0}) +
            uint256_t(third64.upper(), third64 << uint128_t{64}) +
            uint256_t(second64, uint128_t{0}) +
            uint256_t(fourth64);
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t operator*(const T & rhs) const
    {
        return *this * uint256_t(rhs);
    }

    constexpr uint256_t & operator*=(const uint128_t & rhs)
    {
        return *this *= uint256_t(rhs);
    }

    constexpr uint256_t & operator*=(const uint256_t & rhs)
    {
        *this = *this * rhs;
        return *this;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t & operator*=(const T & rhs)
    {
        return *this = *this * uint256_t(rhs);
    }

  private:
    constexpr std::pair<uint256_t, uint256_t> divmod(const uint256_t & lhs, const uint256_t & rhs) const
    {
        // Save some calculations /////////////////////
        if (rhs == uint256_t{0})
        {
            throw std::domain_error("Error: division or modulus by 0");
        }
        else if (rhs == uint256_t{1})
        {
            return std::pair <uint256_t, uint256_t> (lhs, uint256_t{0});
        }
        else if (lhs == rhs)
        {
            return std::pair <uint256_t, uint256_t> (uint256_t{1}, uint256_t{0});
        }
        else if ((lhs == uint256_t{0}) || (lhs < rhs))
        {
            return std::pair <uint256_t, uint256_t> (uint256_t{0}, lhs);
        }

        std::pair <uint256_t, uint256_t> qr(uint256_t{0}, lhs);
        uint256_t copyd = rhs << (lhs.bits() - rhs.bits());
        uint256_t adder = uint256_t{1} << (lhs.bits() - rhs.bits());
        if (copyd > qr.second)
        {
            copyd >>= uint256_t{1};
            adder >>= uint256_t{1};
        }
        while (qr.second >= rhs)
        {
            if (qr.second >= copyd)
            {
                qr.second -= copyd;
                qr.first |= adder;
            }
            copyd >>= uint256_t{1};
            adder >>= uint256_t{1};
        }
        return qr;
    }

    void init_from_base(const char * s, uint8_t base)
    {
        *this = 0;

        uint256_t power(1);
        uint8_t digit;
        int pos = strlen(s) - 1;
        while (pos >= 0)
        {
            digit = 0;
            if('0' <= s[pos] && s[pos] <= '9')
            {
                digit = s[pos] - '0';
            } else if('a' <= s[pos] && s[pos] <= 'z')
            {
                digit = s[pos] - 'a' + 10;
            }
            // *this += digit * power;
            *this += power * digit;
            pos--;
            power *= base;
        }
    }

  public:
    constexpr uint256_t operator/(const uint128_t & rhs) const
    {
        return *this / uint256_t(rhs);
    }

    constexpr uint256_t operator/(const uint256_t & rhs) const
    {
        return divmod(*this, rhs).first;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t operator/(const T & rhs) const
    {
        return *this / uint256_t(rhs);
    }


    constexpr uint256_t & operator/=(const uint128_t & rhs)
    {
        return *this /= uint256_t(rhs);
    }

    constexpr uint256_t & operator/=(const uint256_t & rhs)
    {
        *this = *this / rhs;
        return *this;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t & operator/=(const T & rhs)
    {
        return *this = *this / uint256_t(rhs);
    }

    constexpr uint256_t operator%(const uint128_t & rhs) const
    {
        return *this % uint256_t(rhs);
    }

    constexpr uint256_t operator%(const uint256_t & rhs) const
    {
        return *this - (rhs * (*this / rhs));
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t operator%(const T & rhs) const
    {
        return *this % uint256_t(rhs);
    }


    constexpr uint256_t & operator%=(const uint128_t & rhs)
    {
        return *this %= uint256_t(rhs);
    }

    constexpr uint256_t & operator%=(const uint256_t & rhs)
    {
        *this = *this % rhs;
        return *this;
    }

    template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
    constexpr uint256_t & operator%=(const T & rhs)
    {
        return *this = *this % uint256_t(rhs);
    }

    // Increment Operators
    constexpr uint256_t & operator++()
    {
        *this += uint256_t{1};
        return *this;
    }

    constexpr uint256_t operator++(int)
    {
        uint256_t temp(*this);
        ++*this;
        return temp;
    }

    // Decrement Operators
    constexpr uint256_t & operator--()
    {
        *this -= uint256_t{1};
        return *this;
    }

    constexpr uint256_t operator--(int)
    {
        uint256_t temp(*this);
        --*this;
        return temp;
    }

    // Nothing done since promotion doesn't work here
    constexpr uint256_t operator+() const
    {
        return *this;
    }

    // two's complement
    constexpr uint256_t operator-() const
    {
        return ~*this + uint256_t{1};
    }

    // Get private values
    constexpr const uint128_t & upper() const
    {
        return UPPER;
    }

    constexpr const uint128_t & lower() const
    {
        return LOWER;
    }

    // Get bitsize of value
    constexpr uint16_t bits() const
    {
        uint16_t out = 0;
        if (UPPER)
        {
            out = 128;
            uint128_t up = UPPER;
            while (up)
            {
                up >>= uint128_t{1};
                out++;
            }
        }
        else{
            uint128_t low = LOWER;
            while (low)
            {
                low >>= uint128_t{1};
                out++;
            }
        }
        return out;
    }

    std::vector<uint8_t> export_bits() const
    {
        std::vector<uint8_t> ret;
        ret.reserve(32);
        UPPER.export_bits(ret);
        LOWER.export_bits(ret);
        return ret;
    }

    std::vector<uint8_t> export_bits_truncate() const
    {
        std::vector<uint8_t> ret = export_bits();

        //prune the zeroes
        int i = 0;
        while (ret[i] == 0 && i < 64) i++;
        ret.erase(ret.begin(), ret.begin() + i);

        return ret;
    }

    // Get string representation of value
    std::string str(uint8_t base = 10, const unsigned int & len = 0) const
    {
        if ((base < 2) || (base > 36))
        {
            throw std::invalid_argument("Base must be in the range 2-36");
        }
        std::string out = "";
        if (!(*this))
        {
            out = "0";
        }
        else{
            std::pair <uint256_t, uint256_t> qr(*this, uint256_t{0});
            do{
                qr = divmod(qr.first, base);
                out = "0123456789abcdefghijklmnopqrstuvwxyz"[(uint8_t) qr.second] + out;
            } while (qr.first);
        }
        if (out.size() < len)
        {
            out = std::string(len - out.size(), '0') + out;
        }
        return out;
    }
};  // class uint256_y

// Bitwise Operators
constexpr uint256_t operator&(const uint128_t & lhs, const uint256_t & rhs)
{
    return rhs & lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr uint256_t operator&(const T & lhs, const uint256_t & rhs)
{
    return rhs & lhs;
}

constexpr uint128_t & operator&=(uint128_t & lhs, const uint256_t & rhs)
{
    lhs = (rhs & lhs).lower();
    return lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr T & operator&=(T & lhs, const uint256_t & rhs)
{
    return lhs = static_cast <T> (rhs & lhs);
}

constexpr uint256_t operator|(const uint128_t & lhs, const uint256_t & rhs)
{
    return rhs | lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr uint256_t operator|(const T & lhs, const uint256_t & rhs)
{
    return rhs | lhs;
}

constexpr uint128_t & operator|=(uint128_t & lhs, const uint256_t & rhs)
{
    lhs = (rhs | lhs).lower();
    return lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr T & operator|=(T & lhs, const uint256_t & rhs)
{
    return lhs = static_cast <T> (rhs | lhs);
}

constexpr uint256_t operator^(const uint128_t & lhs, const uint256_t & rhs)
{
    return rhs ^ lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr uint256_t operator^(const T & lhs, const uint256_t & rhs)
{
    return rhs ^ lhs;
}

constexpr uint128_t & operator^=(uint128_t & lhs, const uint256_t & rhs)
{
    lhs = (rhs ^ lhs).lower();
    return lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr T & operator^=(T & lhs, const uint256_t & rhs)
{
    return lhs = static_cast <T> (rhs ^ lhs);
}

// Bitshift operators
constexpr uint256_t operator<<(const bool & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) << rhs;
}

constexpr uint256_t operator<<(const uint8_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) << rhs;
}

constexpr uint256_t operator<<(const uint16_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) << rhs;
}

constexpr uint256_t operator<<(const uint32_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) << rhs;
}

constexpr uint256_t operator<<(const uint64_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) << rhs;
}

constexpr uint256_t operator<<(const uint128_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) << rhs;
}

constexpr uint256_t operator<<(const int8_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) << rhs;
}

constexpr uint256_t operator<<(const int16_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) << rhs;
}

constexpr uint256_t operator<<(const int32_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) << rhs;
}

constexpr uint256_t operator<<(const int64_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) << rhs;
}

constexpr uint128_t & operator<<=(uint128_t & lhs, const uint256_t & rhs)
{
    lhs = (uint256_t(lhs) << rhs).lower();
    return lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr T & operator<<=(T & lhs, const uint256_t & rhs)
{
    lhs = static_cast <T> (uint256_t(lhs) << rhs);
    return lhs;
}

constexpr uint256_t operator>>(const bool & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) >> rhs;
}

constexpr uint256_t operator>>(const uint8_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) >> rhs;
}

constexpr uint256_t operator>>(const uint16_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) >> rhs;
}

constexpr uint256_t operator>>(const uint32_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) >> rhs;
}

constexpr uint256_t operator>>(const uint64_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) >> rhs;
}

constexpr uint256_t operator>>(const uint128_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) >> rhs;
}

constexpr uint256_t operator>>(const int8_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) >> rhs;
}

constexpr uint256_t operator>>(const int16_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) >> rhs;
}

constexpr uint256_t operator>>(const int32_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) >> rhs;
}

constexpr uint256_t operator>>(const int64_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) >> rhs;
}

constexpr uint128_t & operator>>=(uint128_t & lhs, const uint256_t & rhs)
{
    lhs = (uint256_t(lhs) >> rhs).lower();
    return lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr T & operator>>=(T & lhs, const uint256_t & rhs)
{
    return lhs = static_cast <T> (uint256_t(lhs) >> rhs);
}

// Comparison Operators
constexpr bool operator==(const uint128_t & lhs, const uint256_t & rhs)
{
    return rhs == lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr bool operator==(const T & lhs, const uint256_t & rhs)
{
    return (!rhs.upper() && ((uint64_t) lhs == rhs.lower()));
}

constexpr bool operator!=(const uint128_t & lhs, const uint256_t & rhs)
{
    return rhs != lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr bool operator!=(const T & lhs, const uint256_t & rhs)
{
    return (rhs.upper() | ((uint64_t) lhs != rhs.lower()));
}

constexpr bool operator>(const uint128_t & lhs, const uint256_t & rhs)
{
    return rhs < lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr bool operator>(const T & lhs, const uint256_t & rhs)
{
    return rhs.upper()?false:((uint128_t) lhs > rhs.lower());
}

constexpr bool operator<(const uint128_t & lhs, const uint256_t & rhs)
{
    return rhs > lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr bool operator<(const T & lhs, const uint256_t & rhs)
{
    return rhs.upper()?true:((uint128_t) lhs < rhs.lower());
}

constexpr bool operator>=(const uint128_t & lhs, const uint256_t & rhs)
{
    return rhs <= lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr bool operator>=(const T & lhs, const uint256_t & rhs)
{
    return rhs.upper()?false:((uint128_t) lhs >= rhs.lower());
}

constexpr bool operator<=(const uint128_t & lhs, const uint256_t & rhs)
{
    return rhs >= lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr bool operator<=(const T & lhs, const uint256_t & rhs)
{
    return rhs.upper()?true:((uint128_t) lhs <= rhs.lower());
}

// Arithmetic Operators
constexpr uint256_t operator+(const uint128_t & lhs, const uint256_t & rhs)
{
    return rhs + lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr uint256_t operator+(const T & lhs, const uint256_t & rhs)
{
    return rhs + lhs;
}

constexpr uint128_t & operator+=(uint128_t & lhs, const uint256_t & rhs)
{
    lhs = (rhs + lhs).lower();
    return lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr T & operator+=(T & lhs, const uint256_t & rhs)
{
    lhs = static_cast <T> (rhs + lhs);
    return lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr uint256_t operator-(const T & lhs, const uint256_t & rhs)
{
    return -(rhs - lhs);
}

constexpr uint256_t operator-(const uint128_t & lhs, const uint256_t & rhs)
{
    return -(rhs - lhs);
}

constexpr uint128_t & operator-=(uint128_t & lhs, const uint256_t & rhs)
{
    lhs = (-(rhs - lhs)).lower();
    return lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr T & operator-=(T & lhs, const uint256_t & rhs)
{
    return lhs = static_cast <T> (-(rhs - lhs));
}

constexpr uint256_t operator*(const uint128_t & lhs, const uint256_t & rhs)
{
    return rhs * lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr uint256_t operator*(const T & lhs, const uint256_t & rhs)
{
    return rhs * lhs;
}

constexpr uint128_t & operator*=(uint128_t & lhs, const uint256_t & rhs)
{
    lhs = (rhs * lhs).lower();
    return lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr T & operator*=(T & lhs, const uint256_t & rhs)
{
    return lhs = static_cast <T> (rhs * lhs);
}

constexpr uint256_t operator/(const uint128_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) / rhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr uint256_t operator/(const T & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) / rhs;
}

constexpr uint128_t & operator/=(uint128_t & lhs, const uint256_t & rhs)
{
    lhs = (uint256_t(lhs) / rhs).lower();
    return lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr T & operator/=(T & lhs, const uint256_t & rhs)
{
    return lhs = static_cast <T> (uint256_t(lhs) / rhs);
}

constexpr uint256_t operator%(const uint128_t & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) % rhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr uint256_t operator%(const T & lhs, const uint256_t & rhs)
{
    return uint256_t(lhs) % rhs;
}

constexpr uint128_t & operator%=(uint128_t & lhs, const uint256_t & rhs)
{
    lhs = (uint256_t(lhs) % rhs).lower();
    return lhs;
}

template <typename T, typename = typename std::enable_if<std::is_integral<T>::value, T>::type >
constexpr T & operator%=(T & lhs, const uint256_t & rhs)
{
    return lhs = static_cast <T> (uint256_t(lhs) % rhs);
}

std::ostream & operator<<(std::ostream & stream, const uint256_t & rhs)
{
    if (stream.flags() & stream.oct)
    {
        stream << rhs.str(8);
    }
    else if (stream.flags() & stream.dec)
    {
        stream << rhs.str(10);
    }
    else if (stream.flags() & stream.hex)
    {
        stream << rhs.str(16);
    }
    return stream;
}

#endif
