//
//  CPU.h
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 14/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#pragma once

#include <array>

#include "RAM.h"
#include "Instruction.h"

enum NamedRegisters {
    PC = 13,
    LR = 14
};


struct StatusRegister {
    unsigned int N : 1; // negative
    unsigned int Z : 1; // zero
    unsigned int C : 1; // carry/borrow
    unsigned int V : 1; // overflow
};

class CPU {
public:
    CPU();
    
    std::array<int32_t, 16> _registers;
    StatusRegister _statusRegister;
    
    void setRAM(RAM* ram);

    void reset();
    void next();

private:
    RAM* _ram;
    
    void processMOVInstruction(const Instruction& instruction);
    void processADDInstruction(const Instruction& instruction);
    void processSUBInstruction(const Instruction& instruction);
    void processCMPInstruction(const Instruction& instruction);
    void processBranchInstruction(const Instruction& instruction);
    
    bool checkBranchCondition(const OpcodeType& opcode);
};
