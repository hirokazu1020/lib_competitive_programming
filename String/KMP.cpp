//verify Codeforces 346B Lucky Common Subsequence 

#include<iostream>
#include<vector>
using namespace std;


class KMP{
	int len;
	int *fail;
	string s;
public:
	KMP(const string &s):s(s){
		len=s.size();
		fail = new int[len+1];
		fail[0]=-1;
		for(int i=1;i<=len;i++){
			fail[i]=next(fail[i-1],s[i-1]);
		}
	}
	~KMP(){delete[] fail;}
	int next(int t,char c)const{
		if(t==len)t=fail[t];
		while(0<=t&&s[t]!=c)t=fail[t];
		return t+1;
	}
	vector<int> search(const string &text)const{
		vector<int> res;
		int t=0;
		for(int i=0;i<(int)text.size();i++){
			t=next(t,text[i]);
			if(t==len)res.push_back(i-len+1);
		}
		return res;
	}
};
