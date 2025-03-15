#include<iostream>
#include<ctime>
#include<map>
using namespace std;


class Nothing{};

template<class Key,class Val=Nothing>
class AVLTree{
	struct Node{
		int height;
		Key key;
		Val value;
		Node *left,*right;
		int size;
		Node(const Key& k,const Val& v)
			:height(1),key(k),value(v),left(0),right(0),size(1){}
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
		Node *insert(const Key &k,const Val &v){
			if(!this)return new Node(k,v);
			if(k < key){
				left = left->insert(k,v);
			}else if(key < k){
				right = right->insert(k,v);
			}else return this;
			return balance();
		}
		Node *find(const Key& k){
			return !this?NULL:k==key?
				this:k<key?left->find(k):right->find(k);
		}
		Node *erase(const Key &k){
			if(!this)return NULL;
			if(k==key){
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
			if(k < key){
				left = left->erase(k);
			}else if(key < k){
				right = right->erase(k);
			}else return this;
			return balance();
		}
		Node *nth_element(int n){
			if(size<=n)return NULL;
			Node *t=this;
			while((t->left?t->left->size:0)!=n){
				if(t->left && n<=t->left->size){
					t=t->left;
				}else {
					if(t->left)n-=t->left->size;
					n--;
					t=t->right;
				}
			}
			return t;
		}
		int rank_lt(const Key& k)const{
			if(!this)return 0;
			if(k==key){
				return left?left->size:0;
			}else if(k<key){
				return left->rank_lt(k);
			}else{
				return 1 + (left?left->size:0) + right->rank_lt(k);
			}
		}
		int rank_gt(const Key& k)const{
			if(!this)return 0;
			if(k==key){
				return right?right->size:0;
			}else if(k<key){
				return 1 + (right?right->size:0) + left->rank_gt(k);
			}else{
				return right->rank_gt(k);
			}
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
	AVLTree():root(0){}
	~AVLTree(){if(root)delete root;}
	void insert(const Key& k,const Val& v=Nothing()){
		root=root->insert(k,v);
	}
	Val* find(const Key& k){
		Node *p=root->find(k);
		if(p)return &p->value;
		return NULL;
	}
	void erase(const Key& k){
		root=root->erase(k);
	}
	Key nth_element(int n)const{
		return root->nth_element(n)->key;
	}
	int rank_lt(const Key& k)const{//k未満の個数
		return root->rank_lt(k);
	}
	int rank_gt(const Key& k)const{
		return root->rank_gt(k);
	}
	int size()const{
		return root?root->size:0;
	}
	int height()const{
		return root?root->height:0;
	}
};


