//
//  Compiler.cpp
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#include "Compiler.h"


// EBNF (http://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_Form)
//
// imm                = "#", digit, {digit} ;
// reg                = "r0" | "r1" | "r2" | .. | "r15" ;
// reg_or_imm         = reg | imm;
// data_transfer_op   = "MOV", reg, ",", reg_or_imm ;
// data_processing_op = "ADD", reg, ",", reg, ",", reg_or_imm ;
// command            = data_transfer_op | data_processing_op ;
// program            = command, "\n", { command, "\n" } ;

// TODO:
// - labels
// - comments
// - memory variables

unsigned char Compiler::reg() {
    unsigned char r = _tokenizer->lookahead().intValue;
    consume(TokenType::REGISTER);
    return r;
}

Operand Compiler::reg_or_imm() {
    Operand op2;
    if (_tokenizer->lookahead().type == TokenType::REGISTER) {
        op2.isImmediate = false;
        op2.offset.rm = _tokenizer->lookahead().intValue;
        consume(TokenType::REGISTER);
    } else if (_tokenizer->lookahead().type == TokenType::IMMEDIATE) {
        op2.isImmediate = true;
        op2.offset.immediate = _tokenizer->lookahead().intValue;
        consume(TokenType::IMMEDIATE);
    }
    
    return op2;
}

Command Compiler::mov() {
    Command command;
    command.opcode = MOV;
    command.dp.rn = 0;
    consume(TokenType::MOV);
    command.dp.rd = reg();
    consume(TokenType::COMMA);
    command.dp.op2 = reg_or_imm();
    return command;
}

Command Compiler::add() {
    Command command;
    command.opcode = ADD;
    consume(TokenType::ADD);
    command.dp.rd = reg();
    consume(TokenType::COMMA);
    command.dp.rn = reg();
    consume(TokenType::COMMA);
    command.dp.op2 = reg_or_imm();
    return command;
}

Command Compiler::sub() {
    Command command;
    command.opcode = SUB;
    consume(TokenType::SUB);
    command.dp.rd = reg();
    consume(TokenType::COMMA);
    command.dp.rn = reg();
    consume(TokenType::COMMA);
    command.dp.op2 = reg_or_imm();
    return command;
}

//Command Compiler::dataProcessingOperation() {
//    
//}

void Compiler::command() {
    Command command;
    if (_tokenizer->lookahead().type == TokenType::MOV) {
        command = mov();
    } else if (_tokenizer->lookahead().type == TokenType::ADD) {
        command = add();
    } else if (_tokenizer->lookahead().type == TokenType::SUB) {
        command = sub();
//    } else if (isIn(_tokenizer->lookahead().value, {"ADD", "SUB"})) {
//        command = sub();
    }

    consume(TokenType::EOL);
    emit(command);
}

void Compiler::program() {
    
    while (_tokenizer->lookahead().type != TokenType::END) {
        command();
//        consume(TokenType::EOL);
    }
    
//    emit(HALT);
}

std::vector<char> Compiler::compile(const std::string& text) {
    
    _tokenizer = std::unique_ptr<Tokenizer>(new Tokenizer(text));
    
    program();
    
    return _binary;
}

void Compiler::consume(const std::string& value) {
    if (_tokenizer->lookahead().value == value) {
        _tokenizer->next();
    } else {
        throw std::exception();
    }
}

void Compiler::consume(const TokenType& type) {
    if (_tokenizer->lookahead().type == type) {
        _tokenizer->next();
    } else {
        throw std::exception();
    }
}

void Compiler::emit(const Command& _command) {
    uint32_t commandVal = pack(_command);
    _binary.push_back(*((char*)&commandVal + 0));
    _binary.push_back(*((char*)&commandVal + 1));
    _binary.push_back(*((char*)&commandVal + 2));
    _binary.push_back(*((char*)&commandVal + 3));
}
