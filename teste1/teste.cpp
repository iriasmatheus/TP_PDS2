#include "doctest.h"
#include "teste.h"
//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

#include <iostream>
#include <string>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <cmath>
#include <dirent.h>

using namespace std;

string converte(string palavra){
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
                //palavra.pop_back();
            }
        }
    }
    return palavra;

}
void Inv(vector<string> &palavras,string filename){
try{
    fstream file;
    filename = "./docs/" + filename;
    file.open(filename.c_str(), fstream::in | fstream::out);
    string palavra;
    if(file.is_open()){
        while(file >> palavra){
            palavra=converte(palavra);
            palavras.push_back(palavra);
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
double similar(double* x, double* y, int tam) {
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

TEST_SUITE("FUNCOES_MB"){
TEST_CASE("converte()"){
    string word="ABACAXI";
    string word1="GUARDA-CHUVA";
    string word2="Ab!?-C";
    CHECK(converte(word)=="abacaxi");
    CHECK(converte(word1)=="guardachuva");
    CHECK(converte(word2)=="abc");

}
TEST_CASE("Inv(vector<string> &palavras,string filename)"){
    vector<string> palavras;
    vector<string>::iterator it=palavras.begin();
    string filename;
    string word1,word2;
    filename="arquivo.txt";
    CHECK(Inv(palavras,filename)=="Arquivo nao encontrado!");
    filename="d1.txt";
    ifstream file;
    file.open(filename.c_str());
    while(file >> word1){
                word2=converte(word1);
                palavras.insert(word2);
}
    CHECK(*it=="a");
    it++;
    CHECK(*it=="a");
    it++;
    CHECK(*it=="a");
    it++;
    CHECK(*it=="b")
}
TEST_CASE("similar(double* x, double* y, int tam)"){
	double num = 0;
    double denom1 = 0;
    double denom2 = 0;
    int i;
    double x[tam];
    double y [tam];
    for(i=0;i<tam;i++)
    	x[i] = 1;
    	y[i] = 1;
    CHECK(similaridade(x, y, tam) == 1);
	x[0] = x[2] = 0;
	y[0] = y[1] = 0;
	CHECK(similaridade(x,y,tam) == 0;
}
}

