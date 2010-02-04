// Hive Mariachi Engine
// Copyright (C) 2008 Hive Solutions Lda.
//
// This file is part of Hive Mariachi Engine.
//
// Hive Mariachi Engine is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Hive Mariachi Engine is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Hive Mariachi Engine. If not, see <http://www.gnu.org/licenses/>.

// __author__    = João Magalhães <joamag@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#include "stdafx.h"

#include "md5.h"

using namespace mariachi;

/**
* Constructor of the class.
*/
Md5::Md5() {
    this->reset();
}

Md5::Md5(const std::string &text) {
    this->reset();
    update((unsigned char *) text.c_str(), text.length());
    finalize();
}

/**
* Destructor of the class.
*/
Md5::~Md5() {
}

// F, G, H and I are basic Md5 functions.
inline unsigned int Md5::F(unsigned int x, unsigned int y, unsigned int z) {
  return x&y | ~x & z;
}

inline unsigned int Md5::G(unsigned int x, unsigned int y, unsigned int z) {
  return x&z | y & ~z;
}

inline unsigned int Md5::H(unsigned int x, unsigned int y, unsigned int z) {
  return x^y^z;
}

inline unsigned int Md5::I(unsigned int x, unsigned int y, unsigned int z) {
  return y ^ (x | ~z);
}

// rotate_left rotates x left n bits.
inline unsigned int Md5::rotate_left(unsigned int x, int n) {
  return (x << n) | (x >> (32 - n));
}

// FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
// Rotation is separate from addition to prevent recomputation.
inline void Md5::FF(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac) {
    a = rotate_left(a + F(b, c, d) + x + ac, s) + b;
}

inline void Md5::GG(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac) {
    a = rotate_left(a + G(b, c, d) + x + ac, s) + b;
}

inline void Md5::HH(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac) {
    a = rotate_left(a + H(b, c, d) + x + ac, s) + b;
}

inline void Md5::II(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac) {
    a = rotate_left(a + I(b, c, d) + x + ac, s) + b;
}

void Md5::reset() {
    finalized = false;

    count[0] = 0;
    count[1] = 0;

    // loads the magic initialization constants
    state[0] = 0x67452301;
    state[1] = 0xefcdab89;
    state[2] = 0x98badcfe;
    state[3] = 0x10325476;
}

// decodes input (unsigned char) into output (unsigned int). Assumes len is a multiple of 4.
void Md5::decode(unsigned int *output, const unsigned char *input, unsigned int size) {
    for(unsigned int i = 0, j = 0; j < size; i++, j += 4) {
        output[i] = ((unsigned int) input[j]) | (((unsigned int) input[j + 1]) << 8) |
            (((unsigned int) input[j + 2]) << 16) | (((unsigned int) input[j + 3]) << 24);
    }
}

// encodes input (unsigned int) into output (unsigned char). Assumes len is
// a multiple of 4.
void Md5::encode(unsigned char output[], const unsigned int input[], unsigned int len) {
    for(unsigned int i = 0, j = 0; j < len; i++, j += 4) {
        output[j] = input[i] & 0xff;
        output[j+1] = (input[i] >> 8) & 0xff;
        output[j+2] = (input[i] >> 16) & 0xff;
        output[j+3] = (input[i] >> 24) & 0xff;
    }
}

void Md5::transform(const unsigned char *block, unsigned int blocksize) {
    unsigned int a = state[0], b = state[1], c = state[2], d = state[3], x[16];

    this->decode(x, block, blocksize);

    // the first round of md5 computation
    FF(a, b, c, d, x[0], MD5_S11, 0xd76aa478);
    FF(d, a, b, c, x[1], MD5_S12, 0xe8c7b756);
    FF(c, d, a, b, x[2], MD5_S13, 0x242070db);
    FF(b, c, d, a, x[3], MD5_S14, 0xc1bdceee);
    FF(a, b, c, d, x[4], MD5_S11, 0xf57c0faf);
    FF(d, a, b, c, x[5], MD5_S12, 0x4787c62a);
    FF(c, d, a, b, x[6], MD5_S13, 0xa8304613);
    FF(b, c, d, a, x[7], MD5_S14, 0xfd469501);
    FF(a, b, c, d, x[8], MD5_S11, 0x698098d8);
    FF(d, a, b, c, x[9], MD5_S12, 0x8b44f7af);
    FF(c, d, a, b, x[10], MD5_S13, 0xffff5bb1);
    FF(b, c, d, a, x[11], MD5_S14, 0x895cd7be);
    FF(a, b, c, d, x[12], MD5_S11, 0x6b901122);
    FF(d, a, b, c, x[13], MD5_S12, 0xfd987193);
    FF(c, d, a, b, x[14], MD5_S13, 0xa679438e);
    FF(b, c, d, a, x[15], MD5_S14, 0x49b40821);

    // the second round of md5 computation
    GG(a, b, c, d, x[1], MD5_S21, 0xf61e2562);
    GG(d, a, b, c, x[6], MD5_S22, 0xc040b340);
    GG(c, d, a, b, x[11], MD5_S23, 0x265e5a51);
    GG(b, c, d, a, x[0], MD5_S24, 0xe9b6c7aa);
    GG(a, b, c, d, x[5], MD5_S21, 0xd62f105d);
    GG(d, a, b, c, x[10], MD5_S22, 0x2441453);
    GG(c, d, a, b, x[15], MD5_S23, 0xd8a1e681);
    GG(b, c, d, a, x[4], MD5_S24, 0xe7d3fbc8);
    GG(a, b, c, d, x[9], MD5_S21, 0x21e1cde6);
    GG(d, a, b, c, x[14], MD5_S22, 0xc33707d6);
    GG(c, d, a, b, x[3], MD5_S23, 0xf4d50d87);
    GG(b, c, d, a, x[8], MD5_S24, 0x455a14ed);
    GG(a, b, c, d, x[13], MD5_S21, 0xa9e3e905);
    GG(d, a, b, c, x[2], MD5_S22, 0xfcefa3f8);
    GG(c, d, a, b, x[7], MD5_S23, 0x676f02d9);
    GG(b, c, d, a, x[12], MD5_S24, 0x8d2a4c8a);

    // the third round of md5 computation
    HH(a, b, c, d, x[5], MD5_S31, 0xfffa3942);
    HH(d, a, b, c, x[8], MD5_S32, 0x8771f681);
    HH(c, d, a, b, x[11], MD5_S33, 0x6d9d6122);
    HH(b, c, d, a, x[14], MD5_S34, 0xfde5380c);
    HH(a, b, c, d, x[1], MD5_S31, 0xa4beea44);
    HH(d, a, b, c, x[4], MD5_S32, 0x4bdecfa9);
    HH(c, d, a, b, x[7], MD5_S33, 0xf6bb4b60);
    HH(b, c, d, a, x[10], MD5_S34, 0xbebfbc70);
    HH(a, b, c, d, x[13], MD5_S31, 0x289b7ec6);
    HH(d, a, b, c, x[0], MD5_S32, 0xeaa127fa);
    HH(c, d, a, b, x[3], MD5_S33, 0xd4ef3085);
    HH(b, c, d, a, x[6], MD5_S34, 0x4881d05);
    HH(a, b, c, d, x[9], MD5_S31, 0xd9d4d039);
    HH(d, a, b, c, x[12], MD5_S32, 0xe6db99e5);
    HH(c, d, a, b, x[15], MD5_S33, 0x1fa27cf8);
    HH(b, c, d, a, x[2], MD5_S34, 0xc4ac5665);

    // the fourth round of md5 computation
    II(a, b, c, d, x[0], MD5_S41, 0xf4292244);
    II(d, a, b, c, x[7], MD5_S42, 0x432aff97);
    II(c, d, a, b, x[14], MD5_S43, 0xab9423a7);
    II(b, c, d, a, x[5], MD5_S44, 0xfc93a039);
    II(a, b, c, d, x[12], MD5_S41, 0x655b59c3);
    II(d, a, b, c, x[3], MD5_S42, 0x8f0ccc92);
    II(c, d, a, b, x[10], MD5_S43, 0xffeff47d);
    II(b, c, d, a, x[1], MD5_S44, 0x85845dd1);
    II(a, b, c, d, x[8], MD5_S41, 0x6fa87e4f);
    II(d, a, b, c, x[15], MD5_S42, 0xfe2ce6e0);
    II(c, d, a, b, x[6], MD5_S43, 0xa3014314);
    II(b, c, d, a, x[13], MD5_S44, 0x4e0811a1);
    II(a, b, c, d, x[4], MD5_S41, 0xf7537e82);
    II(d, a, b, c, x[11], MD5_S42, 0xbd3af235);
    II(c, d, a, b, x[2], MD5_S43, 0x2ad7d2bb);
    II(b, c, d, a, x[9], MD5_S44, 0xeb86d391);

    state[0] += a;
    state[1] += b;
    state[2] += c;
    state[3] += d;

    // invalidates sensitive information
    memset(x, 0, sizeof x);
}


// Md5 block update operation. Continues an Md5 message-digest
// operation, processing another message block
void Md5::update(const unsigned char input[], unsigned int length) {
    // computes the number of bytes modulus 64
    unsigned int index = count[0] / 8 % MD5_BLOCK_SIZE;

    // updates the number of bits
    if((count[0] += (length << 3)) < (length << 3)) {
        count[1]++;
    }

    count[1] += (length >> 29);

    // number of bytes we need to fill in buffer
    unsigned int firstpart = 64 - index;

    unsigned int i;

    // transform as many times as possible
    if(length >= firstpart) {
        // fills the  buffer first
        memcpy(&buffer[index], input, firstpart);

        // transform the buffer
        transform(buffer, MD5_BLOCK_SIZE);

        // transform chunks of blocksize (64 bytes)
        for(i = firstpart; i + MD5_BLOCK_SIZE <= length; i += MD5_BLOCK_SIZE) {
            transform(&input[i], MD5_BLOCK_SIZE);
        }

        index = 0;
    } else {
        i = 0;
    }

    // buffer remaining input
    memcpy(&buffer[index], &input[i], length-i);
}

// Md5 finalization. Ends an Md5 message-digest operation, writing the
// the message digest and zeroizing the context.
Md5& Md5::finalize() {
    static unsigned char padding[64] = {
        0x80, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    if(!finalized) {
        // saves the number of bits
        unsigned char bits[8];
        encode(bits, count, 8);

        // pads out to 56 modulus 64
        unsigned int index = count[0] / 8 % 64;
        unsigned int padLen = (index < 56) ? (56 - index) : (120 - index);
        update(padding, padLen);

        // appends length (before padding)
        update(bits, 8);

        // store the state in the digest
        encode(digest, state, 16);

        // invalidates sensitive information
        memset(buffer, 0, sizeof buffer);
        memset(count, 0, sizeof count);

        finalized = true;
    }

    return *this;
}

/**
* Returns an hexadecimal representation of the
* code.
*
* @return The hexadecimal representation of the code.
*/
std::string Md5::hexdigest() const {
    if(!finalized)
        return "";

    // allocates the buffer state
    char buf[33];

    // iterates over all the values in the buffer
    for(int i = 0; i < 16; i++) {
        sprintf(buf + i * 2, "%02x", digest[i]);
    }

    // sets the last value to end of string
    buf[32] = 0;

    // returns the string value
    return std::string(buf);
}

std::ostream &mariachi::operator<<(std::ostream &outStream, const Md5 &value) {
    // puts the hex digest in the ouput stream
    return outStream << value.hexdigest();
}
