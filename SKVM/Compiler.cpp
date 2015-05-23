//
//  Compiler.cpp
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#include "Compiler.h"
#include "Logger.h"

using namespace std;


// EBNF http://en.wikipedia.org/wiki/Extended_Backus%E2%80%93Naur_Form
//
// imm                = "#", digit, {digit} ;
// reg                = "r0" | "r1" | "r2" | .. | "r15" ;
// reg_or_imm         = reg | imm;
// data_transfer_op   = "MOV", reg, ",", reg_or_imm ;
// data_processing_op = "ADD", reg, ",", reg, ",", reg_or_imm ;
// command            = data_transfer_op | data_processing_op ;
// comment            = ";", text ;
// line               = ""
//                    | comment
//                    | [label, ":",] command, [comment]
// program            = line, "\n", {line, "\n"}, line, END ;

// TODO:
// - labels
// - comments
// - memory variables

unsigned char Compiler::reg() {
    unsigned char r = _tokenizer->lookahead().intValue;
    if (r < 0 || r > 15) {
        throw runtime_error(str("Invalid register index: ", r));
    }
    consume(TokenType::REGISTER);
    return r;
}

Operand Compiler::reg_or_imm() {
    Operand op2;
    if (_tokenizer->lookahead().type == TokenType::REGISTER) {
        op2.isImmediate = false;
        op2.offset.rm = reg();
    } else if (_tokenizer->lookahead().type == TokenType::IMMEDIATE) {
        op2.isImmediate = true;
        op2.offset.immediate = _tokenizer->lookahead().intValue;
        consume(TokenType::IMMEDIATE);
    }
    
    return op2;
}

template<typename T>
bool isIn(const T& t, const initializer_list<T>& list) {
    return find(list.begin(), list.end(), t) != list.end();
}

bool Compiler::isDataProcessingOperation2Ops() {
    return isIn(_tokenizer->lookahead().type, {TokenType::MOV, TokenType::CMP});
}

Command Compiler::dataProcessingOperation2Ops() {
    Command command;
    command.dp.rn = 0;
    switch (_tokenizer->lookahead().type) {
        case TokenType::MOV:
            command.opcode = OpcodeType::MOV;
            consume(TokenType::MOV);
            break;
        case TokenType::CMP:
            command.opcode = OpcodeType::CMP;
            consume(TokenType::CMP);
            break;
        default:
            throw runtime_error(str("Unexpected token: ", _tokenizer->lookahead()));
    }
    command.dp.rd = reg();
    consume(TokenType::COMMA);
    command.dp.op2 = reg_or_imm();
    return command;
}

bool Compiler::isDataProcessingOperation3Ops() {
    return isIn(_tokenizer->lookahead().type, {TokenType::ADD, TokenType::SUB});
}

Command Compiler::dataProcessingOperation3Ops() {
    Command command;
    switch (_tokenizer->lookahead().type) {
        case TokenType::ADD:
            command.opcode = OpcodeType::ADD;
            consume(TokenType::ADD);
            break;
        case TokenType::SUB:
            command.opcode = OpcodeType::SUB;
            consume(TokenType::SUB);
            break;
        case TokenType::CMP:
            command.opcode = OpcodeType::CMP;
            consume(TokenType::CMP);
            break;
        default:
            throw exception();
    }
    command.dp.rd = reg();
    consume(TokenType::COMMA);
    command.dp.rn = reg();
    consume(TokenType::COMMA);
    command.dp.op2 = reg_or_imm();
    return command;
}

void Compiler::command() {
    Command command;
    if (isDataProcessingOperation2Ops()) {
        command = dataProcessingOperation2Ops();
    } else if (isDataProcessingOperation3Ops()) {
        command = dataProcessingOperation3Ops();
    } else {
        throw runtime_error(str("Unexpected token: ", _tokenizer->lookahead()));
    }

    emit(command);
}

void Compiler::line() {
    
    if (_tokenizer->lookahead().type == TokenType::EOL) {
        consume(TokenType::EOL);
        return;
    }

    if (_tokenizer->lookahead().type == TokenType::COMMENT) {
        consume(TokenType::COMMENT);
    } else {
        if (_tokenizer->lookahead().type == TokenType::IDENTIFIER) {
            consume(TokenType::IDENTIFIER);
            consume(TokenType::COLON);
        }
        
        command();
        
        if (_tokenizer->lookahead().type == TokenType::COMMENT) {
            consume(TokenType::COMMENT);
        }
    }
}

void Compiler::program() {
    
    while (_tokenizer->lookahead().type != TokenType::END) {
        line();
        if (_tokenizer->lookahead().type != TokenType::END) {
            consume(TokenType::EOL);
        }
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
        throw runtime_error(str("Unexpected token: ", _tokenizer->lookahead()));
    }
}

void Compiler::consume(const TokenType& type) {
    if (_tokenizer->lookahead().type == type) {
        _tokenizer->next();
    } else {
        throw runtime_error(str("Unexpected token: ", _tokenizer->lookahead()));
    }
}

void Compiler::emit(const Command& _command) {
    uint32_t commandVal = pack(_command);
    _binary.push_back(*((char*)&commandVal + 0));
    _binary.push_back(*((char*)&commandVal + 1));
    _binary.push_back(*((char*)&commandVal + 2));
    _binary.push_back(*((char*)&commandVal + 3));
}
