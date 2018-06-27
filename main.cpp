#include "montador.h"
#include "maquinaVirtual.h"
#include <stdio.h>
//CONSIDERACOES: WIN32 USA BIBLIO DO WINDOWS
//USA BIBLITECAS COM C++11 (TO_STRING)

using namespace std;

int main()
{
    string login, pasta, base;
    bool fim = 0;
    bool fim_prog = 0;
    string comando, operacao, operando;
    char delimiter = ' ';


    // CRIA MAQUINA VIRTUAL. NA INICIALIZACAO, O LOADER É CARREGADO
    maquinaVirtual * mv = new maquinaVirtual();

    // IDENTIFICACAO DO USUARIO
    while (fim_prog == 0){
        fim = 0;
        cout << "Digite seu login" << endl;
        cin >> login;

        pasta = "./" + login;
        // VERIFICA SE USUARIO JA EXISTE (PASTA CRIADA OU NAO)
        DWORD existe = GetFileAttributesA(pasta.c_str());
        if (existe == INVALID_FILE_ATTRIBUTES){ // pasta não existe

            // CRIAR PASTA
            base = "./" + login;
            mkdir(login.c_str());

        }
        else{ // PASTA JA EXISTE
            cout << "Digite um comando:" << endl;
            cout << "$DIR: Informa programos disponiveis" << endl;
            cout << "$DEL: Remove um programa" << endl;
            cout << "$RUN: Executa programa:" << endl;
            cout << "$END: Encerra operacao" << endl;

            while(fim == 0){
                getline(cin, comando);

                //SEPARA EM OPERACAO E OPERANDO
                //VARIAVEIS NECESSARIAS P SEPARAR
                vector <string> tokens;
                stringstream check1(comando);
                string intermediate;
                //SEPARACAO(COLOCA EM TOKENS)
                while(getline(check1, intermediate, ' '))
                {
                    tokens.push_back(intermediate);
                }
                //COLOCA EM OPERACAO E OPERANDO
                for(int i = 0; i < tokens.size(); i++){
                    if (i == 0) operacao = tokens[i];
                    if (i == 1) operando = tokens[i];
                }

                if (operacao == "DIR") {
                    // LISTA TODOS ARQUIVOS NA PASTA
                    string pastatxt = pasta + "\\*txt";

                    WIN32_FIND_DATA arquivos;
                    memset(&arquivos, 0, sizeof(WIN32_FIND_DATA));
                    HANDLE handle = FindFirstFile(pastatxt.c_str(), &arquivos);
                    while(handle != INVALID_HANDLE_VALUE)
                    {
                        cout<<"\n"<<arquivos.cFileName;
                        if(FindNextFile(handle, &arquivos) == FALSE)
                        break;
                    }
                    //Close the handle after use or memory/resource leak
                    FindClose(handle);

                    cout << endl << endl;
                }

                else if (operacao == "DEL"){
                    DeleteFile((pasta  + "/" + operando).c_str());
                  //    const int result = remove(operando.c_str());
                    //CRIA COPIA E "APAGA" ARQUIVO ORIGINAL
                   // FILLE *f = fopen(operando,w);
                   // fclose(f);
                   // DeleteFile(operando)
                }

                else if (operacao == "RUN"){
                    string programa = pasta + "/" + operando;
                    vector<string> codigoAbs;
                    //ACIONA MONTADOR
                    montador* monta = new montador();
                    codigoAbs = monta->leCodigo(programa);
cout << endl << "Montagem finalizada" << endl;
                    //CODIGO MONTADO, NO VECTOR codigoAbs. AGORA ENVIA PARA LOADER (CARREGA VECTOR E DEIXA-O PRONTO PARA SER LIDO)

                  //CARREGA PROGRAMA NA MEMORIA
                    mv->carrega(codigoAbs);

                    mv->run(codigoAbs.at(0));
                }

                else if (operacao == "END"){
                    // ENCERRA OPERACAO DO USUARIO E VOLTA PRA TELA DE LOGIN
                    fim = 1;
                    cout << endl << "Fim da sessão" << endl << endl;
                }
            }
        }

    }
return 0;
}
