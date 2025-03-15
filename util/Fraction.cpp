long gcd(long a,long b){
	if(b==0)return a;
	return gcd(b,a%b);
}

//ï™êîÉNÉâÉX
struct Fraction{
	long long x,y;
	Fraction(){}
	Fraction(long long x,long long y){
		long long z = gcd_m(x,y);
		if(y<0){
			x=-x;
			y=-y;
		}
		this->x=x/z;
		this->y=y/z;
	}
	Fraction inverse()const{
		return Fraction(y,x);
	}
	Fraction operator + (Fraction a)const{
		long long z = gcd_m(y,a.y);
		return Fraction(a.y/z*x+y/z*a.x, y/z*a.y);
	}
	Fraction operator - (Fraction a)const{
		long long z = gcd_m(y,a.y);
		return Fraction(a.y/z*x-y/z*a.x, y*a.y);
	}
	Fraction operator * (Fraction a)const{
		return Fraction(x*a.x,y*a.y);
	}
	Fraction operator / (Fraction a)const{
		return (*this)*a.inverse();
	}

	Fraction operator + (int a)const{
		return Fraction(x+a*y, y);
	}
	Fraction operator - (int a)const{
		return Fraction(x-a*y, y);
	}
	Fraction operator * (int a)const{
		long long z = gcd_m(a,y);
		return Fraction(a/z*x,y/z);
	}
	Fraction operator / (int a)const{
		long long z = gcd_m(a,x);
		return Fraction(x/z,a/z*y);
	}
	friend Fraction operator + (int a,Fraction r){return r+a;}
	friend Fraction operator - (int a,Fraction r){return Fraction(r.y*a-r.x,r.y);}
	friend Fraction operator * (int a,Fraction r){return r*a;}
	friend Fraction operator / (int a,Fraction r){return r.inverse()*a;}
	long long comp(Fraction a)const{
		if(x==0&&a.x==0)return 0;
		long long s,t;
		s = gcd_m(x,a.x);
		t = gcd_m(y,a.y);
		return (x/s)*(a.y/t) - (a.x/s)*(y/t);
	}
	bool operator == (Fraction a)const{return comp(a)==0;}
	bool operator != (Fraction a)const{return comp(a)!=0;}
	bool operator < (Fraction a)const{return comp(a)<0;}
	bool operator > (Fraction a)const{return comp(a)>0;}
	bool operator <= (Fraction a)const{return comp(a)<=0;}
	bool operator >= (Fraction a)const{return comp(a)>=0;}
	static long long gcd_m(long long a,long long b){
		return gcd(a<0?-a:a,b<0?-b:b);
	}
};


