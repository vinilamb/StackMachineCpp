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

    //Loop principal de execu��o
    while(exit == 0){
        //Testa para o fim do arquivo
        if(script.peek() == EOF){
            cout << "Fim do programa.\n";
            break;
        }

        lineId++;
        //Limpa a tag e o par�metro
        tags.clear();

        //L� a pr�xima linha do arquivo
        getline(script, lineBuff);
        //Joga fora quaisquer coment�rios
        ss.str("");
        ss.clear();
        ss.str(lineBuff);
        getline(ss, curLine, ';');

        ss.str("");
        ss.clear();
        ss.str(curLine);


        //Separa a string resultante em tags, sendo a primeira a opera��o e os seguintes os par�metros
        while(getline(ss, tag, ' ')){
            tags.push_back(tag);
        }

        //Caso haja uma instru��o, procura por ela na lista de instru��es
        if(tags.size()){
            transform(tags[0].begin(), tags[0].end(), tags[0].begin(), ::toupper);
            //Opera��es aritm�ticas
            if(tags[0] == "ADD"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. N�o � poss�vel realizar a opera��o ADD sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais par�metros excessivos. A opera��o ADD possui 0 (zero) par�metros.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC] + pilha[PC - 1];
                    PC = PC - 2;
                }
            }
            else if(tags[0] == "SUB"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. N�o � poss�vel realizar a opera��o SUB sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais par�metros excessivos. A opera��o SUB possui 0 (zero) par�metros.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC] - pilha[PC - 1];
                    PC = PC - 2;
                }
            }
            else if(tags[0] == "MUL"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. N�o � poss�vel realizar a opera��o MUL sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais par�metros excessivos. A opera��o MUL possui 0 (zero) par�metros.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC]*pilha[PC - 1];
                    PC = PC - 2;
                }
            }
            else if(tags[0] == "DIV"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. N�o � poss�vel realizar a opera��o DIV sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais par�metros excessivos. A opera��o DIV possui 0 (zero) par�metros.\n";
                    exit = 1;
                }
                else{
                    if (pilha[PC - 1] == 0)
                    {
                        cout << "Linha " << lineId << " - Erro. Divis�o por 0. \n";
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
                    cout << "Linha " << lineId << " - Erro. N�o � poss�vel realizar a opera��o MOD sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais par�metros excessivos. A opera��o MOD possui 0 (zero) par�metros.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC]%pilha[PC - 1];
                    PC = PC - 2;
                }
            }
            //Opera��es l�gicas
            else if(tags[0] == "NOT"){
                if(PC == -1){
                    cout << "Linha " << lineId << " - Erro. N�o � poss�vel realizar a opera��o NOT sobre uma pilha com zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() >1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais par�metros excessivos. A opera��o NOT possui 0 (zero) par�metros.\n";
                    exit = 1;
                }
                else{
                    $R = ~pilha[PC];
                    PC = PC - 1;
                }
            }
            else if(tags[0] == "OR"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. N�o � poss�vel realizar a opera��o OR sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() >1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais par�metros excessivos. A opera��o OR possui 0 (zero) par�metros.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC] | pilha[PC - 1];
                    PC = PC - 2;
                }
            }
            else if(tags[0] == "AND"){
                if(PC == -1 || PC == 0){
                    cout << "Linha " << lineId << " - Erro. N�o � poss�vel realizar a opera��o AND sobre uma pilha com um ou zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() >1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais par�metros excessivos. A opera��o AND possui 0 (zero) par�metros.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC] & pilha[PC - 1];
                    PC = PC - 2;
                }
            }
            else if(tags[0] == "MIR"){
                if(PC == -1){
                    cout << "Linha " << lineId << " - Erro. N�o � poss�vel realizar a opera��o MIR sobre uma pilha com zero valores.\n";
                    exit = 1;
                }
                else if(tags.size() >1){
                    cout << "Linha " << lineId << " - Erro. Um ou mais par�metros excessivos. A opera��o MIR possui 0 (zero) par�metros.\n";
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
            //Opera��es de controle
            else if(tags[0] == "PUSH"){
                if(PC == 127){
                    cout << "Linha " << lineId << " - Erro. N�o � poss�vel inserir valores com a opera��o PUSH em uma pilha cheia (o tamanho m�ximo da pilha � de 128 valores).\n";
                    exit = 1;
                }
                else if(tags.size() != 2){
                    cout << "Linha " << lineId << " - Erro. N�mero incorreto de par�metros. A opera��o PUSH possui 1 (um) par�metro.\n";
                    exit = 1;
                }
                else{
                    if((tags[1][0] == '-' && all_of(tags[1].begin() + 1, tags[1].end(), ::isdigit)) || all_of(tags[1].begin(), tags[1].end(), ::isdigit)){
                        if(stoi(tags[1]) >= -32768 && stoi(tags[1]) <= 32767){
                            PC++;
                            pilha[PC] = stoi(tags[1]);
                        }
                        else{
                            cout << "Linha " << lineId << " - Erro. N�mero passado como par�metro excede a capacidade de representa��o da m�quina ([-32768, 32767]).\n";
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
                            cout << "Linha " << lineId << " - Erro. O par�metro " << sym << " � um par�metro inv�lido.\n";
                            exit = 1;
                        }
                    }
                }
            }
            else if(tags[0] == "POP"){
                if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. N�mero incorreto de par�metros. A opera��o POP possui 0 (zero) par�metros.\n";
                    exit = 1;
                }
                else if(PC == -1){
                    cout << "Linha " << lineId << " - Erro. N�o � poss�vel remover de uma pilha vazia.\n";
                    exit = 1;
                }
                else{
                    $R = pilha[PC];
                    PC--;
                }
            }
            else if(tags[0] == "CLEAR"){
                if(tags.size() > 1){
                    cout << "Linha " << lineId << " - Erro. N�mero incorreto de par�metros. A opera��o CLEAR possui 0 (zero) par�metros.\n";
                    exit = 1;
                }
                else{
                    PC = -1;
                }
            }
            //Opera��es de entrada e sa�da
            else if(tags[0] == "OUT"){
                if(PC == -1){
                    cout << "Linha " << lineId << " - Erro. N�o � poss�vel mostrar o topo de uma pilha vazia.\n";
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
                        cout << "Linha " << lineId << " - Erro. S�mbolo " << tags[0] << " j� declarado.\n";
                        exit = 1;
                    }
                }
                else
                {
                    cout << "Linha " << lineId << " - Erro. A opera��o " << tags[0] << " � inv�lida.\n";
                    exit = 1;
                }
            }
        }
    }


    script.close();

    return 0;
}
