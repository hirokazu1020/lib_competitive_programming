


//verified AGC#002 D

class PartiallyPersistentUnionFind {
	vector<pair<int,int>> par;
	vector<int> rank;
	vector<vector<pair<int, int>>> sz;
	int time;
public:
	PartiallyPersistentUnionFind(int max_n):time(0) ,par(max_n) , rank(max_n, 0) ,sz(max_n) {
		for (int i = 0; i < max_n;i++)par[i] = make_pair(INF+1, i);
		for (auto &v : sz)v.emplace_back(time, 1);
	}
	pair<int,int> find(int x,int t) const{ //<root,size>
		if (par[x].first <= t)return find(par[x].second, t);
		return make_pair(x, (upper_bound(all(sz[x]), make_pair(t+1,-INF))-1)->second);
	}
	void unite(int x, int y) {
		time++;
		x = find(x,time).first;
		y = find(y,time).first;
		if (x == y)return;
		if (rank[x] < rank[y]) {
			par[x] = make_pair(time, y);
			sz[y].emplace_back(time, sz[x].back().second + sz[y].back().second);
		}
		else {
			par[y] = make_pair(time, x);
			if (rank[x] == rank[y])rank[x]++;
			sz[x].emplace_back(time, sz[x].back().second + sz[y].back().second);
		}
	}
};










class PersistentUnionFind {
	PersistentArray<tuple<int,int,int>> dat; //<par,rank,size>

public:
	PersistentUnionFind(int n) :dat(n,make_tuple(-1,0,1)) {} //initial_id:0

	pair<int, int> find(int id, int x) const { //<vertex,size>
		const auto &d = dat.get(id, x);
		if (std::get<0>(d) == -1)return make_pair(x, std::get<2>(d));
		return find(id, std::get<0>(d));
	}
	int unite(int id, int x, int y) {
		x = find(id, x).first;
		y = find(id, y).first;
		if (x == y) return id;
		const auto dx = dat.get(id, x);
		const auto dy = dat.get(id, y);
		if (get<1>(dx) < get<1>(dy)) {
			id = dat.update(id, x, make_tuple(y, -1, -1));
			id = dat.update(id, y, make_tuple(-1, get<1>(dy), get<2>(dx) + get<2>(dy)));
		}
		else {
			id = dat.update(id, y, make_tuple(x, -1, -1));
			if (get<1>(dx) == get<1>(dy)) {
				id = dat.update(id, x, make_tuple(-1, get<1>(dx) + 1, get<2>(dx) + get<2>(dy)));
			}
			else {
				id = dat.update(id, x, make_tuple(-1, get<1>(dx), get<2>(dx) + get<2>(dy)));
			}
		}
		return id;
	}
	bool same(int id, int x, int y) const{
		return find(id, x).first == find(id, y).first;
	}
};














//2^B分木 ポインタベース コピーして使う　遅い
template<class T, int B = 3>
class PersistentArray {
	struct Node{};
	struct Inner :Node { array<Node*, 1 << B > ch; };
	struct Leaf :Node { T val; };
	int h;
	Node * root;
public:
	PersistentArray() :root(0), h(0) {} //要init
	void init(int n, T val = T()) {
		h = 1;
		while ((n - 1) & ‾((1 << h * B) - 1))h++;
		Node *p, *q;
		p = root = new Inner();
		for (int i = 0; i < h - 1; i++) {
			q = new Inner();
			((Inner*)p)->ch.fill(q);
			p = q;
		}
		q = new Leaf();
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
		root = q = new Inner();
		for (int i = h - 1; i >= 1; i--) {
			int b = k >> (i * B) & ((1 << B) - 1);
			*(Inner*)q = *(Inner*)p;
			((Inner*)q)->ch[b] = new Inner();
			q = ((Inner*)q)->ch[b];
			p = ((Inner*)p)->ch[b];
		}
		*(Inner*)q = *(Inner*)p;
		((Inner*)q)->ch[k & ((1 << B) - 1)] = new Leaf();
		q = ((Inner*)q)->ch[k & ((1 << B) - 1)];
		p = ((Inner*)p)->ch[k & ((1 << B) - 1)];
		*(Leaf*)q = *(Leaf*)p;
		return ((Leaf*)q)->val;
	}
};

//コピーして使う　遅い
class PersistentUnionFind {
	PersistentArray<tuple<int, int, int>> dat; //<par,rank,size>

public:
	PersistentUnionFind() {}
	void init(int n) { dat.init(n, make_tuple(-1, 0, 1)); }

	pair<int, int> find(int x) const { //<vertex,size>
		const auto &d = dat.immutable_get(x);
		if (std::get<0>(d) == -1)return make_pair(x, std::get<2>(d));
		return find(std::get<0>(d));
	}
	void unite(int x, int y) {
		x = find(x).first;
		y = find(y).first;
		if (x == y) return;
		auto &dx = dat.mutable_get(x);
		auto &dy = dat.mutable_get(y);
		if (get<1>(dx) < get<1>(dy)) {
			dy = make_tuple(-1, get<1>(dy), get<2>(dx) + get<2>(dy));
			dx = make_tuple(y, -1, -1);
		}
		else {
			dx = make_tuple(-1, get<1>(dx), get<2>(dx) + get<2>(dy));
			if (get<1>(dx) == get<1>(dy)) get<1>(dx)++;
			dy = make_tuple(x, -1, -1);
		}
	}
	bool same(int x, int y) const {
		return find(x).first == find(y).first;
	}
};






