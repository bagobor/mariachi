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

#pragma once

#define MD5_BLOCK_SIZE 64

#define MD5_S11 7
#define MD5_S12 12
#define MD5_S13 17
#define MD5_S14 22
#define MD5_S21 5
#define MD5_S22 9
#define MD5_S23 14
#define MD5_S24 20
#define MD5_S31 4
#define MD5_S32 11
#define MD5_S33 16
#define MD5_S34 23
#define MD5_S41 6
#define MD5_S42 10
#define MD5_S43 15
#define MD5_S44 21

namespace mariachi {
    /**
    * Class used to calculate md5 hashes.
    *
    * @see wikipedia - http://en.wikipedia.org/wiki/MD5
    */
    class Md5 {
        private:
            bool finalized;

            /**
            * Buffer used to hold the bytes that didn't fit
            * in the last 64 byte chunk.
            */
            unsigned char buffer[MD5_BLOCK_SIZE];

            /**
            * Counter of 64 bit used for number of bits (lo, hi).
            */
            unsigned int count[2];

            /**
            * The state of the digest.
            */
            unsigned int state[4];

            /**
            * The result of the digest.
            */
            unsigned char digest[16];

            void transform(const unsigned char *block, unsigned int blocksize);
            static void decode(unsigned int *output, const unsigned char *input, unsigned int size);
            static void encode(unsigned char output[], const unsigned int input[], unsigned int len);

            // low level logic operations
            static inline unsigned int F(unsigned int x, unsigned int y, unsigned int z);
            static inline unsigned int G(unsigned int x, unsigned int y, unsigned int z);
            static inline unsigned int H(unsigned int x, unsigned int y, unsigned int z);
            static inline unsigned int I(unsigned int x, unsigned int y, unsigned int z);
            static inline unsigned int rotate_left(unsigned int x, int n);
            static inline void FF(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac);
            static inline void GG(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac);
            static inline void HH(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac);
            static inline void II(unsigned int &a, unsigned int b, unsigned int c, unsigned int d, unsigned int x, unsigned int s, unsigned int ac);

        public:
            Md5();
            Md5(const std::string &text);
            ~Md5();
            void update(const unsigned char *buffer, unsigned int size);
            void reset();
            Md5 &finalize();
            std::string hexdigest() const;
            friend std::ostream &operator<<(std::ostream &outStream, const Md5 &value);
    };

    std::ostream &operator<<(std::ostream &outStream, const Md5 &value);
}
