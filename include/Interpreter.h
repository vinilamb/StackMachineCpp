#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <StackMachine.h>
#include <vector>
#include <string>
#include "parsing.h"

bool ExecuteStatement(StackMachine* sm, Statement* stmt);

void changeLineNumber(int ln);

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
