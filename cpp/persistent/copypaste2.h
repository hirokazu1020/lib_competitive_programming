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



template<class T>
class GCPointer {
	static pair<T, int> * pool;
	static int index, size, capacity;

	static int alloc() {
		while (1) {
			if (++index == capacity) {
				if (capacity * 3 < size * 4) { //参照GCPointer&は死ぬ
					pool = (pair<T, int> *)realloc(pool, 2 * capacity* sizeof(pair<T, int>));
					memset(pool + capacity, 0, capacity *  sizeof(pair<T, int>));
					capacity *= 2;
				}
				else index = 0;
			}
			if (pool[index].second == 0)break;
		}
		size++;
		return index;
	}

	int offset;

	void release() {
		if (offset != -1 && --pool[offset].second == 0) {
			size--;
			pool[offset].first.‾T();
		}
	}
public:
	GCPointer() :offset(-1) {}
	GCPointer(T v) {
		offset = alloc();
		new(&pool[offset].first) T(v);
		pool[offset].second++;
	}
	GCPointer(const GCPointer &q) {
		offset = q.offset;
		if (offset != -1)pool[offset].second++;
	}
	GCPointer(GCPointer &&q) {
		offset = q.offset;
		q.offset = -1;
	}
	‾GCPointer() {
		release();
	}
	T* operator->() const {
		return &pool[offset].first;
	}
	T& operator*() const {
		return pool[offset].first;
	}
	GCPointer& operator =(const GCPointer &q) {
		if (offset == q.offset)return *this;
		release();
		offset = q.offset;
		if (offset != -1)pool[offset].second++;
		return *this;
	}
	GCPointer& operator =(GCPointer &&q) {
		release();
		offset = q.offset;
		q.offset = -1;
		return *this;
	}
	bool operator ==(nullptr_t null)const {
		return offset == -1;
	}
	bool operator !=(nullptr_t null)const {
		return offset != -1;
	}
	GCPointer& operator =(nullptr_t null) {
		release();
		offset = -1;
		return *this;
	}
};
template<class T>
pair<T, int> * GCPointer<T>::pool = (pair<T, int> *)calloc(1, sizeof(pair<T, int>));
template<class T>
int GCPointer<T>::index = -1;
template<class T>
int GCPointer<T>::size = 0;
template<class T>
int GCPointer<T>::capacity = 1;







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

	static Pointer find(Pointer p, int k) {
		if (p == nullptr)return Pointer();
		int si = (p->left != nullptr ? p->left->size : 0);
		return k == si ?
			p : k<si ? find(p->left, k) : find(p->right, k - si - 1);
	}

	static Pointer merge(Pointer _a, Pointer _b) {
		if (_a == nullptr)return _b;
		if (_b == nullptr)return _a;
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
	static std::pair<Pointer, Pointer> split(Pointer _a, int k) {//[0,k),[k..)に分割
		if (_a == nullptr)return std::make_pair(Pointer(), Pointer());
		Pointer a(*_a);
		int sz = a->left != nullptr ? a->left->size : 0;
		if (k > sz) {
			if (a->right != nullptr)a->size -= a->right->size;
			std::pair<Pointer, Pointer> p = split(a->right, k - 1 - sz);
			a->right = p.first;
			if (a->right != nullptr)a->size += a->right->size;
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
			if (a->left != nullptr)a->size -= a->left->size;
			Pointer b = a->left;
			a->left = nullptr;
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
		return root != nullptr ? root->size : 0;
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