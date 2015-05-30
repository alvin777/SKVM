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

#include "Command.h"
#include "Tokenizer.h"

class Assembler {
public:
    std::vector<char> assemble(const std::string& text);
    
private:
    std::unique_ptr<Tokenizer> _tokenizer;
    std::vector<char> _binary;
    
    void consume(const std::string& value);
    void consume(const TokenType& type);
    void emit(const Command& command);

    unsigned char reg();
    int imm();
    Operand reg_or_imm();
    Command mov();
    
    bool isDataProcessingOperation2Ops();
    Command dataProcessingOperation2Ops();
    bool isDataProcessingOperation3Ops();
    Command dataProcessingOperation3Ops();
    
    bool isBranchOperation();
    Command branchOperation();
    
    void command();
    void line();
    void program();
};