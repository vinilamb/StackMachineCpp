#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>

#include "StackMachine.h"
#include "Interpreter.h"

using namespace std;

int main3(int argc, char *argv[])
{

}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, "Portuguese");
    Interpreter it;
    it.ExecuteFile("primen.txt");
    //it.ExecuteInteractive();
}

int main2(int argc, char *argv[]){
    setlocale(LC_ALL, "Portuguese");

    __int16 pilha[128], $R;
    int exit = 0, lineId = 0, PC = -1;
    string curLine, lineBuff, tag, par;
    vector <string> tags;
    vector <string> symbols;
    vector <int> values;
    ifstream script(argv[1]);

    //Abertur do arquivo de entrada
    if(!script.is_open()){
        cout << "Erro ao abrir arquivo.\n";

        return -1;
    }

    istringstream ss(lineBuff);

    //Loop principal de execução
    while(exit == 0){
        //Testa para o fim do arquivo
        if(script.peek() == EOF){
            cout << "Fim do programa.\n";
            break;
        }

        lineId++;
        //Limpa a tag e o parâmetro
        tags.clear();

        //Lê a próxima linha do arquivo
        getline(script, lineBuff);
        //Joga fora quaisquer comentários
        ss.str("");
        ss.clear();
        ss.str(lineBuff);
        getline(ss, curLine, ';');

        ss.str("");
        ss.clear();
        ss.str(curLine);


        //Separa a string resultante em tags, sendo a primeira a operação e os seguintes os parâmetros
        while(getline(ss, tag, ' ')){
            tags.push_back(tag);
        }

        //Caso haja uma instrução, procura por ela na lista de instruções
        if(tags.size()){
            transform(tags[0].begin(), tags[0].end(), tags[0].begin(), ::toupper);
            //Operações aritméticas
            if(tags[0] == "ADD"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. Não é possível realizar a operação ADD sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais parâmetros excessivos. A operação ADD possui 0 (zero) parâmetros.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC] + pilha[PC - 1];
                    PC = PC - 2;
                }
            }
            else if(tags[0] == "SUB"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. Não é possível realizar a operação SUB sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais parâmetros excessivos. A operação SUB possui 0 (zero) parâmetros.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC] - pilha[PC - 1];
                    PC = PC - 2;
                }
            }
            else if(tags[0] == "MUL"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. Não é possível realizar a operação MUL sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais parâmetros excessivos. A operação MUL possui 0 (zero) parâmetros.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC]*pilha[PC - 1];
                    PC = PC - 2;
                }
            }
            else if(tags[0] == "DIV"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. Não é possível realizar a operação DIV sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais parâmetros excessivos. A operação DIV possui 0 (zero) parâmetros.\n";
                    exit = 1;
                }
                else{
                    if (pilha[PC - 1] == 0)
                    {
                        cout << "Linha " << lineId << " - Erro. Divisão por 0. \n";
                        exit = 1;
                    }
                    else
                    {
                    $R = pilha[PC]/pilha[PC - 1];
                    PC = PC - 2;
                    }
                }
            }
            else if(tags[0] == "MOD"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. Não é possível realizar a operação MOD sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais parâmetros excessivos. A operação MOD possui 0 (zero) parâmetros.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC]%pilha[PC - 1];
                    PC = PC - 2;
                }
            }
            //Operações lógicas
            else if(tags[0] == "NOT"){
                if(PC == -1){
                    cout << "Linha " << lineId << " - Erro. Não é possível realizar a operação NOT sobre uma pilha com zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() >1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais parâmetros excessivos. A operação NOT possui 0 (zero) parâmetros.\n";
                    exit = 1;
                }
                else{
                    $R = ~pilha[PC];
                    PC = PC - 1;
                }
            }
            else if(tags[0] == "OR"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. Não é possível realizar a operação OR sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() >1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais parâmetros excessivos. A operação OR possui 0 (zero) parâmetros.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC] | pilha[PC - 1];
                    PC = PC - 2;
                }
            }
            else if(tags[0] == "AND"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. Não é possível realizar a operação AND sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() >1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais parâmetros excessivos. A operação AND possui 0 (zero) parâmetros.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC] & pilha[PC - 1];
                    PC = PC - 2;
                }
            }
            else if(tags[0] == "MIR"){
                if(PC == -1){
                    cout << "Linha " << lineId << " - Erro. Não é possível realizar a operação MIR sobre uma pilha com zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() >1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais parâmetros excessivos. A operação MIR possui 0 (zero) parâmetros.\n";
                    exit = 1;
                }
                else{
                    pilha[PC] = ((pilha[PC] & 0x5555) << 1) | ((pilha[PC] & 0xAAAA) >> 1);
                    pilha[PC] = ((pilha[PC] & 0x3333) << 2) | ((pilha[PC] & 0xCCCC) >> 2);
                    pilha[PC] = ((pilha[PC] & 0x0F0F) << 4) | ((pilha[PC] & 0xF0F0) >> 4);
                    pilha[PC] = ((pilha[PC] & 0x00FF) << 8) | ((pilha[PC] & 0xFF00) >> 8);
                    $R = pilha[PC];
                    PC = PC - 1;
                }
            }
            //Operações de controle
            else if(tags[0] == "PUSH"){
                if(PC == 127){
                    cout << "Linha " << lineId << " - Erro. Não é possível inserir valores com a operação PUSH em uma pilha cheia (o tamanho máximo da pilha é de 128 valores).\n";
                    exit = 1;
                }
                else if(tags.size() != 2){
                    cout << "Linha " << lineId << " - Erro. Número incorreto de parâmetros. A operação PUSH possui 1 (um) parâmetro.\n";
                    exit = 1;
                }
                else{
                    if((tags[1][0] == '-' && all_of(tags[1].begin() + 1, tags[1].end(), ::isdigit)) || all_of(tags[1].begin(), tags[1].end(), ::isdigit)){
                        if(stoi(tags[1]) >= -32768 && stoi(tags[1]) <= 32767){
                            PC++;
                            pilha[PC] = stoi(tags[1]);
                        }
                        else{
                            cout << "Linha " << lineId << " - Erro. Número passado como parâmetro excede a capacidade de representação da máquina ([-32768, 32767]).\n";
                            exit = 1;
                        }
                    }
                    else if(tags[1] == "$R"){
                        PC++;
                        pilha[PC] = $R;
                    }
                    else{
                        string sym;
                        sym.resize(tags[1].size());
                        transform(tags[1].begin(), tags[1].end(), sym.begin(), ::toupper);
                        if (count(symbols.begin(), symbols.end(), sym) > 0)
                        {
                            PC++;
                            int pos;
                            for (int i = 0; i < symbols.size(); i++)
                            {
                                if (sym.compare(symbols[i]) == 0)
                                {
                                    pos = i;
                                    break;
                                }
                            }
                            pilha[PC] = values[pos];
                        }
                        else
                        {
                            cout << "Linha " << lineId << " - Erro. O parâmetro " << sym << " é um parâmetro inválido.\n";
                            exit = 1;
                        }
                    }
                }
            }
            else if(tags[0] == "POP"){
                if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. Número incorreto de parâmetros. A operação POP possui 0 (zero) parâmetros.\n";
                    exit = 1;
                }
                else if(PC == -1){
                    cout << "Linha " << lineId << " - Erro. Não é possível remover de uma pilha vazia.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC];
                    PC--;
                }
            }
            else if(tags[0] == "CLEAR"){
                if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. Número incorreto de parâmetros. A operação CLEAR possui 0 (zero) parâmetros.\n";
                    exit = 1;
                }
                else{
                    PC = -1;
                }
            }
            //Operações de entrada e saída
            else if(tags[0] == "OUT"){
                if(PC == -1){
                    cout << "Linha " << lineId << " - Erro. Não é possível mostrar o topo de uma pilha vazia.\n";
                    exit = 1;
                }
                else{
                    cout << pilha[PC] << "\n";
                }
            }
            else{
                if (tags.size() > 2 && tags[1][0] == '=')
                {
                    if (count(symbols.begin(), symbols.end(), tags[0]) == 0)
                    {
                        symbols.push_back(tags[0]);
                        values.push_back(stoi(tags[2]));
                    }
                    else
                    {
                        cout << "Linha " << lineId << " - Erro. Símbolo " << tags[0] << " já declarado.\n";
                        exit = 1;
                    }
                }
                else
                {
                    cout << "Linha " << lineId << " - Erro. A operação " << tags[0] << " é inválida.\n";
                    exit = 1;
                }
            }
        }
    }


    script.close();

    return 0;
}
