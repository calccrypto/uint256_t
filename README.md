uint256_t
An unsigned 256 bit integer type for C++
Copyright (c) 2014 Jason Lee @ calccrypto at gmail.com

With much help from Auston Sterling

Thanks to François Dessenne for convincing me
to do a general rewrite of this class.

Please see LICENSE file for license.

This is simple implementation of an unsigned 256 bit
integer type in C++. It's meant to be used like a standard
uintX_t, except with a larger bit size than those provided
by C/C++.

uint256_t requires uint128_t, which is included.

Be careful when initializing a uint128_t value. -1 does not
create the value 2**256 - 1. Rather, it uses 2**64 - 1, or
whatever the built-in max unsigned value is, NOT 2**128 - 1.