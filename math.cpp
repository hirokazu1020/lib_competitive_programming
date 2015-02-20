#include<vector>
#include<map>
#include<algorithm>
using namespace std;

//�ő���� O(log max(a,b))�@<algorithm>��__gcd(a,b)
int gcd(int a,int b){
	if(b==0)return a;
	return gcd(b,a%b);
}
long gcd(long a,long b){
	if(b==0)return a;
	return gcd(b,a%b);
}
//���g�������񐔂̘a
int sun_divisors(int n){
	if(n<=1)return 0;
	int i,s=1;
	for(i=2;i*i<n;i++)
		if(n%i==0)s+=i+n/i;
	if(i*i==n&&n%i==0)s+=i;
	return s;
}

//�f������ O(��n)
bool isPrime(int n){
	if(n<=1)return false;
	for(int i=2;i*i<=n;i++)
		if(n%i==0)return false;
	return true;
}
//�f���� O(��n)
vector<int> divisor(int n){
	vector<int> res;
	for(int i=1;i*i<=n;i++){
		if(n%i==0){
			res.push_back(i);
			if(i!=n/i)res.push_back(n/i);
		}
	}
}
//�f�������� O(��n)
map<int,int> prime_factor(int n){
	map<int,int> res;
	for(int i=2;i*i<=n;i++){
		while(n%i==0){
			++res[i];
			n/=i;
		}
	}
	if(1<n)res[n]=1;
	return res;
}
//�G���g�X�e�l�X��� O(n loglog n)
#define MAX_N 1000000
int prime[MAX_N];//i�Ԗڂ̑f��
bool is_prime[MAX_N+1];
int sieve(int n){
	int p=0;
	memset(is_prime,true,sizeof(is_prime));
	is_prime[0]=is_prime[1]=false;
	for(int i=2;i<=n;i++){
		if(is_prime[i]){
			prime[p++]=i;
			for(int j=2*i;j<=n;j+=i)is_prime[j]=false;
		}
	}
	return p;
}

//osa_k�@ �����f�������� �O����O(n loglog n)�@�{�v�ZO(log n)
#define MAX_N 1000000
int prime[MAX_N];//i�Ԗڂ̑f��
bool is_prime[MAX_N+1];
int min_factor[MAX+1];//�ŏ��̑f����
int sieve(int n){
	int p=0;
	memset(is_prime,true,sizeof(is_prime));
	is_prime[0]=is_prime[1]=false;
	min_factor[0] = 0;
	min_factor[1] = 1;
	for(int i=2;i<=n;i++){
		if(is_prime[i]){
			min_factor[i]=i;
			prime[p++]=i;
			for(int j=2*i;j<=n;j+=i){
				is_prime[j]=false;
				if(min_factor[j]==0)min_factor[j]=i;
			}
		}
	}
	return p;
}
vector<pair<int,int> > prime_factor(int n){
	vector<pair<int,int> > res;
	int prev=-1,fact,num;
	while(1<n){
		fact=min_factor[n];
		if(prev!=fact){
			if(prev!=-1)res.push_back(make_pair(prev,num));
			num=1;
			prev=fact;
		}
		else num++;
		n/=fact;
	}
	if(prev!=-1)res.push_back(make_pair(prev,num));
	return res;
}


//�������\�i�\�L��
string todemical(int x,int y){
	string s="0.";
	while(x){
		x*=10;
		s+=x/y;
		x%=y;
	}
	return s;
}


#define MOD 1000000007
//�p�X�J���̎O�p�` nCr���O(n^2)
int c[MAXN+1][MAXN+1];
void init(){
	c[0][0]=1;
	for(int i=1;i<=MAXN;i++){
		c[i][0]=1;
		for(int j=1;j<=i;j++)
			c[i][j] = (c[i-1][j-1]+c[i-1][j])%MOD;
	}
}
//�t���œ񍀌W�����v�Z(MOD�͑f��)�@�O����O(n),�{�v�ZO(1)
#define MAXN 1000000
long long inv[MAXN+1];//MOD��@�Ƃ����@�̋t��
long long fact[MAXN+1];//�K��
long long ifact[MAXN+1];//�K��̋t��
void init(){
	inv[1] = 1;
	for(int i=2;i<=MAXN;i++) inv[i] = inv[MOD%i] * (MOD - MOD/i) % MOD;
	fact[0]=ifact[0]=1;
	for(int i=1;i<=MAXN;i++){
		fact[i]=i*fact[i-1]%MOD;
		ifact[i]=ifact[i-1]*inv[i]%MOD;
	}
}
long long C(int n, int r){
	if(n < 0 || r < 0 || r > n)return 0;
	if(r > n / 2)r = n - r;
	return fact[n]*ifact[n-r]%MOD*ifact[r]%MOD;
}
//x^y%MOD
long long pow_mod(long long x,long long y){
	int res=1;
	while(y){
		if(y&1)res=(res*x)%MOD;
		y>>=1;
		x=(x*x)%MOD;
	}
	return res;
}
//�g�����[�N���b�h�̌ݏ��@
long long extgcd(int a,int b,int &x,int &y){
	int gcd_=a;
	if(b){
		gcd_=extgcd(b,a%b,y,x);
		y-=a/b*x;
	}else{
		x=1;y=0;
	}
	return gcd_;
}
//�t�����v�Z O(log MOD)
int inv_mod(int a){
	//return mod_pow(a,MOD-2);//MOD���f���̂Ƃ�����
	int x,y;
	extgcd(a,MOD,x,y);
	return (MOD+x%MOD)%MOD;
}
