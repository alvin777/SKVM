//
//  RAM.cpp
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#include "RAM.h"

void RAM::reset() {
    std::fill(_memory.begin(), _memory.end(), 0);
}

uint32_t RAM::readUint32(uint32_t address) {
    return *(uint32_t *)&_memory[address];
}

void RAM::writeUint32(uint32_t address, uint32_t value) {
    _memory.at(address) = value;
}

uint8_t RAM::readUint8(uint32_t address) {
    return _memory.at(address);
}

void RAM::writeUint8(uint32_t address, uint8_t value) {
    _memory.at(address) = value;
}
