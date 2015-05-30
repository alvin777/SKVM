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
    Instruction instruction = unpack(value);

    Logger::log("Executing next instruction, PC: 0x", setfill('0'), setw(4), hex, _registers[PC], ", instruction: ", setw(8), value);

    switch (instruction.opcode) {
        case OpcodeType::MOV:
            processMOVInstruction(instruction);
            break;
        case OpcodeType::ADD:
            processADDInstruction(instruction);
            break;
        case OpcodeType::SUB:
            processSUBInstruction(instruction);
            break;
        case OpcodeType::CMP:
            processCMPInstruction(instruction);
            break;
        case OpcodeType::B:
        case OpcodeType::BEQ:
        case OpcodeType::BNE:
        case OpcodeType::BGE:
        case OpcodeType::BGT:
        case OpcodeType::BLE:
        case OpcodeType::BLT:
            processBranchInstruction(instruction);
            return;
        default:
            throw runtime_error(str("Unknown instruction ", setfill('0'), setw(8), hex, value,
                                    " at PC: 0x", setw(4), _registers[PC]));
    }
    
    _registers[PC] = address + 4;
}

void CPU::processMOVInstruction(const Instruction& instruction) {
    uint32_t value = instruction.dp.op2.isImmediate ?
                            instruction.dp.op2.offset.immediate :
                            _registers.at(instruction.dp.op2.offset.rm);
    _registers.at(instruction.dp.rd) = value;
}

void CPU::processADDInstruction(const Instruction& instruction) {
    int32_t sum = _registers.at(instruction.dp.rn);
    if (instruction.dp.op2.isImmediate) {
        sum += instruction.dp.op2.offset.immediate;
    } else {
        sum += _registers.at(instruction.dp.op2.offset.rm);
    }
    
    _registers.at(instruction.dp.rd) = sum;
    _statusRegister.Z = sum == 0;
    _statusRegister.N = sum < 0;
}

void CPU::processSUBInstruction(const Instruction& instruction) {
    int32_t sum = _registers.at(instruction.dp.rn);
    if (instruction.dp.op2.isImmediate) {
        sum -= instruction.dp.op2.offset.immediate;
    } else {
        sum -= _registers.at(instruction.dp.op2.offset.rm);
    }
    
    _registers.at(instruction.dp.rd) = sum;
    _statusRegister.Z = sum == 0;
    _statusRegister.N = sum < 0;
}

void CPU::processCMPInstruction(const Instruction& instruction) {
    int32_t sum = _registers.at(instruction.dp.rn);
    if (instruction.dp.op2.isImmediate) {
        sum -= instruction.dp.op2.offset.immediate;
    } else {
        sum -= _registers.at(instruction.dp.op2.offset.rm);
    }
    
    _statusRegister.Z = sum == 0;
    _statusRegister.N = sum < 0;
}

void CPU::processBranchInstruction(const Instruction& instruction) {
    if (checkBranchCondition(instruction.opcode)) {
        _registers[PC] += instruction.b.immediate;
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
