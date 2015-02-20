//幾何ライブラリ 2013/6/23
//蟻本ベース

#include<iostream>
#include<cstdio>
#define _USE_MATH_DEFINES
#include<cmath>
#include<vector>
#include<map>
#include<algorithm>
using namespace std;


#define INF 1e+10
#define EPS 1e-10
#define EQ(a,b) (abs(a-b)<EPS)

//誤差を考慮して足し算
double add(double a,double b){
	if(abs(a+b) < EPS*(abs(a)+abs(b)))return 0;
	return a+b;
}
struct P{//2次元ベクトル
	double x,y;
	P(){}
	P(double x,double y):x(x),y(y){}
	P operator + (P p){
		return P(add(x,p.x),add(y,p.y));
	}
	P operator - (P p){
		return P(add(x,-p.x),add(y,-p.y));
	}
	P operator * (double d){
		return P(x*d,y*d);
	}
	P operator / (double d){
		return P(x/d,y/d);
	}
	double dot(P p){//内積
		return add(x*p.x,y*p.y);
	}
	double det(P p){//外積
		return add(x*p.y,-y*p.x);
	}
	bool equal(P p){
		return (x-p.x)*(x-p.x)+(y-p.y)*(y-p.y) < EPS*EPS;
	}
	double norm()const{
		return sqrt(x*x+y*y);
	}
};
//typedef pair<P,P> Line;

//線分p1-p2上に点qがあるか判定
bool on_seg(P p1,P p2,P q){
	return (p1-q).det(p2-q)==0 && (p1-q).dot(p2-q)<=0;
}
//直線p1-p2と直線q1-q2の交点
P intersection(P p1,P p2,P q1,P q2){
	return p1+(p2-p1)*( (q2-q1).det(q1-p1)/(q2-q1).det(p2-p1) );
}
//直線p1-p2でqに線対称な点
P line_symmetry(P p1,P p2,P q){
	double dx,dy;
	dx=p2.x-p1.x;
	dy=p2.y-p1.y;
	P inter=intersection(p1,p2,q,q+P(dy,-dx));
	return q+(inter-q)*2;
}

//線分p1-p2と線分q1-q2が交差するか
bool is_intersect(P p1,P p2,P q1,P q2){
	if(on_seg(p1,p2,q1)||on_seg(p1,p2,q2)||on_seg(q1,q2,p1)||on_seg(q1,q2,p2))return true;
	if((p1-p2).det(q1-q2)==0 )return false;//平行
	P a=intersection(p1,p2,q1,q2);
	return on_seg(p1,p2,a) && on_seg(q1,q2,a);
}
//原点中心回転
P rotate(P p,double rad){
	double s=sin(rad),c=cos(rad);
	return P(c*p.x-s*p.y, s*p.x+c*p.y);
}
//originを中心に回転
P rotate(P p,double rad,P origin){
	return origin + rotate(p - origin,rad);
}
//二点間の距離
double dist(P p,P q){
	p = p-q;
	return sqrt(p.x*p.x+p.y*p.y);
}
//単位ベクトルに変換
P to_unit(P p){
	double d = sqrt(p.x*p.x+p.y*p.y);
	return p/d;
}
//円同士の交点を計算
pair<P,P> circle_intersection(double x1,double y1,double r1,double x2,double y2,double r2){
	double rad = atan2(y2-y1,x2-x1);
	x2 -= x1;
	y2 -= y1;
	P tmp = rotate(P(x2,y2),-rad);
	x2 = tmp.x;
	y2 = tmp.y;
	double x = (r1*r1-r2*r2+x2*x2)/(2*x2);
	if(abs(r1) < abs(x))return make_pair(P(0,0),P(0,0));//交点なし
	double y = sqrt(r1*r1-x*x);
	return make_pair( P(x1,y1)+rotate(P(x,y),rad) ,  P(x1,y1)+rotate(P(x,-y),rad) );
}
//2点を通る半径rの円の中心座標
vector<P> circle_on_2P(P p,P q,double r){
	vector<P> res; 
	if(2*r+EPS < dist(p,q))return res;//解なし
	if(q.x < p.x)swap(p,q);
	q = q - p;
	double rad = atan2(q.y,q.x);
	P tmp = rotate(q,-rad);
	double x = tmp.x/2;
	double y = sqrt(4*r*r-tmp.x*tmp.x)/2;
	P a = p+rotate(P(x,y),rad) , b = p+rotate(P(x,-y),rad);
	res.push_back(a);
	if(!a.equal(b))res.push_back(b);
	return res;
}
//線分と点の距離
double dist_LP(P p1,P p2,P q){
	double dx,dy;
	dx = p1.x-p2.x;
	dy = p1.y-p2.y;
	double d = min(dist(p1,q) ,dist(p2,q));
	pair<P,P> normal = make_pair(q+to_unit(P(-dy,dx))*d ,q+to_unit(P(dy,-dx))*d);//法線ベクトル
	if(is_intersect(p1,p2,normal.first,normal.second)){
		return dist(q, intersection(p1,p2,normal.first,normal.second));
	}
	return d;
}
//線分間の距離 精度1e-5? EPSで結果変わる
double dist_LL(P p1,P p2,P q1,P q2){
	if(is_intersect(p1,p2,q1,q2))return 0;
	return min( min(dist_LP(p1,p2,q1),dist_LP(p1,p2,q2))
		,min(dist_LP(q1,q2,p1),dist_LP(q1,q2,p2)) );
}
//ラジアンに変換
double radian(double angle){
	return angle/180.0*M_PI;
}
//多角形内部に点pがあるか
bool contains(vector<P> &poly,P p){
	int f=0;
	for(int i=0;i<poly.size();i++){
		double d=(p-poly[i]).det(poly[i]-poly[(i+1)%poly.size()]);
		if(d<0)f|=1;
		else if(d>0)f|=2;
	}
	return f==(f&-f);
}
//ccw
int ccw(P a, P b, P c){
	b = b-a;
	c = c-a;
	if (b.det(c) > 0)   return +1;       // counter clockwise
	if (b.det(c) < 0)   return -1;       // clockwise
	if (b.dot(c) < 0)     return +2;       // c--a--b on line
	if (b.norm() < c.norm()) return -2;       // a--b--c on line
	return 0;
}
//凸包
bool cmp_x(const P& p,const P& q){
	if(p.x!=q.x)return p.x<q.x;
	return p.y<q.y;
}
vector<P> convex_hull(vector<P>& ps){
	if(ps.empty())return ps;
	sort(ps.begin(),ps.end(),cmp_x);
	int k=0;
	vector<P> qs(ps.size()*2);
	for(int i=0;i<ps.size();i++){
		while(k>1 && (qs[k-1]-qs[k-2]).det(ps[i]-qs[k-1]) <= 0 )k--;
		qs[k++]=ps[i];
	}
	for(int i=ps.size()-2,t=k;i>=0;i--){
		while(k>t && (qs[k-1]-qs[k-2]).det(ps[i]-qs[k-1]) <= 0 )k--;
		qs[k++]=ps[i];
	}
	qs.resize(k-1);
	return qs;
}

//直線p1-p2でqに線対称な点
P line_symmetry(P p1,P p2,P q){
	double dx,dy;
	dx=p2.x-p1.x;
	dy=p2.y-p1.y;
	P inter=intersection(p1,p2,q,q+P(dy,-dx));
	return q+(inter-q)*2;
}

/*----以下要検証-----*/
//多角形内部に点pがあるか  this is wrong
bool contains2(vector<P> &poly,P p,double inf=INF){
	int cnt=0;
	P q(p.x+inf,p.y);
	for(int i=0;i<poly.size();i++){
		if(is_intersect(poly[i],poly[(i+1)%poly.size()],p,q))cnt++;
	}
	return cnt&1;
}
//多角形の面積
double plain_area(vector<P> &poly){
	double s=0;
	for(int i=0;i<poly.size();i++){
		s += poly[i].det(poly[(i+1)%poly.size()]);
	}
	return abs(s/2);
}
//点pを通る円の接線
pair<P,P> tangent_CP(double x,double y,double r,P p){
	double d=sqrt((x-p.x)*(x-p.x)+(y-p.y)*(y-p.y))/2;
	return circle_intersection(x,y,r,(x+p.x)/2,(y+p.y)/2,d);
}
//円と長方形の重なり
bool overlap(P p,double w,double h,P c,double r){
	if(p.x<=c.x-r&&c.x+r<=p.x+w&&p.y<=c.y-r&&c.y+r<=p.y+h)return true;
	if(dist_LP(p,P(p.x,p.y+h),c)<r+EPS)return true;
	if(dist_LP(P(p.x,p.y+h),P(p.x+w,p.y+h),c)<r+EPS)return true;
	if(dist_LP(P(p.x+w,p.y+h),P(p.x+w,p.y),c)<r+EPS)return true;
	if(dist_LP(P(p.x+w,p.y),p,c)<r+EPS)return true;
	return false;
}
