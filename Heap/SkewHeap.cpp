struct SkewHeap{
	SkewHeap *l,*r;
	int val;
};

SkewHeap* meld(SkewHeap* a, SkewHeap* b) {
    if (a==NULL) return b;
    if (b==NULL) return a;
    if (a->val < b->val) swap(a, b);
    a->r = meld(a->r, b);
	std:: swap(a->l, a->r);
    return a;
}
SkewHeap* insert(SkewHeap* a,int v) {
    SkewHeap* b=new SkewHeap;
	b->l=b->r=NULL;
	b->val=v;
	return meld(a,b);
}
int getmax(SkewHeap* a){
	return a->val;
}
SkewHeap* pop(SkewHeap* a){
	SkewHeap* res = meld(a->l,a->r);
	delete a;
	return res;
}
void del(SkewHeap* a){
	if(a->l)del(a->l);
	if(a->r)del(a->r);
	delete a;
}