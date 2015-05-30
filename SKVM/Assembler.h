//
//  Assembler.h
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#pragma once

#include <memory>
#include <vector>

#include "Instruction.h"
#include "Tokenizer.h"

class Assembler {
public:
    std::vector<char> assemble(const std::string& text);
    
private:
    std::unique_ptr<Tokenizer> _tokenizer;
    std::vector<char> _binary;
    
    void consume(const std::string& value);
    void consume(const TokenType& type);
    void emit(const Instruction& instruction);

    unsigned char reg();
    int imm();
    Operand reg_or_imm();
    Instruction mov();
    
    bool isDataProcessingOperation2Ops();
    Instruction dataProcessingOperation2Ops();
    bool isDataProcessingOperation3Ops();
    Instruction dataProcessingOperation3Ops();
    
    bool isBranchOperation();
    Instruction branchOperation();
    
    void instruction();
    void line();
    void program();
};