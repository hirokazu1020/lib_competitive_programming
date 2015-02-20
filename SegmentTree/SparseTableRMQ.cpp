#include<vector>
#include<algorithm>
#include<iostream>
using namespace std;


//前処理O(n log n) 区間最小値クエリO(1)
class SparseTableRMQ{
	int maxlog;
	std::vector<char> log;
	std::vector<int> M;
public:
	SparseTableRMQ(const std::vector<int> &a){
		int n=a.size();
		log.resize(n+1);
		log[0]=0;
		for(int i=1,a=0;i<=n;i++){
			if((1<<a+1)<i)a++;
			log[i]=a;
		}
		maxlog=log[n]+1;
		M.resize(n*maxlog);
		for(int i=0;i<n;i++)M[i*maxlog]=a[i];
		for(int j=1;1<<j<=n;j++){
			for(int i=0;i+(1<<j)-1<n;i++){
				M[i*maxlog+j] = std::min(M[i*maxlog+j-1], M[(i+(1<<j-1))*maxlog+j-1]);
			}
		}
	}
	int query(int i,int j)const{//[i,j)の最小値
		int k=log[j-i];
		return std::min(M[i*maxlog+k], M[(j-(1<<k))*maxlog+k]);
	}
};

int main(){
	int a[]={2,4,3,1,6,7,8,9,1,7};
	SparseTableRMQ s(vector<int>(a,a+10));
	for(int i=0;i<10;i++){
		for(int j=i+1;j<=10;j++){
			cout<<i<<' '<<j<<' '<<s.query(i,j)<<endl;
		}
	}
}