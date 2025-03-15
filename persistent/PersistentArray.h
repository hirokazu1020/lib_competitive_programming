

template<class T>
class PartiallyPersistentArray {
	vector<vector<T>> dat;
	vector<vector<int>> ts;
	int time;
public:
	PartiallyPersistentArray(int n)
		:dat(n, vector<T>({ T() })), ts(n, vector<int>({ 0 })), time(0) {}
	const T& get(int t, int i) const{
		return dat[upper_bound(all(ts[i]), t) - ts[i].begin() - 1];
	}
	int update(int i,const T& v) {
		dat[i].emplace_back(v);
		ts[i].emplace_back(++time);
		return time;
	}
};





//verified cf368div2D   http://agc002.contest.atcoder.jp/tasks/agc002_d
//vector<bool> RE注意

template<class T>
class PersistentArray {
	vector<T> dat;
	vector<pair<int,int>> node;
	int h;
public:
	PersistentArray(int n) { //initial_id:0
		if (n == 1) n = 2;
		while (n&(n - 1))n += n&-n;
		dat.reserve(n);
		for (int i = 0; i < n; i++)dat.emplace_back();
		for (int i = 0; i < n / 2 - 1; i++) {
			node.emplace_back(i * 2 + 1, i * 2 + 2);
		}
		for (int i = n / 2 - 1; i < n - 1; i++) {
			int j = i - (n / 2 - 1);
			node.emplace_back(j * 2, j * 2 + 1);
		}
		h = 0;
		while (!(n >> h & 1))h++;
	}
	const T& get(int id, int i) const {
		for (int k = 0; k < h; k++) {
			if (i >> (h - 1 - k) & 1) id = node[id].second;
			else id = node[id].first;
		}
		return dat[id];
	}
	int update(int id, int i, const T& v) {
		int root = node.size();
		for (int k = 0; k < h; k++) {
			node.push_back(node[id]);
			if (i >> (h - 1 - k) & 1) {
				node.back().second = node.size();
				id = node[id].second;
			}
			else {
				node.back().first = node.size();
				id = node[id].first;
			}
		}
		if (i & 1)node.back().second = dat.size();
		else node.back().first = dat.size();
		dat.push_back(v);
		return root;
	}
};




//2^B分木
template<class T,int B = 3>
class PersistentArray {
	vector<T> dat;
	vector<array<int, 1 << B>> node;
	int h;

	void build(int d, int bits,int n) {
		int m = node.size();
		node.emplace_back();
		for (int i = 0; i < (1<<B); i++) {
			int b = bits | i << (d * B);
			if (n <= b)break;
			if (d == 0) {
				node[m][i] = b;
			}
			else {
				node[m][i] = node.size();
				build(d - 1, b, n);
			}
		}
	}
public:
	PersistentArray(int n, T val = T()) { //initial_id:0
		h = 1;
		while (n & ‾((1 << h * B) - 1))h++;
		for (int i = 0; i < h; i++) {
			node.emplace_back();
			node.back().fill(i + 1);
		}
		node.back().fill(0);
		dat.push_back(val);
	}
	PersistentArray(const vector<T> &v) { //initial_id:0
		int n = v.size();
		h = 1;
		while (n & ‾((1 << h * B) - 1))h++;
		build(h - 1, 0, n);
		for (int i = 0; i < n; i++)dat.push_back(v[i]);
	}
	const T& get(int id, int i) const {
		for (int k = h - 1; k >= 0; k--) {
			id = node[id][i >> (k * B) & ((1 << B) - 1)];
		}
		return dat[id];
	}
	int update(int id, int i, const T& v) {
		int root = node.size();
		for (int k = h - 1; k >= 0; k--) {
			int b = i >> (k * B) & ((1 << B) - 1);
			node.push_back(node[id]);
			node.back()[b] = node.size();
			id = node[id][b];
		}
		node.back()[i & ((1 << B) - 1)] = dat.size();
		dat.push_back(v);
		return root;
	}
};





//使い勝手 i番目の操作後のIDはi+1
template<class T,int B = 3>
class PersistentArray {
	vector<T> dat;
	vector<array<int, 1 << B>> node;
	int h;
public:
	PersistentArray(int n, T val = T()) { //initial_id:0
		h = 1;
		while (n & ‾((1 << h * B) - 1))h++;
		for (int i = 0; i < h; i++) {
			node.emplace_back();
			node.back().fill(i + 1);
		}
		node.back().fill(0);
		dat.push_back(val);
	}
	const T& get(int id, int i) const {
		id *= h;
		for (int k = h - 1; k >= 0; k--) {
			id = node[id][i >> (k * B) & ((1 << B) - 1)];
		}
		return dat[id];
	}
	int update(int id, int i, const T& v) {
		id *= h;
		for (int k = h - 1; k >= 0; k--) {
			int b = i >> (k * B) & ((1 << B) - 1);
			node.push_back(node[id]);
			node.back()[b] = node.size();
			id = node[id][b];
		}
		node.back()[i & ((1 << B) - 1)] = dat.size();
		dat.push_back(v);
		return dat.size() - 1;
	}
};







//2^B分木 ポインタベース コピーして使う
//10^5*log(10^5)回の更新があるならメモリプールの使用を検討する

char* memory_pool = (char*)malloc(hogehoge); //メモリリークなんか知らん

template<class T, int B = 3>
class PersistentArray {
	struct Node {};
	struct Inner :Node { array<Node*, 1 << B > ch; };
	struct Leaf :Node { T val; };
	int h;
	Node * root;

	template<class C>
	static C* alloc() { //new は遅い
		return new C();
		/*memory_pool += sizeof(C);
		return new(memory_pool - sizeof(C)) C;*/
	}
public:
	PersistentArray() :root(0), h(0) {} //要init
	void init(int n, T val = T()) {
		h = 1;
		while ((n - 1) & ‾((1 << h * B) - 1))h++;
		Node *p, *q;
		p = root = alloc<Inner>();
		for (int i = 0; i < h - 1; i++) {
			q = alloc<Inner>();
			((Inner*)p)->ch.fill(q);
			p = q;
		}
		q = alloc<Leaf>();
		((Inner*)p)->ch.fill(q);
		((Leaf*)q)->val = val;
	}
	const T& immutable_get(int k) const {
		Node *p = root;
		for (int i = h - 1; i >= 0; i--) {
			p = ((Inner*)p)->ch[k >> (i * B) & ((1 << B) - 1)];
		}
		return ((Leaf*)p)->val;
	}
	T& mutable_get(int k) {
		Node *p = root, *q;
		root = q = alloc<Inner>();
		for (int i = h - 1; i >= 1; i--) {
			int b = k >> (i * B) & ((1 << B) - 1);
			*(Inner*)q = *(Inner*)p;
			((Inner*)q)->ch[b] = alloc<Inner>();
			q = ((Inner*)q)->ch[b];
			p = ((Inner*)p)->ch[b];
		}
		*(Inner*)q = *(Inner*)p;
		((Inner*)q)->ch[k & ((1 << B) - 1)] = alloc<Leaf>();
		q = ((Inner*)q)->ch[k & ((1 << B) - 1)];
		p = ((Inner*)p)->ch[k & ((1 << B) - 1)];
		*(Leaf*)q = *(Leaf*)p;
		return ((Leaf*)q)->val;
	}
};


