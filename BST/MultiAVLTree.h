#include<iostream>
#include<algorithm>
#include<utility>
#include<cassert>
#include<vector>
using namespace std;
#define rep(i,n) for(int i=0;i<(int)(n);i++)
#define INF (1<<30)



template<class Key>
class MultiAVLTree {
	struct Node {
		int h;
		int bf;// balance factor
		Key key;
		mutable Node *child[2];
		int m_size;
		Node(const Key& k)
			:h(1),bf(0), key(k), m_size(1) {
			child[0] = child[1] = NULL;
		}
		‾Node() {
			if (child[0])delete child[0];
			if (child[1])delete child[1];
		}
	};
	template<int X>
	static Node *rotate(Node *u) {
		Node *s = u->child[X];
		u->child[X] = s->child[X^1];
		s->child[X^1] = u;
		update(u);
		return s;
	}

	static std::pair<bool,Node *> insert(const Key &k, Node *u) {
		if (!u)return make_pair(true,new Node(k));
		if (k <= u->key) {
			std::pair<bool, Node *> res = insert(k, u->child[0]);
			u->child[0] = res.second;
			if (res.first) {
				if(++u->bf==0)res.first = false;
				else if (u->bf == 2) {
					res.first = false;
					u = balance<1>(u);
				}
			}
			return make_pair(res.first, update(u));
		}
		else if (u->key < k) {
			std::pair<bool, Node *> res = insert(k, u->child[1]);
			u->child[1] = res.second;
			if (res.first) {
				if (--u->bf == 0)res.first = false;
				else if (u->bf == -2) {
					res.first = false;
					u = balance<-1>(u);
				}
			}
			return make_pair(res.first, update(u));
		}
		else return make_pair(false, u);
	}
	static Node *find(const Key& k,const Node *u){
		return !u ? NULL : k == u->key ?
			u : k<u->key ? find(k,u->child[0]) :find(k,u->child[1]);
	}
	static std::pair<bool, Node*> erase(const Key &k, Node *u) {
		if (!u)return make_pair(false,u);
		std::pair<bool, Node*> res;
		if (k < u->key) {
			res = erase(k, u->child[0]);
			u->child[0] = res.second;
			if (res.first) {
				if (u->bf-- == 0)res.first=false;
				else if (u->bf == -2) {
					res = balance_erase<-1>(u);
					u = res.second;
				}
			}
			return make_pair(res.first, update(u));
		}
		if (u->key < k) {
			res = erase(k, u->child[1]);
			u->child[1] = res.second;
			if (res.first) {
				if (u->bf++ == 0)res.first = false;
				else if (u->bf == 2) {
					res = balance_erase<1>(u);
					u = res.second;
				}
			}
			return make_pair(res.first, update(u));
		}
		if (k == u->key) {
			Node *le = u->child[0], *ri = u->child[1];
			u->child[0] = u->child[1] = NULL;
			int bf = u->bf;
			delete u;
			if (!le)return make_pair(true, ri);
			if (!ri)return make_pair(true, le);
			res = erase_max(&u, le);
			le = res.second;
			u->bf = bf;
			u->child[0] = le;
			u->child[1] = ri;
			if (res.first) {
				if (u->bf-- == 0)res.first = false;
				else if (u->bf == -2) {
					res = balance_erase<-1>(u);
					u = res.second;
				}
			}
			return make_pair(res.first, update(u));
		}
		else return make_pair(false,u);
	}
	static std::pair<bool,Node*> erase_max(Node **maxnode, Node *u) {
		if (u->child[1]) {
			std::pair<bool, Node*> res= erase_max(maxnode,u->child[1]);
			u->child[1] = res.second;
			if (res.first) {
				if (u->bf++ == 0)res.first = false;
				else if (u->bf == 2) {
					res = balance_erase<1>(u);
					u = res.second;
				}
			}
			return make_pair(res.first, update(u));
		}
		else {
			*maxnode = u;
			return make_pair(true,u->child[0]);
		}
	}
	template<int X>
	static std::pair<bool, Node*> balance_erase(Node *u) {
		const int d = X == 1 ? 0 : 1;
		if (u->child[d]->bf == 0) {
			u->bf -= X;
			u->child[d]->bf -= X;
			return make_pair(false, rotate<d>(u));
		}
		else {
			return make_pair(true, balance<X>(u));
		}
	}

	template<int X>
	static Node* balance(Node *u) {
		const int d = X == 1 ? 0 : 1;
		if (u->child[d]->bf == X) {
			u->bf = u->child[d]->bf = 0;
			return rotate<d>(u);
		}
		else {
			if (u->child[d]->child[d ^ 1]->bf == 0) {
				u->child[d]->child[d ^ 1]->bf = -u->child[d]->bf;
				u->child[d]->bf = 0;
				u->child[d] = rotate<d ^ 1>(u->child[d]);
				u->bf = u->child[d]->bf = 0;
				return rotate<d>(u);
			}
			if (u->child[d]->child[d ^ 1]->bf == -X) {
				u->child[d]->bf = X;
				u->child[d] = rotate<d ^ 1>(u->child[d]);
				u->bf = u->child[d]->bf = 0;
				return rotate<d>(u);
			}
			else {
				u->child[d]->bf = 0;
				u->child[d] = rotate<d ^ 1>(u->child[d]);
				u->bf = -X;
				u->child[d]->bf = 0;
				return rotate<d>(u);
			}
		}
	}

	static const Node *nth_element(int n,const Node *u) {
		if (size(u) <= n)return NULL;
		while (size(u->child[0]) != n) {
			if ( n < size(u->child[0])) {
				u = u->child[0];
			}
			else {
				n -= size(u->child[0]);
				n--;
				u = u->child[1];
			}
		}
		return u;
	}
	static int rank_lt(const Key& k,const Node *u) {
		if (!u)return 0;
		if (k <= u->key) {
			return rank_lt(k, u->child[0]);
		}
		else {
			return 1 + size(u->child[0]) + rank_lt(k, u->child[1]);
		}
	}
	static int rank_gt(const Key& k,const Node *u) {
		if (!u)return 0;
		if (k<u->key) {
			return 1 + size(u->child[1]) + rank_gt(k, u->child[0]);
		}
		else {
			return rank_gt(k,u->child[1]);
		}
	}
	static const Key& nextkey(const Key& k,const Node *u) {
		if (!u)return k;
		if (k<u->key) {
			const Key &a =nextkey(k,u->u->child[0]);
			return k<a ? a : u->key;
		}
		return nextkey(k, u->child[1]);
	}
	static const Key& prevkey(const Key& k,const Node *u) {
		if (!u)return k;
		if (u->key<k) {
			const Key &a = prevkey(k,u- u->child[1]);
			return a<k ? a : u->key;
		}
		return prevkey(k, u->child[0]);
	}

	static Node *update(Node *u) {
		//if (!u)return u;
		update_size(u);
		update_height(u);
		assert(u->bf == height(u->child[0])- height(u->child[1]));
		return u;
	}
	static void update_size(Node *u) {
		if (!u)return;
		u->m_size = 1 + size(u->child[0]) + size(u->child[1]);
	}
	static int size(const Node *u) {
		return u ? u->m_size : 0;
	}
	static int height(const Node *u) {
		return u ? u->h : 0;
	}
	static void update_height(Node *u) {
		u->h = 1+max(height(u->child[0]),height(u->child[1]) );
	}

	Node *root;
public:
	MultiAVLTree() :root(0) {}
	‾MultiAVLTree() { if (root)delete root; }
	void insert(const Key& k) {
		root = insert(k, root).second;
	}
	const Key* find(const Key& k)const {
		return find(k, root);
	}
	void erase(const Key& k) {
		root = erase(k,root).second;
	}
	Key nth_element(int n)const {
		return nth_element(n,root)->key;
	}
	int rank_lt(const Key& k)const {//k未満の個数
		return rank_lt(k,root);
	}
	int rank_gt(const Key& k)const {
		return rank_gt(k,root);
	}
	Key nextkey(const Key& k)const {//kより大の最小のkey,なければkを返す
		return nextkey(k,root);
	}
	Key prevkey(const Key& k)const {//k未満の最大のkey,なければkを返す
		return prevkey(k,root);
	}
	int size()const {
		return size(root);
	}
};



int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	int q;
	cin >> q;
	MultiAVLTree<int> s;
	vector<int> v;
	int t, x;
	rep(i, q) {
		cin >> t >> x;
		if (t == 1) {
			s.insert(x);
			v.push_back(x); sort(v.begin(),v.end());
		}
		else {
			int k = s.nth_element(x - 1);
			cout << v[x - 1] << endl;
			cout << k << endl;
			s.erase(k);
			v.erase(v.begin()+(x-1));
		}
		//rep(i, v.size())cout<< ' ' << v[i]; cout << endl;
		//rep(i, s.size())cout << ' ' << s.nth_element(i); cout << endl;
	}
	return 0;
}