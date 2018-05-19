#include <iostream>
using namespace std;

 //Implementação do nó
template<typename T>
class No {
public:
	T item;
	No<T>*prox;

	No() {
		prox = NULL;
	}
};


 //Implementação da Pilha
template<typename T>
class Pilha {
private:
	No<T> *topo;
	No<T> *fundo;
public:
	Pilha() {
		fundo = new No<T>();
		topo = fundo;
	}

	void empilha(T item) {
		No<T>* aux = new No<T>();
		topo->item = item;
		aux->prox = topo;
		topo = aux;
	}

	void desempilha(T &item) {
		if (fundo != topo) {
			No<T>*aux = new No<T>();
			aux = topo;
			topo = topo->prox;
			item = topo->item;
			delete aux;
		} else {
			item = -1;
		}
	}
};


 //Implementação da Lista estática
template<typename T>
class ListaEstatica {
private:
	T *item;
	int tam;
	int n;
public:
	ListaEstatica(int max) {
		item = new T[max];
		n = 0;
		tam = max;
	}

	void insere(T item) {
		if (n < tam) {

			this->item[n] = item;
			n++;
		} else {
			cout << "Lista Cheia";
		}
	}

	int busca(T item) {
		for (int i = 0; i < n; i++) {
			if (item == this->item[i]) {
				return i;
			}
		}
		return -1;
	}

	void remove(T chave) {
		if (n > 0) {
			for (int i = 0; i < n; i++) {
				if (item[i] == chave) {
					for (int j = 1 + i; j < n; j++) {
						item[j - 1] = item[j];
					}
					i = n;
					n--;
				}
			}
		}
	}

	void print() {
		for (int i = 0; i < n; i++) {
			cout << item[i];
		}
	}

	T getItem(int pos) {
		return item[pos];
	}
};

//Implementação do interpretador
class Interpretador {
private:
	ListaEstatica<string>*lista;
	Pilha<int>*pilha;

public:
	Interpretador(ListaEstatica<string>&); //Construtor
	string catchWord(int); //retorna o comando de cada linha
	void printNi(int); //imprime as saídas do programa
	void executar(); //faz todo o processo de interpretação
};

Interpretador::Interpretador(ListaEstatica<string> &lista) {
	this->lista = &lista;
	pilha = new Pilha<int>();
	executar();
}

string Interpretador::catchWord(int posLinha) {

	string novaString = "";

	bool continua = true;

	string texto = lista->getItem(posLinha); //Pega na lista a string na posição posLinha

	int i = 0;

	while (texto[i] == ' ') { //pega a posição da primeira letra da string
		i += 1;
	}
	while (continua) { //pega a primeira palavra da string
		char charAux = texto[i];
		if (charAux != ' ' && charAux != '\n') {
			novaString += charAux;
			i++;
		} else {
			continua = false;
		}
	}
	if (novaString == "") { //Se a string estiver vazia retorna \n
		return "\n";
	}
	return novaString;
}

void Interpretador::printNi(int pos) {
	string atual = lista->getItem(pos); //Pega na lista a string na posição pos
	string novaString = "";
	int i = 0;

	while (atual[i] == ' ') { //pega a posição da primeira letra da string
		i++;
	}
	while (atual[i] != ' ') { //Pega a posição do próximo espaço
		i++;
	}

	//Pega o número a ser exibido
	novaString += atual[i + 1];
	if (atual[i + 2] != '\n') {
		novaString += atual[i + 2];
	}

	//Exibe o número
	cout << novaString << " ";
}

void Interpretador::executar() {
	int pos = this->lista->busca("Z :\n"); //Busca o main do programa

	pos++; //Vai para a próxima linha

	string comando = catchWord(pos);

	bool continua = true;

	while (continua) {
		if (comando == "PRINT") { //Caso encontre PRINT na linha
			printNi(pos);

		} else if (comando != "\n") { //Caso seja uma chamada de função
			comando += " :\n";
			pilha->empilha(pos); //Empilha a posição atual
			int linhaChamada = lista->busca(comando); //Busca a linha de implementação da função
			pos = linhaChamada;

		} else {
			int volta = 0;
			pilha->desempilha(volta);
			if (volta != -1) { //se volta for -1 o programa acabou
				pos = volta;
			} else {
				continua = false;
			}
		}

		if (continua) {
			pos++;
			comando = catchWord(pos);
		}
	}
}


int main() {
	string programa = "";
	string temp = "";
	int contaLinha = 1;

	//Pega o programa do usuário e coloca em uma string
	while (temp != "~") {
		getline(cin, temp);
		programa += temp;
		programa += '\n';
		contaLinha += 1;
	}
//Cria uma lista com com tamanho do total de linhas do programa
	ListaEstatica<string> lista(contaLinha);


//Coloca cada linha do programa na lista
	char auxChar = '0';
	int i = 0;
	temp = "";

	while (auxChar != '~') {
		auxChar = programa[i];
		if (auxChar != '\n' && auxChar != '~') {
			temp += auxChar;
			i++;
		}
		if (auxChar == '\n') {
			temp += '\n';
			lista.insere(temp);
			temp = "";
			i++;
		}
	}
//Cria um Interpretador, e passa a lista para ele
	Interpretador interpretador(lista);

	return 0;
}
