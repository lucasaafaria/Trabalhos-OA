#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>

#include "./lib/btree.h"

#define TAM_NOME    3
#define TAM_MATRIC  5
#define TAM_REG     54

#define POS_MATRIC  41
#define PROX_REG    8

using namespace std;

void buildTree(BTree * tree) {
  int       num_reg = 0;        // variável que guarda o número relativo do registro
  int       cont    = 0;
  char      chave_nome[TAM_NOME+1];
  char      chave_matric[TAM_MATRIC+1];
  ifstream  lista;
  string    key;

  lista.open("lista.txt", ios::in);

  if (lista.is_open()) {
    // Salvar a posição final do arquivo, para saber quando parar a leitura
    lista.seekg(0, ios::end);
    int end = lista.tellg();
    lista.seekg(0, ios::beg);
    num_reg = end/TAM_REG;

  while (cont <= num_reg) { // Compara a posição atual com o final
    lista.seekg(cont * TAM_REG, ios::beg);
    lista.read(chave_nome, TAM_NOME);
    key.assign(chave_nome);

    lista.seekg((cont * TAM_REG) + POS_MATRIC, ios::beg);
    lista.read(chave_matric, TAM_MATRIC);
    key.append(chave_matric);

    transform(key.begin(), key.end(),key.begin(), ::toupper);
    tree->insert(key);
    ++cont;
  }

  } else {
    cout << "Não foi possível abrir algum do arquivo" << endl;
  }

  lista.close();
}
 
int main() {

  BTree * tree = new BTree(3);
  buildTree(tree);

  cout << "Traversal of tree constructed is:\n\n"; 
  tree->traverse(); 
  cout << endl;

  delete(tree);
  
  return 0; 
}