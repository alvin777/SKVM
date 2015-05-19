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
    unsigned char r = std::stoi(_tokenizer->lookahead().value.substr(1, 1));
    consume(REG_IDENTIFIER);
    return r;
}

Operand Compiler::reg_or_imm() {
    Operand op2;
    if (_tokenizer->lookahead().type == REG_IDENTIFIER) {
        op2.offset.immediate = false;
        op2.offset.rm = std::stoi(_tokenizer->lookahead().value.substr(1, 1));
        consume(REG_IDENTIFIER);
    } else if (_tokenizer->lookahead().type == CONST_INTEGER) {
        op2.offset.immediate = true;
        op2.offset.immediate = std::stoi(_tokenizer->lookahead().value.substr(1, 1));
        consume(CONST_INTEGER);
    }
    
    return op2;
}

Command Compiler::mov() {
    Command command;
    command.opcode = MOV;
    consume("MOV");
    command.dp.rd = reg();
    consume(",");
    command.dp.op2 = reg_or_imm();
    return command;
}

Command Compiler::add() {
    Command command;
    command.opcode = ADD;
    consume("ADD");
    command.dp.rd = reg();
    consume(",");
    command.dp.rn = reg();
    consume(",");
    command.dp.op2 = reg_or_imm();
    return command;
}

Command Compiler::sub() {
    Command command;
    command.opcode = SUB;
    consume("SUB");
    command.dp.rd = reg();
    consume(",");
    command.dp.rn = reg();
    consume(",");
    command.dp.op2 = reg_or_imm();
    return command;
}

void Compiler::command() {
    Command command;
    if (_tokenizer->lookahead().value == "MOV") {
        command = mov();
    } else if (_tokenizer->lookahead().value == "ADD") {
        command = add();
    } else if (_tokenizer->lookahead().value == "SUB") {
        command = sub();
    }
    consume(EOL);
    emit(command);
}

void Compiler::program() {
    
    while (_tokenizer->lookahead().type != END) {
        command();
//        _tokenizer->consume("\n");
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
