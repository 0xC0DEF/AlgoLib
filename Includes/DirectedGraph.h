#pragma once
#include "Graph.h"

struct DirectedGraph: public Graph<int>{
	DirectedGraph(int n=0):Graph<int>(n) {}

	void add_edge(int s, int e, bool dir=true) {
		Graph::add_edge(s, e, 1, dir);
	}

	Arr<int> topo_sort() {
		Arr<int> in(n);
		hfor(i, 0, n){
			for(auto &j:g[i])
				in[j.e]++;
		}
		queue<int> q;
		hfor(i, 0, n)
			if (!in[i])
				q.push(i);
		Arr<int> ret;
		while (sz(q)) {
			auto cur = q.front();
			ret.pushb(cur);
			q.pop();
			for (auto &i : g[cur])
				if (--in[i.e] == 0)
					q.push(i.e);
		}
		return ret;
	}

	Arr<int> topo_sort_lex() {
		Arr<int> in(n);
		hfor(i, 0, n){
			for(auto &j:g[i])
				in[j.e]++;
		}
		PQ<int,Arr<int>,greater<int>> q;
		hfor(i, 0, n)
			if (!in[i])
				q.push(i);
		Arr<int> ret;
		while (sz(q)) {
			auto cur = q.top();
			ret.pushb(cur);
			q.pop();
			for (auto &i : g[cur])
				if (--in[i.e] == 0)
					q.push(i.e);
		}
		return ret;
	}

	pair<Arr<Arr<int>>, DirectedGraph> scc(){
		Arr<int> state(n), ord(n), scc_id(n, -1);
		stack<int> stk;
		int o=0, scc_cnt=0;
		hfor(i, 0, n){
			if(!state[i])
				dfs_scc(i, o, scc_cnt, state, ord, scc_id, stk);
		}

		auto group=Arr<Arr<int>>(scc_cnt);
		hfor(i, 0, n)
			group[scc_id[i]].pushb(i);
		DirectedGraph sccg(scc_cnt);
		hfor(i, 0, n){
			for(auto& j:g[i])
				if(scc_id[i]!=scc_id[j.e])
					sccg.add_edge(scc_id[i], scc_id[j.e]);
		}

		return {group, sccg};
	}

private:
	int dfs_scc(int v, int& o, int &scc_cnt, Arr<int>& state, Arr<int>& ord, Arr<int>& scc_id, stack<int>& stk){
		state[v]=1;
		stk.push(v);
		int ret = ord[v]=o++;
		for(auto i: g[v]){
			switch(state[i.e]){
				case 0:
					ret = min(ret, dfs_scc(i.e, o, scc_cnt, state, ord, scc_id, stk));
					break;
				case 1://back
					ret = min(ret, ord[i.e]);
					break;
				case 2:
					if(ord[i.e]>ord[v])//fwd
						;
					else if(ord[i.e]<ord[v]){//cross
						if(scc_id[i.e]==-1)
							ret = min(ret, ord[i.e]);
					}
					else//loop
						;
					break;
			}
		}
		if(ret == ord[v]){
			int cur;
			do{
				cur = stk.top();
				stk.pop();
				scc_id[cur]=scc_cnt;
			}while(sz(stk) && cur!=v);
			scc_cnt++;
		}

		state[v]=2;
		return ret;
	}
};