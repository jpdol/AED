#include <iostream>
#include <fstream>
#include <cstdlib>
using namespace std;

//Compara dois inteiros e retorna o maior
int maior(int x,int y){
	if(x > y) return x;
	else return y;
}

//Transforma uma string xy em 00xy
string trataChave(string chave) {
	int i = 0;
	string aux = "";
	while (chave[i] != '\n') {
		i++;
	}
	while (4 - i > 0) {
		aux += '0';
		i++;
	}
	aux += chave;
	return aux;
}

class Item {
public:
	int chave;
	string chaveHash;
	Item() {
	}
	Item(int h) {
		chave = h;
	}
};

template <typename T>
class No{
public:
//Atributos
	T item; //Guarda o item
	int alt;//Guarda a altura do nó na AVL
	No<T>* esq;
	No<T>* dir;

//Métodos
	No(){
		esq = NULL;
		dir = esq;
		alt =0;
	}
};

//Implementação da Árvore AVL
template <typename T>
class AVL{
private:
	//Métodos privados
	void insere(T&, No<T>*&);
	void remove(int, No<T>*&);
	void removeAux(No<T>*,No<T>*&);
	void percorrePre(No<T>*&);
	void percorrePos(No<T>*&);
	void percorreCentral(No<T>*&);
	void busca(int,No<T>*&,bool&);
	void rotSD(No<T>*&);//Rotação simples a direita
	void rotSE(No<T>*&);//Rotação simples a esquerda
	void LR(No<T>*&);//Rotação dupla a direita
	void RL(No<T>*&);//rotação dupla a esquerda
	int fatorBL(No<T>*&);//Calcula o fator de balanceamento do nó
	int alt_No(No<T>*);//Retorna a altura do nó

public:
	//Atributos
		No<T>* raiz;
		bool volta;
	//Métodos públicos
	AVL();
	void insere(T&);
	void remove(int);
	void percorrePre();
	void percorrePos();
	void percorreCentral();
	bool busca(int);

};

//Implementação dos métodos públicos

template <typename T>
AVL<T>::AVL() {
	raiz = NULL;
	volta = false;
}

template <typename T>
void AVL<T>::insere(T& item){
	insere(item,raiz);
}

template <typename T>
void AVL<T>::remove(int chave){
	remove(chave,raiz);
}

template <typename T>
void AVL<T>::percorrePre(){
	percorrePre(raiz);
}

template <typename T>
void AVL<T>::percorrePos(){
	percorrePos(raiz);
}

template <typename T>
void AVL<T>::percorreCentral(){
	percorreCentral(raiz);
}

template <typename T>
bool AVL<T>::busca(int chave){
	bool achou;
	busca(chave,raiz,achou);
	return achou;
}

//Implementação dos métodos privados

template<typename T>
void AVL<T>::insere(T &item, No<T>*& pont) {
	if (pont == NULL) {
		pont = new No<T>();
		pont->item = item;
		volta = true;

	} else if (item.chave > pont->item.chave) {
		insere(item, pont->dir);
	} else if (item.chave < pont->item.chave) {
		insere(item, pont->esq);
	}

	//Recalcula as alturas dos nós
	if (volta) {
		if (pont == raiz) {
			volta = false;
		}
		pont->alt = maior(alt_No(pont->esq), alt_No(pont->dir)) + 1;
	}

	//Faz o balanceamento da árvore
	if (fatorBL(pont) > 1) {
		if (fatorBL(pont->dir) >= 0) {
			rotSE(pont);
		} else {
			RL(pont);
		}
	} else if (fatorBL(pont) < -1) {
		if (fatorBL(pont->esq) <= 0) {
			rotSD(pont);
		} else {
			LR(pont);
		}
	}
}

template<typename T>
void AVL<T>::remove(int chave, No<T>*& pont) {

	if (pont == NULL) {
		cout << "Item não encontrado";
	} else if (chave < pont->item.chave) {
		remove(chave, pont->esq);
	} else if (chave > pont->item.chave) {
		remove(chave, pont->dir);
	} else if (pont->dir == NULL) {
		No<T>* aux = pont;
		pont = pont->esq;
		delete aux;
		volta = true;
	} else if (pont->esq == NULL) {
		No<T>* aux = pont;
		pont = pont->dir;
		delete aux;
		volta = true;
	} else {
		removeAux(pont, pont->esq);
		volta = true;
	}

	//Recalcula as alturas dos nós
	if (volta && pont != NULL) {
		if (pont == raiz) {
			volta = false;
		}
		pont->alt = maior(alt_No(pont->esq), alt_No(pont->dir)) + 1;
	}

	//Faz o balanceamento da árvore
	if (pont != NULL) {
		if (fatorBL(pont) > 1) {
			if (fatorBL(pont->dir) >= 0) {
				rotSE(pont);
			} else {
				RL(pont);
			}
		} else if (fatorBL(pont) < -1) {
			if (fatorBL(pont->esq) <= 0) {
				rotSD(pont);
			} else {
				LR(pont);
			}
		}
	}
}

template <typename T>
void AVL<T>::removeAux(No<T>* q,No<T>*& r){
	if(r->dir != NULL){
		removeAux(q,r->dir);
	}else{
		q->item = r->item;
		q = r;
		r = r->esq;
		delete q;
	}
}

template <typename T>
void AVL<T>::percorrePre(No<T>*& pont){
	if(pont!=NULL){
		//processa-Item(pont->item);
		percorrePre(pont->esq);
		percorrePre(pont->dir);
	}

}

template<typename T>
void AVL<T>::percorreCentral(No<T>* &pont) {
	if (pont != NULL) {
		string num = pont->item.chaveHash;
		percorreCentral(pont->esq);
		for (int i = 0; num[i] != '\n'; i++) {
			cout << num[i];
		}
		cout << " ";
		percorreCentral(pont->dir);
	}
}

template <typename T>
void AVL<T>::percorrePos(No<T>* &pont){
	if(pont!=NULL){
		percorrePos(pont->esq);
		percorrePos(pont->dir);
		//processa-Item(pont->item);
	}

}

template <typename T>
void AVL<T>::busca(int chave, No<T>* &pont,bool& achou){
	if(pont == NULL){
            achou = false;
	}else if(chave > pont->item.chave){
		busca(chave,pont->dir,achou);
	}else if(chave < pont->item.chave){
		busca(chave,pont->esq,achou);
	}else{
            achou = true;
	}
}

template <typename T>
void AVL<T>::rotSD(No<T>* &pont){
	No<T>* no = pont->esq;
	pont->esq = no->dir;
	no->dir = pont;
	pont->alt = maior(alt_No(pont->esq),alt_No(pont->dir))+1;
	no->alt = maior(alt_No(no->esq),pont->alt)+1;
	pont = no;
}

template <typename T>
void AVL<T>::rotSE(No<T>* &pont){
	No<T>* no = pont->dir;
	pont->dir = no->esq;
	no->esq = pont;
	pont->alt = maior(alt_No(pont->esq),alt_No(pont->dir))+1;
	no->alt = maior(alt_No(no->dir),pont->alt)+1;
	pont = no;
}

template <typename T>
void AVL<T>::LR(No<T>*& pont){
	rotSE(pont->esq);
	rotSD(pont);
}

template <typename T>
void AVL<T>::RL(No<T>*& pont){
	rotSD(pont->dir);
	rotSE(pont);
}

template <typename T>
int AVL<T>::alt_No(No<T>* no){
	if(no == NULL) return -1;
	else return no->alt;
}

template <typename T>
int AVL<T>::fatorBL(No<T>*& no){
	return -1*alt_No(no->esq) + alt_No(no->dir);
}



template <typename T>
class TabHash{
private:
    AVL<T>* tabela;
    int hk;
public:
    TabHash(){
        tabela = new AVL<T>[787];
        hk = -1;
    }

    void inserir(T& item, string chaveString){
        string aux = trataChave(chaveString);
        //Calcula a chave da tabela rash a partir da chave do item
        hk = ((aux[0]*128*128*128)+(aux[1]*128*128)+(aux[2]*128)+(aux[3]))%787;
        //Adiciona a chave da tabela rash ao item
        item.chaveHash = aux;
        //Insere o item na AVL que se encontra na posição hk da tabela
        tabela[hk].insere(item);
    }

	void remover(string chave) {
		string aux = trataChave(chave);
		hk = ((aux[0] * 128 * 128 * 128) + (aux[1] * 128 * 128) + (aux[2] * 128) + (aux[3])) % 787;
		int auxToInt = atoi(aux.c_str());
		if (tabela[hk].busca(auxToInt)) {
			tabela[hk].remove(auxToInt);
		}else{
			cout<<"Item não encontrado";
		}

	}

    void busca(string chave){
    	string aux = trataChave(chave);
    	//Calcula a posição onde o item possivelmente está na tabela
        hk = ((aux[0]*128*128*128)+(aux[1]*128*128)+(aux[2]*128)+(aux[3]))%787;
        int auxToInt = atoi(aux.c_str());
        if(tabela[hk].busca(auxToInt)){//Se encontrar o item, realiza a percurso
        	tabela[hk].percorreCentral();
        }else{
        	cout<<"Chave nÃ£o encontrada.";
        }
    }
};



int main() {

    TabHash <Item>*th = new TabHash<Item>();//Cria uma Tabela Rash para o tipo Item
    ifstream arquivo;
    arquivo.open("chaves.txt"); // Abre o arquivo chaves.txt
    if(arquivo.is_open()){ //Se conseguiu abrir realiza o procedimento
       string linha;
       while(true){ // Pega todos as chaves dos arquivos e adiciona a um item
           if(arquivo.eof()){
               break;
           }
           else{
               getline(arquivo,linha);
               int linhaToInt = atoi(linha.c_str());
               Item novoItem(linhaToInt);//Cria um novo item com uma chave
               linha+= '\n';
               th->inserir(novoItem, linha);//Adiciona o item a tabela Rash
           }
       }
       //Pega um chave do usuário e pesquisa o item na tabela rash
       string entrada;
       getline(cin,entrada);
       entrada+='\n';
       if(entrada != "\n"){//Se encontrar realiza o percurso da árvore
    	   th->busca(entrada);
       }
       arquivo.close();
    }else{
    	cout<<"O arquivo não foi aberto";
    }
    return 0;
}

