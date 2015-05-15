//
//  Compiler.h
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 13/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#pragma once

#include <vector>

class Compiler {
public:
    std::vector<char> compile(const std::string& text);
};