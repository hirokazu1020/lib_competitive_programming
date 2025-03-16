#include <cstdint>
#include <bit>
#include <nmmintrin.h>
#include <immintrin.h>


inline int popCount(uint64_t x) {
	return std::popcount(x);
}

inline int popSelect(uint64_t block1, uint32_t k){
#if 1
	// -march=native
	uint64_t z = 1ULL << (k - 1);
	uint64_t y = _pdep_u64(z, block1);
	return _tzcnt_u64(y);
#else
	uint64_t block2 = (block1>>1  & 0x5555555555555555)+(block1  & 0x5555555555555555);
	uint64_t block4 = (block2>>2  & 0x3333333333333333)+(block2  & 0x3333333333333333);
	uint64_t block8 = (block4>>4  & 0x0f0f0f0f0f0f0f0f)+(block4  & 0x0f0f0f0f0f0f0f0f);
	uint64_t block16= (block8>>8  & 0x00ff00ff00ff00ff)+(block8  & 0x00ff00ff00ff00ff);
	uint64_t block32= (block16>>16& 0x0000ffff0000ffff)+(block16 & 0x0000ffff0000ffff);
	int pos = 0;
	if((block32 & 0xffffffff) < k){
		k -= block32 & 0xffffffff;
		pos |= 32;
	}
	if((block16 >> pos & 0x0000ffff) < k){
		k -= block16 >> pos & 0x0000ffff;
		pos |= 16;
	}
	if((block8 >> pos & 0x000000ff) < k){
		k -= block8 >> pos & 0x000000ff;
		pos |= 8;
	}
	if((block4 >> pos & 0x0000000f) < k){
		k -= block4 >> pos & 0x0000000f;
		pos |= 4;
	}
	if((block2 >> pos & 0x00000003) < k){
		k -= block2 >> pos & 0x00000003;
		pos |= 2;
	}
	if((block1 >> pos & 0x00000001) < k)pos |= 1;
	return pos;
#endif
}


//メモリ使用量:2nビット
class BitVector{
	int n;
	int blocks;
	std::vector<unsigned int> B;
	std::vector<int> r;
public:
	BitVector(){}
	BitVector(int size){
		init(size);
	}
	void init(int size){
		n = size;
		blocks = (n>>5)+1;
		B.assign(blocks ,0);
		r.assign(blocks ,0);
	}
	void set(int k){
		B[k>>5] |= 1<<(k&31);
	}
	void build(){
		r[0]=0;
		for(int i=1;i<blocks;i++){
			r[i] = popCount(B[i-1]) + r[i-1];
		}
	}	
	bool access(int k)const{
		return B[k>>5] & 1<<(k&31);
	}
	//[0,k)の1の個数
	int rank(int k)const{
		return r[k>>5] + popCount(B[k>>5] & ((1<<(k&31))-1));
	}
	//k+1番目の１の場所
	//O(log n)
	int select1(int k)const{
		int lb=0,ub=blocks;
		if(k==-1)return -1;
		while(ub-lb>1){
			int m = (lb+ub)/2;
			if(k<r[m])ub=m;
			else lb=m;
		}
		k -= r[lb];
		return lb<<5 | popSelect(B[lb],k);
	}
	//O(log n)
	int select0(int k)const{
		int lb=0,ub=blocks;
		if(k==-1)return -1;
		while(ub-lb>1){
			int m = (lb+ub)/2;
			if(k<(m<<5)-r[m])ub=m;
			else lb=m;
		}
		k -= (lb<<5)-r[lb];
		return lb<<5 | popSelect(~B[lb],k);
	}
};
