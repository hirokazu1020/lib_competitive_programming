#include<iostream>
#include<utility>
#include<algorithm>
#include<cassert>

using namespace std;

//空間O(謎)
//Uが大きいと遅くなる U=20程度ならsetより速い
//重複挿入重複削除の挙動は未知
template<int U=30> 
class vanEmdeBoasTree{
	friend class vanEmdeBoasTree<U*2>;
	friend class vanEmdeBoasTree<U*2+1>;
	int min,max;
	vanEmdeBoasTree<(U+1)/2> summary;
	vanEmdeBoasTree<U/2> **cluster;

	static int low(int x){return x&((1u<<U/2)-1);}
	static int high(int x){return x>>U/2;}
	static int index(int x,int y){return x<<U/2|y;}
	void empty_insert(int x){min=max=x;}
public:
	vanEmdeBoasTree():max(-1),min(-1){
		cluster=new vanEmdeBoasTree<U/2>*[1<<(U+1)/2];
		for(int i=0;i<(1<<(U+1)/2);i++){
			cluster[i]=NULL;
		}
	}
	~vanEmdeBoasTree(){
		clear();
		delete[] cluster;
	}
	int maximum()const{
		if(!this)return -1;
		return max;
	}
	int minimum()const{
		if(!this)return -1;
		return min;
	}
	bool member(int x)const{
		if(x==min||x==max)return true;
		return cluster[high(x)] && cluster[high(x)]->member(low(x));
	}
	void insert(int x){
		assert(x<(1u<<U) );
		if(min==-1){
			min=max=x;
		}else{
			if(x<min){
				std::swap(x,min);
			}
			if(!cluster[high(x)]){
				summary.insert(high(x));
				cluster[high(x)]=new vanEmdeBoasTree<U/2>();
				cluster[high(x)]->empty_insert(low(x));
			}else{
				cluster[high(x)]->insert(low(x));
			}
			if(x>max){
				max=x;
			}
		}
	}
	int successor(int x)const{
		if(min!=-1&&x<min)return min;
		int max_low=cluster[high(x)]->maximum();
		if(max_low!=-1&&low(x)<max_low){
			int offset=cluster[high(x)]->successor(low(x));
			return index(high(x),offset);
		}else{
			int succ_cluster=summary.successor(high(x));
			if(succ_cluster==-1){
				return -1;
			}else{
				int offset=cluster[succ_cluster]->minimum();
				return index(succ_cluster,offset);
			}
		}
	}
	int predecessor(int x)const{
		if(max!=-1&&x>max)return max;
		int min_low=cluster[high(x)]->minimum();
		if(min_low!=-1&&low(x)>min_low){
			int offset=cluster[high(x)]->predecessor(low(x));
			return index(high(x),offset);
		}else{
			int pred_cluster=summary.predecessor(high(x));
			if(pred_cluster==-1){
				if(min!=-1&&x>min)return min;
				return -1;
			}else{
				int offset=cluster[pred_cluster]->maximum();
				return index(pred_cluster,offset);
			}
		}
	}
	void erase(int x){
		if(min==max){
			min=max=-1;
		}else{
			if(x==min){
				int first_cluster=summary.minimum();
				x=index(first_cluster,cluster[first_cluster]->minimum());
				min=x;
			}
			cluster[high(x)]->erase(low(x));
			if(cluster[high(x)]->minimum()==-1){
				delete cluster[high(x)];
				cluster[high(x)]=NULL;
				summary.erase(high(x));
				if(x==max){
					int summary_max=summary.maximum();
					if(summary_max==-1){
						max=min;
					}else{
						max=index(summary_max,cluster[summary_max]->maximum());
					}
				}
			}else if(x==max){
				max=index(high(x),cluster[high(x)]->maximum());
			}
		}
	}
	void clear(){
		if(min==-1)return;
		min=max=-1;
		summary.clear();
		for(int i=0;i<(1<<(U+1)/2);i++){
			delete cluster[i];
			cluster[i]=NULL;
		}
	}
};
class vanEmdeBoasLeaf{
	template<int> friend class vanEmdeBoasTree;
	unsigned int s;
	vanEmdeBoasLeaf():s(0){}
	static int highest(unsigned int x){}
	static int lowest(int x){}
	void empty_insert(int x){s=1u<<x;}
	int maximum()const{
		if(!this)return -1;
		return highest(s);
	}
	int minimum()const{
		if(!this)return -1;
		return lowest(s);
	}
	bool member(int x)const{
		return s>>x&1;
	}
	void insert(int x){
		s|=1u<<x;
	}
	int successor(int x)const{
		return lowest(s&~((1u<<x+1)-1));
	}
	int predecessor(int x)const{
		return highest(s&((1u<<x)-1));
	}
	void erase(int x){
		s&=~(1u<<x);
	}
	void clear(){s=0;}
};

#ifdef __GNUC__
template<>
class vanEmdeBoasTree<6>:public vanEmdeBoasLeaf{
	static int highest(unsigned int x){
		if(!x)return -1;
		return 64-1-__builtin_clzll(x);
	}
	static int lowest(int x){
		return __builtin_ffsll(x)-1;
	}
};
#endif
template<>
class vanEmdeBoasTree<5>:public vanEmdeBoasLeaf{
	unsigned int s;
	static int highest(unsigned int x){
		if(!x)return -1;
		int pos = 0;
		if(x&0xffff0000){
			x >>= 16;
			pos |= 16;
		}
		if(x&0x0000ff00){
			x >>= 8;
			pos |= 8;
		}
		if(x&0x000000f0){
			x >>= 4;
			pos |= 4;
		}
		if(x&0x0000000c){
			x >>= 2;
			pos |= 2;
		}
		if(x&0x00000002){
			pos |= 1;
		}
		return pos;
	}
	static int lowest(int x){
		if(!x)return -1;
		int pos = 0;
		if(!(x&0x0000ffff)){
			x >>= 16;
			pos |= 16;
		}
		if(!(x&0x000000ff)){
			x >>= 8;
			pos |= 8;
		}
		if(!(x&0x0000000f)){
			x >>= 4;
			pos |= 4;
		}
		if(!(x&0x00000003)){
			x >>= 2;
			pos |= 2;
		}
		if(!(x&0x00000001)){
			pos |= 1;
		}
		return pos;
	}
};

template<>
class vanEmdeBoasTree<4>:public vanEmdeBoasTree<5>{};
template<>
class vanEmdeBoasTree<3>:public vanEmdeBoasTree<4>{};
template<>
class vanEmdeBoasTree<2>:public vanEmdeBoasTree<3>{};
template<>
class vanEmdeBoasTree<1>:public vanEmdeBoasTree<2>{};
