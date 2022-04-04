#include "Interpreter.h"

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

bool op_is_valid(string opName);

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
        else if (stmt->argument == "M1")
        {
            dado = sm->m1;
        }
        else if (stmt->argument == "M2")
        {
            dado = sm->m2;
        }
        else if (stmt->argument == "M3")
        {
            dado = sm->m3;
        }
        else
        {
            dado = stoi(stmt->argument);
        }
        result = sm->Push(dado);
    }
    else if (stmt->operation == "POP")
    {
        result = sm->Pop_$r();
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
        cout << "Não foi possível abrir o arquivo '" << programFileName << "'.";
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
                cout << "Erro de execução na linha " << lineNumber << "!\n";
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
                cout << "Erro de execução!\n"
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

string get_parser_error_msg(ParseResult result, Statement* stmt)
{
    ostringstream out;

    switch (result)
    {
    case ParseResult::OK:
        return "Não houve erro.";
    case ParseResult::BadArgument:
        out << "Instrução '" << stmt->operation << "' não aceita argumento '" << stmt->argument << "'.";
        return out.str();
    case ParseResult::Comment:
        return "Linha é um comentário.";
    case ParseResult::MissingArgument:
        out << "Falta argumento para operação '" << stmt->operation << "'.";
        return out.str();
    case ParseResult::UncommentedJunk:
        return "Há caracteres não comentados após a instrução válida.";
    case ParseResult::UnknownOperation:
        out << "Operação não reconhecida '" << stmt->operation << "'.";
        return out.str();
    case ParseResult::MissingOperation:
        return "Linha deve ter caracteres alfabéticos para operação.";
    default:
        return "Erro de parser desconhecido.";
    }
}

string get_execute_error_msg(StackMachine* sm)
{
    switch (sm->failReason)
    {
    case FailReason::DivisionByZero:
        return "Tentativa de divisão por zero.";
    case FailReason::MissingArgument:
        return "Argumentos insuficientes na pilha para executar a operação.";
    case FailReason::PopOnStackEmpty:
        return "PUSH falhou pois a pilha está cheia.";
    case FailReason::PushOnStackFull:
        return "POP falhou pois a pilha está vazia.";
    default:
        return "Erro de execuçãod desconhecido.";
    }
}

