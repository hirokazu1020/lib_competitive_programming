#include<iostream>
#include<utility>
#include<algorithm>
#include<cassert>

using namespace std;


//空間O(2^U)
//ver1を葉をintにして高速化
//重複挿入重複削除の挙動は未知
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
	void empty_insert(int x){min=max=x;}
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
			if(cluster[high(x)].maximum()==-1){
				summary.insert(high(x));
				cluster[high(x)].empty_insert(low(x));
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
class vanEmdeBoasTree<5>{
	template<int> friend class vanEmdeBoasTree;
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
	void empty_insert(int x){s=1u<<x;}
public:
	vanEmdeBoasTree():s(0){}
	int maximum()const{
		return highest(s);
	}
	int minimum()const{
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

template<>
class vanEmdeBoasTree<4>:public vanEmdeBoasTree<5>{};
template<>
class vanEmdeBoasTree<3>:public vanEmdeBoasTree<4>{};
template<>
class vanEmdeBoasTree<2>:public vanEmdeBoasTree<3>{};
template<>
class vanEmdeBoasTree<1>:public vanEmdeBoasTree<2>{};




unsigned int xor128(){
	static unsigned int x=123456789,y=362436069,z=521288629,w=88675123; 
	unsigned int t=(x^(x<<11));
	x=y;y=z;z=w;
	return w=(w^(w>>19))^(t^(t>>8));
}
#include<set>
#include<cstdlib>
#include<ctime>
#include<string>
int main(){
	
	/*vanEmdeBoasTree<> vEBT;
	vEBT.insert(1);
	vEBT.insert(15);
	vEBT.insert(18);
	vEBT.insert(54);
	vEBT.insert(15645);
	//vEBT.erase(15645);
	for(int i=0;i<(1<<16);i++){
		if(vEBT.member(i))cout<<i<<endl;
	}
	cout<<vEBT.predecessor(1)<<' '<<vEBT.successor(1)<<endl;
	cout<<vEBT.predecessor(5)<<' '<<vEBT.successor(5)<<endl;
	cout<<vEBT.predecessor(20)<<' '<<vEBT.successor(20)<<endl;
	cout<<vEBT.predecessor(55)<<' '<<vEBT.successor(55)<<endl;*/
	
	clock_t t1=clock();
	/*for(int i=0;i<200;i++){
		set<int> s;
		vanEmdeBoasTree<> veb;
		//for(int i=0;i<1000000;i++) s.insert(xor128()&((1<<31)-1));
		for(int i=0;i<1000000;i++)veb.insert(xor128()&((1<<31)-1));
	}*/
	//set<int> s; 
	vanEmdeBoasTree<20> veb;
	for(int i=0;i<10;i++){
		//for(int i=0;i<1000000;i++) s.insert(xor128()&((1<<20)-1));s.clear();
		for(int i=0;i<1000000;i++)veb.insert(xor128()&((1<<20)-1));veb.clear();
	}
	cout<<(double)(clock()-t1)/CLOCKS_PER_SEC<<"sec"<<endl;



	/*for(int i=0;i<10;i++)veb.insert(i);
	veb.erase(5);
	ull x=0;
	for(int i=1;i<10;i++){
		x=veb.successor(x);
		cout<<x<<endl;
	}*/
	/*for(int i=0;i<1000000;i++){
		//int a= (rand()%100000000);
		//int b=rand()%3;
		ull a= (xor128()&((1<<16)-1));
		ull b=xor128()%2;
		if(b==0){
			int x,y;
			x=veb.member(a)?1:0;
			//y=hs.find(a)?1:0;
			//if(x^y)cout<<a<<' '<<x<<y<<endl;
		}else if(b==1){
			veb.insert(a);
			//hs.insert(a);
			//cout<<a<<endl;
		}else{
			veb.erase(a);		
			//hs.erase(a);
		}
	}*/

	//AOH Heap
	/*vanEmdeBoasTree<31> veb;
	while(1){
		string s;
		cin>>s;
		if(s=="end")break;
		if(s=="insert"){
			int n;
			cin>>n;
			veb.insert(n);
		}else if(s=="extract"){
			cout<<veb.maximum()<<endl;
			veb.erase(veb.maximum());
		}
	}*/
}




