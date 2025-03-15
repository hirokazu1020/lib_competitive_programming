//二部マッチング O(VE)
#define N 500
vector<vector<int> > edge,edge_rev;//正辺,逆辺
namespace Match{
	bool match[N];
	bool flow[N][N];
	bool visit[2][N];
	bool g(int);
	bool f(int i){
		if(visit[0][i])return false;
		visit[0][i]=true;
		for(int j=0;j<edge[i].size();j++){
			int k = edge[i][j];
			if(!flow[i][k] && g(k)){
				flow[i][k]=true;
				return true;
			}
		}
		return false;
	}
	bool g(int i){
		if(visit[1][i])return false;
		visit[1][i]=true;
		if(!match[i]){
			match[i]=true;
			return true;
		}
		for(int j=0;j<edge_rev[i].size();j++){
			int k = edge_rev[i][j];
			if(flow[k][i] && f(k)){
				flow[k][i]=false;
				return true;
			}	
		}
		return false;
	}
};
int bipartite_matching(){
	memset(Match::match,false,sizeof(Match::match));
	memset(Match::flow,false,sizeof(Match::flow));
	int res=0;
	for(int i=0;i<edge.size();i++){
		memset(Match::visit,false,sizeof(Match::visit));	
		if(Match::f(i))res++;
	}
	return res;
}