// AC https://atcoder.jp/contests/tenka1-2016-final-open/submissions/34779611
class DoubleArray {
public:
	DoubleArray() {}
	DoubleArray(const std::vector<std::string>& p) { build(p); }
	int headNode() const { return 0; }
	int nextNode(int node_id, char ch) const {
		if (node_id == -1) return -1;
		int nxt = static_cast<int>(base_[node_id] + toint(ch));
		if (node_id == check_[nxt]) return nxt;
		else return -1;
	}
	bool match(int node_id) const { return node_id != -1 && match_[node_id]; }
	bool match(const char* str) const {
		int node_id = headNode();
		const char* p = str;
		while (*p) {
			node_id = nextNode(node_id, *p);
			if (node_id == -1)return false;
			p++;
		}
		return match(node_id);
	}
	size_t maxNodeId() const { return maxNodeId_; }
	size_t nodeSize() const { return nodeSize_; }
	void build(const std::vector<std::string> &p) {
		base_.assign(2 * CSIZE + 1, 0);
		check_.assign(2 * CSIZE + 1, -1);
		match_.assign(2 * CSIZE + 1, false);
		maxNodeId_ = 0;
		nodeSize_ = 0;
		std::vector<uint64_t> used((2 * CSIZE + 1) / 64 + 2, 0);
		std::vector<std::vector<uint8_t>> v(p.size());
		for (int i = 0; i < p.size(); i++) {
			v[i].reserve(p[i].size());
			for (char ch : p[i]) v[i].push_back(toint(ch));
		}
		std::sort(v.begin(), v.end());
		v.erase(std::unique(v.begin(), v.end()), v.end());
		size_t search_id = 0;
		std::queue<std::tuple<size_t, size_t, size_t, size_t>> q;
		q.emplace(0, 0, 0, v.size());
		while (!q.empty()) {
			size_t node_id, i, s, e;
			tie(node_id, i, s, e) = q.front();
			q.pop();
			nodeSize_++;
			maxNodeId_ = std::max(maxNodeId_, node_id);
			if (i == v[s].size()) {
				s++;
				match_[node_id] = true;
			}
			if (s == e) continue;
			uint64_t mask = 0;
			for (size_t j = s; j < e; j++) mask |= 1ULL << v[j][i];
			while (true) {
				if ((used[search_id / 64] & (mask << (search_id % 64))) == 0 && 
					(search_id % 64 == 0 || (used[search_id / 64 + 1] & (mask >> (64 - search_id % 64))) == 0))
					break;
				search_id++;
			}
			resize(search_id + CSIZE + 1, used);
			base_[node_id] = search_id;
			for (size_t j = s; j < e;) {
				size_t k = j;
				while (k < e && v[j][i] == v[k][i]) k++;
				size_t nxt = base_[node_id] + v[j][i];
				check_[nxt] = static_cast<int>(node_id);
				used[nxt / 64] |= 1ULL << nxt % 64;
				q.emplace(nxt, i + 1, j, k);
				j = k;
			}
		}
	}
private:
	static constexpr uint32_t CSIZE = 26;
	static uint32_t toint(char ch) {
		return 1 + ch - 'a';
	}
	void resize(size_t require, std::vector<uint64_t> &used) {
		if (base_.size() < require) {
			base_.resize(2 * base_.size(), 0);
			check_.resize(2 * check_.size(), -1);
			match_.resize(2 * match_.size(), false);
			used.resize(2 * used.size(), 0);
		}
	}
	std::vector<size_t> base_;
	std::vector<int> check_;
	std::vector<bool> match_;
	size_t nodeSize_, maxNodeId_;
};





int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);

	//char aa = -2;
	//cout << (int)(unsigned char)aa << endl;
	//vector<int> v(300);
	//cout << v[aa] << endl;

	DoubleArray da;
	da.build(vector<string>({ "abc", "abcde", "xxxxxx" }));

	int x = da.headNode();
	x = da.nextNode(x, 'a');
	cout << da.match(x) << endl;
	x = da.nextNode(x, 'b');
	cout << da.match(x) << endl;
	x = da.nextNode(x, 'c');
	cout << da.match(x) << endl;
	x = da.nextNode(x, 'd');
	cout << da.match(x) << endl;
	x = da.nextNode(x, 'e');
	cout << da.match(x) << endl;

	//cout << da.str(x) << endl;
	cout << da.match("abc") << endl;


	return 0;
}
