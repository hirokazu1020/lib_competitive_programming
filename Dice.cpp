class Dice{
public:
	int top,front,r;
	Dice():top(1),front(2),r(3){}
	void north(){
		int a=top;
		top=front;
		front=7-a;
	}
	void east(){
		int a=top;
		top=7-r;
		r=a;
	}
	void west(){
		int a=top;
		top=r;
		r=7-a;
	}
	void south(){
		int a=top;
		top=7-front;
		front=a;
	}
	void right(){
		int a=front;
		front=r;
		r=7-a;
	}
	void left(){
		int a=front;
		front=7-r;
		r=a;
	}
};