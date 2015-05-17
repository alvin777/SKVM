//
//  Tokenizer.cpp
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 14/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#include "Tokenizer.h"

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

Tokenizer::Tokenizer(const std::string& text) : _text(text), _pos(0), _line(1), _column(1){
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

void Tokenizer::processNext() {
    Token token;

    if (_pos >= _text.length()) {
        token.line = _line;
        token.column = _column;
        token.type = END;
        _token = token;
        return;
    }
    
    // skip spaces
    while (_text[_pos] == ' ' || _text[_pos] == '\t') {
        _pos++;
        _column++;
    };

    token.line = _line;
    token.column = _column;

    if (_text[_pos] == 'r' && isdigit(_text[_pos + 1]) && (isspace(_text[_pos + 2]) || _text[_pos + 2] == ',')) {
        token.type = REG_IDENTIFIER;
        token.value = _text.substr(_pos, 2);
        _pos += 2;
        _column += 2;
    } else if ((_text.substr(_pos, 3) == "MOV" || _text.substr(_pos, 3) == "ADD") && isspace(_text[_pos + 3])) {
        token.type = OPERATION;
        token.value = _text.substr(_pos, 3);
        _pos += 3;
        _column += 3;
    } else if (_text[_pos] == ',') {
        token.type = COMMA;
        token.value = _text.substr(_pos, 1);;
        _pos += 1;
        _column += 1;
    } else if (_text[_pos] == '#' && isdigit(_text[_pos + 1])) {
        token.type = CONST_INTEGER;
        token.value = _text.substr(_pos, 2);;
        _pos += 2;
        _column += 2;
    } else if (_text[_pos] == '\n') {
        token.type = EOL;
        token.value = _text.substr(_pos, 1);;
        _pos += 1;
        _column = 1;
        _line++;
    } else {
        throw exception();
    }
    
    _token = token;
}