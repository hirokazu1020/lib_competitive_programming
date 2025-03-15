#include <cstdint>
#include <bit>
#include <nmmintrin.h>
#include <immintrin.h>


inline int popCount(uint64_t x) {
	return std::popcount(x);
}

inline int popSelect(uint64_t block1, uint32_t k){
#if 1
	// -march=native
	uint64_t z = 1ULL << (k - 1);
	uint64_t y = _pdep_u64(z, block1);
	return _tzcnt_u64(y);
#else
	uint64_t block2 = (block1>>1  & 0x5555555555555555)+(block1  & 0x5555555555555555);
	uint64_t block4 = (block2>>2  & 0x3333333333333333)+(block2  & 0x3333333333333333);
	uint64_t block8 = (block4>>4  & 0x0f0f0f0f0f0f0f0f)+(block4  & 0x0f0f0f0f0f0f0f0f);
	uint64_t block16= (block8>>8  & 0x00ff00ff00ff00ff)+(block8  & 0x00ff00ff00ff00ff);
	uint64_t block32= (block16>>16& 0x0000ffff0000ffff)+(block16 & 0x0000ffff0000ffff);
	int pos = 0;
	if((block32 & 0xffffffff) < k){
		k -= block32 & 0xffffffff;
		pos |= 32;
	}
	if((block16 >> pos & 0x0000ffff) < k){
		k -= block16 >> pos & 0x0000ffff;
		pos |= 16;
	}
	if((block8 >> pos & 0x000000ff) < k){
		k -= block8 >> pos & 0x000000ff;
		pos |= 8;
	}
	if((block4 >> pos & 0x0000000f) < k){
		k -= block4 >> pos & 0x0000000f;
		pos |= 4;
	}
	if((block2 >> pos & 0x00000003) < k){
		k -= block2 >> pos & 0x00000003;
		pos |= 2;
	}
	if((block1 >> pos & 0x00000001) < k)pos |= 1;
	return pos;
#endif
}


class DynamicBitVector {
	struct Node {
		int height;
		Node *left, *right, *par;
		int rank, len;
		int sumlen;
		unsigned long long val;

		Node(int val)
			:height(1), left(0), right(0), par(0), rank(val != 0), len(1), sumlen(1), val(val) {}
		‾Node() {
			if (left)delete left;
			if (right)delete right;
		}
		Node *rotate_right() {
			Node *s = left;
			s->par = par;
			left = s->right;
			if (left)left->par = this;
			s->right = this;
			par = s;
			update();
			return s;
		}
		Node *rotate_left() {
			Node *s = right;
			s->par = par;
			right = s->left;
			if (right)right->par = this;
			s->left = this;
			par = s;
			update();
			return s;
		}
		Node *balance() {
			int bf = (right ? right->height : 0) - (left ? left->height : 0);
			if (2 == bf) {
				Node *t = right;
				if ((t->right ? t->right->height : 0) < (t->left ? t->left->height : 0)) {
					right = t->rotate_right();
				}
				return rotate_left()->update();
			}
			else if (bf == -2) {
				Node *t = left;
				if ((t->right ? t->right->height : 0) > (t->left ? t->left->height : 0)) {
					left = t->rotate_left();
				}
				return rotate_right()->update();
			}
			return update();
		}
		Node *update() {
			if (!this)return this;
			rank = popCount(val);
			sumlen = len;
			height = 0;
			if (left) {
				rank += left->rank;
				sumlen += left->sumlen;
				height = left->height;
			}
			if (right) {
				rank += right->rank;
				sumlen += right->sumlen;
				if (right->height > height)height = right->height;
			}
			height++;
			return this;
		}
	};


	static Node* insert(Node *u, int k, int v) {
		Node *root = u;
		if (root == nullptr)return new Node(v);
		int dir = 0;
		while (1) {
			int si = (u->left != nullptr ? u->left->sumlen : 0);
			if (k < si) {
				if (u->left == nullptr) {
					u->left = new Node(v);
					u->left->par = u;
					break;
				}
				u->rank += v;
				u->sumlen++;
				u = u->left;
				dir = dir << 1 | 1;
			}
			else if (k - si <= u->len) {
				u->rank += v;
				u->sumlen++;
				k -= si;
				if (u->len < 64) {
					u->len++;
					u->val = (‾((1ULL << k) - 1)&u->val) << 1 | (unsigned long long)v << k | (((1ULL << k) - 1)&u->val);
					return root;
				}
				else {
					int a = v;
					if (k < 64) {
						a = u->val >> 63 & 1;
						u->val = (‾((1ULL << k) - 1)&u->val) << 1 | (unsigned long long)v << k | (((1ULL << k) - 1)&u->val);
						v = a;
					}
					k = 0;
					if (u->right == nullptr) {
						u->right = new Node(v);
						u->right->par = u;
						break;
					}
					u = u->right;
					dir = dir << 1 | 0;
				}
			}
			else {
				if (u->right == nullptr) {
					u->right = new Node(v);
					u->right->par = u;
					break;
				}
				k -= si + u->len;
				u->rank += v;
				u->sumlen++;
				u = u->right;
				dir = dir << 1 | 0;
			}
		}
		while (u->par != nullptr) {
			int h = u->height;
			u = u->balance();
			if (dir & 1)u->par->left = u;
			else u->par->right = u;
			dir >>= 1;
			if (h == u->height)break;
			u = u->par;
		}
		if (u == root)return u->balance();
		return root;
		//while (u->par != nullptr) u = u->par;
		//return u;
	}
	static int access(const Node *u, int k) {
		while (1) {
			int si = (u->left ? u->left->sumlen : 0);
			if (k < si)u = u->left;
			else if (k < si + u->len)return u->val >> k - si & 1;
			else {
				k -= si + u->len;
				u = u->right;
			}
		}
	}
	static Node* erase(Node *u, int k) {
		Node *root = u;
		if (root == nullptr)return root;
		if (root->sumlen == 1) {
			delete root;
			return nullptr;
		}
		if (k < 0 || u->sumlen <= k)return root;
		int dir = 0;
		while (1) {
			int si = (u->left != nullptr ? u->left->sumlen : 0);
			if (k < si) {
				u->sumlen--;
				u = u->left;
				dir = dir << 1 | 1;
			}
			else if (k - si < u->len) {
				u->len--;
				u->sumlen--;
				k -= si;
				int del = u->val >> k & 1;
				if (0 < u->len) {
					u->val = (‾((1ULL << k) | (1ULL << k) - 1)&u->val) >> 1 | (((1ULL << k) - 1)&u->val);
					if (del == 0)return root;
					while (u) {
						u->rank--;
						u = u->par;
					}
					return root;
				}
				else {
					break;
				}
			}
			else {
				k -= si + u->len;
				u->sumlen--;
				u = u->right;
				dir = dir << 1 | 0;
			}
		}

		if (u->left == nullptr) {
			Node *ri = u->right;
			u->left = u->right = nullptr;
			if (ri)ri->par = u->par;
			if (u->par) {
				if (dir & 1)u->par->left = ri;
				else u->par->right = ri;
				dir >>= 1;
			}
			Node *s = u->par;
			delete u;
			u = s;
		}
		else {
			Node *s = u->left;
			dir = dir << 1 | 1;
			while (s->right) {
				s = s->right;
				dir = dir << 1 | 0;
			}
			u->len = s->len;
			u->val = s->val;

			Node *t;
			if (s->left) {
				t = s->left;
				t->par = s->par;
			}
			else {
				t = s->par;
				if (dir & 1)t->left = nullptr;
				else t->right = nullptr;
				dir >>= 1;
			}
			s->left = nullptr;
			delete s;
			u = t;
		}
		while (u->par != nullptr) {
			u = u->balance();
			if (dir & 1)u->par->left = u;
			else u->par->right = u;
			dir >>= 1;
			u = u->par;
		}
		return u->balance();
	}
	static int rank_lt(const Node *u, int k) {
		int r = 0;
		while (1) {
			int si = (u->left ? u->left->sumlen : 0);
			if (k < si)u = u->left;
			else if (k < si + u->len)return r + (u->left ? u->left->rank : 0) + popCount(((1ULL << k - si) - 1)&u->val);
			else {
				if (u->right == nullptr)return r + u->rank;
				k -= si + u->len;
				r += u->rank - u->right->rank;
				u = u->right;
			}
		}
		return r;
	}
	static int select1(const Node *u, int k) {
		int r = 0;
		while (1) {
			int sr = (u->left ? u->left->rank : 0);
			if (k < sr)u = u->left;
			else if (k < sr + popCount(u->val))return r + (u->left ? u->left->sumlen : 0) + popSelect(u->val, k - sr);
			else {
				k -= u->rank - u->right->rank;
				r += u->sumlen - u->right->sumlen;
				u = u->right;
			}
		}
		return r;
	}
	static int select0(const Node *u, int k) {
		int r = 0;
		while (1) {
			int sr = (u->left ? u->left->sumlen - u->left->rank : 0);
			if (k < sr)u = u->left;
			else if (k < sr + u->len - popCount(u->val))return r + (u->left ? u->left->sumlen : 0) + popSelect(‾u->val, k - sr);
			else {
				k -= (u->sumlen - u->right->sumlen) - (u->rank - u->right->rank);
				r += u->sumlen - u->right->sumlen;
				u = u->right;
			}
		}
		return r;
	}

	Node *root;
public:
	DynamicBitVector() :root(0) {}
	‾DynamicBitVector() { if (root)delete root; }
	void insert(int k, int v) {
		root = insert(root, k, v);
	}
	void erase(int k) {
		root = erase(root, k);
	}
	int access(int n)const {
		return access(root, n);
	}
	int size()const {
		return root ? root->sumlen : 0;
	}
	int rank(int k)const {//[0,k)の1の個数
		return rank_lt(root, k);
	}
	int rank()const {
		return root ? root->rank : 0;
	}
	int select1(int k)const { return select1(root, k); }//k+1番目の1
	int select0(int k)const { return select0(root, k); }
};
