#ifndef STACKMACHINE_H_INCLUDED
#define STACKMACHINE_H_INCLUDED

#include <vector>
#include <string>

using namespace std;

typedef int word_t;

enum class FailReason
{
    MissingArgument,
    DivisionByZero,
    PushOnStackFull,
    PopOnStackEmpty,
};

class StackMachine {
    public:
        StackMachine();
        vector<word_t> pilha; // a pilha (que na vdd � um vetor)

        int pc;     // �nice do topo da pilha
        word_t $r;  // registrador $r que guarda resultado de opera��es
        int lineNumber;
        // registradores de mem�ria m1-m3
        word_t m1;
        word_t m2;
        word_t m3;

        bool z;

        FailReason failReason;

        // Estes m�todos realizam as opera��es definidas na m�quina de pilha.
        bool Push(word_t dado);
        bool Pop();
        bool Add();
        bool Sub();
        bool Mul();
        bool Div();
        bool Mod();
        bool Not();
        bool Or();
        bool And();
        bool Mir();
        bool Clear();
        bool Out();
        bool Pop_$r();
        bool Jz(int line);

        // mem�ria
        // write - salva r$ no mN equivalente.
        void w_m1();
        void w_m2();
        void w_m3();
        // read - escreve mN no r$.
        void r_m1();
        void r_m2();
        void r_m3();

        // Imprime representa��o da m�quina de pilha na tela.
        void Print();
};

#endif // STACKMACHINE_H_INCLUDED
