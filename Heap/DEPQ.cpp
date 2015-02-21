//—vverify

#include<queue>
#include<functional>
#include<vector>

//Double-Ended priority queue
//‚È‚ç‚µO(log n)
template<class T>
class DEPQ{
	mutable std::priority_queue<T,std::vector<T>,greater<T> > minheap,mindel;	
	mutable std::priority_queue<T,std::vector<T>,less<T> > maxheap,maxdel;
public:
	T popmin(){
		maxdel.push(minheap.top());
		minheap.pop();
	}
	T popmax(){
		mindel.push(maxheap.top());
		maxheap.pop();
	}
	T min()const{
		while(minheap.top()==mindel.top()){
			minheap.pop();
			mindel.pop();
		}
		return minheap.top();
	}
	T max()const{
		while(maxheap.top()==maxdel.top()){
			maxheap.pop();
			maxdel.pop();
		}
		return maxheap.top();
	}
	void push(T v){
		minheap.push(v);
		maxheap.push(v);
	}
};



class MiddleHeap{
	DEPQ<int> higher;	
	DEPQ<int> lower;
	int nlow,nhigh;
	void balance(){
		while(nlow+1<nhigh){
			lower.push(higher.max());
			higher.popmax();
			nlow++;
			nhigh--;
		}
		while(nhigh+1<nlow){
			higher.push(lower.min());
			lower.popmin();
			nhigh++;
			nlow--;
		}
	}
public:
	void push(int v){
		if(higher.empty()){
			higher.push(v);
			nhigh++;
			return;
		}
		if(lower.empty()){
			lower.push(v);
			nlow++;
			return;
		}
		if(higher.top()<=v){
			higher.push(v);
			nhigh++;
		}else {
			lower.push(v);
			nlow++;
		}
		balance();
	}
	int middle()const{//a[n/2]
		if(nlow<nhigh)return higher.min();
		return lower.max();
	}
	int min()const{
		return lower.min();
	}
	int max()const{
		return higher.max();
	}
	int popmid(){
		if(nlow<nhigh)return {
			higher.popmin();
			nhigh--;
		}
		else {
			lower.popmax();
			nlow--;
		}
	}
	int popmin()const{
		if(!lower.empty()){
			lower.popmin();
			nlow--;
		}
		else {
			higher.popmin();
			nhigh--;
		}
		balance();
	}
	int popmax()const{
		if(!higher.empty()){
			higher.popmax();
			nhigh--;
		}
		else {
			lower.popmax();
			nlow--:
		}
		balance();
	}
};