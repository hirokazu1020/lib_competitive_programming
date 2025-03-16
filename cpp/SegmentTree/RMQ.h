#include<algorithm>
#include<functional>
#include<climits>

template<class S=int, S nil=INT_MAX, class T=std::less<S> >
class RMQ{//RangeMinimum(Maximum)Query
	int n;
	S *dat;
	S query(int a,int b,int k,int l,int r)const{
		if(r<=a||b<=l)return nil;
		if(a<=l&&r<=b)return dat[k];
		return std::min(query(a,b,k*2+1,l,(l+r)/2),query(a,b,k*2+2,(l+r)/2,r),T());
	}
public:
	RMQ(int n=1<<20):n(n){//サイズは2のべき乗にしたほうが速い
		dat=new S[2*n-1];
		for(int i=0;i<2*n-1;i++)dat[i]=nil;
	}
	~RMQ(){
		delete[] dat;
	}
	void update(int k,S a){//k番目をaに変更
		k+=n-1;
		dat[k]=a;
		while(k>0){
			k=(k-1)/2;
			dat[k]=std::min(dat[k*2+1],dat[k*2+2],T());
		}
	}
	S query(int a,int b)const{//[a,b)の最小(大)値
		return query(a,b,0,0,n);
	}
};
