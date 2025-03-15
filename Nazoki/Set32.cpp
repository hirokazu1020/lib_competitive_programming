#include<iostream>
#include<utility>
#include<algorithm>

using namespace std;

	
int highest_pop(unsigned int b){
#ifdef __GNUC__
	return 31-__builtin_clz(b);
#else 
	int res=0;
	for(int i=4;i>=0;i--){
		int shift=1<<i;
		if(b>>shift){
			b>>=shift;
			res|=shift;
		}
	}
	return res;
#endif
}

//驚きの32分木 速くてメモリも少なくて実装も短くて素敵
template<int U=30> 
class Set{
	unsigned int summary;
	Set<U-5> *ch[1<<5];
	static int low_bits(unsigned int a){
		return a&((1<<U-5)-1);
	}
	static int high_bits(unsigned int a){
		return a>>(U-5);
	}
	static int index(unsigned int high,unsigned int low){
		return high<<U-5|low;
	}
public:
	Set():summary(0){
		for(int i=0;i<(1<<5);i++)ch[i]=NULL;
	}
	~Set(){clear();}
	void clear(){
		summary=0;
		for(int i=0;i<(1<<5);i++){
			if(ch[i])delete ch[i];
			ch[i]=NULL;
		}
	}
	void insert(int v){
		int high=high_bits(v);
		summary|=1<<high;
		if(!ch[high])ch[high]=new Set<U-5>();
		ch[high]->insert(low_bits(v));
	}
	bool erase(int v){
		int high=high_bits(v);
		if(!ch[high])return true;
		if(!ch[high]->erase(low_bits(v))){
			delete ch[high];
			ch[high]=NULL;			
			return summary&=~(1<<high);
		}
		return true;
	}
	int nextvalue(int v)const{
		int high=high_bits(v);
		int low=low_bits(v);
		int a;
		if(ch[high] && (a=ch[high]->nextvalue(low))!=-1){
			return index(high,a);
		}else{
			high=summary&~((1<<high)-1)&~(1<<high);
			if(!high)return -1;
			high=highest_pop(high&-high);
			return index(high,ch[high]->min());
		}
	}
	int prevvalue(int v)const{
		int high=high_bits(v);
		int low=low_bits(v);
		int a;
		if(ch[high] && (a=ch[high]->prevvalue(low))!=-1){
			return index(high,a);
		}else{
			high=summary&((1<<high)-1);
			if(!high)return -1;
			high=highest_pop(high);
			return index(high,ch[high]->max());
		}
	}
	int min()const{
		if(!summary)return -1;
		int high=highest_pop(summary&-summary);
		return index(high,ch[high]->min());
	}
	int max()const{
		if(!summary)return -1;
		int high=highest_pop(summary);
		return index(high,ch[high]->max());
	}
	bool member(int v)const{
		int high=high_bits(v);
		return ch[high] && ch[high]->member(low_bits(v));
	}
};


template<> 
class Set<5>{
	unsigned int summary;
public:
	Set():summary(0){}
	void clear(){summary=0;}
	void insert(int v){
		summary|=1<<v;
	}
	bool erase(int v){
		return summary&=~(1<<v);
	}
	int nextvalue(int v)const{
		int a=summary&~((1<<v)-1)&~(1<<v);
		if(!a)return -1;
		return highest_pop(a&-a);
	}
	int prevvalue(int v)const{
		int a=summary&((1<<v)-1);
		if(!a)return -1;
		return highest_pop(a);
	}
	int min()const{
		if(!summary)return -1;
		return highest_pop(summary&-summary);
	}
	int max()const{
		if(!summary)return -1;
		return highest_pop(summary);
	}
	bool member(int v)const{
		return summary>>v&1;
	}
};






