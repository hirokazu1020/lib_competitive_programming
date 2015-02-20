要verify
//ver A
long long pow_mod(long long x,long long y){
	int res=1;
	while(y){
		if(y&1)res=(res*x)%MOD;
		y>>=1;
		x=(x*x)%MOD;
	}
	return res;
}

#include<set>

template <int B=17,long long M=10000000000000061>
class RollingHash{
	std::set<long long> s;
public:
	void add(long long h){s.insert(h);}
	void clear(){s.clear();}
	bool contain(long long h)const{return s.find(h)!=s.end();}
	static long long push_back(long long h,char c){
		return (h*B+c)%M;
	}
	static long long push_front(int x,long long h,char c){
		return (h+c*pow_mod(B,x))%M;
	}
};

//ver B
//ロバスト版2重ハッシュ Mと2^64のmod
#include<utility>
template <int B=17,long long  M=10000000000000061>
class RollingHash{
	std::set<HashV> s;
public:
	typedef std::pair<long,long> HashV;
	void add(HashV h){s.insert(h);}
	void clear(){s.clear();}
	bool contain(HashV h)const{return s.find(h)!=s.end();}
	static HashV push_back(HashV h,char c){
		return std::make_pair((h*B+c)%M,h*B+c);
	}
};

//ver C
#include<utility>
class RollingHash{
	typedef unsigned long long ull;
	static const ull B=17,M=10000000000000061;
	ull b1,b2;
	std::pair<ull,ull> v;
public:
	RollingHash(){clear();}
	bool operator<(const RollingHash &rh)const{return v<rh.v;}
	bool operator==(const RollingHash &rh)const{return v==rh.v;}
	bool operator!=(const RollingHash &rh)const{return v!=rh.v;}	
	void clear(){
		b1=b2=1;
		v=std::make_pair(0,0);
	}
	void push_back(char c){
		(v.first=v.first*B+c)%=M;
		v.second=v.second*B+c;
		(b1*=B)%=M;
		b2*=B;
	}
	void push_front(char c){
		(v.first+=b1*c)%=M;
		v.second+=b2*c;
		(b1*=B)%=M;
		b2*=B;
	}
};
