#include<vector>
#include<functional>
#include<queue>
using namespace std;

const int INF=2000000000;

struct Edge{
	int dst;
	int weight;
	Edge(){}
	Edge(int a,int b):dst(a),weight(b){}
};

struct Node{
	int v,cost;
	Node(){}
	Node(int a,int b):v(a),cost(b){}
	bool operator>(const Node &a)const{
		return cost>a.cost;
	}
};

int dijkstra(const vector<vector<Edge> > &G,int s,int g){
	int n=G.size();
	vector<int> dist(n,INF);
	priority_queue<Node,vector<Node>,greater<Node> > pq;
	dist[s]=0;	
	pq.push(Node(s,0));
	while(!pq.empty()){
		int v=pq.top().v;
		pq.pop();
		for(int i=0;i<G[v].size();i++){
			Node d(G[i][j].dest,dist[v]+G[i][j].wight);
			if(d.cost<dist[]){
			}
		}
	}
	return INF;
}