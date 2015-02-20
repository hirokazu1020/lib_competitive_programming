#include<sstream>
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<cstring>
#include<cmath>
#include<string>
#include<vector>
#include<set>
#include<queue>
#include<numeric>
#include<functional>
#include<algorithm>
#include<climits>
using namespace std;
#define INF (1<<29)
#define rep(i,n) for(int i=0;i<(int)(n);i++)




//verified
class SuffixArray{
	string text;
	vector<int> sa;

	static bool eq(const vector<int> &v,int idx1,int idx2,const vector<bool> &type){
		if(v[idx1]!=v[idx2])return false;
		int last=v.size()-1;
		for(int i=1;;i++){
			if(v[idx1+i]!=v[idx2+i])return false;
			if(idx1+i==last||type[idx1+i-1]&&!type[idx1+i])
				return idx2+i==last||type[idx2+i-1]&&!type[idx2+i];
			else if(idx2+i==last||type[idx2+i-1]&&!type[idx2+i])return false;
		}
		return true;
	}
	template<bool erase>
	static inline void Lsort(const vector<int> &v,const vector<bool> &type,vector<int> &bucket,vector<int> &rank_less){
		for(int i=0;i<(int)bucket.size();i++){
			if(bucket[i]>0 && type[bucket[i]-1]){
				bucket[rank_less[v[bucket[i]-1]]++]=bucket[i]-1;
				if(erase)bucket[i]=-1;
			}
		}
	}
	template<bool erase>
	static inline void Ssort(const vector<int> &v,const vector<bool> &type,vector<int> &bucket,vector<int> &rank_less_eq){
		for(int i=bucket.size()-1;i>=0;i--){
			if(bucket[i]>0 && !type[bucket[i]-1]){
				bucket[--rank_less_eq[v[bucket[i]-1]]]=bucket[i]-1;
				if(erase)bucket[i]=-1;
			}	
		}
	}
	
	static vector<int> induced_sorting(const vector<int> &v,int alpha){
		vector<int> bucket(v.size());
		vector<int> rank(alpha),c(alpha);
		vector<bool> type(v.size());//false=>S,true=>L
		vector<int> lmsidx,lmsord;
		type.back()=true;
		for(int i=v.size()-2;i>=0;i--){
			if(v[i]<v[i+1])type[i]=false;
			else if(v[i]>v[i+1])type[i]=true;
			else type[i]=type[i+1];
		}
		for(int i=0;i<(int)v.size();i++)rank[v[i]]++;
		for(int i=1;i<alpha;i++)rank[i]+=rank[i-1];
			
		//sorting LMS-substring
		fill(bucket.begin(),bucket.end(),-1);
		bool first=true;
		copy(rank.begin(),rank.end(),c.begin());
		for(int i=1;i<(int)v.size();i++){
			if(type[i-1]&&!type[i]){//LMS
				if(first)first=false;
				else bucket[--c[v[i]]]=i;
			}
		}
		copy(rank.begin(),rank.begin()+alpha-1,c.begin()+1);
		c[0]=0;
		bucket[c[v.back()]++]=v.size()-1;
		Lsort<true>(v,type,bucket,c);
		copy(rank.begin(),rank.end(),c.begin());
		Ssort<true>(v,type,bucket,c);
		for(int i=0;i<(int)bucket.size();i++)
			if(bucket[i]>0)lmsidx.push_back(bucket[i]);
		if(!lmsidx.empty()){
			lmsord.resize(lmsidx.size());
			lmsord[0]=0;
			for(int i=1;i<(int)lmsidx.size();i++){
				if(eq(v,lmsidx[i-1],lmsidx[i],type))lmsord[i]=lmsord[i-1];
				else lmsord[i]=lmsord[i-1]+1;
			}
			if(lmsord.back()+1!=lmsord.size()){
				vector<int> lmssuffix;
				fill(bucket.begin(),bucket.end(),-1);
				for(int i=0;i<(int)lmsidx.size();i++){
					if(lmsidx[i])bucket[lmsidx[i]]=lmsord[i];
				}
				lmsidx.clear();
				for(int i=0;i<(int)bucket.size();i++){
					if(bucket[i]!=-1){
						lmssuffix.push_back(bucket[i]);
						lmsidx.push_back(i);
					}
				}
				vector<int> lmssa(induced_sorting(lmssuffix,lmsord.back()+1));
				for(int i=0;i<(int)lmsidx.size();i++){
					lmsord[i]=lmsidx[lmssa[i]];
				}
				lmsidx.swap(lmsord);
			}
			lmsord.clear();
		}
		//ended sorting LMS-substring
		fill(bucket.begin(),bucket.end(),-1);
		copy(rank.begin(),rank.end(),c.begin());
		for(int i=lmsidx.size()-1;i>=0;i--)
			bucket[--c[v[lmsidx[i]]]]=lmsidx[i];
		copy(rank.begin(),rank.begin()+alpha-1,c.begin()+1);
		c[0]=0;
		bucket[c[v.back()]++]=v.size()-1;
		Lsort<false>(v,type,bucket,c);
		copy(rank.begin(),rank.end(),c.begin());
		Ssort<false>(v,type,bucket,c);
		return bucket;
	}
	
public:
	SuffixArray(const string &s){
		build(s);
	}
	const string& get_text()const{return text;}
	void build(const string &s){
		text=s;
		vector<int> v(s.size());
		for(int i=0;i<(int)s.size();i++)v[i]=s[i]-'a';
		sa=induced_sorting(v,26);
	}
	int lower(string &s)const{
		int a=-1,b=text.size();
		while(b-a>1){
			int c=(a+b)/2;
			if(text.compare(sa[c],s.size(),s)<0)a=c;
			else b=c;
		}
		return b;
	}
	int upper(string &s)const{
		int a=-1,b=text.size();
		while(b-a>1){
			int c=(a+b)/2;
			if(text.compare(sa[c],s.size(),s)<=0)a=c;
			else b=c;
		}
		return b;
	}
	string kth_suffix(int k)const{
		return text.substr(sa[k]);
	}
};




int main(){
	string s("abracadabra");
	SuffixArray sa(s);
	for(int i=0;i<s.size();i++){
		cout<<sa.kth_suffix(i)<<endl;
	}
 }
