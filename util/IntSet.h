class IntSet {
public:
	IntSet(int min, int max) : v(max - min + 1), base(-min) {}
	void insert(int x) {
		++v[x + base];
	}
	void erase_vals(int x) {
		v[x + base] = 0;
	}
	void add_all(int x) {
		base -= x;
	}
	int count(int x) const {
		return v[x + base];
	}
private:
	int base;
	vector<int> v;
};



class IntSet {
public:
	void insert(int x) {
		++v[x + base];
	}
	void erase_vals(int x) {
		v.erase(x);
	}
	void add_all(int x) {
		base -= x;
	}
	int count(int x) const {
		auto it = v.find(x + base);
		return it == v.end() ? 0 : it->second;
	}
private:
	int base = 0;
	map<int, int> v;
};

