char *week[]={"Sunday","Monday","Tuesday","Wednesday","Thursday","Friday","Saturday"};
int zellar(int y,int m,int d){
	if(m<3){y--;m+=12;}
	return (y+y/4-y/100+y/400+(13*m+8)/5+d)%7;
}



//verified AOJ0125
int days[12]={31,28,31,30,31,30,31,31,30,31,30,31};
int calc(int y,int m,int d){
	int res=d-1;
	if(2<m && y%4==0&&(y%100||y%400==0))res++;
	for(int i=0;i<m-1;i++)res+=days[i];
	y--;
	res+=y/400*146097;
	y%=400;
	res+=y/100*36524;
	y%=100;
	res+=y/4*1461;
	y%=4;
	res+=y*365;
	return res;
}
int calc(int y,int m,int d){//1”N1Œ1“ú‚©‚ç‚ÌŒo‰ß“ú”
	int res=0;
	for(int i=1;i<y;i++){
		if(i%4==0&&(i%100||i%400==0))res+=366;
		else res += 365;
	}
	for(int i=1;i<m;i++){
		res += days[i-1];	
	}
	if(2<m && y%4==0&&(y%100||y%400==0))res++;
	return res+d-1;
}



bool isLeapYear(int y){
	return y%4==0 && (y%100 || y%400==0);
}