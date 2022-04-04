#include "parsing.h"

using std::string;

bool op_is_valid(string opName);

ParseResult ParseLine(Statement* stmt, string programLine)
{
    stmt->operation.clear();
    stmt->argument.clear();

    int i = 0;
    while (programLine[i] == ' ') i++;

    if (programLine[i] == ';' || programLine.size() == i)
    {
        return ParseResult::Comment;
    }

    int opStart = i;
    while (isalpha(programLine[i])) i++;
    if (opStart == i)
    {
        return ParseResult::MissingOperation;
    }
    stmt->operation = programLine.substr(opStart, i - opStart);
    for (auto & c : stmt->operation) c = toupper(c);

    if (!op_is_valid(stmt->operation))
    {
        return ParseResult::UnknownOperation;
    }

    while (programLine[i] == ' ') i++;

    if (stmt->operation == "PUSH")
    {
        if (i + 1 < programLine.size()
            && programLine[i] == '$'
            && toupper(programLine[i+1]) == 'R')
        {
            stmt->argument = "$R";
            i += 2;
        }
        else if (i + 1 < programLine.size()
                 && toupper(programLine[i]) == 'M')
        {
                if (programLine[i+1] == '1')
                {
                    stmt->argument = "M1";
                    i += 2;
                }
                if (programLine[i+1] == '2')
                {
                    stmt->argument = "M2";
                    i += 2;
                }
                if (programLine[i+1] == '3')
                {
                    stmt->argument = "M3";
                    i += 2;
                }
        }
        else
        {
            int argStart = i;
            if (programLine[i] == '-')
            {
                i++;
            }
            while (isdigit(programLine[i])) i++;
            if (opStart == i)
            {
                return ParseResult::MissingArgument;
            }
            else
            {
                stmt->argument = programLine.substr(argStart, i - argStart);
            }
        }
    }
    else if (stmt->operation == "READ" || stmt->operation == "WRITE")
    {
        int argStart = i;
        while (isalnum(programLine[i])) i++;
        if (argStart == i)
            return ParseResult::MissingArgument;

        stmt->argument = programLine.substr(argStart, i - argStart);
        for (auto & c : stmt->argument) c = toupper(c);

        if (stmt->argument != "M1"
            && stmt->argument != "M2"
            && stmt->argument != "M3")
        {
            return ParseResult::BadArgument;
        }
    }
    else if (stmt->operation == "JZ")
    {
        int argStart = i;
        while (isalnum(programLine[i])) i++;
        if (argStart == i)
            return ParseResult::MissingArgument;

        stmt->argument = programLine.substr(argStart, i - argStart);
    }
    else if (stmt->operation == "JNZ")
    {
        int argStart = i;
        while (isalnum(programLine[i])) i++;
        if (argStart == i)
            return ParseResult::MissingArgument;

        stmt->argument = programLine.substr(argStart, i - argStart);
    }
    else if (stmt->operation == "JMP")
    {
        int argStart = i;
        while (isalnum(programLine[i])) i++;
        if (argStart == i)
            return ParseResult::MissingArgument;

        stmt->argument = programLine.substr(argStart, i - argStart);
    }

    while (programLine[i] == ' ') i++;
    if (i != programLine.size() && programLine[i] != ';')
    {
        return ParseResult::UncommentedJunk;
    }

    return ParseResult::OK;
}

bool op_is_valid(string opName)
{
    bool isValid = false;
    for (auto const & validOp : OPERATION_NAMES)
    {
        if (opName == validOp)
        {
            isValid = true;
            break;
        }
    }
    return isValid;
}
