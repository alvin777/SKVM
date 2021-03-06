//
//  Tokenizer.h
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 14/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#pragma once

#include <string>
#include <iostream>
#include <regex>

enum class TokenType {
    NONE,
    
    MOV,
    ADD,
    SUB,
    CMP,

    B,                  // EQ, NE, GE, GT, LE, LT
    BEQ,
    BNE,
    BGE,
    BGT,
    BLE,
    BLT,
    BL,
    
    IMMEDIATE,          // /#(\d+)/
    REGISTER,           // /r\d(\d)?/

    IDENTIFIER,         // /\w+/
    COLON,              // /:/
    COMMA,              // /,/
    WHITESPACE,         // / \t/
    COMMENT,            // /;.*$/
    EOL,                // /\n/
    END
};

struct Token {
    int line;
    int column;
    TokenType type;
    std::string value;
    int intValue;
    std::string::const_iterator it;
};

bool operator==(const Token& t1, const Token& t2);
bool operator!=(const Token& t1, const Token& t2);
std::ostream& operator<<(std::ostream& o, const Token& t);

class Tokenizer {
public:
    Tokenizer(const std::string& text);
    Token next();
    Token lookahead();
    
    std::string getUnexpectedTokenMessage();

private:
    Token _token;
    std::string _text;
    std::string::iterator _it;
    std::string::iterator _lastLineBegin;
    int _line;
    
    void processNext();
    bool matches(std::match_results<std::string::iterator>& m, const std::regex& regex);
    std::string getTokenLine(const Token& token);
};

std::ostream& operator<<(std::ostream& o, const TokenType& t);
std::ostream& operator<<(std::ostream& o, const Token& t);
