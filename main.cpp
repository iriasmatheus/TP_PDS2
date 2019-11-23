#include <cmath>
#include <dirent.h>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using namespace std;
//FUNÇÃO QUE TIRA OS CARACTERES ESPECIAIS E CONVERTE PRA MINUSCULO
string converte(string palavra) {
    int caract=1;
    while(caract){
        caract=0;
        for(int i=0;i<palavra.size();i++){
            palavra[i]=towlower(palavra[i]);
            if(palavra[i]=='-'  || palavra[i]=='.' || palavra[i]=='?' || palavra[i]==' ' || palavra[i]==',' || palavra[i]=='´'){
                caract=1;
                for(int j=i;j<palavra.size();j++){
                    palavra[j]=palavra[j+1];
                }
            }
        }
    }
    return palavra;
}

double similar(double* x, double* y, int tam) { //X É O W DOCUMENTO/PALAVRA (um doc por chamada) E Y É O W QUERY/PALAVRA
    double num = 0;
    double denom1 = 0;
    double denom2 = 0;
    int i;
    for(i=0;i<tam;i++) {
        num = num + (x[i] * y[i]);
        denom1 = denom1 + (x[i] * x[i]);
        denom2 = denom2 + (y[i] * y[i]);
    }
    denom1 = sqrt(denom1);
    denom2 = sqrt(denom2);
    return num/(denom1 * denom2);
}
//FUNÇÃO DO INDICE INVERTIDO
//FUNÇÃO QUE ABRE CADA ARQUIVO E INSERE CADA PALAVRA EM UM SET
void Inv(vector<string> &palavras,string filename, set<string> &vocabulario){
    try{
        fstream file;
        filename = "./docs/" + filename;
        file.open(filename.c_str(), fstream::in | fstream::out);
        string palavra;
        if(file.is_open()){
            while(file >> palavra){
                palavra=converte(palavra);
                palavras.push_back(palavra);
                vocabulario.insert(palavra); //hehehers
            }
            file.close();
        }
        else{
            throw 0;
        }

    }catch(int v){
        cout<<"Arquivo nao encontrado!"<<endl;
    }
}

double calcula_tf(vector<string> &palavras, string word) {
    int i = 0;
    vector<string>:: iterator it;
    for(it=palavras.begin();it!=palavras.end();it++) {
        if(word==*it)
            i++;
    }
    return i;
}

int main(void)
{
    //TESTE DO INDICE INVERTIDO
    DIR *dir;
    struct dirent *ent;
    unsigned int i, j;
    double maior;
    string arquivo;
    string query;
    vector<double>sim;
    vector<string> docs;
    vector<string> palavras;
    vector<string> palavras_chave;
    vector<string>::iterator vet_it;
    vector<string>::iterator it_vet;
    vector<double>::iterator itera;
    set<string> vocabulario;
    set<string>::iterator set_it;
    map<double, string> similaridade;
    map<double, string>::iterator map_it;
    if ((dir = opendir (".\\docs")) != NULL) {
    /* print all the files and directories within directory */
      while ((ent = readdir (dir)) != NULL) {
        arquivo = ent->d_name;
        if((arquivo.size()>2)&&(arquivo.substr(arquivo.size() - 3, 3)=="txt"))
            docs.push_back(arquivo);
    };
      closedir (dir);
    } else {
      /* could not open directory */
      perror ("");
      return 1;
    }
    cout << endl << endl << endl << "\t" << "Pesquisa: ";
    while(cin >> query and query!=".") {
        query = converte(query);
        palavras_chave.push_back(query);
    }
    cout << endl << "\t" <<"----------------------" << endl << endl;
    for(it_vet=docs.begin();it_vet!=docs.end();it_vet++){
        Inv(palavras,*it_vet, vocabulario);
        palavras.clear();
    }
    double W[docs.size()][vocabulario.size()];
    i = 0;
    for(it_vet=docs.begin();it_vet!=docs.end();it_vet++){
        Inv(palavras,*it_vet, vocabulario);
        j = 0;
            for(set_it=vocabulario.begin();set_it!=vocabulario.end();++set_it) {
                    W[i][j] = calcula_tf(palavras, *set_it);
                j++;
            }
        palavras.clear();
        i++;
    }
    double idf[vocabulario.size()] = {0};
    for(j=0;j<vocabulario.size();j++) {
        for(i=0;i<docs.size();i++) {
            if(W[i][j]>0)
                idf[j]++;
        }
        if(idf[j]!=0)
			idf[j] = log(docs.size()/idf[j]);
		else
			idf[j] = 0;
    }
    for(i=0;i<docs.size();i++) {
        for(j=0;j<vocabulario.size();j++) {
            W[i][j] = W[i][j] * idf[j];
        }
    }
    double q[vocabulario.size()];
    i = 0;
    for(set_it=vocabulario.begin();set_it!=vocabulario.end();set_it++) {
        q[i] = calcula_tf(palavras_chave, *set_it);
        i++;
    }
    for(j=0;j<vocabulario.size();j++) {
        q[j] = q[j] * idf[j];
    }
    //testando o similaridade
	for(i=0;i<docs.size();i++) {
        sim.push_back(similar(W[i], q, vocabulario.size()));
        similaridade.insert(pair<double,string>(sim[i], docs[i]));
	}
	while(sim.size()>0) {
        maior = 0;
        for(itera=sim.begin();itera!=sim.end();itera++) {
            if(*itera>maior)
                maior = *itera;
        }
        if(maior>0)
            map_it = similaridade.find(maior);
        else {
            cout << "\tNao foi possivel encontrar mais resultados." << endl;
            return 0;
        }
        cout << "\t" << map_it->second << endl << endl;
        for(itera=sim.begin();itera!=sim.end();itera++) {
            if(*itera==maior) {
                sim.erase(itera);
                break;
            }
        }
	}
    return 0;
}
