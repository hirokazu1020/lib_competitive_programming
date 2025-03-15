class ConvexHullTrick {
	using Line = std::pair<int64_t, int64_t>;
	std::vector<Line> lines;
	bool f(const Line &a, const Line &b, const Line &c) const {
		return (int64_t)(a.first - b.first) * (b.second - c.second) < (int64_t)(c.first - b.first)*(b.second - a.second);
	}
	static int64_t eval(const Line &line, int64_t x) {
		return line.first * x + line.second;
	}
public:
	void add(int64_t a, int64_t b) { // strictly increase, strictly decrease
		Line line(a, b);
		while (2 <= lines.size() && !f(lines[lines.size() - 2], lines[lines.size() - 1], line)) {
			lines.pop_back();
		}
		lines.push_back(line);
	}
	int64_t min(int64_t x) const {
		int lb = 0, ub = lines.size();
		while (ub - lb > 1) {
			int mid = (lb + ub) / 2;
			if (eval(lines[mid - 1], x) > eval(lines[mid], x)) {
				lb = mid;
			}
			else {
				ub = mid;
			}
		}
		return eval(lines[lb], x);
	}
};
