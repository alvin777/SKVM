//
//  Logger.h
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 16/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#pragma once

#include <string>
#include <iostream>

enum LogLevel {
    Verbose,
    Info,
    Warning,
    Error
};

class Logger {
public:
    template<class ... Types>
    static void log(Types ... args) {
        printArg(args...);
        std::cout << std::endl;
    };
    
    template<class T, class ... Types>
    static void printArg(const T& t, Types ... args) {
        std::cout << t;
        printArg(args...);
    }
    
    template<class T>
    static void printArg(const T& t) {
        std::cout << t;
    }
};
