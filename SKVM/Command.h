//
//  Command.h
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#pragma once

#include <stdint.h>

// Data processsing
// - ADD, SUB, MUL
// - AND, ORR, NOT, SHR, SHL
// - MOV
// - CMP, TST
// Data transfer
// - LDR, STR
// Branch
// - B, BL
// - B<cond>, BL<cond>
// Conditions
// - EQ, NE, LT, LE, GT, GE
// Other
// - HLT, RST?

enum OpcodeType {
    MOV, ADD, SUB
};

// Operand = immediate
//           | "[", rn, [",", (rm | #offset)] "]"

struct Operand {
    bool isImmediate : 1;
    union {
        int immediate : 12;
        unsigned char rm : 4;
    } offset;
};

struct DataProcessingCommand {
    unsigned char rd : 4;
    unsigned char rn : 4;
    Operand op2;
};

// LDR/STR
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
