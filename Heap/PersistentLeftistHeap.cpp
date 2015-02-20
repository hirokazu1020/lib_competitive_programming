//—vverify

#include<algorithm>

struct PersistentLeftistHeap{
	PersistentLeftistHeap *l,*r;
	int s;
	int val;
};

PersistentLeftistHeap* meld(PersistentLeftistHeap* a, PersistentLeftistHeap* b) {
    if (a==NULL) return b;
    if (b==NULL) return a;
    if (a->val < b->val) std::swap(a, b);
	PersistentLeftistHeap* c=new PersistentLeftistHeap;
	*c=*a;
    c->r = meld(a->r, b);
	if(c->l==NULL || c->l->s<c->r->s)std::swap(c->l,c->r);
	c->s = (c->r==NULL?0:c->r->s)+1;
    return c;
}
PersistentLeftistHeap* insert(PersistentLeftistHeap* a,int v) {
    PersistentLeftistHeap* b=new PersistentLeftistHeap;
	b->l=b->r=NULL;
	b->s=1;
	b->val=v;
	return meld(a,b);
}
int getmax(PersistentLeftistHeap* a){
	return a->val;
}
PersistentLeftistHeap* pop(PersistentLeftistHeap* a){
	return meld(a->l,a->r);
}


