//
//  CPU.cpp
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 14/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#include "CPU.h"

#include "Logger.h"

using namespace std;

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
    uint32_t address = _registers[PC];
    uint32_t value = _ram->readUint32(address);
    Command command = unpack(value);

    Logger::log("Executing next command, PC: ", _registers[PC], ", command: ", hex, value);

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
    
    _registers[PC] = address + 4;
}

void CPU::processMOVCommand(const Command& command) {
    uint32_t value = command.dp.op2.isImmediate ?
                            command.dp.op2.offset.immediate :
                            _registers.at(command.dp.op2.offset.rm);
    _registers.at(command.dp.rd) = value;
}

void CPU::processADDCommand(const Command& command) {
    uint32_t sum = _registers.at(command.dp.rn);
    if (command.dp.op2.isImmediate) {
        sum += command.dp.op2.offset.immediate;
    } else {
        sum += _registers.at(command.dp.op2.offset.rm);
    }
    
    _registers.at(command.dp.rd) = sum;
}