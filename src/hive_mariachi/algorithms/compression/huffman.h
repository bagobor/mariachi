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

// __author__    = Tiago Silva <tsilva@hive.pt>
// __version__   = 1.0.0
// __revision__  = $LastChangedRevision$
// __date__      = $LastChangedDate$
// __copyright__ = Copyright (c) 2008 Hive Solutions Lda.
// __license__   = GNU General Public License (GPL), Version 3

#pragma once

#define HUFFMAN_SYMBOL_SIZE 8

#define HUFFMAN_SYMBOL_INVALID_SIZE 255

#define HUFFMAN_SYMBOL_TABLE_SIZE 256

#define HUFFMAN_SYMBOL_TABLE_EXTRA_SIZE 257

#define HUFFMAN_SYMBOL_TABLE_EXTRA_VALUE HUFFMAN_SYMBOL_TABLE_EXTRA_SIZE - 1

#define HUFFMAN_FILE_BUFFER_SIZE 10240

namespace mariachi {
    typedef struct HuffmanNode_t {
        unsigned int value;
        int symbol;
        std::string code;
        HuffmanNode_t *left;
        HuffmanNode_t *right;
        HuffmanNode_t *parent;
    } HuffmanNode;

    typedef struct HuffmanPartialByte_t {
        unsigned char byte;
        unsigned char numberBits;
    } HuffmanPartialByte;

    typedef struct HuffmanStream_t {
        std::stringstream *stream;
        unsigned char bitCounter;
        unsigned char currentByte;
    } HuffmanStream;

    class Huffman {
        private:
            /**
            * The occurrence count list, to count the number of
            * occurrences of each symbol.
            */
            unsigned int occurrenceCountList[HUFFMAN_SYMBOL_TABLE_SIZE];

            /**
            * The file stream tath holds the reference to the file.
            */
            std::fstream *fileStream;

            std::string huffmanTable[HUFFMAN_SYMBOL_TABLE_EXTRA_SIZE];
            std::vector<HuffmanPartialByte_t> huffmanComputedTable[HUFFMAN_SYMBOL_TABLE_EXTRA_SIZE];

            inline void initOccurrenceCountList();
            inline void updateOccurrenceValues(char *buffer, unsigned int size);
            inline void encodeData(char *buffer, HuffmanStream_t *bitStream, unsigned int size);
            inline void writeHuffmanStream(HuffmanStream_t *bitStream, unsigned char byte, unsigned char numberBits);
            inline void computeTable();
            inline std::vector<HuffmanPartialByte_t> computeCode(std::string &code);
            inline void cleanStructures(HuffmanNode *node);
            void _generateTable(HuffmanNode *node, std::string &code = std::string(""));

        public:
            Huffman();
            ~Huffman();
            void encode(const std::string &filePath, const std::string &targetFilePath);
            void encode(const std::string &filePath, std::stringstream *targetStream);
            void generateTable(const std::string &filePath);
            void generateTable(std::fstream *fileStream);
            void printTable();
    };

    class HuffmanNodeCompare {
        private:

        public:
            bool operator()(HuffmanNode_t *firstElement, HuffmanNode_t *secondElement);
    };
}
