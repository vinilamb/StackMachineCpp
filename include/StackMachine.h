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
        vector<word_t> pilha; // a pilha (que na vdd é um vetor)

        int pc;     // ínice do topo da pilha
        word_t $r;  // registrador $r que guarda resultado de operações
        int lineNumber;
        // registradores de memória m1-m3
        word_t m1;
        word_t m2;
        word_t m3;

        bool z;

        FailReason failReason;

        // Estes métodos realizam as operações definidas na máquina de pilha.
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

        // memória
        // write - salva r$ no mN equivalente.
        void w_m1();
        void w_m2();
        void w_m3();
        // read - escreve mN no r$.
        void r_m1();
        void r_m2();
        void r_m3();

        // Imprime representação da máquina de pilha na tela.
        void Print();
};

#endif // STACKMACHINE_H_INCLUDED
