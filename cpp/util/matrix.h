#include<cassert>
#include<algorithm>

#define MOD 1000000007



template<class T=long long>
class Matrix{
	int _row,_col;
	T* p;
public:
	Matrix():_row(0),_col(0),p(0){}
	Matrix(int row,int col):_row(row),_col(col){
		p=new T[_row*_col];
	}
	Matrix(const Matrix& a):_row(-1),p(0){
		copy(a);
	}
	~Matrix(){
		delete[] p;
	}
	int row()const{return _row;}
	int col()const{return _col;}
	void resize(int r,int c){
		if(_row*_col!=r*c){
			delete[] p;
			p=new T[r*c];		
		}
		_row=r;
		_col=c;
	}
	void swap(Matrix& a){
		std::swap(_row,a._row);
		std::swap(_col,a._col);
		std::swap(p,a.p);
	}
	void copy(const Matrix& a){
		resize(a._row,a._col);
		for(int i=0;i<row();i++)
			for(int j=0;j<col();j++)
				p[i*col()+j]=a[i][j];
	}
	T* operator[](int i)const{return p+(i*_col);}
	Matrix& operator =(const Matrix &a){
		copy(a);
		return *this;
	}
	Matrix& operator *=(const Matrix &a){
		(*this*a).swap(*this);
		return *this;
	}
	Matrix operator *(const Matrix &a)const{
		assert(col()==a.row());
		Matrix res(row(),a.col());
		for(int i=0;i<row();i++){
			for(int j=0;j<a.col();j++)res[i][j]=0;
			for(int k=0;k<col();k++){
				T da=p[i*col()+k];
				const T * const __restrict pb=a[k];
				T * const __restrict pc=res[i];
				for(int j=0;j<a.col();j++){
					pc[j] = (pc[j]+da*pb[j])%MOD;
				}
			}
		}
		return res;
	}
	Matrix pow(int n)const{
		assert(row()==col());
		int size=row();
		Matrix res(size,size),x(*this);
		for(int i=0;i<size;i++){
			for(int j=0;j<size;j++)res[i][j]=i==j;
		}
		while(n){
			if(n&1){
				(res*x).swap(res);
			}
			(x*x).swap(x);
			n>>=1;
		}
		return res;
	}
};

long long fibonacchi(int k){//k+1番目のフィボナッチ数
	Matrix<> mat(2,2),a(1,2);
	mat[0][0]=1;mat[0][1]=1;
	mat[1][0]=1;mat[1][1]=0;
	a[0][0]=1;a[0][1]=0;
	return (a*mat.pow(k))[0][0];
}
