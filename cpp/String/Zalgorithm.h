//verified Cf246div2D PROBLEMSET432D

#include<string>
#include<vector>
using namespace std;


vector<int> Zalgorithm(const string &s){
	vector<int> a(s.size());
	a[0] = s.size();
	int i = 1, j = 0;
	while (i < (int)s.size()) {
		while (i+j < (int)s.size() && s[j] == s[i+j]) ++j;
		a[i] = j;
		if (j == 0) { ++i; continue;}
		int k = 1;
		while (i+k < (int)s.size() && k+a[k] < j) a[i+k] = a[k], ++k;
		i += k; j -= k;
	}
	return a;
}