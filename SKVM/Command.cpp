//
//  Command.cpp
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#include "Command.h"

#include <bitset>
#include <iostream>

using namespace std;

void store(std::bitset<32>& b, char high, char low, uint32_t value) {
    for (int i = 0; i <= high - low; i++) {
        b[low + i] = (value >> i) & 0x1;
    }
}

uint32_t load(uint32_t word, char high, char low) {
    uint32_t mask = 0;
    for (int i = 0; i <= high - low; i++) {
        mask |= 1 << (low + i);
    }
    
    return (word & mask) >> low;
}

uint32_t pack(const Command& command) {
    std::bitset<32> b;
    store(b, 31, 28, command.opcode);
    store(b, 27, 24, command.dp.rd);
    store(b, 23, 20, command.dp.rn);
    store(b, 12, 12, command.dp.op2.isImmediate);
    store(b, 11,  0, command.dp.op2.offset.immediate);
    return static_cast<uint32_t>(b.to_ulong());
}

Command unpack(uint32_t word) {
    Command command;
    command.opcode = static_cast<OpcodeType>(load(word, 31, 28));
    command.dp.rd                   = load(word, 27, 24);
    command.dp.rn                   = load(word, 23, 20);
    command.dp.op2.isImmediate      = load(word, 12, 12);
    command.dp.op2.offset.immediate = load(word, 11,  0);
    
    return command;
}

