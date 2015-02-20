#include<iostream>
using namespace std;

//要verify
//挿入削除をサポートする動的な列
template<class Val>
class DynamicArray{
	struct Node{
		int height;
		Node *left,*right;
		int size;
		Val val;
		
		Node(int k,Val val)
			:height(1),left(0),right(0),size(1),val(val){}
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
		Node *insert(int k,Val v){
			if(!this)return new Node(k,v);
			int si=(left?left->size:0);
			if(k <= si){
				left = left->insert(k,v);
			}else {
				right = right->insert(k-si-1,v);
			}
			return balance();
		}
		Node *find(int k){
			if(!this)return NULL;
			int si=(left?left->size:0);
			return k==si?
				this:k<si?left->find(k):right->find(k-si-1);
		}
		Node *erase(int k){
			if(!this)return NULL;
			int si=(left?left->size:0);
			if(k==si){
				Node *le=left,*ri=right;
				left=right=NULL;
				delete this;
				if(!le)return ri;
				if(!ri)return le;
				Node *t;
				le = le->erase_max(&t);
				t->left = le;
				t->right = ri;
				return t->balance();
			}
			if(k < si){
				left = left->erase(k);
			}
			else right = right->erase(k-si-1);
			return balance();
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
	DynamicArray():root(0){}
	~DynamicArray(){if(root)delete root;}
	void insert(int k,int v){
		root=root->insert(k,v);
	}
	void erase(int k){
		root=root->erase(k);	
	}
	Val access(int n)const{
		return root->find(n)->val;
	}
	int size()const{
		return root->size;
	}
};
