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
  string    aux;
  Key       key;

  lista.open("lista.txt", ios::in);

  if (lista.is_open()) {
    // Salvar a posição final do arquivo, para saber quando parar a leitura
    lista.seekg(0, ios::end);
    int end = lista.tellg();
    lista.seekg(0, ios::beg);
    num_reg = end/TAM_REG;

    while (cont < num_reg) { // Compara a posição atual com o final
      lista.seekg(cont * TAM_REG, ios::beg);
      lista.read(chave_nome, TAM_NOME);
      aux.assign(chave_nome);

      lista.seekg((cont * TAM_REG) + POS_MATRIC, ios::beg);
      lista.read(chave_matric, TAM_MATRIC);
      aux.append(chave_matric);

      transform(aux.begin(), aux.end(),aux.begin(), ::toupper);
      key.setKey(aux);
      key.setPos(cont);
      tree->insert(key);
      ++cont;
    }

  } else {
    cout << "Não foi possível abrir algum do arquivo" << endl;
  }

  lista.close();
}

string geraChave(string registro) {
  string chave;

  for (int i = 0; i < TAM_NOME; ++i)
    chave.append(1, registro[i]);

  for (int i = POS_MATRIC; i < POS_MATRIC + TAM_MATRIC; ++i)
    chave.append(1, registro[i]);

  transform(chave.begin(), chave.end(), chave.begin(), ::toupper);

  return chave;
}

void formatarInsercao(string * nome, string * matric, string * curso, string * turma) {
  int tamanho = nome->length();

  if (tamanho < 39) {
    for (int i = 0; i < 39 - tamanho; ++i)
      nome->append(" ");
  }

  nome->append("  ");
  matric->append("  ");
  curso->append("  ");
  turma->append("\n");
}

void formatarRemocao(string * nome) {
  int tamanho = nome->length();

  if (tamanho < 39) {
    for (int i = 0; i < 39 - tamanho; ++i)
      nome->append(" ");
  }

  nome->append("  ");
}

void inserir(BTree * tree) {
  string    nome;
  string    matric;
  string    curso;
  string    turma;
  string    registro;
  string    chave;
  Key       key;
  ofstream  destino;

  destino.open("lista.txt", ios::out | ios::app);

  cout  << "Dados do registro a ser inserido no arquivo:\n\nNome: ";
  getline(cin, nome);
  cout  << "Matrícula: ";
  cin   >> matric;
  getchar();
  cout  << "Curso: ";
  cin   >> curso;
  getchar();
  cout  << "Turma: ";
  cin   >> turma;
  getchar();

  formatarInsercao(&nome, &matric, &curso, &turma);
  registro = nome + matric + curso + turma;

  if (destino.is_open()) {
    int end = destino.tellp();
    int num_reg = end/TAM_REG;

    destino << registro;
    chave = geraChave(registro);
    key.setKey(chave);
    key.setPos(num_reg);
    tree->insert(key);
  
  } else {
    cout << "Não foi possível abrir o arquivo" << endl;
  } 

  destino.close();
}

void remover(BTree * tree) {
  int       cont = 0;
  int       num_reg;
  char      tmp[TAM_REG+1];
  string    nome;
  string    matric;
  string    registro;
  string    chave;
  ifstream  lista;
  ofstream  aux;
  Key       key;
  BTreeNode *procurado;


  lista.open("lista.txt", ios::in);
  aux.open("aux.txt", ios::out);

  if(lista.is_open() && aux.is_open()) {
    cout  << "Dados do registro a ser removido do arquivo:\n\nNome: ";
    getline(cin, nome);
    cout  << "Matrícula: ";
    cin   >> matric;
    getchar();

    formatarRemocao(&nome);
    registro = nome + matric;
    chave = geraChave(registro);
    key.setKey(chave);
    procurado = tree->search(key);

    if (procurado != NULL) {
      int pos = tree->findPos(key);
      tree->remove(key);
      lista.seekg(0, ios::end);
      int end = lista.tellg();
      lista.seekg(0, ios::beg);
      num_reg = end/TAM_REG;

      while(cont < num_reg) {
        lista.getline(tmp, TAM_REG);
        if (cont != pos)
          aux << tmp << endl;
        cont++;
      }

      remove("lista.txt");
      rename("aux.txt", "lista.txt");
    
    } else {
      cout << "Registro Inexistente" << endl;
    }

  } else {
    cout << "Não foi possível abrir o arquivo" << endl;
  }

  aux.close();
  lista.close();
}
 
int main() {
  int num;

  system("clear || cls");

  cout << "Setup de Árvore:\n\n" << "Qual o número mínimo de chaves que a árvore B deve ter por nó?\n\t>> ";
  cin >> num;
  cout << "Isso gerará uma árvore B na qual cada nó possuirá " << (2*num)-1 << " chaves e " << 2*num << " filhos.";
  cout << endl << endl;

  BTree * tree = new BTree(num);
  buildTree(tree);

  cout << "A árvore construída foi a seguinte:\n\n"; 
  tree->traverse(); 
  cout << endl << endl;

  getchar();
  // cout << "INSERÇÃO" << endl << endl;
  // inserir(tree);

  // cout << "A árvore construída foi a seguinte:\n\n"; 
  // tree->traverse(); 
  // cout << endl << endl;

  cout << "REMOÇÃO" << endl << endl;
  remover(tree);

  cout << "A árvore construída foi a seguinte:\n\n"; 
  tree->traverse(); 
  cout << endl << endl;

  delete(tree);
  
  return 0; 
}