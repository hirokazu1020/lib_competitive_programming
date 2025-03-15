//verify Codeforces 356A
#include<vector>

class RangeSubstituteTree{//範囲代入
	int n,t;
	std::vector<int> ts,val;
	void Substitute(int a,int b,int x,int k,int l,int r){
		if(r<=a || b<=l)return;
		if(a<=l && r<=b){
			ts[k] = t;
			val[k] = x;
			return;
		}
		int m=(l+r)/2;
		Substitute(a,b,x,2*k+1,l,m);
		Substitute(a,b,x,2*k+2,m,r);
	}
public:
	RangeSubstituteTree(int n=1<<20):n(n){
		ts.assign(2*n-1,t=0);
		val.assign(2*n-1,0);
	}
	void Substitute(int a,int b,int x){//[a,b)にxを代入
		t++;
		Substitute(a,b,x,0,0,n);
	}
	int get(int k)const{//k番目の値
		int res,s;
		k+=n-1;
		res=val[k];
		s=ts[k];
		while(k>0){
			k=(k-1)/2;
			if(s<ts[k]){
				s=ts[k];
				res=val[k];
			}
		}
		return res;
	}
};
