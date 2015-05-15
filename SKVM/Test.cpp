//
//  Test.cpp
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#include "Test.h"

#include <vector>
#include <string>

#include "Tokenizer.h"
#include "Compiler.h"
#include "SKVM.h"

template<typename T>
void assertEquals(const T& expected, const T& actual) {
    if (expected != actual) {
        std::cout << "Failed, expected: " << expected << std::endl
                  << "          actual: " << actual   << std::endl;
    } else {
        std::cout << "Ok" << std::endl;
    }
}


using namespace std;

bool operator==(const Token& t1, const Token& t2) {
    return t1.line == t2.line &&
    t1.column == t2.column &&
    t1.type == t2.type &&
    t1.value == t2.value;
}

bool operator!=(const Token& t1, const Token& t2) {
    return !(t1 == t2);
}

ostream& operator<<(ostream& o, const Token& t) {
    o << "Token line: " << t.line
      << ", column: " << t.column
      << ", type: " << t.type
      << ", value: " << t.value;
    return o;
}

void Test::testTokenizer() {
    std::string programText =
        "MOV r1, #2\n"
        "MOV r2, #2\n"
        "ADD r0, r1, r2\n";
    
    Tokenizer tokenizer(programText);
    assertEquals(Token {1, 1, OPERATION, "MOV"}, tokenizer.next());
    assertEquals(Token {1, 5, REG_IDENTIFIER, "r1"}, tokenizer.next());
    assertEquals(Token {1, 7, COMMA, ","}, tokenizer.next());
    assertEquals(Token {1, 9, CONST_INTEGER, "#2"}, tokenizer.next());
    assertEquals(Token {1, 11, EOL, "\n"}, tokenizer.next());
    
    assertEquals(Token {2, 1, OPERATION, "MOV"}, tokenizer.next());
    assertEquals(Token {2, 5, REG_IDENTIFIER, "r2"}, tokenizer.next());
    assertEquals(Token {2, 7, COMMA, ","}, tokenizer.next());
    assertEquals(Token {2, 9, CONST_INTEGER, "#2"}, tokenizer.next());
    assertEquals(Token {2, 11, EOL, "\n"}, tokenizer.next());
    
    assertEquals(Token {3, 1, OPERATION, "ADD"}, tokenizer.next());
    assertEquals(Token {3, 5, REG_IDENTIFIER, "r0"}, tokenizer.next());
    assertEquals(Token {3, 7, COMMA, ","}, tokenizer.next());
    assertEquals(Token {3, 9, REG_IDENTIFIER, "r1"}, tokenizer.next());
    assertEquals(Token {3, 11, COMMA, ","}, tokenizer.next());
    assertEquals(Token {3, 13, REG_IDENTIFIER, "r2"}, tokenizer.next());
    assertEquals(Token {3, 15, EOL, "\n"}, tokenizer.next());
    
    assertEquals(Token {4, 1, END, ""}, tokenizer.next());
    assertEquals(Token {4, 1, END, ""}, tokenizer.next());
    assertEquals(Token {4, 1, END, ""}, tokenizer.next());
}

void Test::testSimple() {
    std::string programText =
        "MOV r1, #2"
        "MOV r2, #2"
        "ADD r0, r1, r2";
    
    Compiler compiler;
    
    std::vector<char> compiled = compiler.compile(programText);
    
    SKVM vm;
    for (int i = 0; i < compiled.size(); i++) {
        vm.setMemoryUint8(i, compiled[i]);
    }
    
    assertEquals(0u, vm.getRegisterUint32(0));
    assertEquals(0u, vm.getRegisterUint32(1));
    assertEquals(0u, vm.getRegisterUint32(2));
    
    vm.next();
    assertEquals(0u, vm.getRegisterUint32(0));
    assertEquals(2u, vm.getRegisterUint32(1));
    assertEquals(0u, vm.getRegisterUint32(2));
    
    vm.next();
    assertEquals(0u, vm.getRegisterUint32(0));
    assertEquals(2u, vm.getRegisterUint32(1));
    assertEquals(2u, vm.getRegisterUint32(2));
    
    vm.next();
    assertEquals(4u, vm.getRegisterUint32(0));
    assertEquals(2u, vm.getRegisterUint32(1));
    assertEquals(2u, vm.getRegisterUint32(2));
}
