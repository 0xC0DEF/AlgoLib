#pragma once
#include "Core.h"

//Not Tested
template<typename T, typename U, T id=T()>
struct SegLazyDynPers{
	struct Node{
		Node *l=0, *r=0;
		T val=id;
		U lz=inf<U>();//lz_id=inf
	};
	SegLazyDynPers(int n=0):n(n){}
	
	T q(int p, Node* base){return q(base,p,p+1);}
	T q(int s, int e, Node* base){return q(base,0,n,s,e);}
	Node* upd(int p, U val, Node* base){return upd(p, p+1, val, base);}
	Node* upd(int s, int e, U val, Node* base/*=new Node()*/){
		Node* ret=new Node(*base);
		upd(ret,0,n,s,e,val,base);
		return ret;
	}
protected:
	int n;
	
	virtual T fq(const T& a, const T& b)=0;
	virtual T fupd(const T& a, const U& b, signed c)=0;
	virtual U fpropa(const U& a, const U& b)=0;

	T q(Node* cur, int cs, int ce, int s, int e){
		bool nl=false, nr=false;
		if(!cur->l) cur->l=new Node(), nl=true;
		if(!cur->r) cur->r=new Node(), nr=true;
		if(cur->lz!=inf<U>()){
			cur->val=fupd(cur->val,cur->lz,ce-cs);
			if(ce-cs>1){
				addlz(cur->l, cur->lz);
				addlz(cur->r, cur->lz);
			}
			cur->lz=inf<U>();
		}
	
		if (s>=ce||e<=cs){
			if(nl) delete cur->l, cur->l=nullptr;
			if(nr) delete cur->r, cur->r=nullptr;
			return id;
		}
		if (s<=cs&&ce<=e){
			if(nl) delete cur->l, cur->l=nullptr;
			if(nr) delete cur->r, cur->r=nullptr;
			return cur->val;
		}
		int m=(cs+ce)/2;
		T ret=fq(q(cur->l,cs,m,s,e),q(cur->r,m,ce,s,e));
		if(nl) delete cur->l, cur->l=nullptr;
		if(nr) delete cur->r, cur->r=nullptr;
		return ret;
	}

	void upd(Node* cur, int cs, int ce, int s, int e, U val, Node* base){
		propa(cur, cs, ce);
		propa(base, cs, ce);
		cur->l=new Node(*base->l);
		cur->r=new Node(*base->r);
		if (s>=ce||e<=cs)
			return;
		if (s<=cs&&ce<=e){
			addlz(cur, val);
			propa(cur, cs, ce);
			return;
		}
		int m=(cs+ce)/2;
		upd(cur->l,cs,m,s,e,val,base->l);
		upd(cur->r,m,ce,s,e,val,base->r);
		cur->val=fq(cur->l->val,cur->r->val);
	}
	
	void propa(Node* cur, int cs, int ce){
		if(!cur->l)	cur->l=new Node();
		if(!cur->r) cur->r=new Node();
		if(cur->lz!=inf<U>()){
			cur->val=fupd(cur->val,cur->lz,ce-cs);
			if(ce-cs>1){
				addlz(cur->l, cur->lz);
				addlz(cur->r, cur->lz);
			}
			cur->lz=inf<U>();
		}
	}
	
	void addlz(Node* v, U val){v->lz=v->lz==inf<U>()?val:fpropa(v->lz,val);}
};

template<typename T, typename U>
struct SegLazyDynPersSumAdd:public SegLazyDynPers<T,U>{
	SegLazyDynPersSumAdd(int n=0):SegLazyDynPers<T,U>(n){}
	T fq(const T& a, const T& b)override{return a+b;}
	T fupd(const T& a, const U& b, signed c)override{return a+b*c;}
	U fpropa(const U& a, const U& b)override{return a+b;}
};
template<typename T, typename U>
struct SegLazyDynPersMaxAdd:public SegLazyDynPers<T,U,-inf<T>()>{
	SegLazyDynPersMaxAdd(int n=0):SegLazyDynPers<T,U,-inf<T>()>(n){}
	T fq(const T& a, const T& b)override{return max(a,b);}
	T fupd(const T& a, const U& b, signed c)override{return a+b;}
	U fpropa(const U& a, const U& b)override{return a+b;}
};
template<typename T, typename U>
struct SegLazyDynPersMinAss:public SegLazyDynPers<T,U,inf<T>()>{
	SegLazyDynPersMinAss(int n=0):SegLazyDynPers<T,U,inf<T>()>(n){}
	T fq(const T& a, const T& b)override{return min(a,b);}
	T fupd(const T& a, const U& b, signed c)override{return b;}
	U fpropa(const U& a, const U& b)override{return b;}
};