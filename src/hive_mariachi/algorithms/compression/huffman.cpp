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

#include "../../exceptions/exceptions.h"

#include "huffman.h"

using namespace mariachi;

/**
* Constructor of the class.
*/
Huffman::Huffman() {
    this->initFileStream();
    this->initOccurrenceCountList();
    this->initLongestCodeSize();
}

/**
* Destructor of the class.
*/
Huffman::~Huffman() {
    // in case the file stream is valid
    if(this->fileStream) {
        delete this->fileStream;
    }
}

inline void Huffman::initFileStream() {
    // invalidates the file stream
    this->fileStream = NULL;
}

inline void Huffman::initOccurrenceCountList() {
    // resets the occurrence count list
    memset(this->occurrenceCountList, NULL, sizeof(unsigned int) * HUFFMAN_SYMBOL_TABLE_SIZE);
}

inline void Huffman::initLongestCodeSize() {
    this->longestCodeSize = 0;
}

void Huffman::encode(const std::string &filePath, const std::string &targetFilePath) {
    // creates the target file stream to be used
    std::fstream targetFileStream = std::fstream(targetFilePath.c_str(), std::fstream::out | std::fstream::binary);

    // in case the opening of the file fails
    if(targetFileStream.fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem while loading file: " + filePath);
    }

    // encodes the file
    this->encode(filePath, &targetFileStream);

    // closes the target file stream
    targetFileStream.close();
}

void Huffman::encode(const std::string &filePath, std::iostream *targetStream) {
    // generates the table
    this->generateTable(filePath);

    // allocates space for the read size
    unsigned int readSize;

    // allocates the file buffer
    char fileBuffer[HUFFMAN_FILE_BUFFER_SIZE];

    // creates the target huffman stream
    HuffmanStream_t targetHuffmanStream = { targetStream, 0, 0};

    // iterates continuously
    while(1) {
        // reads the buffer
        this->fileStream->read(fileBuffer, HUFFMAN_FILE_BUFFER_SIZE);

        // retrieves the read size
        readSize = this->fileStream->gcount();

        // encodes the data to the target stream
        this->encodeData(fileBuffer, &targetHuffmanStream, readSize);

        // in case the enf of file was reached
        if(this->fileStream->eof()) {
            // breaks the cycle
            break;
        }
    }

    // seeks to the beginning of the file
    targetStream->seekg(0, std::fstream::beg);

    // closes the file
    this->fileStream->close();

    // deletes the file stream
    delete this->fileStream;

    // invalidates the file stream
    this->fileStream = NULL;
}

void Huffman::generateTable(const std::string &filePath) {
    // creates the file stream to be used
    std::fstream *fileStream = new std::fstream(filePath.c_str(), std::fstream::in | std::fstream::binary);

    // in case the opening of the file fails
    if(fileStream->fail()) {
        // throws a runtime exception
        throw RuntimeException("Problem while loading file: " + filePath);
    }

    // generates the table for the file stream
    this->generateTable(fileStream);
}

void Huffman::generateTable(std::fstream *fileStream) {
    // sets the file stream
    this->fileStream = fileStream;

    // allocates the file buffer
    char fileBuffer[HUFFMAN_FILE_BUFFER_SIZE];

    // allocates space for the read size
    unsigned int readSize;

    // allocates space for the lowest huffman node
    HuffmanNode_t *lowestHuffmanNode;

    // allocates space for the second lowest huffman node
    HuffmanNode_t *secondLowestHuffmanNode;

    // iterates continuously
    while(1) {
        // reads the buffer
        this->fileStream->read(fileBuffer, HUFFMAN_FILE_BUFFER_SIZE);

        // retrieves the read size
        readSize = this->fileStream->gcount();

        // updates the occurence values for the file buffer
        this->updateOccurrenceValues(fileBuffer, readSize);

        // in case the enf of file was reached
        if(this->fileStream->eof()) {
            // breaks the cycle
            break;
        }
    }

    // clears the error bits
    this->fileStream->clear();

    // seeks to the beginning of the file
    this->fileStream->seekg(0, std::fstream::beg);

    // creates the code table priority queue
    std::priority_queue<HuffmanNode_t *, std::vector<HuffmanNode_t *>, HuffmanNodeCompare> codeTable;

    // allocates space for the huffman nodes buffer
    HuffmanNode_t huffmanNodesBuffer[HUFFMAN_SYMBOL_TABLE_SIZE];

    // iterates over all the huffman symbols
    for(unsigned int index = 0; index < HUFFMAN_SYMBOL_TABLE_SIZE; index++) {
        // retrieves the current huffman node
        HuffmanNode_t &currentHuffmanNode = huffmanNodesBuffer[index];

        // sets the current huffman node values
        currentHuffmanNode.value = this->occurrenceCountList[index];
        currentHuffmanNode.symbol = index;
        currentHuffmanNode.code = std::string();
        currentHuffmanNode.left = NULL;
        currentHuffmanNode.right = NULL;
        currentHuffmanNode.parent = NULL;

        // adds the current huffman node to the code table
        codeTable.push(&currentHuffmanNode);
    }

    //test the contents of the priority_queue
    while(1) {
        // the lowest value node
        lowestHuffmanNode = codeTable.top();

        // pops the code table
        codeTable.pop();

        // in case
        if(codeTable.empty()) {
            // breaks the cycle
            break;
        } else {
            // the second lowest value node
            secondLowestHuffmanNode = codeTable.top();

            // pops the code table
            codeTable.pop();

            // creates the merged huffman node
            HuffmanNode_t *mergedHuffmanNode = new HuffmanNode_t();

            // sets the merged huffman node values
            mergedHuffmanNode->value = lowestHuffmanNode->value + secondLowestHuffmanNode->value;
            mergedHuffmanNode->symbol = HUFFMAN_SYMBOL_TABLE_EXTRA_VALUE;
            mergedHuffmanNode->code = std::string();
            mergedHuffmanNode->left = secondLowestHuffmanNode;
            mergedHuffmanNode->right = lowestHuffmanNode;
            mergedHuffmanNode->parent = NULL;

            // sets the parent in the huffman nodes
            lowestHuffmanNode->parent = mergedHuffmanNode;
            secondLowestHuffmanNode->parent = mergedHuffmanNode;

            // adds the merged node to the priority_queue
            codeTable.push(mergedHuffmanNode);
        }
    }

    // generates the huffman table
    this->_generateTable(lowestHuffmanNode);

    // computes the table
    this->computeTable();

    // cleans the structures
    this->cleanStructures(lowestHuffmanNode);
}

/**
* Generates the lookup table for the current huffman
* table values.
*/
void Huffman::generateLookupTable() {
    // in case the longest code size exceeds the limit
    if(this->longestCodeSize > HUFFMAN_LOOKUP_TABLE_MAXIMUM_CODE_SIZE) {
        // throws runtime exception
        throw RuntimeException("Unable to create lookup table maximum code size limit exceeded");
    }

    // allocates space for the lookup table
    std::map<std::string, unsigned int> lookupTable;

    // iterates over all the symbols
    for(unsigned int index = 0; index < HUFFMAN_SYMBOL_TABLE_SIZE; index++) {
        // retrieves the current symbol
        int currentSymbol = index;

        // retrieves the current code
        std::string &currentCode = this->huffmanTable[index];

        // retrieves the current code size
        unsigned int currentCodeSize = currentCode.size();

        // sets the index in the lookup table
        lookupTable[currentCode] = index;

        // calculates the size difference between the longest code size
        // and the current code size
        unsigned int deltaSize = this->longestCodeSize - currentCodeSize;

        std::vector<std::string> permutationStringValuesList;

        this->_generatePermutations(&permutationStringValuesList, currentCode, deltaSize);
    }
}

void Huffman::_generatePermutations(std::vector<std::string> *stringValuesList, std::string &stringValue, unsigned int count) {
    // iterates over the binary numbers
    for(unsigned char index = 0; index < 2; index++) {
        char indexValue = 0x30 | index;

        // adds the current index to the
        // string value
        std::string newStringValue = stringValue + indexValue;

        if(count > 1) {
            this->_generatePermutations(stringValuesList, newStringValue, count - 1);
        } else {
            stringValuesList->push_back(newStringValue);
        }
    }
}

/**
* Prints the huffman table information to the standard output.
* The information contained is pretty printed with the symbol and
* the associated code information.
*/
void Huffman::printTable() {
    // iterates over all the symbols
    for(unsigned int index = 0; index < HUFFMAN_SYMBOL_TABLE_SIZE; index++) {
        // retrieves the current symbol
        int currentSymbol = index;

        // retrieves the current code
        std::string &currentCode = this->huffmanTable[index];

        // prints the table line information
        std::cout << currentSymbol << " (" << (char) currentSymbol << "): " << currentCode << "\n";
    }
}

/**
* Updates the occurrence values, using the values in the given
* buffer with the given size.
*
* @param buffer The buffer to calculate the occurrence values.
* @param size The size of the buffer to calculate the occurrence values.
*/
inline void Huffman::updateOccurrenceValues(char *buffer, unsigned int size) {
    // allocates the current byte value
    unsigned char currentByte;

    // iterates over all the read bytes
    for(unsigned int index  = 0; index < size; index++) {
        // retrieves the current byte
        currentByte = buffer[index];

        // increments the current byte value
        this->occurrenceCountList[currentByte] += 1;
    }
}

inline void Huffman::encodeData(char *buffer, HuffmanStream_t *bitStream, unsigned int size) {
    // iterates over all the symbols in the buffer
    for(unsigned int index = 0; index < size; index++) {
        // retrieves te current symbol
        unsigned char currentSymbol = buffer[index];

        // retrieves the computed code for the current symbol
        std::vector<HuffmanPartialByte_t> &computedCode  = this->huffmanComputedTable[currentSymbol];

        // retrieves the computed code size
        unsigned int computedCodeSize = computedCode.size();

        // iterates over all the computed code partial bytes
        for(unsigned int _index = 0; _index < computedCodeSize; _index++) {
            // retrieves the computed code partial byte
            HuffmanPartialByte_t &computedCodePartialByte = computedCode[_index];

            // writes the computed code partial byte to the huffman stream
            this->writeHuffmanStream(bitStream, computedCodePartialByte.byte, computedCodePartialByte.numberBits);
        }
    }

    // flushes the bit stream
    bitStream->stream->write(bitStream->buffer, bitStream->bufferSize);

    // resets the buffer size
    bitStream->bufferSize = 0;
}

inline void Huffman::writeHuffmanStream(HuffmanStream_t *bitStream, unsigned char byte, unsigned char numberBits) {
    // calculates the number of available bits
    char numberAvailableBits = HUFFMAN_SYMBOL_SIZE - bitStream->bitCounter;

    // calculates the number of extra bits
    char numberExtraBits = numberBits - numberAvailableBits;

    // in case the number of extra bits is valid
    // the number of bits to be writen is the sames as the available bits
    // otherwise the number of extra bits is zero
    numberExtraBits > 0 ? numberBits = numberAvailableBits : numberExtraBits = 0;

    // shifts the current byte value (by the number of bits)
    bitStream->currentByte <<= numberBits;

    // adds the valid bits of the current byte (bits excluding the extra ones)
    // to the current byte (shifts the byte the number of extra bits)
    bitStream->currentByte = bitStream->currentByte | (byte >> numberExtraBits);

    // increments the bit counter by the number of bits
    bitStream->bitCounter += numberBits;

    // in case the bit counter reached the limit
    // a flush is required
    if(bitStream->bitCounter == HUFFMAN_SYMBOL_SIZE) {
        // sets the buffer value
        bitStream->buffer[bitStream->bufferSize] = bitStream->currentByte;

        // increments the buffer size
        bitStream->bufferSize++;

        // resets the bit counter
        bitStream->bitCounter = 0;

        // resets the current byte
        bitStream->currentByte = 0;

        // in case the buffer is full
        if(bitStream->bufferSize == HUFFMAN_STREAM_BUFFER_SIZE) {
            // flushes the bit stream
            bitStream->stream->write(bitStream->buffer, bitStream->bufferSize);

            // resets the buffer size
            bitStream->bufferSize = 0;
        }
    }

    // in case there are extra bits to be written
    if(numberExtraBits > 0) {
        // sets the current byte as the extra bits value
        // creates an and mask to mask the byte and "show" only the extra bits
        bitStream->currentByte = bitStream->currentByte | (byte & ((0x1 << numberExtraBits) - 1));

        // increments the bit counter by the number of extra bits
        bitStream->bitCounter += numberExtraBits;
    }
}

inline void Huffman::computeTable() {
    // iterates over all the symbols
    for(unsigned int index = 0; index < HUFFMAN_SYMBOL_TABLE_SIZE; index++) {
        // retrieves the current symbol
        int currentSymbol = index;

        // retrieves the current code
        std::string &currentCode = this->huffmanTable[index];

        // retrieves the current code size
        unsigned int currentCodeSize = currentCode.size();

        // in case the current code size is bigger than
        // the current longest code size
        if(currentCodeSize > this->longestCodeSize) {
            this->longestCodeSize = currentCodeSize;
        }

        // computes the current code
        std::vector<HuffmanPartialByte_t> &currentComputedCode = this->computeCode(currentCode);

        // sets the current computed code in the huffman computed table
        this->huffmanComputedTable[currentSymbol] = currentComputedCode;
    }
}

inline std::vector<HuffmanPartialByte_t> Huffman::computeCode(std::string &code) {
    // creates the code vector
    std::vector<HuffmanPartialByte_t> codeVector;

    // allocates space for the partial byte
    HuffmanPartialByte_t partialByte;

    // retrieves the code iterator
    std::string::iterator codeIterator = code.begin();

    // creates the bit counter
    unsigned char bitCounter = HUFFMAN_SYMBOL_INVALID_SIZE;

    // sets the current char
    unsigned char currentChar = 0;

    // iterates over the code characters
    while(codeIterator != code.end()) {
        // switches over the bit counter
        switch(bitCounter) {
            // in case the bit counter is invalid (initial iteration)
            case HUFFMAN_SYMBOL_INVALID_SIZE:
                // resets the bit counter
                bitCounter = 0;

                break;
            // in case the bit counter is the symbol size (end of byte)
            case HUFFMAN_SYMBOL_SIZE:
                // creates the partial byte
                partialByte.byte = currentChar;
                partialByte.numberBits = bitCounter;

                // adds the partial byte to the code vector
                codeVector.push_back(partialByte);

                // resets the current char
                currentChar = 0;

                // resets the bit counter
                bitCounter = 0;

                break;
            default:
                // shift the current char value
                currentChar <<= 1;

                break;
        }

        // retrieves the current code character
        char currentCodeCharacter = *codeIterator;

        // updates the current char value
        currentChar = currentChar | (currentCodeCharacter & 0x01);

        // increments the code iterator
        codeIterator++;

        // increments the bit counter
        bitCounter++;
    }

    // in case there are bits left to be added
    // to the code vector
    if(bitCounter > 0) {
        // creates the partial byte
        partialByte.byte = currentChar;
        partialByte.numberBits = bitCounter;

        // adds the partial byte to the code vector
        codeVector.push_back(partialByte);
    }

    // returns the code vector
    return codeVector;
}

inline void Huffman::cleanStructures(HuffmanNode *node) {
    // in case it's not a leaf ndoe
    if(node->left && node->right) {
        this->cleanStructures(node->left);
        this->cleanStructures(node->right);

        // deletes the node
        delete node;
    }
}

void Huffman::_generateTable(HuffmanNode *node, std::string &code) {
    // in case it's not a leaf ndoe
    if(node->left && node->right) {
        this->_generateTable(node->left, code + "0");
        this->_generateTable(node->right, code + "1");
    }

    // sets the code fot the current symbol
    // in the huffman table
    this->huffmanTable[node->symbol] = code;
}

bool HuffmanNodeCompare::operator()(HuffmanNode_t *firstElement, HuffmanNode_t *secondElement) {
    return firstElement->value > secondElement->value;
}
