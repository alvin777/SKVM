//
//  SKVM.cpp
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#include "SKVM.h"

#include <array>

#include "CPU.h"
#include "RAM.h"
#include "Instruction.h"

using namespace std;


class SKVMImpl {
    friend SKVM;
    
    void processMOVInstruction(const Instruction& instruction);
    void processADDInstruction(const Instruction& instruction);
    
    uint32_t _stateRegister;
    
    CPU _cpu;
    RAM _ram;
};


SKVM::SKVM() : pImpl(new SKVMImpl()) {
    
};

void SKVM::reset() {
    
    pImpl->_ram.reset();
}

void SKVM::next() {
    pImpl->_cpu.next();
}

uint32_t SKVM::getRegisterUint32(uint8_t registerIndex) {
    return pImpl->_cpu._registers.at(registerIndex);
}

void SKVM::setRegisterUint32(uint8_t registerIndex, uint32_t value) {
    pImpl->_cpu._registers.at(registerIndex) = value;
}

uint32_t SKVM::getMemoryUint32(uint32_t address) {
    return pImpl->_ram.readUint32(address);
}

void SKVM::setMemoryUint32(uint32_t address, uint32_t value) {
    pImpl->_ram.writeUint32(address, value);
}

uint8_t SKVM::getMemoryUint8(uint32_t address) {
    return pImpl->_ram.readUint8(address);
}

void SKVM::setMemoryUint8(uint32_t address, uint8_t value) {
    pImpl->_ram.writeUint8(address, value);
}
