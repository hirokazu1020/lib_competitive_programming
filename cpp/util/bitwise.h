#define rep(i,n) for(int i=0;i<(int)(n);i++)

/*ビット操作関連のライブラリ*/
//ある集合の部分集合の列挙
for(int i=s;;i=(i-1)&s){
	// 処理
	if(i==0)break;
}

//ある集合を包含する集合の列挙
for(int i=s;i<(1<<n);i=(i+1)|s){
	// 処理
}

//高速ゼータ変換 O(n2^n)
rep (i, n) rep (b, 1 << n) if (0 == (b & (1 << i))) a[b] += a[b | (1 << i)];

//{0,1,...,n-1} に含まれるサイズkの部分集合の列挙
int comb = (1 << k) - 1;
while (comb < 1 << n) {
	// 処理
	int x = comb & -comb, y = comb + x;
	comb = ((comb & ‾y) / x >> 1) | y;
}

//最下位popの位置
int bits_ctzll(unsigned long long b){
	b&=-b;
	int res=0;
	for(int i=5;i>=0;i--){
		int shift=1<<i;
		if(b>>shift){
			b>>=shift;
			res|=shift;
		}
	}
	return res;
}

uint32_t bits_reverse(uint32_t x) {
	x = ((x & 0x55555555) << 1) | ((x >> 1) & 0x55555555);
	x = ((x & 0x33333333) << 2) | ((x >> 2) & 0x33333333);
	x = ((x & 0x0f0f0f0f) << 4) | ((x >> 4) & 0x0f0f0f0f);
	x = ((x & 0x00ff00ff) << 8) | ((x >> 8) & 0x00ff00ff);
	x = ((x & 0x0000ffff) << 16) | ((x >> 16) & 0x0000ffff);
	return x;
}
