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

#include "Logger.h"
#include "Tokenizer.h"
#include "Compiler.h"
#include "RAM.h"
#include "CPU.h"
#include "Command.h"

using namespace std;


template<typename T>
void assertEquals(const T& expected, const T& actual) {
    if (expected != actual) {
        std::cout << "Failed, expected: " << expected << std::endl
                  << "          actual: " << actual   << std::endl;
    } else {
        std::cout << "Ok" << std::endl;
    }
}

#include <bitset>
void store(std::bitset<32>& b, char high, char low, uint32_t value);

void testSandbox() {
    std::bitset<32> b;
    store(b, 31, 28, 0xF);
    cout << hex << b.to_ulong() << dec << endl;
}

void testTokenizer() {
    Logger::log("testTokenizer");
    
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

void testSimple() {
    Logger::log("testSimple");
    
    std::string programText =
        "MOV r1, #2\n"
        "MOV r2, #2\n"
        "ADD r0, r1, r2\n";
    
    Compiler compiler;
    
    std::vector<char> compiled = compiler.compile(programText);
    
    CPU cpu;
    RAM ram;
    cpu.setRAM(&ram);
    
    for (int i = 0; i < compiled.size(); i++) {
        ram.writeUint8(i, compiled[i]);
    }
    
    assertEquals(0, cpu._registers[0]);
    assertEquals(0, cpu._registers[1]);
    assertEquals(0, cpu._registers[2]);
    
    cpu.next();
    assertEquals(0, cpu._registers[0]);
    assertEquals(2, cpu._registers[1]);
    assertEquals(0, cpu._registers[2]);
    
    cpu.next();
    assertEquals(0, cpu._registers[0]);
    assertEquals(2, cpu._registers[1]);
    assertEquals(2, cpu._registers[2]);
    
    cpu.next();
    assertEquals(4, cpu._registers[0]);
    assertEquals(2, cpu._registers[1]);
    assertEquals(2, cpu._registers[2]);
}

void testFlags() {
    Logger::log("testFlags");

    std::string programText =
        "MOV r1, #2\n"
        "ADD r0, r1, #2\n"              // Z=0, C=0, V=0, N=0
        "SUB r0, r1, #2\n"              // Z=1, C=0, V=0, N=0
        "SUB r0, r1, #3\n";             // Z=0, C=0, V=0, N=1

    Compiler compiler;
    
    std::vector<char> compiled = compiler.compile(programText);
    
    CPU cpu;
    RAM ram;
    cpu.setRAM(&ram);
    
    for (int i = 0; i < compiled.size(); i++) {
        ram.writeUint8(i, compiled[i]);
    }
    
    cpu.next();
    cpu.next();
    assertEquals(4,  cpu._registers[0]);
    assertEquals(0u, cpu._statusRegister.N);
    assertEquals(0u, cpu._statusRegister.Z);
    assertEquals(0u, cpu._statusRegister.C);
    assertEquals(0u, cpu._statusRegister.V);

    cpu.next();
    assertEquals(0,  cpu._registers[0]);
    assertEquals(0u, cpu._statusRegister.N);
    assertEquals(1u, cpu._statusRegister.Z);
    assertEquals(0u, cpu._statusRegister.C);
    assertEquals(0u, cpu._statusRegister.V);

    cpu.next();
    assertEquals(-1, cpu._registers[0]);
    assertEquals(1u, cpu._statusRegister.N);
    assertEquals(0u, cpu._statusRegister.Z);
    assertEquals(0u, cpu._statusRegister.C);
    assertEquals(0u, cpu._statusRegister.V);
}

void testBranch() {
    Logger::log("testBranch");

    std::string programText =
        "MOV r0, #0\n"      // for (r1 = 0; r1 < 4; r1++) {
        "MOV r1, #0\n"      //
        "ADD r0, r0, #5\n"  //     r0 += 5;
        "ADD r1, r1, #1\n"  //
        "CMP r1, #4\n"      //
        "BLT 0x4\n"         // }
    ;
    
    Compiler compiler;
    
    std::vector<char> compiled = compiler.compile(programText);
    
    CPU cpu;
    RAM ram;
    cpu.setRAM(&ram);
    
    for (int i = 0; i < compiled.size(); i++) {
        ram.writeUint8(i, compiled[i]);
    }

    cpu.next(); // MOV r0, #0
    cpu.next(); // MOV r1, #0
    cpu.next(); // ADD r0, r0, #5
    assertEquals(5, cpu._registers[0]);
    assertEquals(0, cpu._registers[1]);

    cpu.next(); // ADD r1, r1, #1
    assertEquals(5, cpu._registers[0]);
    assertEquals(1, cpu._registers[1]);

    cpu.next(); // CMP r1, #4
    assertEquals(5,  cpu._registers[0]);
    assertEquals(1,  cpu._registers[1]);
    assertEquals(1u, cpu._statusRegister.N);
    assertEquals(0u, cpu._statusRegister.Z);
    assertEquals(1u, cpu._statusRegister.C);
    assertEquals(0u, cpu._statusRegister.V);
}

void Test::runTests() {
    testSandbox();
    testTokenizer();
    testSimple();
    testFlags();
//    testBranch();
}
