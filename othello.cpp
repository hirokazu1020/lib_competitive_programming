
typedef unsigned long long BitBoard;

int popCount(BitBoard bits){
	bits = (bits & 0x5555555555555555) + (bits >> 1 & 0x5555555555555555);
	bits = (bits & 0x3333333333333333) + (bits >> 2 & 0x3333333333333333);
	bits = (bits & 0x0f0f0f0f0f0f0f0f) + (bits >> 4 & 0x0f0f0f0f0f0f0f0f);
	bits = (bits & 0x00ff00ff00ff00ff) + (bits >> 8 & 0x00ff00ff00ff00ff);
	bits = (bits & 0x0000ffff0000ffff) + (bits >>16 & 0x0000ffff0000ffff);
	return ((int)bits & 0xffffffff) + (int)(bits >>32);
}
BitBoard put(BitBoard *own,BitBoard *opponent,BitBoard pos){
	BitBoard a;
	BitBoard rev=0;
	BitBoard w = *opponent & 0x7e7e7e7e7e7e7e7e; 
	BitBoard t = w & (pos << 1);
	do{
		a=t;
		t |= w & (t << 1);
	}while(a!=t);
	if(*own & t<<1)rev|=t;
	t = w & (pos >> 1);
	do{
		a=t;
		t |= w & (t >> 1);
	}while(a!=t);
	if(*own & t>>1)rev|=t;
	w = *opponent & 0x00ffffffffffff00;
	t = w & (pos << 8);
	do{
		a=t;
		t |= w & (t << 8);
	}while(a!=t);
	if(*own & t<<8)rev|=t;
	t = w & (pos >> 8);
	do{
		a=t;
		t |= w & (t >> 8);
	}while(a!=t);
	if(*own & t>>8)rev|=t;
	w= *opponent & 0x007e7e7e7e7e7e00;
	t = w & (pos << 7);
	do{
		a=t;
		t |= w & (t << 7);
	}while(a!=t);
	if(*own & t<<7)rev|=t;
	t = w & (pos >> 7);
	do{
		a=t;
		t |= w & (t >> 7);
	}while(a!=t);
	if(*own & t>>7)rev|=t;
	t = w & (pos << 9);
	do{
		a=t;
		t |= w & (t << 9);
	}while(a!=t);
	if(*own & t<<9)rev|=t;
	t = w & (pos >> 9);
	do{
		a=t;
		t |= w & (t >> 9);
	}while(a!=t);
	if(*own & t>>9)rev|=t;
	*own ^= rev | pos;
	*opponent ^= rev;
	return rev;
}
BitBoard putPositions(BitBoard own,BitBoard opponent){
	BitBoard mobility;
	BitBoard blank= ~(own|opponent);
	BitBoard w = opponent & 0x7e7e7e7e7e7e7e7e; 
	BitBoard t = w & (own << 1 | own>>1);         
	t |= w & (t << 1 | t >> 1);
	t |= w & (t << 1 | t >> 1);
	t |= w & (t << 1 | t >> 1);
	t |= w & (t << 1 | t >> 1);
	t |= w & (t << 1 | t >> 1);
	mobility = blank & (t << 1 | t>>1);

	w = own & 0x00ffffffffffff00;
	t = w & (own << 8 | own>>8); 
 	t |= w & (t << 8 | t >> 8); 
	t |= w & (t << 8 | t >> 8); 
	t |= w & (t << 8 | t >> 8);
	t |= w & (t << 8 | t >> 8);
	t |= w & (t << 8 | t >> 8);
	mobility |= blank & (t << 8 | t>>8);

	w = opponent & 0x007e7e7e7e7e7e00;
	t = w & (own << 7 | own>>7); 
	t |= w & (t << 7 | t >> 7); 
	t |= w & (t << 7 | t >> 7); 
	t |= w & (t << 7 | t >> 7);
	t |= w & (t << 7 | t >> 7);
	t |= w & (t << 7 | t >> 7);
	mobility |= blank & (t << 7 | t>>7);

	t = w & (own << 9 | own>>9); 
	t |= w & (t << 9 | t >> 9); 
	t |= w & (t << 9 | t >> 9); 
	t |= w & (t << 9 | t >> 9);
	t |= w & (t << 9 | t >> 9);
	t |= w & (t << 9 | t >> 9);
	mobility |= blank & (t << 9 | t >> 9);
	return mobility;
}
BitBoard rotate(BitBoard b){
	b=(0x0055005500550055&b>> 8)|((0x0055005500550055&b)<<1)|(0xaa00aa00aa00aa00&b<< 8)|((0xaa00aa00aa00aa00&b)>>1);
	b=(0x0000333300003333&b>>16)|((0x0000333300003333&b)<<2)|(0xcccc0000cccc0000&b<<16)|((0xcccc0000cccc0000&b)>>2);
	b=(0x000000000f0f0f0f&b>>32)|((0x000000000f0f0f0f&b)<<4)|(0xf0f0f0f000000000&b<<32)|((0xf0f0f0f000000000&b)>>4);
	return b;
}
void print(BitBoard b){
	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++)cout<<(b>>i*8+j&1);
		cout<<endl;
	}cout<<endl;
}