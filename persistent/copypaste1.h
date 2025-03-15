#define _CRT_SECURE_NO_WARNINGS
#include<sstream>
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<climits>
#include<cmath>
#include<string>
#include<vector>
#include<set>
#include<map>
#include<queue>
#include<numeric>
#include<functional>
#include<algorithm>
#include<bitset>
using namespace std;
#define INF (1<<29)
#define rep(i,n) for(int i=0;i<(int)(n);i++)
#define all(v) v.begin(),v.end()
#define uniq(v) v.erase(unique(all(v)),v.end())


//verified  copypaste - コピー＆ペースト


template<class T, int MAXSIZE = 1500000>
class GCPointer {
	static pair<T, int> * const pool;
	static int index;

	static pair<T, int>* alloc() {
		//int cnt = 0; 
		while (1) {
			if (++index == MAXSIZE)index = 0;
			if (pool[index].second == 0)break;
			//if (++cnt == MAXSIZE)abort();
		}
		return pool + index;
	}

	pair<T, int>* p;

	void release() {
		if (p && --p->second == 0) {
			p->first.‾T();
		}
	}
public:
	GCPointer() :p(nullptr) {}
	GCPointer(const T& v) {
		p = alloc();
		new(&p->first) T(v);
		p->second++;
	}
	GCPointer(const GCPointer &q) {
		p = q.p;
		if (p)p->second++;
	}
	GCPointer(GCPointer &&q) {
		p = q.p;
		q.p = nullptr;
	}
	‾GCPointer() {
		release();
	}
	T* operator->() const {
		return &p->first;
	}
	T& operator*() const {
		return p->first;
	}
	GCPointer& operator =(const GCPointer &q) {
		if (p == q.p)return *this;
		release();
		p = q.p;
		if (p)p->second++;
		return *this;
	}
	bool null() const {
		return p == nullptr;
	}
	bool exist() const {
		return p != nullptr;
	}
	void _new() {
		p = alloc();
		new(&p->first) T;
		p->second++;
	}
	void clear() {
		release();
		p = nullptr;
	}
};
template<class T, int MAXSIZE>
pair<T, int> * const GCPointer<T, MAXSIZE>::pool = (pair<T, int> *)calloc(MAXSIZE, sizeof(pair<T, int>));
template<class T, int MAXSIZE>
int GCPointer<T, MAXSIZE>::index = -1;







unsigned int xor128() {
	static unsigned int x = 123456789, y = 362436069, z = 521288629, w = 88675123;
	unsigned int t = (x ^ (x << 11));
	x = y; y = z; z = w;
	return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}



template<class Val>
class PersistentRBSTArray {
	struct Node;
	using Pointer = GCPointer<Node>;

	struct Node {
		Pointer left, right;
		int size;
		Val val;

		Node(Val val)
			:left(), right(), size(1), val(val) {}
		‾Node() {}
	};

	static Pointer find(const Pointer &p, int k) {
		if (p.null())return Pointer();
		int si = (p->left.exist() ? p->left->size : 0);
		return k == si ?
			p : k<si ? find(p->left, k) : find(p->right, k - si - 1);
	}

	static Pointer merge(const Pointer &_a, const Pointer &_b) {
		if (_a.null())return _b;
		if (_b.null())return _a;
		if (((long long)_a->size << 32) >(long long)(_a->size + _b->size)*xor128()) {
			Pointer a(*_a);
			a->size += _b->size;
			a->right = merge(_a->right, _b);
			return std::move(a);
		}
		else {
			Pointer b(*_b);
			b->size += _a->size;
			b->left = merge(_a, _b->left);
			return std::move(b);
		}
	}
	static std::pair<Pointer, Pointer> split(const Pointer &_a, int k) {//[0,k),[k..)に分割
		if (_a.null())return std::make_pair(Pointer(), Pointer());
		Pointer a(*_a);
		int sz = a->left.exist() ? a->left->size : 0;
		if (k > sz) {
			if (a->right.exist())a->size -= a->right->size;
			std::pair<Pointer, Pointer> p = split(a->right, k - 1 - sz);
			a->right = p.first;
			if (a->right.exist())a->size += a->right->size;
			return make_pair(a, p.second);
		}
		else if (k < sz) {
			a->size -= a->left->size;
			std::pair<Pointer, Pointer> p = split(a->left, k);
			a->left = p.second;
			a->size += a->left->size;
			return make_pair(p.first, a);
		}
		else {
			if (a->left.exist())a->size -= a->left->size;
			Pointer b = a->left;
			a->left.clear();
			return std::make_pair(b, a);
		}
	}


	PersistentRBSTArray(Pointer p) :root(p) {}

	Pointer root;
public:
	PersistentRBSTArray() {}
	‾PersistentRBSTArray() {}
	PersistentRBSTArray insert(int k, Val v) const {
		std::pair<Pointer, Pointer> p = split(root, k);
		return merge(p.first, merge(Pointer(v), p.second));
	}
	PersistentRBSTArray merge(const PersistentRBSTArray &a) const {//*thisとaを合併
		return merge(root, a.root);
	}
	PersistentRBSTArray erase(int k) const {
		std::pair<Pointer, Pointer> p, q;
		p = split(root, k);
		q = split(p.second, 1);
		return merge(p.first, q.second);
	}
	std::pair<PersistentRBSTArray, PersistentRBSTArray> split(int k) const {//[0,k),[k..)に分割
		pair<Pointer, Pointer> a = split(root, k);
		return make_pair(PersistentRBSTArray(a.first), PersistentRBSTArray(a.second));
	}
	Val access(int n)const {
		return find(root, n)->val;
	}
	int size()const {
		return root.exist() ? root->size : 0;
	}
};




void out(const PersistentRBSTArray<char> &x) {
	rep(i, x.size())putchar(x.access(i));
	puts("");
}


char s[1000001];

int main() {
	PersistentRBSTArray<char> x;

	int m;
	scanf("%d%s", &m, s);
	for (int i = 0; s[i]; i++) x = x.insert(i, s[i]);

	int n;
	scanf("%d", &n);

	rep(i, n) {
		int a, b, c;
		scanf("%d%d%d", &a, &b, &c);

		auto sp = x.split(c);
		x = x.split(b).first.split(a).second;
		x = sp.first.merge(x).merge(sp.second);

		if (x.size() > m) {
			x = x.split(m).first;
		}
	}
	out(x);
	return 0;
}