#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

const std::vector<std::string> OPERATION_NAMES = {
    "ADD",
    "SUB",
    "MUL",
    "DIV",
    "MOD",
    "NOT",
    "OR",
    "AND",
    "MIR",
    "CLEAR",
    "OUT",
    "PUSH",
    "POP",
    "READ",
    "WRITE",
};

class Statement
{
    public:
        std::string operation;
        std::string argument;
};

enum class ParseResult
{
    OK,
    Comment,
    MissingArgument,
    ExcessArgument,
    BadArgument,
    MissingOperation,
    UnknownOperation,
    UncommentedJunk,
};

ParseResult ParseLine(Statement* stmt, std::string programLine);

#endif // PARSER_H
