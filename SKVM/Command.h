//
//  Command.h
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#pragma once

enum {
    MOV, ADD
} Opcodes;


struct SecondOperand {
    bool immediate : 1;
    union {
        unsigned int immediateVal : 12;
        unsigned char reg : 4;
    };
};

struct DataProcessingCommand {
    unsigned char rd : 4;
    unsigned char rn : 4;
    SecondOperand op2;
};

struct DataTransferCommand {
    unsigned char rb : 4;
    unsigned char rm : 4;
    SecondOperand offset;
};

struct Command {
    unsigned char opcode : 4;

    union {
        DataProcessingCommand dp;
        DataTransferCommand dt;
    };
};

