#include <iostream>
#include "vector"

using namespace std;
#define nullptr NULL
#define FILL 8
#define INFINITY 2147483647
#define BOARD_SQUARES 64
#define HORSEMEN_NUMBER 4
#define KINGS_NUMBER 1
#define KING_POSITION 4

template <typename T>
class No{
public:
	T item;
	No* next;
};

// Lista
template <typename T>
class List{
private:
	No<T>* first;
	No<T>* last;
public:
	List();
	void destroy();
	void insert(T);
	T search(int);
	No<T>* predecessor(T);
	void remove(T);
	void printList();
	No<T>* getFirst();
	No<T>* getLast();
};

template <typename T>
No<T>* List<T>::getFirst(){
	return first;
}

template <typename T>
List<T>::List(){
	first = new No<T>();
	first->next = nullptr;
	last = first;
}


template <typename T>
void List<T>::destroy(){
	while((first->next != last) && (first != last)){
		No<T>* aux = first->next;
		first->next = aux->next;
		delete aux;
	}
	delete first;
	delete last;

}

template <typename T>
void List<T>::insert(T item){
		last->next = new No<T>();
		last = last->next;
		last->next = nullptr;
		last->item = item;
}

template <typename T>
T List<T>::search(int key){
	No<T>* aux = first->next;
	while(aux != nullptr){
		if(aux->item.key == key){
			return aux->item;
		}
		aux = aux->next;
	}
	return aux->item;
}

template <typename T>
No<T>* List<T>::predecessor(T item){
	No<T>* predNo = first;
	while(predNo != nullptr){
		if(predNo->next->item->key == item->key){
			return predNo;
		}
		predNo = predNo->next;
	}
	return predNo;
}

template <typename T>
void List<T>::remove(T item){
	if(first != last){

		No<T>* predItem = predecessor(item);
		if(predItem != nullptr){
			if(predItem->next == last){
				this->last = predItem;
			}
			No<T>* aux = predItem->next;
			predItem->next = predItem->next->next;
			delete aux;
		}


	}
}

template <typename T>
void List<T>::printList(){
	No<T>* aux = first->next;
	while(aux != nullptr){
		cout<<aux->item->key<<" ";
		aux = aux->next;
	}
}


//Queue
template <typename T>
class Queue{
private:
	No<T>* front;
	No<T>* back;
public:
	Queue();
	~Queue();
	void enqueue(T);
	void dequeue(T&);
	bool isEmpty();
};

template <typename T>
Queue<T>::Queue(){
	front = new No<T>();
	front->next = nullptr;
	back = front;
}

template <typename T>
Queue<T>::~Queue(){
	T aux;
	while (!isEmpty()){
		dequeue(aux);
	}
	delete front;
}

template <typename T>
void Queue<T>::enqueue(T item){
	back->next = new No<T>();
	back = back->next;
	back->item = item;
	back->next = nullptr;
}

template <typename T>
void Queue<T>::dequeue(T &item){
	No<T>* aux = front;
	front = front->next;
	item = front->item;
	delete aux;
}

template <typename T>
bool Queue<T>::isEmpty (){
	return front == back;
}



struct Vertex{
	Vertex* pi;
	int distance;
	int key;
	char color;
	List<Vertex*> list;
};

void merge(vector<int> &A, int p, int q, int r){
	int n1 =q-p+1;
	int n2 = r-q;

	int L[n1+1];
	int R[n2+1];

	for(int i = 1; i<=n1; i++) L[i] = A[p+i-1];

	for(int j = 1; j<=n2; j++) R[j] = A[q+j];

	L[n1+1] = INFINITY;
	R[n2+1] = INFINITY;
	int i = 1;
	int j = 1;

	for(int k = p; k<=r; k++){
		if(L[i]<=R[j]){
			A[k] = L[i];
			i++;
		}else{
			A[k]=R[j];
			j++;
		}
	}
}


void mergeSort(vector<int> &A, int p, int r){
	if(p<r){
		int q = (p+r)/2;
		mergeSort(A,p,q);
		mergeSort(A,q+1,r);
		merge(A,p,q,r);
	}
}

int convertIntoVertex(string square){
	int column = square[0] - 'a';
	int line = square[1] - '1';
	return FILL*column + line + 1;
}

//Grafo
class Graph{
private:
	Vertex* adj;
	int n,m;
public:
	Graph(int);
	~Graph();
	void initialize(int);
	void insertEdge(int,int);
	void BFS (string);
	void print();
	void setN(int);
	void setM(int);
	int distance(string);
	void setAdj(Vertex&);
	int getN();
	int getM();
	Vertex* search(int);
	Vertex* getAdj();
};

//inicializa o graph a partir do numero de vertices
Graph::Graph(int n){
	initialize(n);
}

Graph::~Graph(){
	for(int i = 1; i<n; i++){
		adj[i].list.destroy();
	}

	n=m=0;
}

void Graph::initialize(int n){
	this->n = n;
	adj = new Vertex[n+1];
	for(int i = 1; i<=n; i++){
		adj[i].key = i;
	}
}

void Graph::insertEdge(int u,int v){
	adj[u].list.insert(&adj[v]);
	m++;
}

Vertex* Graph::search(int key){
	for(int i = 1; i<=n; i++){
		if(adj[i].key == key){
			return &adj[i];
		}
	}
	return nullptr;
}

void Graph::print(){
	for(int i=1; i<=n; i++){
		cout<<"v["<<i<<"]=";
		adj[i].list.printList();
		cout << endl;
	}
}

int Graph::distance(string square){
	int position = convertIntoVertex(square);
	int temp =adj[position].distance;
	return temp;
}


void Graph::setN(int n){
	this->n = n;
}
void Graph::setM(int m){
	this->m = m;
}
void Graph::setAdj(Vertex &list){
	this->adj = &list;
}
int Graph::getN(){
	return n;
}
int Graph::getM(){
	return m;
}
Vertex* Graph::getAdj(){
	return adj;
}

void Graph::BFS (string position){
	int intPosition = convertIntoVertex(position);
	Vertex* s = &adj[intPosition];
	for (int i=1; i <= n; i++){
		adj[i].color = 'B';
		adj[i].distance = INFINITY;
		adj[i].pi = nullptr;
	}
	s->color = 'C';
	s->distance = 0;
	Queue<Vertex*> q;
	q.enqueue(s);
	Vertex* u;
	No<Vertex*>* aux;
	while(!q.isEmpty()){
		q.dequeue(u);
		aux = adj[u->key].list.getFirst()->next;
		while (aux != nullptr){
			if (aux->item->color == 'B'){
				aux->item->color = 'C';
				aux->item->distance = u->distance+1;
				aux->item->pi = u;
				q.enqueue(aux->item);
			}
			aux = aux->next;
		}
		u->color = 'P';
	}
}

void createGrid(int board[FILL][FILL]){
	for(int line = 0;line < FILL;line++){
		for(int column = 0; column < FILL; column++){
			board[line][column] = FILL * line + column + 1;
		}
	}
}

void turnPositionIntoGraph(Graph &graph){
	int board[FILL][FILL];
	int auxVertex;
	createGrid(board);
	vector<int> ord;
	for(int line = 0; line < FILL; line++){
		for(int column = 0; column < FILL; column++){
			auxVertex = board[line][column];
			if(line -2 >= 0 && column -1 >= 0) ord.push_back(board[line-2][column-1]);

			if(line -2 >= 0 && column +1 < FILL) ord.push_back(board[line-2][column+1]);

			if(line -1 >= 0 && column -2 >= 0) ord.push_back(board[line-1][column-2]);

			if(line -1 >= 0 && column +2 < FILL) ord.push_back(board[line-1][column+2]);

			if(line +1 < FILL && column -2 >= 0) ord.push_back(board[line+1][column-2]);

			if(line +1 < FILL && column +2 < FILL) ord.push_back(board[line+1][column+2]);

			if(line +2 < FILL && column -1 >= 0) ord.push_back(board[line+2][column-1]);

			if(line +2 < FILL && column +1 < FILL) ord.push_back(board[line+2][column+1]);

			mergeSort(ord,0,ord.size()-1);
			for(unsigned int i = 0; i<ord.size();i++){
				graph.insertEdge(auxVertex,ord[i]);
			}
			while(!ord.empty()){
				ord.pop_back();
			}
		}
	}
}


int smaller(int v[], int size){
	int smaller = v[0];
	for (int i =0; i<size; i++){
		if(v[i]<smaller){
			smaller = v[i];
		}
	}
	return smaller;
}

int smallersCount(int v[], int size, int smaller){
	int cont =0;
	for (int i =0; i<size; i++){
		if (v[i]==smaller) cont++;
	}
	return cont;
}

void printSmaller(int v[], int size){
	int theSmaller = smaller(v,size);
	int smallerQuantity = smallersCount(v,size,theSmaller);
	for (int i=0;i<smallerQuantity ;i++){
		cout<< theSmaller<<' ';
	}
}



int main(){
	Graph board(BOARD_SQUARES);
	turnPositionIntoGraph(board);
	int entriesNumber;
	cin>>entriesNumber;
	string** entries = new string*[entriesNumber];
	for (int i = 0; i<entriesNumber; i++){
		entries[i] = new string[HORSEMEN_NUMBER + KINGS_NUMBER];
	}
	for(int i =0; i<entriesNumber; i++){
		for(int k = 0; k<HORSEMEN_NUMBER + KINGS_NUMBER; k++){
			cin>>entries[i][k];
		}
	}
	int distances[HORSEMEN_NUMBER];
	for (int i=0;i<entriesNumber;i++){
		for (int j = 0 ; j < HORSEMEN_NUMBER; j++){
			board.BFS(entries[i][j]);
			distances[j] = board.distance(entries[i][KING_POSITION])-1;
		}

		printSmaller(distances, HORSEMEN_NUMBER);
		cout<<endl;
	}



	return 0;
}
