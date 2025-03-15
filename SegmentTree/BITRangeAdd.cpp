#include<cstring>
#include<vector>


class BIT{
	std::vector<long long> bit;
public:
	BIT(int size):bit(size+1,0){}
	void add(int i,long long x){
		i++;
		while(i<(int)bit.size()){
			bit[i]+=x;
			i+= i&-i;
		}
	}
	long long sum(int a)const{
		a++;
		long long res=0;
		while(0<a){
			res+=bit[a];
			a-= a&-a;
		}
		return res;
	}
};

//要verify
class BITRangeAdd{
	BIT bit0,bit1;
public:
	BITRangeAdd(int size):bit0(size+1),bit1(size+1){}
	void add(int a,int b,long long x){//[a,b)にx加算
		bit0.add(a,-x*a);
		bit0.add(b,x*b);
		bit1.add(a,x);
		bit1.add(b,-x);
	}
	long long sum(int a)const{//[0,a]の和
		return bit0.sum(a)+(a+1)*bit1.sum(a);
	}
};