// 最大独立集合 
// verified https://codeforces.com/contest/1105/problem/E

int _mis_rec(int i, int n, uint64_t vanish, const uint64_t nei[]) {
	if (i == n)return 0;
	if (vanish >> i & 1)return _mis_rec(i + 1, n, vanish, nei);
	uint64_t rem = ‾vanish & nei[i];
	vanish |= 1LL << i;
	if (rem == 0 || (rem&(rem - 1)) == 0)return 1 + _mis_rec(i + 1, n, vanish | nei[i], nei);
	return max(_mis_rec(i + 1, n, vanish, nei)
		, 1 + _mis_rec(i + 1, n, vanish | nei[i], nei));
}
// O(1.466^n)
int maximum_independent_set(int n, const vector<int> edge[]) {
	uint64_t nei[40] = {};
	rep(i, n)for (int x : edge[i])nei[i] |= 1LL << x;
	return _mis_rec(0, n, 0, nei);
}
