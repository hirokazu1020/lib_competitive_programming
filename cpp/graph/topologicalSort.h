#include<iostream>
#include<vector>
using namespace std;

//verified AOJ0119
vector<int> topologicalSort(int v,const vector<int> edge[]){
	vector<int> zeros;
	vector<int> in(v),res;
	for(int i=0;i<v;i++){
		for(int j=0;j<(int)edge[i].size();j++)in[edge[i][j]]++;
	}
	for(int i=0;i<v;i++)if(in[i]==0)zeros.push_back(i);
	while(!zeros.empty()){
		int u=zeros.back();
		zeros.pop_back();
		res.push_back(u);
		for(int i=0;i<(int)edge[u].size();i++){
			if(--in[edge[u][i]]==0)zeros.push_back(edge[u][i]);
		}
	}
	return res;
}



//辞書順トポロジカルソート 要verify
#include<set>
#include<utiliry>
Val val[MAX_V];
vector<int> topologicalSort(int v,const vector<int> edge[]){
	set<pair<Val,int> > zeros;
	vector<int> in(v),res;
	for(int i=0;i<v;i++){
		for(int j=0;j<(int)edge[i].size();j++)in[edge[i][j]]++;
	}
	for(int i=0;i<v;i++)if(in[i]==0)zeros.insert(make_pair(val[i],i));
	while(!zeros.empty()){
		pair<Val,int> p=*zeros.begin();
		int u=p.second;
		zeros.erase(zeros.begin());
		res.push_back(u);
		for(int i=0;i<(int)edge[u].size();i++){
			int dest=edge[u][i];
			if(--in[edge[u][i]]==0)zeros.insert(make_pair(val[dest],dest));
		}
	}
	return res;
}
