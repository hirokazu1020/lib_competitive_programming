#include<string>
#include<cstring>
using namespace std;


class Trie{
	Trie *child[256];
public:
	Trie(){
		std::memset(child,0,sizeof(child));
	}
	Trie* next(char c){
		int n = c;
		if(child[n])return child[n];
		//return child[n] = new Trie();
		return NULL;
	}
	void add(string s){
		Trie *t = this;
		for(int i=0;i<(int)s.size();i++){
			int c = s[i];
			if(t->child[c])t = t->child[c];
			else t = t->child[c] = new Trie();
		}
	}
	~Trie(){
		for(int i=0;i<256;i++)
			if(child[i])delete child[i];
	}
};



//”ñÄ‹A
class Trie{
	struct Node{
		int child[26];
		Node(){memset(child,-1,sizeof(child));}
	};
	vector<Node> t;
public:
	Trie(){
		t.push_back(Node());
	}
	int next(int v,char c){
		int n = c-'a';
		if(t[v].child[n]!=-1)return t[v].child[n];
		//return -1;
		t[v].child[n]=t.size();
		t.push_back(Node());
		return t[v].child[n];
	}
	void add(string s){
		int v=0;
		for(int i=0;i<(int)s.size();i++){
			v=next(v,s[i]);
			t[v].cnt++;
		}
	}
};




