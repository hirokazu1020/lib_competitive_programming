#include<cstring>
#include<vector>
#include<cstring>


class BIT{//binary indexed tree, Fenwick tree�Ƃ�
	int lg;
	std::vector<int> bit;
public:
	BIT(int size):bit(size+1,0){
		lg=size;
		while(lg&lg-1)lg&=lg-1;
	}
	void add(int i,int x){//i�Ԗڂ�x������
		i++;//BIT�Y������1�`��������
		while(i<(int)bit.size()){
			bit[i]+=x;
			i+= i&-i;
		}
	}
	int sum(int a)const{//[0,a]�̘a
		a++;
		int res=0;
		while(0<a){
			res+=bit[a];
			a-= a&-a;
		}
		return res;
	}
	int sum(int a,int b)const{//[a,b]�̍��v
		return sum(b)-sum(a-1);
	}
	void zeroclear(){
		bit.assign(bit.size(),0);
	}
	//verified UTPC2013I
	//�W����prevvalue,nextvalue�Ƃ��Ă��g���� lower_bound(sum(s))
	int lower_bound(int w)const{//sum([0,a])��w�ȏ�ƂȂ�ŏ���index (�������܂܂�Ă͂���)
		if(w<=0)return 0;
		int x=0;
		for(int k=lg;k;k>>=1){
			if((x|k)<(int)bit.size()&&bit[x|k]<w){
				w-=bit[x|k];
				x|=k;
			}
		}
		return x+1==bit.size()?-1:x;
	}
	//�vverify
	//BIT��set�Ƃ��Ďg�� 
	int nextvalue(int x)const{//x����̍ŏ��̒l �ȈՔ�
		return lower_bound(sum(x)+1);
	}
	int prevvalue(int x)const{
		if(x==0)return -1;
		int s=sum(x-1);
		if(s==0)return -1;
		return lower_bound(s);
	}
	void insert(int x){add(x,1);}
	void erase(int x){if(sum(x,x))add(x,-1);}
};


class BIT2D{
	std::vector<BIT> bit;
public:
	BIT2D(int w,int h):bit(w+1,BIT(h)){}
	void add(int x,int y,int val){
		x++;
		while(x<(int)bit.size()){
			bit[x].add(y,val);
			x += x&-x;
		}
	}
	int sum(int x,int y)const{
		int s=0;
		while(x>0){
			s += bit[x].sum(y);
			x -= x&-x;
		}
		return s;
	}
	int sum(int x0,int y0,int x1,int y1)const{
		int a,b,c,d;
		a=sum(x0-1,y0-1);
		b=sum(x0-1,y1);
		c=sum(x1,y0-1);
		d=sum(x1,y1);
		return d-b-c+a;
	}
};

/*--- ����m�F�p ---*/
#include<stdio.h>
#include<time.h>
#include<stdlib.h>
#define N 10
int sum(int array[],int a,int b){
	int res=0;
	for(;a<=b;a++)res+=array[a];
	return res;
}
int main(){
	int a[N];
	std::memset(a,0,sizeof(a));
	BIT b(N);
	srand(time(NULL));
	for(int i=0;i<20;i++){
		int s=rand()%N;
		int t=s+rand()%(N-s);
		int n=1;
		int j= s+rand()%(t-s+1);
		a[j]+=n;
		b.add(j,n);
		if(sum(a,s,t)!=b.sum(s,t)){
			printf("NO:%8d ,%8d\n",sum(a,s,t),b.sum(s,t));
			return 0;
		}
		for(int i=0;i<N;i++)printf("%3d ",a[i]);
		puts("");
		for(int i=0;i<N;i++)printf("%3d ",b.sum(0,i));
		puts("\n");
	}
	puts("ok");
}