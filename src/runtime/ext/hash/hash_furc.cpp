/*
   +----------------------------------------------------------------------+
   | HipHop for PHP                                                       |
   +----------------------------------------------------------------------+
   | Copyright (c) 2010- Facebook, Inc. (http://www.facebook.com)         |
   | Copyright (c) 1997-2010 The PHP Group                                |
   +----------------------------------------------------------------------+
   | This source file is subject to version 3.01 of the PHP license,      |
   | that is bundled with this package in the file LICENSE, and is        |
   | available through the world-wide-web at the following url:           |
   | http://www.php.net/license/3_01.txt                                  |
   | If you did not receive a copy of the PHP license and are unable to   |
   | obtain it through the world-wide-web, please send a note to          |
   | license@php.net so we can mail you a copy immediately.               |
   +----------------------------------------------------------------------+
*/

/**
 * This file contains code from MurmurHash, by Austin Appleby. The licensing
 * information for MurmurHash as specified on its website,
 * http://sites.google.com/site/murmurhash/, is as follows: "For business
 * purposes, Murmurhash is under the MIT license.
 */

/**
 * FurcHash -- a consistent hash function using a binary decision tree.
 * Based on an algorithm by Mark Rabkin with two changes:
 *    1) Uses MurmurHash64A to hash the original key and to generate
 *       additional bits by recursively rehashing.
 *    2) The original recursive algorithm for the decision tree has been
 *       made iterative.
 *
 * Assumes that |m| is 8 million or less (2^FURC_SHIFT).  Making FURC_SHIFT
 * bigger also makes furc_hash modestly slower.
 *
 * Performance is in the sub-500ns range to over 100,000 shards with 13-byte
 * keys.  This version of furc_hash is fairly insensitive to key length since
 * additional bits are generated by re-hashing the initial MurmurHash64A.
 */

#include <assert.h>
#include <stdint.h>
#include <sys/time.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

namespace HPHP {
///////////////////////////////////////////////////////////////////////////////

/* Seed constant for MurmurHash64A selected by search for optimum diffusion
 * including recursive application.
 */
#define SEED 4193360111ul

/* Maximum number tries for in-range result before just returning 0. */
#define MAX_TRIES 32

/* Gap in bit index per try; limits us to 2^FURC_SHIFT shards.  Making this
 * larger will sacrifice a modest amount of performance.
 */
#define FURC_SHIFT 23

/* Size of cache for hash values; should be > MAXTRIES * (FURCSHIFT + 1) */
#define FURC_CACHE_SIZE 1024

/**
 * MurmurHash2, 64-bit versions, by Austin Appleby
 *
 * The same caveats as 32-bit MurmurHash2 apply here - beware of alignment
 * and endian-ness issues if used across multiple platforms.
 *
 * 64-bit hash for 64-bit platforms
 */
uint64_t murmur_hash_64A(const void* const key, const size_t len,
                         const uint32_t seed) {
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    uint64_t h = seed ^ (len * m);

    const uint64_t * data = (const uint64_t *)key;
    const uint64_t * end = data + (len/8);

    while(data != end) {
        uint64_t k = *data++;

        k *= m;
        k ^= k >> r;
        k *= m;

        h ^= k;
        h *= m;
    }

    const uint8_t * data2 = (const uint8_t*)data;

    switch(len & 7) {
        case 7: h ^= (uint64_t)data2[6] << 48;
        case 6: h ^= (uint64_t)data2[5] << 40;
        case 5: h ^= (uint64_t)data2[4] << 32;
        case 4: h ^= (uint64_t)data2[3] << 24;
        case 3: h ^= (uint64_t)data2[2] << 16;
        case 2: h ^= (uint64_t)data2[1] << 8;
        case 1: h ^= (uint64_t)data2[0];
                h *= m;
    };

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

/* MurmurHash64A performance-optimized for hash of uint64_t keys and seed = M0 */
static uint64_t murmur_rehash_64A(uint64_t k) {
    const uint64_t m = 0xc6a4a7935bd1e995ULL;
    const int r = 47;

    uint64_t h = (uint64_t)SEED ^ (sizeof(uint64_t) * m);

    k *= m;
    k ^= k >> r;
    k *= m;

    h ^= k;
    h *= m;

    h ^= h >> r;
    h *= m;
    h ^= h >> r;

    return h;
}

/**
 * furc_get_bit -- the bitstream generator
 *
 * Given a key and an index, provides a pseudorandom bit dependent on both.
 * Caches hash values; a NULL key clears the cache.
 */
static uint32_t furc_get_bit(const char* const key, const size_t len,
                             const uint32_t idx, uint64_t* hash,
                             int32_t* old_ord_p) {
    int32_t ord = (idx >> 6);
    int n;

    if (key == NULL) {
        *old_ord_p = -1;
        return 0;
    }

    if (*old_ord_p < ord) {
        for (n = *old_ord_p + 1; n <= ord; n++) {
            hash[n] = ((n == 0)
                       ? murmur_hash_64A(key, len, SEED)
                       : murmur_rehash_64A(hash[n-1]));
        }
        *old_ord_p = ord;
    }

    return (hash[ord] >> (idx&0x3f)) & 0x1;
}

inline uint32_t furc_maximum_pool_size(void) {
    return (1 << FURC_SHIFT);
}

uint32_t furc_hash_internal(const char* const key, const size_t len, const uint32_t m) {
    uint32_t tries;
    uint32_t d;
    uint32_t num;
    uint32_t i;
    uint32_t a;
    uint64_t hash[FURC_CACHE_SIZE];
    int32_t old_ord;

    assert(m <= furc_maximum_pool_size());

    if (m <= 1) {
        return 0;
    }

    furc_get_bit(NULL, 0, 0, hash, &old_ord);
    for (d = 0; m > (1ul << d); d++)
        ;

    a = d;
    for (tries = 0; tries < MAX_TRIES; tries++) {
        while (!furc_get_bit(key, len, a, hash, &old_ord)) {
            if (--d == 0) {
                return 0;
            }
            a = d;
        }
        a += FURC_SHIFT;
        num = 1;
        for (i = 0; i < d-1; i++) {
            num = (num << 1) | furc_get_bit(key, len, a, hash, &old_ord);
            a += FURC_SHIFT;
        }
        if (num < m) {
            return num;
        }
    }

    // Give up; return 0, which is a legal value in all cases.
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
}
