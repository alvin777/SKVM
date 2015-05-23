//
//  Logger.h
//  SKVM
//
//  Created by Stanislav Krasnoyarov on 16/05/15.
//  Copyright (c) 2015 Redsteep. All rights reserved.
//

#pragma once

#include <string>
#include <sstream>
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
        printArg(t);
        printArg(args...);
    }
    
    template<class T>
    static void printArg(const T& t) {
        std::cout << t;
    }
};

template<class T>
static void to_str_impl(std::ostream& ss, const T& t) {
    ss << t;
}

template<class T, class ... Types>
static void to_str_impl(std::ostream& ss, const T& t, Types ... args) {
    to_str_impl(ss, t);
    to_str_impl(ss, args...);
}

template<class ... Types>
static std::string str(Types ... args) {
    std::stringstream ss;
    to_str_impl(ss, args...);
    return ss.str();
}
