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



template<class T, int MAXSIZE = 250000>
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
	GCPointer& operator =(GCPointer &&q) {
		release();
		p = q.p;
		q.p = nullptr;
		return *this;
	}
	bool operator ==(nullptr_t null)const {
		return p == nullptr;
	}
	bool operator !=(nullptr_t null)const {
		return p != nullptr;
	}
	GCPointer& operator =(nullptr_t null) {
		release();
		p = nullptr;
		return *this;
	}
	template<class... Types>
	void construct(Types... values){
		release();
		p = alloc();
		new(&p->first) T(values...);
		p->second++;
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


//verified ARC#030D グラフではない

class PersistentRBSTSegtree {
	struct Node;
	using Pointer = GCPointer<Node>;

	struct Node {
		Pointer left, right;
		int size;
		long long v, sum, lazy;

		Node(int v)
			:left(), right(), size(1), v(v), sum(v), lazy(0) {}
		‾Node() {}
	};


	static Pointer merge(const Pointer &_a, const Pointer &_b) {
		if (_a == nullptr)return _b;
		if (_b == nullptr)return _a;
		if (((long long)_a->size << 32) >(long long)(_a->size + _b->size)*xor128()) {
			Pointer a(push(_a));
			a->size += _b->size;
			a->sum += _b->sum;
			a->right = merge(a->right, _b);
			return std::move(a);
		}
		else {
			Pointer b(push(_b));
			b->size += _a->size;
			b->sum += _a->sum;
			b->left = merge(_a, b->left);
			return std::move(b);
		}
	}
	static pair<Pointer, Pointer> split(const Pointer &_a, int k) {//[0,k),[k..)に分割
		if (_a == nullptr)return make_pair(Pointer(), Pointer());
		Pointer a(push(_a));
		int sz = a->left != nullptr ? a->left->size : 0;
		if (k > sz) {
			if (a->right != nullptr) {
				a->size -= a->right->size;
				a->sum -= a->right->sum;
			}
			auto p = split(a->right, k - 1 - sz);
			a->right = p.first;
			if (a->right != nullptr) {
				a->size += a->right->size;
				a->sum += a->right->sum;
			}
			return make_pair(a, p.second);
		}
		else if (k < sz) {
			a->size -= a->left->size;
			a->sum -= a->left->sum;
			auto p = split(a->left, k);
			a->left = p.second;
			a->size += a->left->size;
			a->sum += a->left->sum;
			return make_pair(p.first, a);
		}
		else {
			if (a->left != nullptr) {
				a->size -= a->left->size;
				a->sum -= a->left->sum;
			}
			Pointer b = a->left;
			a->left = nullptr;
			return make_pair(b, a);
		}
	}

	static Pointer push(const Pointer &_a) {
		if (_a->lazy == 0)return Pointer(*_a);
		Pointer a(*_a);
		if (a->right != nullptr) {
			a->right = Pointer(*a->right);
			a->right->lazy += a->lazy;
			a->right->sum += a->right->size * a->lazy;
			a->right->v += a->lazy;
		}
		if (a->left != nullptr) {
			a->left = Pointer(*a->left);
			a->left->lazy += a->lazy;
			a->left->sum += a->left->size * a->lazy;
			a->left->v += a->lazy;
		}
		a->lazy = 0;
		return std::move(a);
	}




	PersistentRBSTSegtree(Pointer p) :root(p) {}

	Pointer root;
public:
	PersistentRBSTSegtree() {}
	‾PersistentRBSTSegtree() {}
	PersistentRBSTSegtree insert(int k, int v) const {
		auto p = split(root, k);
		return merge(p.first, merge(Pointer(v), p.second));
	}
	PersistentRBSTSegtree merge(const PersistentRBSTSegtree &a) const {//*thisとaを合併
		return merge(root, a.root);
	}
	PersistentRBSTSegtree erase(int k) const {
		pair<Pointer, Pointer> p, q;
		p = split(root, k);
		q = split(p.second, 1);
		return merge(p.first, q.second);
	}
	pair<PersistentRBSTSegtree, PersistentRBSTSegtree> split(int k) const {//[0,k),[k..)に分割
		auto a = split(root, k);
		return make_pair(PersistentRBSTSegtree(a.first), PersistentRBSTSegtree(a.second));
	}
	int size()const {
		return root != nullptr ? root->size : 0;
	}
	PersistentRBSTSegtree add(int a, int b, long long v) const{
		auto y = split(root, b);
		auto x = split(y.first, a);
		x.second->lazy += v;
		x.second->sum += v * (b - a);
		x.second->v += v;
		return merge(merge(x.first, x.second), y.second);
	}

	long long sum(int a, int b) const{
		return split(split(root, b).first, a).second->sum;
	}
};




int main() {
	PersistentRBSTSegtree ar;

	int n, q;
	scanf("%d%d", &n, &q);
	rep(i, n) {
		int x;
		scanf("%d", &x);
		ar = ar.insert(i, x);
	}

	rep(i, q) {
		int op, a, b, c, d, v;
		scanf("%d", &op);
		if (op == 1) {
			scanf("%d%d%d", &a, &b, &v);
			a--;
			ar = ar.add(a, b, v);
		}
		else if (op == 2) {
			scanf("%d%d%d%d", &a, &b, &c, &d);
			a--; c--;
			auto cd = ar.split(d).first.split(c).second;
			auto x = ar.split(b);
			auto y = x.first.split(a);
			ar = y.first.merge(cd).merge(x.second);
		}
		else {
			scanf("%d%d", &a, &b);
			a--;
			printf("%lld¥n", ar.sum(a, b));
		}
	}

	return 0;
}