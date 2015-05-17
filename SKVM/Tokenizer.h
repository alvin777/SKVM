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

enum TokenType {
    NONE,
    OPERATION,          // /MOV|ADD/i
    REG_IDENTIFIER,     // /r\d(\d)?/
    CONST_INTEGER,      // /#\d+/
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
};

bool operator==(const Token& t1, const Token& t2);
bool operator!=(const Token& t1, const Token& t2);
std::ostream& operator<<(std::ostream& o, const Token& t);

class Tokenizer {
public:
    Tokenizer(const std::string& text);
    Token next();
    Token lookahead();

private:
    Token _token;
    std::string _text;
    int _pos;
    int _line;
    int _column;
    
    void processNext();
};