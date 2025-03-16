//verified AOJ1196
#include<utility>
#include<vector>
using namespace std;


struct Edge{
	int dest,weight;
	Edge(int a,int b):dest(a),weight(b){}
};
pair<int,int> farthest(vector<Edge> edge[],int u,int par=-1){
	pair<int,int> res(u,0),tmp;
	for(int i=0;i<(int)edge[u].size();i++){
		Edge &e=edge[u][i];
		if(par==e.dest)continue;
		tmp=farthest(edge,e.dest,u);
		if(res.second<e.weight+tmp.second){
			res=tmp;
			res.second+=e.weight;
		}
	}
	return res;
}
int tree_diameter(vector<Edge> edge[]){
	return farthest(edge,farthest(edge,0).first).second;
}


