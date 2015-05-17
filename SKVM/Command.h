//
//  Command.h
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#pragma once

#include <stdint.h>

enum OpcodeType {
    MOV, ADD
};

// Operand = immediate
//           | "[", rn, [",", (rm | #offset)] "]"

struct Operand {
    bool isImmediate : 1;
    union {
        int immediate : 12;
        unsigned char rm : 4; // offset register
    } offset;
};

struct DataProcessingCommand {
    unsigned char rd : 4;
    unsigned char rn : 4;
    Operand op2;
};

// LDR | STR
struct DataTransferCommand {
    unsigned char rd : 4;
    unsigned char rn : 4;
    Operand address;
};

struct Command {
    OpcodeType opcode : 4;

    union {
        DataProcessingCommand dp;
        DataTransferCommand dt;
    };
};

uint32_t pack(const Command& command);
Command unpack(uint32_t word);
