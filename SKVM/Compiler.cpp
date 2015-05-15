//
//  Compiler.cpp
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#include "Compiler.h"

#include "Command.h"

//Operand operand() {
//    Operand operand;
//    if (_parser.lookahead() == "#") {
//        operand.type = IMMEDIATE;
//        operand.value = immediate();
//    }
//    else if (_parser.lookahead() == "[") {
//        operand.type = INDIRECT;
//        consume("[");
//        register();
//        consume("]");
//        
//    }
//    else if (isRegister(_parser.lookahead())) {
//        operand.type = REGISTER;
//        register();
//    }
//}
//
//void mov() {
//    Command command;
//    _parser.consume("MOV");
//    command.op1 = operand();
//    _parser.consume(",");
//    command.op2 = operand();
//    return command;
//}
//
//void add() {
//    
//}
//
//void command() {
//    Command command;
//    if (_parser.lookahead() == "MOV") {
//        command = mov();
//    } else if (_parser.lookahead() == "ADD") {
//        command = add();
//    }
//    emit(command);
//}
//
//void program() {
//    while (_parser.lookahead() == COMMAND) {
//        command();
//        _parser.consume("\n");
//    }
//}

std::vector<char> Compiler::compile(const std::string& text) {
    
//    program();
    
    
    return std::vector<char>();
}
