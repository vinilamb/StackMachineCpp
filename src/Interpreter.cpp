#include "Interpreter.h"

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

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

Interpreter::Interpreter()
{
    //ctor
    lineNumber = 0;
}

bool ExecuteStatement(StackMachine* sm, Statement* stmt)
{
    bool result;
    if (stmt->operation == "ADD")
    {
        result = sm->Add();
    }
    else if (stmt->operation == "SUB")
    {
        result = sm->Sub();
    }
    else if (stmt->operation == "DIV")
    {
        result = sm->Div();
    }
    else if (stmt->operation == "MUL")
    {
        result = sm->Mul();
    }
    else if (stmt->operation == "MOD")
    {
        result = sm->Mod();
    }
    else if (stmt->operation == "NOT")
    {
        result = sm->Not();
    }
    else if (stmt->operation == "OR")
    {
        result = sm->Or();
    }
    else if (stmt->operation == "AND")
    {
        result = sm->Add();
    }
    else if (stmt->operation == "MIR")
    {
        result = sm->Mir();
    }
    else if (stmt->operation == "CLEAR")
    {
        result = sm->Clear();
    }
    else if (stmt->operation == "OUT")
    {
        result = sm->Out();
    }
    else if (stmt->operation == "PUSH")
    {
        word_t dado;
        if (stmt->argument == "$R")
        {
            dado = sm->$r;
        }
        else
        {
            dado = stoi(stmt->argument);
        }
        result = sm->Push(dado);
    }
    else if (stmt->operation == "POP")
    {
        result = sm->Pop();
    }
    else if (stmt->operation == "READ")
    {
        result = true;
        if (stmt->argument == "M1") sm->r_m1();
        if (stmt->argument == "M2") sm->r_m2();
        if (stmt->argument == "M3") sm->r_m3();
    }
    else if (stmt->operation == "WRITE")
    {
        result = true;
        if (stmt->argument == "M1") sm->w_m1();
        if (stmt->argument == "M2") sm->w_m2();
        if (stmt->argument == "M3") sm->w_m3();
    }
    return result;
}

void
Interpreter::ExecuteFile(string programFileName)
{
    string line;
    ifstream fileStream(programFileName);

    if (!fileStream.is_open())
    {
        cout << "N�o foi poss�vel abrir o arquivo '" << programFileName << "'.";
        return;
    }

    int lineNumber = 1;
    while (getline(fileStream, line))
    {
        Statement stmt;
        auto result = ParseLine(&stmt, line);
        if (result == ParseResult::OK)
        {
            auto execOk = ExecuteStatement(&sm, &stmt);
            if (!execOk)
            {
                cout << "Erro de execu��o na linha " << lineNumber << "!\n";
                cout << get_execute_error_msg(&sm) << "\n";
                return;
            }
        }
        else if (result != ParseResult::Comment)
        {
            // tratar erro
            cout << "Erro de sintaxe na linha " << lineNumber << "!\n";
            cout << get_parser_error_msg(result, &stmt) << "\n";
            return;
        }
        lineNumber++;
    }
}

void
Interpreter::ExecuteInteractive()
{
    cout << "Iniciando modo interativo.\n"
         << "Digite '!q' para sair.\n";

    string line;
    cout << "> ";
    while (getline(cin, line))
    {
        if (line == "!q")
        {
            cout << "Encerrando...\n";
            break;
        }

        Statement stmt;
        auto parseResult = ParseLine(&stmt, line);
        if (parseResult == ParseResult::OK)
        {
            auto execOk = ExecuteStatement(&sm, &stmt);
            if (execOk)
            {
                sm.Print();
            } else
            {
                cout << "Erro de execu��o!\n"
                     << get_execute_error_msg(&sm) << "\n";
            }
        }
        else
        {
            cout << "Erro de sintaxe!\n"
                 << get_parser_error_msg(parseResult, &stmt) << "\n";
        }

        cout << "> ";
    }
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

string get_parser_error_msg(ParseResult result, Statement* stmt)
{
    ostringstream out;

    switch (result)
    {
    case ParseResult::OK:
        return "N�o houve erro.";
    case ParseResult::BadArgument:
        out << "Instru��o '" << stmt->operation << "' n�o aceita argumento '" << stmt->argument << "'.";
        return out.str();
    case ParseResult::Comment:
        return "Linha � um coment�rio.";
    case ParseResult::MissingArgument:
        out << "Falta argumento para opera��o '" << stmt->operation << "'.";
        return out.str();
    case ParseResult::UncommentedJunk:
        return "H� caracteres n�o comentados ap�s a instru��o v�lida.";
    case ParseResult::UnknownOperation:
        out << "Opera��o n�o reconhecida '" << stmt->operation << "'.";
        return out.str();
    case ParseResult::MissingOperation:
        return "Linha deve ter caracteres alfab�ticos para opera��o.";
    default:
        return "Erro de parser desconhecido.";
    }
}

string get_execute_error_msg(StackMachine* sm)
{
    switch (sm->failReason)
    {
    case FailReason::DivisionByZero:
        return "Tentativa de divis�o por zero.";
    case FailReason::MissingArgument:
        return "Argumentos insuficientes na pilha para executar a opera��o.";
    case FailReason::PopOnStackEmpty:
        return "PUSH falhou pois a pilha est� cheia.";
    case FailReason::PushOnStackFull:
        return "POP falhou pois a pilha est� vazia.";
    default:
        return "Erro de execu��od desconhecido.";
    }
}

