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

typedef enum {
    PS = 13,
    LR = 14
} NamedRegisters;


class CPU {
public:
    std::array<uint32_t, 16> _registers;
    uint32_t _stateRegister;
    
    void setRAM(RAM* ram);

    void reset();
    void next();

private:
    RAM* _ram;
    
    void processMOVCommand(const Command& command);
    void processADDCommand(const Command& command);
};