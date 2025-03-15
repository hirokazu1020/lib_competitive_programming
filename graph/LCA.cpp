#include<iostream>
#include<vector>
#include<algorithm>
#include<climits>


//doubling実装  <空間O(n log n),前処理O(n log g),クエリO(log n)>
//  短い実装で 
class LCA{
	int lg,n;
	std::vector<int> _depth,par;
	int dfs(const int parent[],int u){
		if(parent[u]==-1)return _depth[u]=1;
		if(_depth[u]!=0)return _depth[u];
		return _depth[u]=dfs(parent,parent[u])+1;
	}
public:
	LCA(const int parent[],int n):n(n),_depth(n,0){
		lg=1;
		while((1<<lg)<n)lg++;
		lg++;
		for(int i=0;i<n;i++)dfs(parent,i);
		par.resize(n*lg);	
		for(int i=0;i<n;i++)par[i]=parent[i];
		for(int i=1;i<lg;i++)
			for(int v=0;v<n;v++)
				if(par[(i-1)*n+v]!=-1)par[i*n+v]=par[(i-1)*n+par[(i-1)*n+v]];
				else par[i*n+v]=-1;
	}
	int query(int u,int v)const{
		if(_depth[u] > _depth[v])std::swap(u,v);
		for(int i=lg-1;i>=0;i--)
			if(_depth[u] <= _depth[v]-(1<<i))
				v=par[i*n+v];
		if(u==v)return u;
		for(int i=lg-1;i>=0;i--){
			if(par[i*n+u]!=par[i*n+v]){
				u=par[i*n+u];
				v=par[i*n+v];
			}
		}
		return par[u];
	}
	int depth(int u)const{//rootの深さが1
		return _depth[u];
	}
};



/*
//Lowest Common Ancestor
//segtreeRMQ実装  <空間O(n),前処理O(n),クエリO(log n)>
//***********スタックオーバーフロー注意*************
class LCA{
	int n;
	std::vector<int> _depth,idx;
	static int calcsize(int n){
		int res=1;
		while(res<n)res<<=1;
		return res;
	}
	int minimum(int a,int b,int k,int l,int r)const{
		if(r<=a||b<=l)return _depth.size()-1;
		if(a<=l&&r<=b)return idx[k];
		else{
			int vl=minimum(a,b,k*2+1,l,(l+r)/2);
			int vr=minimum(a,b,k*2+2,(l+r)/2,r);
			return _depth[vl]<_depth[vr]?vl:vr;
		}
	}
	void buildRMQ(){
		n=calcsize(_depth.size());
		idx.resize(2*n-1);
		for(int i=0;i<(int)_depth.size();i++)idx[n-1+i]=i;
		for(int i=_depth.size();i<n;i++)idx[n-1+i]=_depth.size();
		_depth.push_back(INT_MAX);
		for(int i=n-2;i>=0;i--){
			idx[i]=_depth[idx[i*2+1]]<_depth[idx[i*2+2]]?idx[i*2+1]:idx[i*2+2];
		}
	}
	
	std::vector<int> eulertour;
	std::vector<int> in;
	void dfs(const std::vector<int> edge[],int d,int u){
		in[u]=eulertour.size();	
		eulertour.push_back(u);
		_depth.push_back(d);
		for(int i=0;i<edge[u].size();i++){
			dfs(edge,d+1,edge[u][i]);
			eulertour.push_back(u);
			_depth.push_back(d);
		}
	}
public:
	LCA(const std::vector<int> edge[],int n,int root):in(n){
		dfs(edge,1,root);
		eulertour.push_back(root);
		_depth.push_back(0);
		buildRMQ();
	}
	int query(int u,int v)const{//uとvの最小共通祖先
		if(u==v)return u;
		if(in[u]>in[v])std::swap(u,v);
		return eulertour[minimum(in[u], in[v],0,0,n)];
	}
	int depth(int u)const{//rootの深さが1
		return _depth[in[u]];
	}
};






//doubling インターフェース追加
//深さ計算で再帰
class LCA{
	int lg,n;
	std::vector<int> _depth,par;
	int dfs(const vector<int> &parent,int u){
		if(parent[u]==-1)return _depth[u]=1;
		if(_depth[u]!=0)return _depth[u];
		return _depth[u]=dfs(parent,parent[u])+1;
	}
	void init(const vector<int> &parent){
		lg=1;
		while((1<<lg)<n)lg++;
		lg++;
		for(int i=0;i<n;i++)dfs(parent,i);
		par.resize(n*lg);	
		for(int i=0;i<n;i++)par[i]=parent[i];
		for(int i=1;i<lg;i++)
			for(int v=0;v<n;v++)
				if(par[(i-1)*n+v]!=-1)par[i*n+v]=par[(i-1)*n+par[(i-1)*n+v]];
				else par[i*n+v]=-1;
	}
public:
	LCA(const vector<int> &parent):n(parent.size()),_depth(parent.size(),0){
		init(parent);
	}
	LCA(const int parent[],int n){
		LCA(vector<int>(parent,parent+n));
	}
	LCA(const vector<int> edge[],int n):n(n),_depth(n,0){
		vector<int> parent(n,-1);
		for(int i=0;i<n;i++)
			for(int j=0;j<(int)edge[i].size();j++)parent[edge[i][j]]=i;
		init(parent);
	}
	int query(int u,int v)const{
		if(_depth[u] > _depth[v])std::swap(u,v);
		for(int i=lg-1;i>=0;i--)
			if(_depth[u] <= _depth[v]-(1<<i))
				v=par[i*n+v];
		if(u==v)return u;
		for(int i=lg-1;i>=0;i--){
			if(par[i*n+u]!=par[i*n+v]){
				u=par[i*n+u];
				v=par[i*n+v];
			}
		}
		return par[u];
	}
	int depth(int u)const{//rootの深さが1
		return _depth[u];
	}
};

//doubling インターフェースedgeのみ
//非再帰実装
class LCA{
	int lg,n;
	vector<int> _depth,par;
	void bfs(const vector<int> edge[],int root){
		queue<pair<int,int> > q;
		q.push(make_pair(root,1));
		while(!q.empty()){
			pair<int,int> pi=q.front();
			int u=pi.first;
			_depth[u]=pi.second;
			q.pop();
			for(int i=0;i<(int)edge[u].size();i++){
				q.push(make_pair(edge[u][i],pi.second+1));
			}
		}
	}
	void init(const vector<int> &parent){
		lg=1;
		while((1<<lg)<n)lg++;
		lg++;
		par.resize(n*lg);	
		for(int i=0;i<n;i++)par[i]=parent[i];
		for(int i=1;i<lg;i++)
			for(int v=0;v<n;v++)
				if(par[(i-1)*n+v]!=-1)par[i*n+v]=par[(i-1)*n+par[(i-1)*n+v]];
				else par[i*n+v]=-1;
	}
public:
	LCA(const vector<int> edge[],int n):n(n),_depth(n,0){
		vector<int> parent(n,-1);
		for(int i=0;i<n;i++)
			for(int j=0;j<(int)edge[i].size();j++)parent[edge[i][j]]=i;
		bfs(edge,find(parent.begin(),parent.end(),-1)-parent.begin() );
		init(parent);
	}
	int query(int u,int v)const{
		if(_depth[u] > _depth[v])std::swap(u,v);
		for(int i=lg-1;i>=0;i--)
			if(_depth[u] <= _depth[v]-(1<<i))
				v=par[i*n+v];
		if(u==v)return u;
		for(int i=lg-1;i>=0;i--){
			if(par[i*n+u]!=par[i*n+v]){
				u=par[i*n+u];
				v=par[i*n+v];
			}
		}
		return par[u];
	}
	int depth(int u)const{//rootの深さが1
		return _depth[u];
	}
};
