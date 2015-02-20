#include<iostream>
#include<utility>
//using namespace std;

unsigned int xor128(){
	static unsigned int x=123456789,y=362436069,z=521288629,w=88675123; 
	unsigned int t=(x^(x<<11));
	x=y;y=z;z=w;
	return w=(w^(w>>19))^(t^(t>>8));
}

//要verify
//挿入削除をサポートする動的な列
template<class Val>
class TreapArray{
	struct Node{
		unsigned int priority;
		int height;
		Node *left,*right;
		int size;
		Val val;
		
		Node(Val val)
			:priority(xor128()),height(1),left(0),right(0),size(1),val(val){}
		~Node(){
			if(left)delete left;
			if(right)delete right;
		}
		Node *find(int k){
			if(!this)return NULL;
			int si=(left?left->size:0);
			return k==si?
				this:k<si?left->find(k):right->find(k-si-1);
		}
		Node* updates(){
			update_height();
			update_size();
			return this;
		}
		void update_height(){
			if(!this)return;
			if(right){
				if(left && right->height < left->height)height=left->height+1;
				else height=right->height+1;
			}else if(left)height=left->height+1;
			else height=1;
		}
		void update_size(){
			if(!this)return;
			size = 1 + (left?left->size:0) + (right?right->size:0);
		}
	};
	static Node *merge(Node *a,Node *b){
		if(!a)return b;
		if(!b)return a;
		if( a->priority > b->priority  ){
			a->right=merge(a->right,b);
			return a->updates();
		}else{
			b->left=merge(a,b->left);
			return b->updates();
		}
	}
	static std::pair<Node*,Node*> split(Node *a,int k){//[0,k),[k..)に分割
		if(!a)return std::make_pair((Node*)NULL,(Node*)NULL);
		int sz=a->left?a->left->size:0;
		if(k > sz){
			std::pair<Node*,Node*> p=split(a->right,k-1-sz);
			a->right=NULL;
			a->updates();
			p.first = merge(a,p.first);
			return p;
		}else if(k < sz){
			std::pair<Node*,Node*> p=split(a->left,k);
			a->left=NULL;
			a->updates();
			p.second = merge(p.second,a);
			return p;
		}else{
			Node *b=a->left;
			a->left=NULL;
			a->update_height();
			return std::make_pair(b,a->updates());
		}
	}
	Node *root;
public:
	TreapArray(Node* a):root(a){}
	TreapArray():root(0){}
	~TreapArray(){if(root)delete root;}
	void insert(int k,int v){
		std::pair<Node*,Node*> p=split(root,k);
		root=merge(p.first,merge(new Node(v),p.second));
	}
	void merge(TreapArray &a){//*thisとaを合併
		root=merge(root,a.root);
	}
	void erase(int k){
		std::pair<Node*,Node*> p,q;
		p=split(root,k);
		q=split(p.second,1);
		delete q.first;
		root=merge(p.first,q.second);
	}
	void split(std::pair<TreapArray,TreapArray> &res, int k){//[0,k),[k..)に分割
		pair<Node*,Node*> a=split(root,k);
		root=0;
		if(res.first.root)res.first.root->~Node();
		if(res.second.root)res.second.root->~Node();
		res.first.root=a.first;
		res.second.root=a.second;
	}
	Val access(int n)const{
		return root->find(n)->val;
	}
	int size()const{
		return root?root->size:0;
	}
};
