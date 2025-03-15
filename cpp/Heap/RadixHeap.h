
int nlz(unsigned int b){
	if(b==0)return -1;
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



template<class S,int MAX_LOG=31>
class RadixHeap{
	vector<pair<int,S>> bucket[MAX_LOG+1];
	int last_deleted;
	int sz;
public:
	RadixHeap():sz(0),last_deleted(0){}
	void push(int key,const S &s){
		sz++;
		bucket[nlz(key^last_deleted)+1].push_back(P(key,s));
	}
	pair<int,S> pop(){
		sz--;
		if (bucket[0].empty()) {
			int i = 1;
			while (bucket[i].empty()) i++;
			last_deleted = bucket[i][0].first;
			for(int j=0;j<(int)bucket[i].size();j++){
				pair<int,S> &d=bucket[i][j];
				last_deleted = min(last_deleted, d.first);
			}
			for(int j=0;j<(int)bucket[i].size();j++){
				pair<int,S> &p=bucket[i][j];
				bucket[nlz(last_deleted^p.first)+1].push_back(p);
			}
			bucket[i].clear();
		}
		pair<int,S> r = bucket[0].back();
		bucket[0].pop_back();
		return r;
	}
	int size()const{return sz;}
	bool empty()const{return !sz;}
};