//verifired hackerrank Cipher
#include<vector>
#include<algorithm>

struct RangeAddTree{//”ÍˆÍ‰ÁZA”ÍˆÍ˜a
	int n;
	std::vector<long long> sub,uni;
	void add(int a,int b,int x,int k,int l,int r){
		if(r<=a || b<=l)return;
		if(a<=l && r<=b){
			uni[k] += x;
			return;
		}
		sub[k] += x*(std::min(b,r)-std::max(a,l));
		int m=(l+r)/2;
		add(a,b,x,2*k+1,l,m);
		add(a,b,x,2*k+2,m,r);
	}
	long long get(int a,int b,int k,int l,int r)const{
		if(r<=a || b<=l)return 0;
		if(a<=l && r<=b){
			return uni[k]*(r-l)+sub[k];
		}
		long long res = uni[k]*(std::min(b,r)-std::max(a,l));
		int m=(l+r)/2;
		res += get(a,b,2*k+1,l,m);
		res += get(a,b,2*k+2,m,r);
		return res;
	}
public:
	RangeAddTree(int n=1<<21){
		init(n);
	}
	void init(int n){
		this->n=n;
		sub.assign(2*n-1,0);
		uni.assign(2*n-1,0);
	}
	void add(int a,int b,int x){//[a,b)‚Éx‚ğ‰ÁZ
		add(a,b,x,0,0,n);
	}
	long long get(int a,int b)const{//[a,b)‚Ì˜a
		return get(a,b,0,0,n);
	}
};

