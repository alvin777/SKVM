//
//  Tokenizer.cpp
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 14/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#include "Tokenizer.h"

#include <vector>

using namespace std;

bool operator==(const Token& t1, const Token& t2) {
    return t1.line == t2.line &&
           t1.column == t2.column &&
           t1.type == t2.type &&
           t1.value == t2.value &&
           t1.intValue == t2.intValue;
}

bool operator!=(const Token& t1, const Token& t2) {
    return !(t1 == t2);
}

ostream& operator<<(ostream& o, const TokenType& t) {
    switch (t) {
        case TokenType::NONE:       return o << "NONE";
        case TokenType::MOV:        return o << "MOV";
        case TokenType::ADD:        return o << "ADD";
        case TokenType::SUB:        return o << "SUB";
        case TokenType::CMP:        return o << "CMP";
        case TokenType::B:          return o << "B";
        case TokenType::BEQ:        return o << "BEQ";
        case TokenType::BNE:        return o << "BNE";
        case TokenType::BGE:        return o << "BGE";
        case TokenType::BGT:        return o << "BGT";
        case TokenType::BLE:        return o << "BLE";
        case TokenType::BLT:        return o << "BLT";
        case TokenType::BL:         return o << "BL";
        case TokenType::IMMEDIATE:  return o << "IMMEDIATE";
        case TokenType::REGISTER:   return o << "REGISTER";
        case TokenType::IDENTIFIER: return o << "IDENTIFIER";
        case TokenType::COLON:      return o << "COLON";
        case TokenType::COMMA:      return o << "COMMA";
        case TokenType::WHITESPACE: return o << "WHITESPACE";
        case TokenType::COMMENT:    return o << "COMMENT";
        case TokenType::EOL:        return o << "EOL";
        case TokenType::END:        return o << "END";
    }
}

ostream& operator<<(ostream& o, const Token& t) {
    o << "Token line: " << t.line
      << ", column: "   << t.column
      << ", type: "     << t.type;

    if (!t.value.empty()) {
        o << ", value: "    << t.value;
    }
    
    return o;
}

Tokenizer::Tokenizer(const std::string& text) :
    _text(text), _line(1),
    _it(_text.begin()), _lastLineBegin(_text.begin())
{
    processNext();
}

Token Tokenizer::next() {
    Token temp = _token;
    
    processNext();
    
    return temp;
}

Token Tokenizer::lookahead() {
    return _token;
}

bool Tokenizer::matches(std::match_results<std::string::iterator>& m, const std::regex& regex) {
    return regex_search(_it, _text.end(), m, regex, regex_constants::match_continuous);
}

void Tokenizer::processNext() {

    if (_it >= _text.end()) {
        Token token;
        token.line = _line;
        token.column = static_cast<typeof(token.column)>(distance(_lastLineBegin, _it)) + 1;;
        token.type = TokenType::END;
        token.it = _text.end();
        token.intValue = 0;
        token.value = "";
        _token = token;
        return;
    }
    
    match_results<string::iterator> m;
    
    if (matches(m, regex {"[ \\t]"})) {
        _it = m[0].second;
    }
    
    Token token;
    token.line = _line;
    token.column = static_cast<typeof(token.column)>(distance(_lastLineBegin, _it)) + 1;
    token.it = _it;
    token.intValue = 0;
    token.value = "";

    if (matches(m, regex {"MOV"})) {
        token.type = TokenType::MOV;
    } else if (matches(m, regex {"ADD"})) {
        token.type = TokenType::ADD;
    } else if (matches(m, regex {"SUB"})) {
        token.type = TokenType::SUB;
    } else if (matches(m, regex {"CMP"})) {
        token.type = TokenType::CMP;
    } else if (matches(m, regex {"BEQ"})) {
        token.type = TokenType::BEQ;
    } else if (matches(m, regex {"BNE"})) {
        token.type = TokenType::BNE;
    } else if (matches(m, regex {"BGE"})) {
        token.type = TokenType::BGE;
    } else if (matches(m, regex {"BGT"})) {
        token.type = TokenType::BGT;
    } else if (matches(m, regex {"BLE"})) {
        token.type = TokenType::BLE;
    } else if (matches(m, regex {"BLT"})) {
        token.type = TokenType::BLT;
    } else if (matches(m, regex {"B"})) {
        token.type = TokenType::B;
    } else if (matches(m, regex {"r(\\d\\d?)"})) {
        token.type = TokenType::REGISTER;
        token.intValue = stoi(m[1].str());
    } else if (matches(m, regex {","})) {
        token.type = TokenType::COMMA;
    } else if (matches(m, regex {":"})) {
        token.type = TokenType::COLON;
    } else if (matches(m, regex {"#(-?\\d+)"})) {
        token.type = TokenType::IMMEDIATE;
        token.intValue = stoi(m[1].str());
    } else if (matches(m, regex {"(\\w+)"})) {
        token.type = TokenType::IDENTIFIER;
        token.value = m[1].str();
    } else if (matches(m, regex {";.*"})) {
        token.type = TokenType::COMMENT;
    } else if (matches(m, regex {"\n"})) {
        token.type = TokenType::EOL;
        _lastLineBegin = m[0].second;
        _line++;
    } else {
        throw exception();
    }

    _it = m[0].second;
    
    _token = token;
}

string Tokenizer::getTokenLine(const Token& token) {
    auto before = _text.rfind('\n', distance(_text.cbegin(), token.it));
    auto after = _text.find('\n', distance(_text.cbegin(), token.it));
    return _text.substr(before, after - before);
}

string Tokenizer::getUnexpectedTokenMessage() {
    string tokenLine = getTokenLine(_token);
    tokenLine += '\n';
    tokenLine += string(_token.column - 1, ' ');
    tokenLine += "^ Unexpected token at line: " + to_string(_token.line) + " column: " + to_string(_token.column);
    return tokenLine;
}
