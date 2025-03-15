#include "DynamicBitVector.h"

#include<iostream>
#include<cstdio>
#include<string>
#include<cstring>
#include<vector>
#include<set>
#include<list>
#include<queue>
#include<cmath>
#include<functional>
#include<algorithm>
#include<utility>
#define INF (1<<29)
#define rep(i,n) for(int i=0;i<(int)(n);i++)
using namespace std;




class DynamicWaveletMatrix {
	typedef int Val;
	static const Val NIL = -1;
	static const int BITLEN = 20;
	int len;
	DynamicBitVector bv[BITLEN];
	int encode(Val c)const {
		return c;
	}
	Val decode(int n)const {
		return n;
	}
	struct Node {
		int height, s, e, code;
		Node() {}
		Node(int a, int b, int c, int d) :height(a), s(b), e(c), code(d) {};
		bool operator <(Node a)const { return e - s<a.e - a.s; }
	};
public:
	int length()const {
		return len;
	}
	DynamicWaveletMatrix() :len(0) {
	}
	DynamicWaveletMatrix(const vector<Val> &str) {
		init(str);
	}
	void init(const vector<Val> &str) {
		len = str.size();
		int *bucket;
		bucket = new int[2 * len];
		int *cur, *next;
		cur = bucket;
		next = bucket + len;
		int rank0[BITLEN] = { 0 };
		for (int i = 0; i<len; i++) {
			int n = encode(str[i]);
			cur[i] = n;
			for (int j = BITLEN - 1; j >= 0; j--) {
				if ((n & 1 << j) == 0)rank0[j]++;

			}
		}
		for (int i = BITLEN - 1;; i--) {
			for (int j = 0; j<len; j++) {
				//if(cur[j]&1<<i)bv[i].insert(j,1);
				bv[i].insert(j, cur[j] >> i & 1);
			}
			if (i == 0)break;
			int idx0 = 0, idx1 = rank0[i];
			for (int j = 0; j<len; j++) {
				if (cur[j] & 1 << i)next[idx1++] = cur[j];
				else next[idx0++] = cur[j];
			}
			swap(cur, next);
		}
		delete[] bucket;
	}
	Val access(int k)const {
		int code = 0;
		for (int i = BITLEN - 1; i>0; i--) {
			if (bv[i].access(k)) {
				code |= 1 << i;
				k = len - bv[i].rank() + bv[i].rank(k) - 1;
			}
			else {
				k = k - bv[i].rank(k);
			}
		}
		if (bv[0].access(k))code |= 1;
		return decode(code);
	}
	int rank(int s, int e, Val c)const {
		int n = encode(c);
		for (int i = BITLEN - 1; i >= 0; i--) {
			int ssum = bv[i].rank(s);
			int esum = bv[i].rank(e);
			if (n & 1 << i) {
				s = len - bv[i].rank() + ssum;
				e = s + esum - ssum;
			}
			else {
				s = s - ssum;
				e = e - esum;
			}
		}
		return e - s;
	}

	void insert(int k, Val c) {
		int n = encode(c);
		for (int i = BITLEN - 1; i >= 0; i--) {
			int sum = bv[i].rank(k);
			int prev = k;
			if (n & 1 << i) {
				k = len - bv[i].rank() + sum;
				bv[i].insert(prev, 1);
			}
			else {
				k = k - sum;
				bv[i].insert(prev, 0);
			}
		}
		len++;
	}
	void erase(int k) {
		for (int i = BITLEN - 1; i >= 0; i--) {
			int prev = k;
			if (bv[i].access(k)) {
				k = len - bv[i].rank() + bv[i].rank(k);
			}
			else {
				k = k - bv[i].rank(k);
			}
			bv[i].erase(prev);
		}
		len--;
	}
};





#include<ctime>
#define KEISOKU

int main() {
	clock_t t = clock();
	DynamicBitVector bv;
	vector<int > v;
	/*for(int i=0;i<30;i++){
	int a=rand()&1;
	bv.insert(i,a);
	v.push_back(a);
	}*/
	//bv.insert(0,0);
	//bv.insert(1,1);
	//bv.erase(361);v.erase(v.begin()+361);
	
	for (int i = 0; i<1000000; i++) {
		int a = (rand() & 1);
		//int p = rand() % (bv.size() + 1);
		//int p = i;
		int p=0;
		bv.insert(p, a);
#ifndef KEISOKU
		v.insert(v.begin() + p, a);
#endif
	}
	for (int i = 0; i<0; i++) {
		int a = rand() % bv.size();
		//int a = 100;
		//cout << i << endl;

		bv.erase(a);
#ifndef KEISOKU
		//bv.check();
		v.erase(v.begin() + a);
#endif
		/*if(rand()&1){
		int a=!(rand()&1);
		bv.insert(0,a);
		v.insert(v.begin(),a);
		}else{
		//int a=rand()%(v.size()+1);
		int a=100;
		bv.erase(a);
		v.erase(v.begin()+a);
		}*/
	}
	printf("%.5f¥n", (clock() - t) / (double)CLOCKS_PER_SEC);
#ifndef KEISOKU
	for (int i = 0; i<bv.size(); i++) {
		//cout<<bv.access(i)<<v[i]<<endl;
		if (bv.access(i) != v[i])
			cout << "diff[]" << i << endl;
	}
	int s = 0;
	for (int i = 0; i<bv.size(); i++) {
		s += v[i];
		if (bv.rank(i + 1) != s)cout << "rank" << i << endl;
	}
#endif
	/*//for(int i=20;i>=0;i-=2)bv.erase(i);cout<<endl;
	for(int i=0;i<bv.size();i++){
	cout<<bv.access(i);
	}cout<<endl;
	for(int i=0;i<20;i++){
	cout<<"rank"<<i<<":"<<bv.rank(i)<<endl;
	}*/

	//int p=32;
	//cout<<(1<<(int)32)<<endl;
	//cout<<(1u<<p)<<endl;
	return 0;
}