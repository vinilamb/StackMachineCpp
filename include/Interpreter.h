#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <StackMachine.h>
#include <vector>
#include <string>

const vector<string> OPERATION_NAMES = {
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

bool op_is_valid(string opName);

class Statement
{
    public:
        string operation;
        string argument;
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

ParseResult ParseLine(Statement* stmt, string programLine);

bool ExecuteStatement(StackMachine* sm, Statement* stmt);

class Interpreter
{
    public:
        Interpreter();

        StackMachine sm;

        void ExecuteFile(string programFileName);
        void ExecuteInteractive();
    private:
        int lineNumber;
};

string get_parser_error_msg(ParseResult result, Statement* stmt);
string get_execute_error_msg(StackMachine* sm);

#endif // INTERPRETER_H
