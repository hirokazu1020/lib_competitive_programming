#include<iostream>
#include<algorithm>
#include<utility>
#include<cassert>
#include<vector>
using namespace std;
#define rep(i,n) for(int i=0;i<(int)(n);i++)
#define INF (1<<30)
 
 
 
template<class Key>
class MultiLLRBT {//left leaning red black tree
	struct Node {
		bool color;
		Key key;
		Node *left, *right;
		int m_size;
		Node(const Key& k)
			:color(true), key(k), left(0), right(0), m_size(1) {}
		‾Node() {
			if (left)delete left;
			if (right)delete right;
		}
	};
	static bool isRed(const Node *u) {
		return u->color;
	}
	static Node *rotate_right(Node *u) {
		Node *s = u->left;
		u->left = s->right;
		s->right = u;
		update(s->right);
		return s;
	}
	static Node *rotate_left(Node *u) {
		Node *s = u->right;
		u->right = s->left;
		s->left = u;
		update(s->left);
		return s;
	}
	static Node *insert(const Key &k, Node *u) {
		if (!u)return new Node(k);
		if (k <= u->key) {
			u->left = insert(k, u->left);
		}
		else if (u->key < k) {
			u->right = insert(k, u->right);
			if (isRed(u->right)) {
				if (!isRed(u)) {
					u->right->color = !u->right->color;
					u->color = !u->color;
				}
				u = rotate_left(u);
			}
		}
		else return u;
		if (u->left && isRed(u->left) && u->left->left && isRed(u->left->left)) {
			u = rotate_right(u);
			u->left->color = !u->left->color;
		}
		return update(u);
	}
	static Node *find(const Key& k, const Node *u) {
		return !u ? NULL : k == u->key ?
			u : k<u->key ? find(k, u->left) : find(k, u->right);
	}
	static std::pair<bool, Node*> erase(const Key &k, Node *u) {
		if (!u)return make_pair(false, u);
		std::pair<bool, Node*> res;
		if (k == u->key) {
			Node *le = u->left, *ri = u->right;
			u->left = u->right = NULL;
			bool col = u->color;
			delete u;
			if (!le) {
				if (col)return make_pair(false, ri);
				if (ri && isRed(ri)) {
					ri->color = false;
					return make_pair(false, ri);
				}
				return make_pair(true, ri);
			}
			if (!ri) {
				if (col)return make_pair(false, le);
				if (isRed(le)) {
					le->color = false;
					return make_pair(false, le);
				}
				return make_pair(true, le);
			}
			res = erase_min(&u, ri);
			ri = res.second;
			u->color = col;
			u->left = le;
			u->right = ri;
			if (res.first) {
				res = balance_right(u);
				u = res.second;
			}
			return make_pair(res.first, update(u));
		}
		if (k < u->key) {
			res = erase(k, u->left);
			u->left = res.second;
			if (res.first) {
				res = balance_left(u);
				u = res.second;
			}
			return make_pair(res.first, update(u));
		}
		else if (u->key < k) {
			res = erase(k, u->right);
			u->right = res.second;
			if (res.first) {
				res = balance_right(u);
				u = res.second;
			}
			return make_pair(res.first, update(u));
		}
		else return make_pair(false, u);
	}
	static std::pair<bool, Node*> erase_min(Node **maxnode, Node *u) {
		if (u->left) {
			std::pair<bool, Node*> res = erase_min(maxnode, u->left);
			u->left = res.second;
			if (res.first) {
				res = balance_left(u);
				u = res.second;
			}
			return make_pair(res.first, update(u));
		}
		else {
			*maxnode = u;
			return make_pair(!isRed(u), u->right);
		}
	}
	static std::pair<bool, Node*> balance_left(Node *u) {
		if (!u->right) {
			if (isRed(u)) {
				u->color = !u->color;
				return make_pair(false, u);
			}
			return make_pair(true, u);
		}
		if (!isRed(u)) {
			if (!u->right->left || !isRed(u->right->left)) {
				u->color = !u->color;
				return make_pair(true,rotate_left(u));
			}
			else {
				u->right = rotate_right(u->right);
				u = rotate_left(u);
				u->color = !u->color;
				return make_pair(false, u);
			}
		}
		else {
			if (!u->right->left || !isRed(u->right->left)) {
				return make_pair(false, rotate_left(u));
			}
			else {
				u->right = rotate_right(u->right);
				u = rotate_left(u);
				u->color = !u->color;
				u->right->color = !u->right->color;
				return make_pair(false,u);
			}
		}
	}
	static std::pair<bool, Node*> balance_right(Node *u) {
		if (!u->left) {
			if (isRed(u)) {
				u->color = !u->color;
				return make_pair(false, u);
			}
			return make_pair(true, u);
		}
		if (!isRed(u)) {
			if (!isRed(u->left)) {
				if (!u->left->left || !isRed(u->left->left)) {
					u->left->color = !u->left->color;
					return make_pair(true, u);
				}
				else {
					u = rotate_right(u);
					u->left->color = !u->left->color;
					return make_pair(false, u);
				}
			}
			else {
				if (!u->left->right) {
					u->left->color = !u->left->color;
					return make_pair(false, rotate_right(u));
				}
				else if(!u->left->right->left || !isRed(u->left->right->left)){
					u = rotate_right(u);
					u->color = !u->color;
					u->right->left->color = !u->right->left->color;
					return make_pair(false, u);
				}
				else {
					u->left = rotate_left(u->left);
					u = rotate_right(u);
					u->left->right->color = !u->left->right->color;
					return make_pair(false, u);
				}
			}
		}
		else {
			return make_pair(false, rotate_right(u));
		}
	}
 
	static const Node *nth_element(int n, const Node *u) {
		if (size(u) <= n)return NULL;
		while (size(u->left) != n) {
			if (n < size(u->left)) {
				u = u->left;
			}
			else {
				n -= size(u->left);
				n--;
				u = u->right;
			}
		}
		return u;
	}
	static int rank_lt(const Key& k, const Node *u) {
		if (!u)return 0;
		if (k <= u->key) {
			return rank_lt(k, u->left);
		}
		else {
			return 1 + size(u->left) + rank_lt(k, u->right);
		}
	}
	static int rank_gt(const Key& k, const Node *u) {
		if (!u)return 0;
		if (k<u->key) {
			return 1 + size(u->right) + rank_gt(k, u->left);
		}
		else {
			return rank_gt(k, u->right);
		}
	}
	static const Key& nextkey(const Key& k, const Node *u) {
		if (!u)return k;
		if (k<u->key) {
			const Key &a = nextkey(k, u->left);
			return k<a ? a : u->key;
		}
		return nextkey(k, u->right);
	}
	static const Key& prevkey(const Key& k, const Node *u) {
		if (!u)return k;
		if (u->key<k) {
			const Key &a = prevkey(k, u->right);
			return a<k ? a : u->key;
		}
		return prevkey(k, u->left);
	}
 
	static Node *update(Node *u) {
		if (!u)return u;
		update_size(u);
		return u;
	}
	static void update_size(Node *u) {
		if (!u)return;
		u->m_size = 1 + size(u->left) + size(u->right);
	}
	static int size(const Node *u) {
		return u ? u->m_size : 0;
	}
 
	Node *root;
public:
	MultiLLRBT() :root(0) {}
	‾MultiLLRBT() { if (root)delete root; }
	void insert(const Key& k) {
		root = insert(k, root);
		root->color = false;
	}
	const Key* find(const Key& k)const {
		return find(k, root);
	}
	void erase(const Key& k) {
		root = erase(k, root).second;
		if (root)root->color = false;
	}
	Key nth_element(int n)const {
		return nth_element(n, root)->key;
	}
	int rank_lt(const Key& k)const {//k未満の個数
		return rank_lt(k, root);
	}
	int rank_gt(const Key& k)const {
		return rank_gt(k, root);
	}
	Key nextkey(const Key& k)const {//kより大の最小のkey,なければkを返す
		return nextkey(k, root);
	}
	Key prevkey(const Key& k)const {//k未満の最大のkey,なければkを返す
		return prevkey(k, root);
	}
	int size()const {
		return size(root);
	}
	int height()const {
		return height(root);
	}
};



int main() {
	iossync_with_stdio(0);
	cin.tie(0);
	int q;
	cin  q;
	MultiLLRBTint s;
	int t, x;
	rep(i, q) {
		cin  t  x;
		if (t == 1) {
			s.insert(x);
		}
		else {
			int k = s.nth_element(x - 1);
			cout  k  endl;
			s.erase(k);
		}
	}
	return 0;
}
