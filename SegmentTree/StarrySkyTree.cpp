//verified KOJ0093
#include <iostream>
#include<vector>
#include<algorithm>
#include<climits>
using namespace std;



struct StarrySkyTree{
	int n;
	std::vector<long long> segv,segAdd;
	void add(int a,int b,int x,int k,int l,int r){
		if(r<=a || b<=l)return;
		if(a<=l && r<=b){
			segAdd[k] += x;
			return;
		}
		int m=(l+r)/2;
		add(a,b,x,2*k+1,l,m);
		add(a,b,x,2*k+2,m,r);
		segv[k]=std::min(segAdd[2*k+1]+segv[2*k+1] ,segAdd[2*k+2]+segv[2*k+2]);
	}
	long long min(int a,int b,int k,int l,int r)const{
		if(r<=a || b<=l)return LLONG_MAX;
		if(a<=l && r<=b){
			return segv[k]+segAdd[k];
		}
		int m=(l+r)/2;
		return segAdd[k]+std::min(min(a,b,2*k+1,l,m),min(a,b,2*k+2,m,r));
	}
public:
	StarrySkyTree(int n=1<<18){
		init(n);
	}
	void init(int n){
		this->n=n;
		segv.assign(2*n-1,0);
		segAdd.assign(2*n-1,0);
	}
	void add(int a,int b,int x){//[a,b)‚Éx‚ğ‰ÁZ
		add(a,b,x,0,0,n);
	}
	long long min(int a,int b)const{//[a,b)‚ÌÅ¬’l
		return min(a,b,0,0,n);
	}
};

