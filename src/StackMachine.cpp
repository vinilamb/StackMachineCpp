#include "StackMachine.h"
#include "Interpreter.h"
#include <iostream>

StackMachine::StackMachine()
{
    pc = $r = m1 = m2 = m3 = 0;
    pc = -1;
}

bool
StackMachine::Push(word_t dado)
{
    if (pilha.size() >= 128)
    {
        failReason = FailReason::PushOnStackFull;
        return false;
    }
    pilha.push_back(dado);
    pc++;
    return true;
}

bool
StackMachine::Pop()
{
    if (pilha.empty())
    {
        failReason = FailReason::PopOnStackEmpty;
        return false;
    }
    pilha.pop_back();
    pc--;
    return true;
}

bool
StackMachine::Add()
{
    if (pilha.size() < 2)
    {
        failReason = FailReason::MissingArgument;
        return false;
    }
    $r = pilha[pc] + pilha[pc - 1];

    Pop();
    Pop();
    return true;
}

bool
StackMachine::Sub()
{
    if (pilha.size() < 2)
    {
        failReason = FailReason::MissingArgument;
        return false;
    }
    $r = pilha[pc] - pilha[pc - 1];

    Pop();
    Pop();
    return true;
}

bool
StackMachine::Mul()
{
    if (pilha.size() < 2)
    {
        failReason = FailReason::MissingArgument;
        return false;
    }
    $r = pilha[pc] * pilha[pc - 1];

    Pop();
    Pop();
    return true;
}

bool
StackMachine::Div()
{
    if (pilha.size() < 2)
    {
        failReason = FailReason::MissingArgument;
        return false;
    }
    else if (pilha[pc-1] == 0)
    {
        failReason = FailReason::DivisionByZero;
        return false;
    }
    $r = pilha[pc] / pilha[pc - 1];

    Pop();
    Pop();
    return true;
}

bool
StackMachine::Mod()
{
    if (pilha.size() < 2)
    {
        failReason = FailReason::MissingArgument;
        return false;
    }
    $r = pilha[pc] % pilha[pc - 1];

    Pop();
    Pop();
    return true;
}

bool
StackMachine::Not()
{
    if (pilha.empty())
    {
        failReason = FailReason::MissingArgument;
        return false;
    }
    $r = ~pilha[pc];

    Pop();
    return true;
}

bool
StackMachine::Or()
{
    if (pilha.size() < 2)
    {
        failReason = FailReason::MissingArgument;
        return false;
    }
    $r = pilha[pc] | pilha[pc - 1];

    Pop();
    Pop();
    return true;
}

bool
StackMachine::And()
{
    if (pilha.size() < 2)
    {
        failReason = FailReason::MissingArgument;
        return false;
    }
    $r = pilha[pc] | pilha[pc - 1];

    Pop();
    Pop();
    return true;
}

bool
StackMachine::Mir()
{
    if (pilha.empty())
    {
        failReason = FailReason::MissingArgument;
        return false;
    }
    word_t word = pilha[pc];
    word = ((word & 0x5555) << 1) | ((word & 0xAAAA) >> 1);
    word = ((word & 0x3333) << 2) | ((word & 0xCCCC) >> 2);
    word = ((word & 0x0F0F) << 4) | ((word & 0xF0F0) >> 4);
    word = ((word & 0x00FF) << 8) | ((word & 0xFF00) >> 8);
    $r = word;

    Pop();
    return true;
}

bool
StackMachine::Clear()
{
    pilha.clear();
    pc = -1;
}

bool
StackMachine::Out()
{
    if (pilha.empty())
    {
        failReason = FailReason::MissingArgument;
        return false;
    }

    cout << pilha[pc] << '\n';
    return true;
}

bool
StackMachine::Pop_$r()
{
    if (pilha.empty())
    {
        failReason = FailReason::MissingArgument;
        return false;
    }

    $r = pilha[pc];
    Pop();
    return true;
}

bool
StackMachine::Jz(int line)
{
    if ($r == 0)
        lineNumber = line - 2;
    return true;
}

bool
StackMachine::Jnz(int line)
{
    if ($r != 0)
        lineNumber = line - 2;
    return true;
}

bool
StackMachine::Jmp(int line)
{
    lineNumber = line - 2;
    return true;
}

void StackMachine::w_m1() { m1 = $r; }
void StackMachine::w_m2() { m2 = $r; }
void StackMachine::w_m3() { m3 = $r; }

void StackMachine::r_m1() { $r = m1; }
void StackMachine::r_m2() { $r = m2; }
void StackMachine::r_m3() { $r = m3; }

void
StackMachine::Print()
{
    cout << "$r: " << $r << ", m1: " << m1 << ", m2: " << m2 << ", m3: " << m3 << ", pc: " << pc << "\n" ;
    if (!pilha.empty())
    {
        for (int i = 0; i < pilha.size(); i++)
        {
            cout << i << ": " << pilha[i] << "\n";
        }
    }
    else
    {
        cout << "A pilha está vazia!\n";
    }
}
