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
#include "Command.h"

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
    
    void processMOVCommand(const Command& command);
    void processADDCommand(const Command& command);
    void processSUBCommand(const Command& command);
    void processCMPCommand(const Command& command);
    void processBranchCommand(const Command& command);
    
    bool checkBranchCondition(const OpcodeType& opcode);
};
