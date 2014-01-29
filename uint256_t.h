/*
uint256_t.h
An unsigned 256 bit integer library for C++
Copyright (c) 2014 Jason Lee @ calccrypto at gmail.com

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

#include <iostream>
#include <stdexcept>
#include <stdint.h>

#include "uint128_t.h"

extern const uint128_t uint128_256;

class uint256_t{
    private:
        uint128_t UPPER, LOWER;

    public:
        // Constructors
        uint256_t();
        uint256_t(const uint256_t & rhs);

        template <typename T> uint256_t(const T & rhs){
            UPPER = uint128_0;
            LOWER = rhs;
        }

        template <typename S, typename T> uint256_t(const S & upper_rhs, const T & lower_rhs){
            UPPER = upper_rhs;
            LOWER = lower_rhs;
        }

        //  RHS input args only

        // Assignment Operator
        uint256_t operator=(const uint256_t & rhs);

        template <typename T> uint256_t operator=(const T & rhs){
            UPPER = uint128_0;
            LOWER = rhs;
            return *this;
        }

        // Typecast Operators
        operator bool() const;
        operator char() const;
        operator uint8_t() const;
        operator uint16_t() const;
        operator uint32_t() const;
        operator uint64_t() const;
        operator uint128_t() const;

        // Bitwise Operators
        uint256_t operator&(const uint256_t & rhs) const;
        uint256_t operator|(const uint256_t & rhs) const;
        uint256_t operator^(const uint256_t & rhs) const;
        uint256_t operator&=(const uint256_t & rhs);
        uint256_t operator|=(const uint256_t & rhs);
        uint256_t operator^=(const uint256_t & rhs);
        uint256_t operator~() const;

        template <typename T> uint256_t operator&(const T & rhs) const{
            return uint256_t(0, LOWER & (uint128_t) rhs);
        }

        template <typename T> uint256_t operator|(const T & rhs) const{
            return uint256_t(UPPER, LOWER | uint128_t(rhs));
        }

        template <typename T> uint256_t operator^(const T & rhs) const{
            return uint256_t(UPPER, LOWER ^ (uint128_t) rhs);
        }

        template <typename T> uint256_t operator&=(const T & rhs){
            UPPER = uint128_0;
            LOWER &= rhs;
            return *this;
        }

        template <typename T> uint256_t operator|=(const T & rhs){
            LOWER |= (uint128_t) rhs;
            return *this;
        }

        template <typename T> uint256_t operator^=(const T & rhs){
            LOWER ^= (uint128_t) rhs;
            return *this;
        }

        // Bit Shift Operators
        uint256_t operator<<(const uint256_t & shift) const;
        uint256_t operator>>(const uint256_t & shift) const;
        uint256_t operator<<=(const uint256_t & shift);
        uint256_t operator>>=(const uint256_t & shift);

        template <typename T>uint256_t operator<<(const T & rhs) const{
            return *this << uint256_t(rhs);
        }

        template <typename T>uint256_t operator>>(const T & rhs) const{
            return *this >> uint256_t(rhs);
        }

        template <typename T>uint256_t operator<<=(const T & rhs){
            *this = *this << uint256_t(rhs);
            return *this;
        }

        template <typename T>uint256_t operator>>=(const T & rhs){
            *this = *this >> uint256_t(rhs);
            return *this;
        }

        // Logical Operators
        bool operator!() const;
        bool operator&&(const uint256_t & rhs) const;
        bool operator||(const uint256_t & rhs) const;

        template <typename T> bool operator&&(const T & rhs) const{
            return ((bool) *this && rhs);
        }

        template <typename T> bool operator||(const T & rhs) const{
            return ((bool) *this || rhs);
        }

        // Comparison Operators
        bool operator==(const uint256_t & rhs) const;
        bool operator!=(const uint256_t & rhs) const;
        bool operator>(const uint256_t & rhs) const;
        bool operator<(const uint256_t & rhs) const;
        bool operator>=(const uint256_t & rhs) const;
        bool operator<=(const uint256_t & rhs) const;

        template <typename T> bool operator==(const T & rhs) const{
            return (!UPPER && (LOWER == uint128_t(rhs)));
        }

        template <typename T> bool operator!=(const T & rhs) const{
            return ((bool) UPPER | (LOWER != uint128_t(rhs)));
        }

        template <typename T> bool operator>(const T & rhs) const{
            return ((bool) UPPER | (LOWER > uint128_t(rhs)));
        }

        template <typename T> bool operator<(const T & rhs) const{
            return (!UPPER)?(LOWER < uint128_t(rhs)):false;
        }

        template <typename T> bool operator>=(const T & rhs) const{
            return ((*this > rhs) | (*this == rhs));
        }

        template <typename T> bool operator<=(const T & rhs) const{
            return ((*this < rhs) | (*this == rhs));
        }

        // Arithmetic Operators
        uint256_t operator+(const uint256_t & rhs) const;
        uint256_t operator+=(const uint256_t & rhs);
        uint256_t operator-(const uint256_t & rhs) const;
        uint256_t operator-=(const uint256_t & rhs);
        uint256_t operator*(const uint256_t & rhs) const;
        uint256_t operator*=(const uint256_t & rhs);

    private:
        std::pair <uint256_t, uint256_t> divmod(const uint256_t & lhs, const uint256_t & rhs) const;

    public:
        uint256_t operator/(const uint256_t & rhs) const;
        uint256_t operator/=(const uint256_t & rhs);
        uint256_t operator%(const uint256_t & rhs) const;
        uint256_t operator%=(const uint256_t & rhs);

        template <typename T> uint256_t operator+(const T & rhs) const{
            return uint256_t(UPPER + ((LOWER + (uint128_t) rhs) < LOWER), LOWER + (uint128_t) rhs);
        }

        template <typename T> uint256_t operator+=(const T & rhs){
            UPPER = UPPER + ((LOWER + rhs) < LOWER);
            LOWER = LOWER + rhs;
            return *this;
        }

        template <typename T> uint256_t operator-(const T & rhs) const{
            return uint256_t(UPPER - ((LOWER - rhs) > LOWER), LOWER - rhs);
        }

        template <typename T> uint256_t operator-=(const T & rhs){
            *this = *this - rhs;
            return *this;
        }

        template <typename T> uint256_t operator*(const T & rhs) const{
            return *this * uint256_t(rhs);
        }

        template <typename T> uint256_t operator*=(const T & rhs){
            *this = *this * uint256_t(rhs);
            return *this;
        }

        template <typename T> uint256_t operator/(const T & rhs) const{
            return *this / uint256_t(rhs);
        }

        template <typename T> uint256_t operator/=(const T & rhs){
            *this = *this / uint256_t(rhs);
            return *this;
        }

        template <typename T> uint256_t operator%(const T & rhs) const{
            return *this % uint256_t(rhs);
        }

        template <typename T> uint256_t operator%=(const T & rhs){
            *this = *this % uint256_t(rhs);
            return *this;
        }

        // Increment Operators
        uint256_t operator++();
        uint256_t operator++(int);

        // Decrement Operators
        uint256_t operator--();
        uint256_t operator--(int);

        // Get private values
        uint128_t upper() const;
        uint128_t lower() const;

        // Get bitsize of value
        uint16_t bits() const;

        // Get string representation of value
        std::string str(uint8_t base = 10, const unsigned int & len = 0) const;
};

extern const uint256_t uint256_0;
extern const uint256_t uint256_1;

// lhs type T as first arguemnt

// Bitwise Operators
template <typename T> T operator&(const T & lhs, const uint256_t & rhs){
    return (T) (lhs & rhs.lower());
}

template <typename T> T operator|(const T & lhs, const uint256_t & rhs){
    return (T) (lhs | rhs.lower());
}

template <typename T> T operator^(const T & lhs, const uint256_t & rhs){
    return (T) (lhs ^ (T) rhs.lower());
}

template <typename T> T operator&=(const T & lhs, const uint256_t & rhs){
    lhs &= (T) rhs.lower();
    return lhs;
}

template <typename T> T operator|=(const T & lhs, const uint256_t & rhs){
    lhs |= (T) rhs.lower();
    return lhs;
}

template <typename T> T operator^=(const T & lhs, const uint256_t & rhs){
    lhs ^= (T) rhs.lower();
    return lhs;
}

// Comparison Operators
template <typename T> bool operator==(const T & lhs, const uint256_t & rhs){
    return (!rhs.upper() && (uint128_t) lhs == rhs.lower());
}

template <typename T> bool operator!=(const T & lhs, const uint256_t & rhs){
    return (rhs.upper() | ((uint128_t) lhs != rhs.lower()));
}

template <typename T> bool operator>(const T & lhs, const uint256_t & rhs){
    return rhs.upper()?false:((uint128_t) lhs > rhs.lower());
}

template <typename T> bool operator<(const T & lhs, const uint256_t & rhs){
    return rhs.upper()?true:((uint128_t) lhs < rhs.lower());
}

template <typename T> bool operator>=(const T & lhs, const uint256_t & rhs){
    return rhs.upper()?false:((uint128_t) lhs >= rhs.lower());
}

template <typename T> bool operator<=(const T & lhs, const uint256_t & rhs){

    return (rhs.upper())?true:((uint128_t) lhs <= rhs.lower());
}

// Arithmetic Operators
template <typename T> T operator+(const T & lhs, const uint256_t & rhs){
    return (T) (rhs + lhs);
}

template <typename T> T & operator+=(const T & lhs, const uint256_t & rhs){
    lhs = (T) (rhs + lhs);
    return lhs;
}

template <typename T> T operator-(const T & lhs, const uint256_t & rhs){
    return (T) (uint256_t(lhs) - rhs);
}

template <typename T> T & operator-=(const T & lhs, const uint256_t & rhs){
    lhs = (T) (uint256_t(lhs) - rhs);
    return lhs;
}

template <typename T> T operator*(const T & lhs, const uint256_t & rhs){
    return lhs * rhs.lower();
}

template <typename T> T & operator*=(const T & lhs, const uint256_t & rhs){
    lhs = (T) (lhs * rhs.lower());
    return lhs;
}

template <typename T> T operator/(const T & lhs, const uint256_t & rhs){
    return (T) (uint256_t(lhs) / rhs);
}

template <typename T> T & operator/=(const T & lhs, const uint256_t & rhs){
    lhs = (T) (uint256_t(lhs) / rhs);
    return lhs;
}

template <typename T> T operator%(const T & lhs, const uint256_t & rhs){
    return (T) (uint256_t(lhs) % rhs);
}

template <typename T> T & operator%=(const T & lhs, const uint256_t & rhs){
    lhs = (T) (uint256_t(lhs) % rhs);
    return lhs;
}

// IO Operator
std::ostream & operator<<(std::ostream & stream, const uint256_t & rhs);
#endif
