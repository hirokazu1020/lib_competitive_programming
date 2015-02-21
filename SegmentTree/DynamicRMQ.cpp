
#include<iostream>
#include<algorithm>
using namespace std;

#define INF (1<<30)


class DynamicRMQ{//–§ver AVLTree
	struct Node{
		int height;
		Node *left,*right;
		int size;
		int val,minsub;
		
		Node(int k,int v)
			:height(1),left(0),right(0),size(1),val(v),minsub(v){}
		~Node(){
			if(left)delete left;
			if(right)delete right;
		}
		Node *rotate_right(){
			Node *s=left;
			left=s->right;
			s->right=this;
			return s;
		}
		Node *rotate_left(){
			Node *s=right;
			right=s->left;
			s->left=this;
			return s;
		}
		Node *insert(int k,int v){
			if(!this)return new Node(k,v);
			int si=left?left->size:0;
			if(k <= si){
				left = left->insert(k,v);
			}else {//key < k
				right = right->insert(k-si-1,v);
			}
			return balance();
		}

		Node *erase(int k){
			if(!this)return NULL;
			int si=left?left->size:0;
			if(k==si){
				Node *le=left,*ri=right;
				left=right=NULL;
				delete this;
				if(!le){
					return ri;
				}
				if(!ri){
					return le;
				}
				Node *t;
				le = le->erase_max(&t);
				t->left = le;
				t->right = ri;
				return t->balance();
			}
			if(k < si){
				left = left->erase(k);
			}else if(si < k){
				right = right->erase(k-si-1);
			}
			return balance();
		}
		int min(int a,int b,int l,int r)const{
			if(!this || r<a || b<l)return INF;
			if(a<=l && r<=b)return minsub;
			int si=left?left->size:0;
			int res=a<=l+si&&l+si<b?val:INF;
			res = std::min(res,left->min(a,b,l,l+si));
			res = std::min(res,right->min(a,b,l+si+1,r));
			return res;
		}
		Node *balance(){
			int bf = (right?right->height:0)-(left?left->height:0);
			if(2==bf){
				Node *t=right;
				if((t->right?t->right->height:0) < (t->left?t->left->height:0)){
					right = t->rotate_right();
				}
				return rotate_left()->update();
			}else if(bf==-2){
				Node *t=left;
				if((t->right?t->right->height:0) > (t->left?t->left->height:0)){
					left = t->rotate_left();
				}
				return rotate_right()->update();
			}
			updates();
			return this;
		}
		Node *update(){
			if(left)left->updates();
			if(right)right->updates();
			updates();
			return this;
		}
		void updates(){
			update_height();
			update_size();
			update_min();
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
		void update_min(){
			if(!this)return;
			minsub = val;
			if(left)minsub=std::min(minsub,left->minsub);
			if(right)minsub=std::min(minsub,right->minsub);
		}
		Node* erase_max(Node **maxnode){
			if(right){
				right = right->erase_max(maxnode);
				return balance();
			}else{
				*maxnode=this;
				return left;
			}
		}
	} *root;
public:
	DynamicRMQ():root(0){}
	~DynamicRMQ(){if(root)delete root;}
	void insert(int k,int v){
		root=root->insert(k,v);
	}
	void erase(int k){
		root=root->erase(k);
	}
	void set(int k,int v){
		root=root->erase(k);
		root=root->insert(k,v);
	}	
	int min(int a,int b)const{//”¼ŠJ‹æŠÔ[a,b)
		return root->min(a,b,0,root->size);
	}
};