//
//  CPU.cpp
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 14/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#include "CPU.h"

void CPU::setRAM(RAM* ram) {
    _ram = ram;
}

void CPU::reset() {
    for (int i = 0; i < _registers.size(); i++) {
        _registers[i] = 0;
    }
    _stateRegister = 0;
}

void CPU::next() {
    // fetch
    uint32_t address = _registers[PS];
    uint32_t value = _ram->readUint32(address);
    Command command = *(Command *)&value;
    
    switch (command.opcode) {
        case MOV:
            processMOVCommand(command);
            break;
        case ADD:
            processADDCommand(command);
            break;
        default:
            break;
    }
    
    _registers[PS] = address + 4;
}

void CPU::processMOVCommand(const Command& command) {
    uint32_t value = _registers.at(command.dp.rn);
    _registers.at(command.dp.rd) = value;
}

void CPU::processADDCommand(const Command& command) {
    _registers.at(command.dp.rd) =
    _registers.at(command.dp.rn) +
    _registers.at(command.dp.op2.reg);
}