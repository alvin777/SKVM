//
//  RAM.h
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#pragma once

#include <array>


class RAM {
public:
    void reset();

    uint32_t readUint32(uint32_t address);
    void writeUint32(uint32_t address, uint32_t value);
    uint8_t readUint8(uint32_t address);
    void writeUint8(uint32_t address, uint8_t value);
private:
    std::array<uint8_t, 65536> _memory;
};