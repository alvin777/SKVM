//
//  Instruction.h
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

enum class OpcodeType : unsigned char {
    MOV, ADD, SUB, CMP,
    B, BEQ, BNE, BGE, BGT, BLE, BLT
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

struct DataProcessingInstruction {
    unsigned char rd : 4;
    unsigned char rn : 4;
    Operand op2;
};

// LDR/STR
struct DataTransferInstruction {
    unsigned char rd : 4;
    unsigned char rn : 4;
    Operand address;
};

struct BranchInstruction {
    int immediate : 16;
};

struct Instruction {
    OpcodeType opcode : 4;

    union {
        DataProcessingInstruction dp;
        DataTransferInstruction dt;
        BranchInstruction b;
    };
};

uint32_t pack(const Instruction& instruction);
Instruction unpack(uint32_t word);
