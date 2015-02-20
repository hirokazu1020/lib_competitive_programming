

class PersistentSegtree{
	struct Node{
		Node(){}
		Node(int sum,Node* l,Node*r):sum(sum),l(l),r(r){}
		int sum;
		Node *l,*r;
	};
	int n;
	Node* root;

	Node* build(int l,int r){
		int m=(l+r)/2;
		Node *p=new Node;
		p->sum=0;
		if(r-l==1){
			p->l=p->r=NULL;
			return p;
		}
		p->l=build(l,m);
		p->r=build(m,r);
		return p;
	}
	int query(int a,int b,int l,int r,Node *p)const{
		if(r<=a || b<=l)return 0;
		if(a<=l && r<=b)return p->sum;
		int m=(l+r)/2;
		return query(a,b,l,m,p->l)+query(a,b,m,r,p->r);
	}
	Node* changequery(int k,int x,int l,int r,Node *p)const{
		Node *np=new Node(p->sum+x,p->l,p->r);
		if(r-l==1)return np;
		int m=(l+r)/2;
		if(k<m)np->l=changequery(k,x,l,m,p->l);
		else np->r=changequery(k,x,m,r,p->r);
		return np;
	}
	PersistentSegtree(int n,Node *root):n(n),root(root){}
public:
	PersistentSegtree(int n):n(n){
		root=build(0,n);
	}
	int query(int a,int b)const{//[a,b)‚Ì˜a
		return query(a,b,0,n,root);
	}
	PersistentSegtree* changequery(int k,int x){//k‚Éx‚ğ‰ÁZ
		return new PersistentSegtree(n,changequery(k,x,0,n,root));
	}
};

