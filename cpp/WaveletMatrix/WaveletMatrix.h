#include "BitVector.h"

#include<iostream>
#include<vector>
#include<string>
#include<algorithm>
#include<functional>
#include<queue>
#include<utility>
#include<cstdlib>
#include<ctime>



//ウェーブレット行列
class WaveletMatrix{
	typedef int Val;
	static const Val NIL=-1;
	static const int BITLEN = 30;
	int len;
	BitVector bv[BITLEN];
	int encode(Val c)const{
		return c;
	}
	Val decode(int n)const{
		return n;
	}
	struct Node{
		int height,s,e,code;
		Node(){}
		Node(int a,int b,int c,int d):height(a),s(b),e(c),code(d){};
		bool operator <(Node a)const{return e-s<a.e-a.s;}
	};
public:
	int length()const{
		return len;
	}
	WaveletMatrix(){
	}
	template<class T>
	WaveletMatrix(const T &v){
		init(v);
	}
	//O(n logσ)
	template<class T>
	void init(const T &v){
		len = v.size();
		for(int i=0;i<BITLEN;i++){
			bv[i].init(v.size());
		}
		int *bucket;
		bucket = new int[2*len];
		int *cur,*next;
		cur = bucket;
		next = bucket+len;
		int rank0[BITLEN]={0};
		for(int i=0;i<len;i++){
			int n = encode(v[i]);
			cur[i] = n;
			for(int j=BITLEN-1;j>=0;j--){
				if((n&1<<j)==0)rank0[j]++;
			}
		}
		for(int i=BITLEN-1;;i--){
			for(int j=0;j<len;j++){
				if(cur[j]&1<<i)bv[i].set(j);
			}
			bv[i].build();
			if(i==0)break;
			int idx0=0,idx1=rank0[i];
			for(int j=0;j<len;j++){
				if(cur[j]&1<<i)next[idx1++]=cur[j];
				else next[idx0++]=cur[j];
			}
			std::swap(cur,next);
		}
		delete[] bucket;
	}
	//O(logσ)
	Val access(int k)const{
		int code=0;
		for(int i=BITLEN-1;i>0;i--){
			if(bv[i].access(k)){
				code |= 1<<i;
				k = len-bv[i].rank(len)+bv[i].rank(k);
			}else{
				k = k-bv[i].rank(k);
			}
		}
		if(bv[0].access(k))code |= 1;
		return decode(code);
	}
	//[s,e)中のcの個数
	//O(logσ)
	int rank(int s,int e,Val c)const{
		int n = encode(c);
		for(int i=BITLEN-1;i>=0;i--){
			int ssum = bv[i].rank(s);
			int esum = bv[i].rank(e);
			if(n&1<<i){
				s = len-bv[i].rank(len) + ssum;
				e = s + esum-ssum;
			}else{
				s = s-ssum;
				e = e-esum;
			}
		}
		return e-s;
	}
	//k+1番目のcの位置
	//O(log n logσ)
	int select(int k, Val c)const{
		int n = encode(c);
		int s=0,e=len;
		for(int i=BITLEN-1;i>=0;i--){
			int ssum = bv[i].rank(s);
			int esum = bv[i].rank(e);
			if(n&1<<i){
				s = len-bv[i].rank(len) + ssum;
				e = s + esum-ssum;
			}else{
				s = s-ssum;
				e = e-esum;
			}
		}
		int p = s+k;
		if(e<=p)return -1;
		for(int i=0;i<BITLEN;i++){
			if(n&1<<i)p = bv[i].select1(p-(len-bv[i].rank(len))+1);
			else p = bv[i].select0(p+1);
		}
		return p;
	}
	//[s,e)中のr+1番目に大きい文字
	//O(logσ)
	Val quantile(int s,int e,int r)const{
		int code=0;
		for(int i=BITLEN-1;i>=0;i--){
			int ssum = bv[i].rank(s);
			int esum = bv[i].rank(e);
			int num1 = esum-ssum;
			if(num1<=r){
				r -= num1;
				s = s-ssum;
				e = e-esum;
			}else{
				code |= 1<<i;
				s = len-bv[i].rank(len) + ssum;
				e = s + num1;
			}
			if(s==e)return NIL;
		}
		return decode(code);
	}
	//[s,e)中のx未満の文字の個数
	int rank_lt(int s,int e,Val x)const{
		int n = encode(x);
		int res=0;
		for(int i=BITLEN-1;i>=0;i--){
			int ssum = bv[i].rank(s);
			int esum = bv[i].rank(e);
			if(n&1<<i){
				res += e-s-(esum-ssum);
				s = len-bv[i].rank(len) + ssum;
				e = s + esum-ssum;
			}else{
				s = s-ssum;
				e = e-esum;
			}
			if(s==e)return res;
		}
		return res;
	}
	//[s,e)中の x<=c<y となる文字の個数
	//O(logσ)
	int rangefreq(int s,int e,Val x,Val y)const{
		return rank_lt(s,e,y)-rank_lt(s,e,x);
	}
	//[s,e)過半数の値
	Val moreThanHalf(int s,int e)const{
		int code=0;
		int h=(e-s)/2;
		for(int i=BITLEN-1;i>=0;i--){
			int ssum = bv[i].rank(s);
			int esum = bv[i].rank(e);
			int num1 = esum-ssum;
			int num0 = e-s-num1;
			int s = s-ssum;
			if(h<num1){
				s = len-bv[i].rank(len) + ssum;
				e = s + num1;
				code |= 1<<i;
			}else if(h<num0){
				s = s-ssum;
				e = e-esum;
			}else{
				return NIL;
			}
		}
		return decode(code);
	}
};


struct WMfor2DGird{
	WaveletMatrix grid;
	std::vector<int> firsts;
	WMfor2DGird(std::vector<std::pair<int,int> > vp){
		sort(vp.begin(),vp.end());
		firsts.resize(vp.size());
		std::vector<int> seconds(vp.size());
		for(size_t i=0;i<vp.size();i++){
			firsts[i]=vp[i].first;
			seconds[i]=vp[i].second;
		}
		grid.init(seconds);
	}
	int count(int s,int e,int u,int v)const{//[s,e)*[u,v)内の点の個数
		int a,b;
		a=lower_bound(firsts.begin(),firsts.end(),s)-firsts.begin();
		b=lower_bound(firsts.begin(),firsts.end(),e)-firsts.begin();
		return grid.rangefreq(a,b,u,v);
	}
	int count(int s,int e,int v)const{//[s,e)*[0,v)内の点の個数
		int a,b;
		a=lower_bound(firsts.begin(),firsts.end(),s)-firsts.begin();
		b=lower_bound(firsts.begin(),firsts.end(),e)-firsts.begin();
		return grid.rank_lt(a,b,v);
	}
};
