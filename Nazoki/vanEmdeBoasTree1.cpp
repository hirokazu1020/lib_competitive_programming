#include<iostream>
#include<utility>
#include<algorithm>

using namespace std;



//‹óŠÔO(2^U)
//\’z’x‚¢
//d•¡‘}“üd•¡íœ‚Ì‹““®‚Í–¢’m
template<int U=17> 
class vanEmdeBoasTree{
	friend class vanEmdeBoasTree<U*2>;
	friend class vanEmdeBoasTree<U*2+1>;
	int min,max;
	vanEmdeBoasTree<(U+1)/2> summary;
	vanEmdeBoasTree<U/2> *cluster;

	static int low(int x){return x&((1u<<U/2)-1);}
	static int high(int x){return x>>U/2;}
	static int index(int x,int y){return x<<U/2|y;}
public:
	vanEmdeBoasTree():max(-1),min(-1){
		cluster=new vanEmdeBoasTree<U/2>[1<<(U+1)/2];
	}
	~vanEmdeBoasTree(){
		delete[] cluster;
	}
	int maximum()const{
		return max;
	}
	int minimum()const{
		return min;
	}
	bool member(int x)const{
		if(x==min||x==max)return true;
		return cluster[high(x)].member(low(x));
	}
	void insert(int x){
		if(min==-1){
			min=max=x;
		}else{
			if(x<min){
				std::swap(x,min);
			}
			if(cluster[high(x)].max==-1){
				summary.insert(high(x));
				cluster[high(x)].max=cluster[high(x)].min=low(x);
			}else{
				cluster[high(x)].insert(low(x));
			}
			if(x>max){
				max=x;
			}
		}
	}
	int successor(int x)const{
		if(min!=-1&&x<min)return min;
		int max_low=cluster[high(x)].maximum();
		if(max_low!=-1&&low(x)<max_low){
			int offset=cluster[high(x)].successor(low(x));
			return index(high(x),offset);
		}else{
			int succ_cluster=summary.successor(high(x));
			if(succ_cluster==-1){
				return -1;
			}else{
				int offset=cluster[succ_cluster].minimum();
				return index(succ_cluster,offset);
			}
		}
	}
	int predecessor(int x)const{
		if(max!=-1&&x>max)return max;
		int min_low=cluster[high(x)].minimum();
		if(min_low!=-1&&low(x)>min_low){
			int offset=cluster[high(x)].predecessor(low(x));
			return index(high(x),offset);
		}else{
			int pred_cluster=summary.predecessor(high(x));
			if(pred_cluster==-1){
				if(min!=-1&&x>min)return min;
				return -1;
			}else{
				int offset=cluster[pred_cluster].maximum();
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
				x=index(first_cluster,cluster[first_cluster].minimum());
				min=x;
			}
			cluster[high(x)].erase(low(x));
			if(cluster[high(x)].minimum()==-1){
				summary.erase(high(x));
				if(x==max){
					int summary_max=summary.maximum();
					if(summary_max==-1){
						max=min;
					}else{
						max=index(summary_max,cluster[summary_max].maximum());
					}
				}
			}else if(x==max){
				max=index(high(x),cluster[high(x)].maximum());
			}
		}
	}
	void clear(){
		if(min==-1)return;
		min=max=-1;
		summary.clear();
		for(int i=0;i<(1<<(U+1)/2);i++){
			cluster[i].clear();
		}
	}
};
template<>
class vanEmdeBoasTree<1>{
	friend class vanEmdeBoasTree<2>;
	friend class vanEmdeBoasTree<3>;
	int min,max;
public:
	vanEmdeBoasTree():min(-1),max(-1){}
		int maximum()const{
		return max;
	}
	int minimum()const{
		return min;
	}
	bool member(int x)const{
		if(x==min||x==max)return true;
		return false;
	}
	void insert(int x){
		if(x<min)min=x; 
		else if(max<x)max=x;
	}
	int successor(int x)const{
		if(x==0&&max==1)return 1;
		return -1;
	}
	int predecessor(int x)const{
		if(x==1&&min==0)return 0;
		return -1;
	}
	void erase(int x){
		if(min==max){
			min=max=-1;
		}else{
			if(x==0)min=1;
			else min=0;
			max=min;
		}
	}
	void clear(){min=max=-1;}
};



