//
//  CPU.cpp
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 14/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#include "CPU.h"

#include  <iomanip>

#include "Logger.h"

using namespace std;

CPU::CPU() {
    reset();
}

void CPU::setRAM(RAM* ram) {
    _ram = ram;
}

void CPU::reset() {
    for (int i = 0; i < _registers.size(); i++) {
        _registers[i] = 0;
    }

    _statusRegister.N = 0;
    _statusRegister.V = 0;
    _statusRegister.C = 0;
    _statusRegister.Z = 0;
}

void CPU::next() {
    uint32_t address = _registers[PC];
    uint32_t value = _ram->readUint32(address);
    Command command = unpack(value);

    Logger::log("Executing next command, PC: 0x", setfill('0'), setw(4), hex, _registers[PC], ", command: ", setw(8), value);

    switch (command.opcode) {
        case OpcodeType::MOV:
            processMOVCommand(command);
            break;
        case OpcodeType::ADD:
            processADDCommand(command);
            break;
        case OpcodeType::SUB:
            processSUBCommand(command);
            break;
        case OpcodeType::CMP:
            processCMPCommand(command);
            break;
        case OpcodeType::B:
        case OpcodeType::BEQ:
        case OpcodeType::BNE:
        case OpcodeType::BGE:
        case OpcodeType::BGT:
        case OpcodeType::BLE:
        case OpcodeType::BLT:
            processBranchCommand(command);
            return;
        default:
            throw runtime_error(str("Unknown command ", setfill('0'), setw(8), hex, value,
                                    " at PC: 0x", setw(4), _registers[PC]));
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
    int32_t sum = _registers.at(command.dp.rn);
    if (command.dp.op2.isImmediate) {
        sum += command.dp.op2.offset.immediate;
    } else {
        sum += _registers.at(command.dp.op2.offset.rm);
    }
    
    _registers.at(command.dp.rd) = sum;
    _statusRegister.Z = sum == 0;
    _statusRegister.N = sum < 0;
}

void CPU::processSUBCommand(const Command& command) {
    int32_t sum = _registers.at(command.dp.rn);
    if (command.dp.op2.isImmediate) {
        sum -= command.dp.op2.offset.immediate;
    } else {
        sum -= _registers.at(command.dp.op2.offset.rm);
    }
    
    _registers.at(command.dp.rd) = sum;
    _statusRegister.Z = sum == 0;
    _statusRegister.N = sum < 0;
}

void CPU::processCMPCommand(const Command& command) {
    int32_t sum = _registers.at(command.dp.rn);
    if (command.dp.op2.isImmediate) {
        sum -= command.dp.op2.offset.immediate;
    } else {
        sum -= _registers.at(command.dp.op2.offset.rm);
    }
    
    _statusRegister.Z = sum == 0;
    _statusRegister.N = sum < 0;
}

void CPU::processBranchCommand(const Command& command) {
    if (checkBranchCondition(command.opcode)) {
        _registers[PC] += command.b.immediate;
    } else {
        _registers[PC] += 4;
    }
}

bool CPU::checkBranchCondition(const OpcodeType& opcode) {
    switch (opcode) {
        case OpcodeType::B:   return true;
        case OpcodeType::BEQ: return _statusRegister.Z;
        case OpcodeType::BNE: return !_statusRegister.Z;
        case OpcodeType::BGE: return _statusRegister.N == _statusRegister.V;
        case OpcodeType::BGT: return !_statusRegister.Z && (_statusRegister.N == _statusRegister.V);
        case OpcodeType::BLE: return _statusRegister.N != _statusRegister.V;
        case OpcodeType::BLT: return _statusRegister.Z || _statusRegister.N != _statusRegister.V;
        default:
            throw runtime_error("Invalid opcode in branch condition");
    }
}
