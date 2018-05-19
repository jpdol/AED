#include<iostream>
#include "vector"
#define INFINITY 2147483647
typedef char neuron;
using namespace std;

class Sinapses{
private:
    int v, w;
    float cost;
public:
    Sinapses(){v=w=cost =0;}
    Sinapses(int, int, float);
    int getV();
    int getW();
    float getCost();
    void setCost(float);
};

Sinapses::Sinapses(int v, int w, float cost){
    this->v = v;
    this->w = w;
    this->cost = cost;
}

int Sinapses::getV(){
    return v;
}

int Sinapses::getW(){
    return w;
}

float Sinapses::getCost(){
    return cost;
}

void Sinapses::setCost(float cost){
    this->cost= cost;
}

class Graph{
private:
	float** weight;
	int vertices, edges;
public:
  Graph();
	void initialize(int);
	void insertEdge(int,int,float);
  	void fillWithEdges(vector<Sinapses>&);
	int getVertices();
	int getEdges();
	float** getWeight();
};

Graph::Graph(){
  edges = vertices =0;
  weight = NULL;
}

void Graph::initialize(int vertices){
	this->vertices = vertices+1;
	edges = 0;
	weight = new float*[this->vertices];
	for (int i = 0; i<=vertices; i++){
		weight[i] = new float[this->vertices];
	}

	for(int i = 0; i<=vertices; i++){
		for(int j = 0; j<=vertices; j++){
			weight[i][j] = -1;
		}
	}
}


void Graph::insertEdge(int vertex1,int vertex2, float weightEdge){
	weight[vertex1][vertex2] = weightEdge;
	weight[vertex2][vertex1] = weightEdge;
	edges++;
}

void Graph::fillWithEdges(vector<Sinapses> &sin){
    for(int i = 0; i < vertices; i++){
        for(int j = 0; j < vertices; j++){
            if(weight[i][j]>-1){
                if(i < j){
                	sin.push_back(Sinapses(i, j, weight[i][j]));
                }
            }
        }
    }
}

int Graph::getVertices(){
  return vertices;
}

int Graph::getEdges(){
  return edges;
}

float** Graph::getWeight(){
  return weight;
}


void merge(vector<Sinapses> &A, int p, int q, int r){
	int n1 =q-p+1;
	int n2 = r-q;

	Sinapses L[n1+1];
	Sinapses R[n2+1];

	for(int i = 1; i<=n1; i++) L[i] = A[p+i-1];

	for(int j = 1; j<=n2; j++) R[j] = A[q+j];

	L[n1+1].setCost(INFINITY);
	R[n2+1].setCost(INFINITY);
	int i = 1;
	int j = 1;

	for(int k = p; k<=r; k++){
		if(L[i].getCost()<=R[j].getCost()){
			A[k] = L[i];
			i++;
		}else{
			A[k]=R[j];
			j++;
		}
	}
}


void mergeSort(vector<Sinapses> &A, int p, int r){
	if(p<r){
		int q = (p+r)/2;
		mergeSort(A,p,q);
		mergeSort(A,q+1,r);
		merge(A,p,q,r);
	}
}


class Block{
private:
	Graph neuronSynapse;
	vector<neuron> neurons;
	char color;
public:
    Block();
    Block(int);
    char getColor();
    Graph getNeuronSynapse();
    void insertNeuron(char);
    void fillGraph(int);
    void setColor(char);
    vector<neuron> getNeuron();
};

vector<neuron> Block::getNeuron(){
	return neurons;
}

void Block::insertNeuron(char h){
	neurons.push_back(h);
}

Block::Block(){
  color = 'W';
};

Block::Block(int n){
	neuronSynapse.initialize(n);
	color = 'W';
}

char Block::getColor(){
	return color;
}

Graph Block::getNeuronSynapse(){
	return neuronSynapse;
}

void Block::fillGraph(int n){
	neuronSynapse.initialize(n);
}

void Block::setColor(char color){
	this->color = color;
}

class Brain{
private:
	Graph blocksSynapse;
	vector<Block> blocks;
public:
    Brain(int);
    void insertSynapse(int,int,float);
    Graph getBlocksSynapse();
    vector<Block> getBlocks();
    void insertBlocks(Block);
    void setBlockColor(int, char);
};
void Brain::setBlockColor(int pos, char color){
  blocks[pos].setColor(color);
}
void Brain::insertBlocks(Block block){
	blocks.push_back(block);
}

void Brain::insertSynapse(int i, int j, float weight){
	blocksSynapse.insertEdge(i,j,weight);
}

vector<Block> Brain::getBlocks(){
	return blocks;
}

Brain::Brain(int n){
    blocksSynapse.initialize(n);
    Block temp;
    blocks.push_back(temp);
}

Graph Brain::getBlocksSynapse(){
  return blocksSynapse;
}

class Heap{
private:
	int* heap;
	int size;
	int* sons;
public:
	Heap(int);
	int pop(float[]);
	int* getHeap();
	void reHeapify(int, float[]);
	void heapifyUp(int, float[]);
	void heapifyDown(int, float[]);
	void push(int, float[]);
	void swap(int, int);
	bool isEmpty();
};


Heap::Heap(int size){
	heap = new int[size+1];
	sons = new int[size];
	this->size = 0;
}

bool Heap::isEmpty(){
	if(size == 0) return true;
	return false;
}

int* Heap::getHeap() {return heap;}

void Heap::swap(int i, int j){
	int aux = heap[i];
	heap[i] = heap[j];
	heap[j] = aux;

	sons[heap[i]] = i;
	sons[heap[j]] = j;
}

void Heap::heapifyUp(int value, float priority[]){
	while(value > 1 && priority[heap[value/2]] > priority[heap[value]]){
		swap(value/2, value);
		value = value/2;
	}
}

void Heap::reHeapify(int w, float priority[]){
	heapifyUp(sons[w], priority);
}

void Heap::heapifyDown(int value, float priority[]){
    int aux;
    while(2*value <= size){
    	aux = 2*value;
    	if(aux < size && priority[heap[aux]] > priority[heap[aux+1]]) aux++;
    	if(!(priority[heap[value]] > priority[heap[aux]])) break;
    	swap(value, aux);
    	value = aux;
	}
}

void Heap::push(int value, float priority[]){
    sons[value] = ++size;
    heap[size] = value;
    heapifyUp(size, priority);
}

int Heap::pop(float priority[]){
    swap(1, size);
    --size;
    heapifyDown(1, priority);
    return heap[size+1];
}

class UnionFind{
private:
  int* chief;
  int* subordinates;
public:
  UnionFind(int);
  int uFind(int);
  void uUnion(int, int);
};

UnionFind::UnionFind(int vertex){
    chief = new int[vertex];
    subordinates = new int[vertex];
    for(int i = 0; i < vertex; i++){
        chief[i] = i;
        subordinates[i] = 1;
    }
}

int UnionFind::uFind(int v){
    int v0 = v;
    while(v0 != chief[v0]){
    	v0 = chief[v0];
    }
    return v0;
}

void UnionFind::uUnion(int v0, int w0){
    if (subordinates[v0] < subordinates[w0]){
        chief[v0] = w0;
        subordinates[w0] += subordinates[v0];
    }
    else{
        chief[w0] = v0;
        subordinates[v0] += subordinates[w0];
    }
}

void Dijkstra(Graph g, int s, int* parent, float* distance){
	int* hook = new int[g.getVertices()];
	for(int i = 1; i < g.getVertices(); ++i){
		parent[i] = -1;
		distance[i] = INFINITY;
	}
	parent[s] = s;
	distance[s] = 0;
	for(int i = 1; i < g.getVertices(); i++){
		if(g.getWeight()[i][s]>-1){
			distance[i] = g.getWeight()[i][s];
			hook[i] = s;
		}
	}
	Heap h(g.getVertices());
	for(int v = 1; v < g.getVertices(); ++v){
		if(v != s) h.push(v, distance);
	}
	while(!h.isEmpty()){
		int y = h.pop(distance);
		if(distance[y] == INFINITY) break;
		parent[y] = hook[y];
		for(int i = 1; i<g.getVertices();i++){
			if(g.getWeight()[i][y]>-1){
				int w = i;
				float cost = g.getWeight()[i][y];
				if(distance[y] + cost < distance[w]){
					distance[w] = distance[y] + cost;
					h.reHeapify(w, distance);
					hook[w] = y;
				}
			}
		}
  }
}

void Kruskal(Graph g, vector<Sinapses> &mst){
    vector<Sinapses> sin;
    g.fillWithEdges(sin);
    mergeSort(sin, 0, sin.size()-1);
    UnionFind ufObj(g.getVertices());
    for(unsigned int i = 0; i < sin.size(); i++){
        int v0,w0;
        v0 = ufObj.uFind(sin[i].getV());
        w0 = ufObj.uFind(sin[i].getW());
        if(v0 != w0){
            ufObj.uUnion(v0,w0);
            mst.push_back(sin[i]);
        }
    }
}


int main(){

	//primeria entrada
	int start;
	int finish;
	int brainOrder, brainSize;
	cin>>brainOrder;
	cin>>brainSize;
	float* distance = new float[brainOrder+1];
	int* parent = new int[brainOrder+1];
    Brain brain(brainOrder);
    int v1,v2;
    float w;
    for(int i = 0; i<brainSize; i++){
        cin>>v1;
        cin>>v2;
        cin>>w;
        brain.insertSynapse(v1, v2, w);
    }

    cin>>start;
    cin>>finish;


    //segunda entrada
    int blockOrder, blockSize, sickNumber, index;
    vector<int> sickNeurons;

    for(int i = 1; i <= brainOrder; i++){
        cin>>blockOrder;
        cin>>blockSize;
        cin>>sickNumber;
    	Block block(blockOrder);
        if(sickNumber){
        	block.setColor('G');
        }
        for(int j = 0; j < sickNumber; j++){
            cin>>index;
            sickNeurons.push_back(index);
        }
        for(int k = 1; k <= blockSize; k++){
            cin>> v1;
            cin>> v2;
            cin>>w;
            block.getNeuronSynapse().insertEdge(v1,v2,w);
        }
        for(int l = 0; l < blockOrder;l++) {
        	block.insertNeuron('W');
        }
        for(int m = 0; m < sickNumber; m++) {
        	block.getNeuron()[sickNeurons[m]] = 'B';
        }
        brain.insertBlocks(block);
        sickNeurons.clear();
    }

    //processamento:

	vector<int> path;
	Dijkstra(brain.getBlocksSynapse(), finish, parent, distance);
	path.push_back(start);
	while(parent[start] != start){
        start = parent[start];
		path.push_back(start);
	}

	Block blockAnalisys;
	float totalCost = 0;
	vector<Sinapses> mst;
	for (unsigned int i = 0; i < path.size(); i++) {
		blockAnalisys = brain.getBlocks()[path[i]];
		if (brain.getBlocks()[path[i]].getColor() == 'G') {
      brain.setBlockColor(path[i],'W');
			Kruskal(blockAnalisys.getNeuronSynapse(), mst);
			for (unsigned int j = 0; j < mst.size(); j++) {
				totalCost += mst[j].getCost();
			}
			mst.clear();
		}
		for (unsigned int j = 0; j < blockAnalisys.getNeuron().size(); j++) {
			blockAnalisys.getNeuron()[j] = 'W';
		}
	}

 cout << totalCost;

	return 0;
}
