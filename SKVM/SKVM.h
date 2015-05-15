//
//  SKVM.h
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#pragma once

#include <memory>

class SKVMImpl;

class SKVM {
public:    
    SKVM();
//    ~SKVM();
    SKVM(const SKVM&) = delete;
    void operator=(const SKVM&) = delete;
    
    void reset();
    void next();

    uint32_t getRegisterUint32(uint8_t registerIndex);
    void setRegisterUint32(uint8_t registerIndex, uint32_t value);
    
    uint32_t getMemoryUint32(uint32_t address);
    void setMemoryUint32(uint32_t address, uint32_t value);
    uint8_t getMemoryUint8(uint32_t address);
    void setMemoryUint8(uint32_t address, uint8_t value);

private:
    std::shared_ptr<SKVMImpl> pImpl;
};