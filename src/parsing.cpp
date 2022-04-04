#include "parsing.h"

using std::string;

bool op_is_valid(string opName);

ParseResult ParseLine(Statement* stmt, string programLine)
{
    stmt->operation.clear();
    stmt->argument.clear();

    int i = 0;
    while (programLine[i] == ' ') i++;

    if (programLine[i] == ';')
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
        else
        {
            int argStart = i;
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