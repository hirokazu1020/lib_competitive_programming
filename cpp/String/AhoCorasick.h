#include<iostream>
#include<vector>
#include<queue>
#include<string>
#include<cstring>
using namespace std;


class PMA{//Aho-Corasick
	struct Node{
		bool match;
		int failure;
		int next[256];
		Node():failure(-1),match(false){memset(next,-1,sizeof(next));}
	};
	vector<Node> node;
public:
	PMA(){}
	~PMA(){
		node.clear();
	}
	int size()const{return node.size();}
	bool matched(int id)const{return node[id].match;}
	void build(const vector<string> &p){
		node.clear();
		node.push_back(Node());
		for(int i=0;i<(int)p.size();i++){
			const string &s=p[i];
			int t=0;
			for(int j=0;j<(int)s.size();j++){
				if(node[t].next[s[j]]==-1){
					node[t].next[s[j]]=node.size();
					node.push_back(Node());				
				}
				t=node[t].next[s[j]];
			}
			node[t].match=true;
		}
		queue<int> q;
		for(int i=0;i<256;i++){
			Node &root=node[0];
			if(root.next[i]!=-1){
				q.push(root.next[i]);
				node[root.next[i]].failure=0;
			}else root.next[i]=0;
		}
		while(!q.empty()){
			int t=q.front();q.pop();
			for(int i=0;i<256;i++){
				if(node[t].next[i]!=-1){
					q.push(node[t].next[i]);
					int r=step(node[t].failure,i);
					int v=node[t].next[i];
					node[v].failure = r;
					if(r!=-1)node[v].match |= node[r].match;
				}
			}
		}
	}
	int step(int t,char c)const{
		while(node[t].next[c]==-1)t=node[t].failure;
		return node[t].next[c];
	}
};
