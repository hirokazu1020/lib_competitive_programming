#include<iostream>
#include<vector>
#include<utility>
#include<algorithm>
#include<climits>
#include<stack>
#include<cassert>
using namespace std;



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




class BIT{//binary indexed tree, Fenwick treeとも
	std::vector<int> bit;
public:
	BIT(const vector<int> &v):bit(v.size()+1,0){
		for(int i=0;i<(int)v.size();i++)
			add(i,v[i]);
	}
	void add(int i,int x){//i番目にx加える
		i++;//BIT添え字は1〜ｎだから
		while(i<(int)bit.size()){
			bit[i]+=x;
			i+= i&-i;
		}
	}
	int sum(int a)const{//[0,a]の和
		a++;
		int res=0;
		while(0<a){
			res+=bit[a];
			a-= a&-a;
		}
		return res;
	}
	int sum(int a,int b)const{//[a,b]の合計
		return sum(b)-sum(a-1);
	}
	void zeroclear(){
		bit.assign(bit.size(),0);
	}
};




class HLDecomposition{
	struct S{
		pair<int,int> par;
		BIT segtree;
		S(int a,int b,vector<int> v):par(a,b),segtree(v){}
	};
	void max_subtree(int u,vector<int> edge[],vector<int> &msz){
		stack<int> st;
		vector<int> size(msz.size());
		st.push(u);
		while(!st.empty()){
			u=st.top();
			st.pop();
			if(u<0){
				u=-u-1;
				int res=0,sz=0,idx=-1;
				for(int i=0;i<(int)edge[u].size();i++){
					int s=size[edge[u][i]];
					if(sz<s){
						sz=s;
						idx=i;
					}
					res+=s;
				}
				size[u]=res+1;
				msz[u]=idx;
			}else{
				st.push(-u-1);
				for(int i=0;i<(int)edge[u].size();i++){
					st.push(edge[u][i]);
				}
			}
		}
	}
	void build(int u,vector<int> edge[],int val[],vector<int> &msz){
		vector<int> heavy;
		int v=u;
		int par=compressed.size(),pi=0,s;
		while(1){
			ver[v]=make_pair(par,heavy.size());
			heavy.push_back(val[v]);
			if(msz[v]==-1)break;
			v=edge[v][msz[v]];
		}
		compressed.push_back(S(-1,-1,heavy));
		heavy.clear();
		while(1){
			for(int i=0;i<(int)edge[u].size();i++){
				if(i==msz[u])continue;
				s=compressed.size();
				build(edge[u][i],edge,val,msz);
				compressed[s].par=make_pair(par,pi);
			}
			if(msz[u]==-1)break;
			u=edge[u][msz[u]];
			pi++;
		}
	}
	vector<S> compressed;
	vector<pair<int,int> > ver;
	LCA lca;
public:
	HLDecomposition(vector<int> edge[],int val[],int n):ver(n),lca(edge,n){
		vector<int> maxsz(n,-1);
		max_subtree(0,edge,maxsz);
		build(0,edge,val,maxsz);
	}
	long long query(int u,int v)const{//u->vの頂点の和
		int w=lca.query(u,v);
		pair<int,int> a,b,c;
		a=ver[u];
		b=ver[v];
		c=ver[w];
		long long res=0;
		while(c.first!=a.first){
			res+=compressed[a.first].segtree.sum(a.second);
			a=compressed[a.first].par;
		}
		res+=compressed[c.first].segtree.sum(c.second,a.second);
		while(c.first!=b.first){
			res+=compressed[b.first].segtree.sum(b.second);
			b=compressed[b.first].par;
		}
		if(c.second+1<=b.second)res+=compressed[c.first].segtree.sum(c.second+1,b.second);
		return res;
	}
	void update(int u,int val){
		pair<int,int> a=ver[u];
		int x=compressed[a.first].segtree.sum(a.second,a.second);
		compressed[a.first].segtree.add(a.second,val-x);
	}
};


