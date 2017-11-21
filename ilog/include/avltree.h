
#ifndef _AVLTREE_H_
#define _AVLTREE_H_

#include <string>   //C++标准库文件
#include <memory>
#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <fnmatch.h>
#include <signal.h>
#include <ctype.h>	
#include <errno.h>
#include <pthread.h>

using namespace std;

#define INSERT_FOR_SET  1
#define INSERT_FOR_ADD  2


// 链表定义，类似于vector   
template<class T>
class vector2
{
public:
	vector2();
	virtual ~vector2();
	void insert(T & t);
	void getnode(T & t);
	vector2<T> & operator=(vector2<T> &);
	T * begin();
	T * next();
	void clear();
	struct typeLinkNode
	{
		T t;
		typeLinkNode * p;
	};
	typeLinkNode * start();
	private:
		typeLinkNode * _pt;   // 起始位置指针
		typeLinkNode * _st;   // 用户遍历vector的指针
		typeLinkNode * * _pw; // 指向尾节点地址的指针 
		int num;
};

// 链表定义，类似于vector   
template<class T>
class vector2p
{
public:
	vector2p();
	virtual ~vector2p();
	void insert(T * t);
	void getnode(T * t);
	vector2p<T> & operator=(vector2p<T> &);
	T * begin();
	T * next();
	void clear();
	struct typeLinkNode
	{
		T *t;
		typeLinkNode * p;
	};
	typeLinkNode * start();
	private:
		typeLinkNode * _pt;   // 起始位置指针
		typeLinkNode * _st;   // 用户遍历vector的指针
		typeLinkNode * * _pw; // 指向尾节点地址的指针 
		int num;
};

// 链表定义，类似于deque   
template<class T>
class deque2
{
public:
	deque2(int nummax);
	deque2();
	~deque2();
	struct typeDequeNode
	{
		T *t;
		typeDequeNode * bp;
		typeDequeNode * ap;
	};
	int push_first(T *t);
	int push_top(T *t);
	int setMaxNum(int nummax);
	//liufei add[20100331]
	void setDelayTime(int ntime);

	T * pop();
	deque2<T> & operator=(deque2<T> &);
	T * begin();
	T * next();
	void clear();
	int sum();	
	typeDequeNode * start();
	int isfullfill(); /*0:不满，1：满*/
	int max;
	private:
		typeDequeNode * _pt;   // 起始位置指针
		typeDequeNode * _st;   // 用户遍历deque2的指针
		typeDequeNode * _lp;   // 指向尾节点的指针
		typeDequeNode * * _pw; // 指向尾节点指针的地址的指针 
		int num;
		//int max;
		pthread_mutex_t  mutex,push_lock,pop_lock,proc_lock;//wucj add lock
		pthread_cond_t condIn;
		pthread_cond_t condOut;

		int delaytime;
		bool popModule; //pop 队列节点方式: PopModule->false 队列为空阻塞等待; PopModule->true 队列为空超时等待;
};


// 二叉树一个节点的定义
template <class K,class T>
class binTreeNode
{
public:
	binTreeNode();
	binTreeNode(K k,T & t);
	binTreeNode(K k,T & t,binTreeNode<K,T> * p);
public: 
	binTreeNode<K,T> *pl, *pr, *pp; // 左节点，右节点，父节点
	int bf;                         // 平衡因子
	K _k;                           // 排序得Key值
	T _t;                           // 数据
};

// 平衡二叉树定义
template <class K,class T>
class avlTree
{
public:
	avlTree();
	avlTree(K k,T & t);
	~avlTree();
	T * search(K k);
	T * search(K k,T & t,int flag);	
	int insert(K k,T & t);	
	int insert(K k,T & t,int flag);
	int update(K k,T & t,int flag = 0);
	int delNode(K k);		
	void clear();
	binTreeNode<K,T> * begin();
	binTreeNode<K,T> * next();	
	avlTree<K,T> & operator=(avlTree<K,T> &);
protected:
	binTreeNode<K,T> * root;
	binTreeNode<K,T> * pc;	
private:
	void delTree(binTreeNode<K,T> * p);
	void lBalance(binTreeNode<K,T> * & p);
	void rBalance(binTreeNode<K,T> * & p);
	void lRotate(binTreeNode<K,T> * & p);
	void rRotate(binTreeNode<K,T> * & p);
};

template <class K,class T,int num>
class hashTable
{
public:
	hashTable();
	virtual ~hashTable();
	//void insert(T t);
	void insert(K k,T & t);
	T * search(K k);
	binTreeNode<K,T>  * begin();
	binTreeNode<K,T>  * next();
	void clear(); //wucj add 20090518
private:
	//map<string,T> * _t;
	avlTree<K,T> *   _t;
	int pos;
};


/*
** 解决 string 类型比较占用空间问题
** 自定义 string 类型，用户储存帐单和总量
*/
template<int len>
class cString
{
public:
	cString();
	cString(char *);
	virtual ~cString();
	cString & operator = (const cString &);
	int operator > (cString & other);
	int operator < (cString & other);
	int operator == (cString & other);	
	char _s[len+1];
};

/*
** 解决 map2 类型比较占用空间问题
** 自定义 map2
*/
template<class K,class T>
class map2
{
public:
	map2();
	//map2(int nummax);
	~map2();
	struct typeMapNode
	{
		K k;
		T *t;
		typeMapNode * p;
	};
	int insert(K k,T *t);
	map2<K,T> & operator=(map2<K,T> &);	
	T* search(K k);
	T* delnode(K k);
	T* begin();
	T* next();
	void clear();
	
	
	typeMapNode * start();
	int size();
	private:
		int  num;
		int max;
		typeMapNode * _pt;   // 起始位置指针
		typeMapNode * _st;   // 用户遍历vector的指针
		typeMapNode * * _pw; // 指向尾节点地址的指针 
		pthread_mutex_t  mutex;
		//test
		int count;
		int pos;
};


/* 哈希表与map2的结合*/
template <class K,class T,int num>
class hashLink
{
public:
	hashLink();
	~hashLink();
	void insert(K k,T *t);
	T * search(K k);
	T * delNode(K k);
	int size();
	T  * begin();
	T  * next();	
	int clear();
private:
	map2<K,T> *   _t;
	int count;
	int pos;
};

template<class K,class T>
class list2
{
public:
	list2();
	~list2();
	void insert(K k,T *t,int sortFlag=0);//默认升序
	void deleteNode(T &t);
	void prnObj();
	void deleteNodes(K k,int );
	T* search(T &t);
	struct typeListNode
	{
		K k;
		T *t;
		typeListNode * next;
	};
private:
	typeListNode m_root;   // 起始位置指针
	pthread_mutex_t  m_mutex;
};



// 核心数据结构定义          
// 链表定义，类似于vector    
// 构造函数
template<class T>
vector2<T>::vector2()
{
	_pt = NULL;	
	_st = NULL;
	_pw = &_pt;
	num=0;
}

// 析构函数
template<class T>
vector2<T>::~vector2()
{
	typeLinkNode * lp;
	typeLinkNode * tp;
	for(tp=_pt;tp!=NULL;){
		lp = tp->p;
		delete tp;
		tp = lp;
	}
}

// 清空函数
template<class T>
void vector2<T>::clear()
{
	typeLinkNode * lp;
	typeLinkNode * tp;
	for(tp=_pt;tp!=NULL;){
		lp = tp->p;
		delete tp;
		tp = lp;
	}
	
	_pt = NULL;	
	_st = NULL;
	_pw = &_pt;
	num=0;
}

// 插入函数
template<class T>
void vector2<T>::insert(T & t)
{	
	(*_pw) = new typeLinkNode;
	(*_pw)->t = t;
	(*_pw)->p = NULL;
	_pw = &((*_pw)->p);
	num++;
}

// 定位于链表起始节点函数
template<class T>
T * vector2<T>::begin()
{
	_st = _pt;
	if( _st == NULL ) return NULL;
	else return &_st->t;
}

// 定位于链表起始节点函数
template<class T>
struct vector2<T>::typeLinkNode * vector2<T>::start()
{
	return _pt;
}

// 定位于链表下一节点函数
template<class T>
T * vector2<T>::next()
{
	_st = _st->p;
	if( _st == NULL ) return NULL;
	else return &_st->t;
}

template <class T>
vector2<T> & vector2<T>::operator=(vector2<T> &other)
{
	if(this == &other) return *this;
	
	clear();
	
	T * tp;
	for(tp=other.begin();tp!=NULL;tp=other.next())
	{
		this->insert(*tp);
	}
	
	return *this;
}


// 核心数据结构定义          
// 链表定义，类似于vector    
// 构造函数
template<class T>
vector2p<T>::vector2p()
{
	_pt = NULL;	
	_st = NULL;
	_pw = &_pt;
	num=0;
}

// 析构函数
template<class T>
vector2p<T>::~vector2p()
{
	typeLinkNode * lp;
	typeLinkNode * tp;
	for(tp=_pt;tp!=NULL;){
		lp = tp->p;
		delete tp;
		tp = lp;
	}
}

// 清空函数
template<class T>
void vector2p<T>::clear()
{
	typeLinkNode * lp;
	typeLinkNode * tp;
	for(tp=_pt;tp!=NULL;){
		lp = tp->p;
		delete tp;
		tp = lp;
	}
	
	_pt = NULL;	
	_st = NULL;
	_pw = &_pt;
	num=0;
}

// 插入函数
template<class T>
void vector2p<T>::insert(T * t)
{	
	(*_pw) = new typeLinkNode;
	(*_pw)->t = t;
	(*_pw)->p = NULL;
	_pw = &((*_pw)->p);
	num++;
}

// 定位于链表起始节点函数
template<class T>
T * vector2p<T>::begin()
{
	_st = _pt;
	if( _st == NULL ) return NULL;
	else return _st->t;
}

// 定位于链表起始节点函数
template<class T>
struct vector2p<T>::typeLinkNode * vector2p<T>::start()
{
	return _pt;
}

// 定位于链表下一节点函数
template<class T>
T * vector2p<T>::next()
{
	_st = _st->p;
	if( _st == NULL ) return NULL;
	else return _st->t;
}

template <class T>
vector2p<T> & vector2p<T>::operator=(vector2p<T> &other)
{
	if(this == &other) return *this;
	
	clear();
	
	T * tp;
	for(tp=other.begin();tp!=NULL;tp=other.next())
	{
		this->insert(tp);
	}
	
	return *this;
}
// 一个二叉树节点定义
template <class K,class T>
binTreeNode<K,T>::binTreeNode()
{
	pl = NULL;
	pr = NULL;
	pp = NULL;
	bf = 0;
}

template <class K,class T>
binTreeNode<K,T>::binTreeNode(K k,T & t)
{
	_k = k;
	_t = t;
	bf = 0;
	pl = NULL;
	pr = NULL;
	pp = NULL;
}

template <class K,class T>	
binTreeNode<K,T>::binTreeNode(K k,T & t,binTreeNode<K,T> * p)
{
	_k = k;
	_t = t;
	bf = 0;
	pl = NULL;
	pr = NULL;
	pp = p;
}

/* AVL二叉树定义  */
// 构造函数
template<class K,class T>
avlTree<K,T>::avlTree()
{
	root = NULL;
	pc   = NULL;
}

template<class K,class T>
avlTree<K,T>::avlTree(K k,T & t)
{
	pc = NULL;
	root = new binTreeNode<K,T>(k,t);
}

// 析构函数
template<class K,class T>
avlTree<K,T>::~avlTree()
{
	delTree(root);
	root = NULL;
}

// 清空函数
template<class K,class T>
void avlTree<K,T>::clear()
{
	delTree(root);
	root = NULL;
}

// 插入函数
template<class K,class T>
int avlTree<K,T>::insert(K k,T & t)
{
	binTreeNode<K,T> * * p;
	binTreeNode<K,T> * lp;						
	K * pk;	
	pc = NULL;
	for(p=&root;*p!=NULL;){
		if( k < (*p)->_k){
			pc = *p;
			p = &((*p)->pl);
		} 
		else if( k > (*p)->_k){
			pc = *p;
			p = &((*p)->pr);
		}
		else return 0;
	}		
	*p = new binTreeNode<K,T>(k,t,pc);
	
	// 旋转
	pk  = &k;	          
	while ( pc != NULL){						
		pc->bf += (pc->_k > *pk )?1:-1;
		
		//printf("insert %d tp->bf is %d,tp->k is %d\n",k,pc->bf,pc->_k);
		
		if (pc->bf == -2) lBalance((pc->pp?((pc->pp->pl==pc)?pc->pp->pl:pc->pp->pr):root));
		else if (pc->bf == 2) rBalance((pc->pp?((pc->pp->pl == pc)?pc->pp->pl:pc->pp->pr):root));
		
		//printf("after turn current is %d,bf is %d\n",pc->_k,pc->bf);
		if ( pc->bf == 0 ) break;
		pk = &(pc->_k);
		pc = pc->pp;
	}		
	return 1;
}

/*删除一个结点*/
template<class K,class T>
int avlTree<K,T>::delNode(K k)
{
	binTreeNode<K,T>  ** p;
	binTreeNode<K,T>  * tp;
	binTreeNode<K,T>  * pc;
	binTreeNode<K,T>  * tpl;
	binTreeNode<K,T>  * tpr;
	binTreeNode<K,T>  * fp;
	K *pk;
	
	for(p=&root;(*p)!=NULL;){
		if( k < (*p)->_k  )
		{
			fp = *p;
			p = &((*p)->pl);
		}
		else if( k > (*p)->_k  ) 
		{
			fp = *p;
			p = &((*p)->pr);
		}
		else
		{
			tp = (*p); // 查找到
			break;
		}
	}
	
	
	pc = tp;  
	// 旋转
	pk  = &k;	          
	while ( pc != NULL){						
		pc->bf += (pc->_k > *pk )?1:-1;
		
		//printf("insert %d tp->bf is %d,tp->k is %d\n",k,pc->bf,pc->_k);
		
		if (pc->bf == -2) lBalance((pc->pp?((pc->pp->pl==pc)?pc->pp->pl:pc->pp->pr):root));
		else if (pc->bf == 2) rBalance((pc->pp?((pc->pp->pl == pc)?pc->pp->pl:pc->pp->pr):root));
		
		//printf("after turn current is %d,bf is %d\n",pc->_k,pc->bf);
		if ( pc->bf == 0 ) break;
		pk = &(pc->_k);
		pc = pc->pp;
	}		
	delete tp;
	return 0;
}


// 插入函数
template<class K,class T>
int avlTree<K,T>::insert(K k,T & t,int flag)
{
	binTreeNode<K,T> * * p;
	binTreeNode<K,T> * lp;						
	K * pk;	
	pc = NULL;
	for(p=&root;*p!=NULL;){
		if( k < (*p)->_k){
			pc = *p;
			p = &((*p)->pl);
		} 
		else if( k > (*p)->_k){
			pc = *p;
			p = &((*p)->pr);
		}
		else {
			if( flag == INSERT_FOR_SET ) (*p)->_t =  t;
			if( flag == INSERT_FOR_ADD ) (*p)->_t += t; 
			return 0;
		}
	}		
	*p = new binTreeNode<K,T>(k,t,pc);
	
	// 旋转
	pk  = &k;	          
	while ( pc != NULL){						
		pc->bf += (pc->_k > *pk )?1:-1;
		
		//printf("insert %d tp->bf is %d,tp->k is %d\n",k,pc->bf,pc->_k);
		
		if (pc->bf == -2) lBalance((pc->pp?((pc->pp->pl==pc)?pc->pp->pl:pc->pp->pr):root));
		else if (pc->bf == 2) rBalance((pc->pp?((pc->pp->pl == pc)?pc->pp->pl:pc->pp->pr):root));
		
		//printf("after turn current is %d,bf is %d\n",pc->_k,pc->bf);
		if ( pc->bf == 0 ) break;
		pk = &(pc->_k);
		pc = pc->pp;
	}		
	return 1;
}

// 查找函数
template<class K,class T>
T  *avlTree<K,T>::search(K k)
{
	binTreeNode<K,T> * * p;
	
	for(p=&root;(*p)!=NULL;){
		if( k < (*p)->_k  ) p = &((*p)->pl);
		else if( k > (*p)->_k  ) p = &((*p)->pr);
		else return &((*p)->_t); // 查找到
	}
	
	return NULL;
}  

// 查找函数,flag = 0,只是查找,返回查找节点;
// flag = 1,如果查找不到，创建它并且插入
template<class K,class T>
T * avlTree<K,T>::search(K k,T & t,int flag)
{
	binTreeNode<K,T> * * p;
	int Oper;
	
	Oper  = 0;
	for(p=&root;(*p)!=NULL;){
		if( k < (*p)->_k  ) p = &((*p)->pl);
		else if( k > (*p)->_k  ) p = &((*p)->pr);
		else {
			Oper=1;
			break;
		}
	}
	
	if(Oper == 1)	 return &((*p)->_t);
	if(flag == 1)  insert(k,t);
	
	return NULL;
}  

// 更新节点函数,成功返回1，否则返回0
template<class K,class T>
int avlTree<K,T>::update(K k,T & t,int flag)
{
	binTreeNode<K,T> * * p;
	int Oper;
	
	Oper  = 0;
	for(p=&root;(*p)!=NULL;){
		if( k < (*p)->_k  ) p = &((*p)->pl);
		else if( k > (*p)->_k  ) p = &((*p)->pr);
		else{
			Oper=1; // 查找到
			break;
		}
	}
	
	if(Oper == 1){
		if(flag ==1)
		{
			delete (*p)->_t;
		}
		(*p)->_t = t;
		return 1;
	}
	
	return 0; 
} 

// 遍历函数
template<class K,class T>
binTreeNode<K,T> * avlTree<K,T>::begin()
{
	if( root == NULL ) return NULL;
	pc = root; 
	while (pc->pl != NULL) pc = pc->pl;
	return  pc;
}

// 取下一个节点地址
template<class K,class T>
binTreeNode<K,T> * avlTree<K,T>::next()
{
	if( pc == NULL) return NULL;
	if (pc->pr != NULL){
		pc = pc->pr;
		while (pc->pl != NULL) pc = pc->pl;
	}
	else{
		binTreeNode<K,T> * py = pc->pp;
		while (py  != NULL && pc == py->pr){
			pc = py; 
			py = py->pp; 
		}
		pc = py;
	}
	return pc;
}

// 删除一个树得成员函数
template<class K,class T>
void avlTree<K,T>::delTree(binTreeNode<K,T> * p)
{
	if (p != NULL){
		delTree(p->pl);
		delTree(p->pr);
		delete p;
	}
}

// 左平衡
template<class K,class T>
void avlTree<K,T>::lBalance(binTreeNode<K,T> * & p)
{
	if (p->pr->bf == 1) rRotate(p->pr);
	lRotate(p); 
	pc = p;
}

// 右平衡
template<class K,class T>
void avlTree<K,T>::rBalance(binTreeNode<K,T> * & p)
{
	if (p->pl->bf == -1) lRotate(p->pl);
	rRotate(p); 
	pc = p;
}

// 左旋转
template<class K,class T>      
void avlTree<K,T>::lRotate(binTreeNode<K,T> * & p)
{
	binTreeNode<K,T> * tp = p->pr;
	tp->pp = p->pp;
	p->pp  = tp; 
	p->pr  = tp->pl;
	if (tp->pl != NULL ) tp->pl->pp = p;
	tp->pl = p;
	++(p->bf)  -= tp->bf < 0 ? tp->bf:0; 
	++(tp->bf) += p->bf > 0 ? p->bf:0;
	p = tp;
}

// 右旋转  
template<class K,class T> 
void avlTree<K,T>::rRotate(binTreeNode<K,T> * & p)
{
	binTreeNode<K,T> * tp = p->pl;
	tp->pp = p->pp;
	p->pp = tp;
	p->pl = tp->pr;
	if (tp->pr != NULL) tp->pr->pp = p;
	tp->pr = p;
	--(p->bf) -= tp->bf > 0?tp->bf:0;
	--(tp->bf) += p->bf < 0?p->bf:0;
	p = tp;
}

// avlTree 赋值
template<class K,class T> 
avlTree<K,T> & avlTree<K,T>::operator=(avlTree<K,T> & other)
{
	if(this == &other) return *this;
	
	clear();
	
	binTreeNode<K,T> * tp;
	for(tp=other.begin();tp!=NULL;tp=other.next()){
		this->insert(tp->_k,tp->_t);
	}	
	
	return *this;
}



/* 哈希表与binTree的结合         */
// 构造函数
template<class K,class T,int num>
hashTable<K,T,num>::hashTable()
{
	_t = new avlTree<K,T>[num];
};

// 析构函数
template<class K,class T,int num>
hashTable<K,T,num>::~hashTable()
{
	if(_t != NULL) delete [] _t;
};

// 插入函数
template <class K,class T,int num> 
void hashTable<K,T,num>::insert(K k,T & t)
{
	int hashKey;
	
	/* 根据类型T定义的成员函数计算哈希值 */
	hashKey = k.getHashKey() % num;
	
	if( hashKey < 0 ) hashKey = 0;
	
	/* 在哈希堆对应binTree里插入         */	
	_t[hashKey].insert(k,t);
};

// 查找函数
template <class K,class T,int num> 
T * hashTable<K,T,num>::search(K k)
{
	int hashKey;
	
	/* 根据类型T定义的静态函数计算哈希值 */
	hashKey = k.getHashKey() % num;
	
	/* 在binTree里查找key对应位置        */	
	return _t[hashKey].search(k);
};

// 哈希表遍历函数                    
template <class K,class T,int num> 
binTreeNode<K,T> * hashTable<K,T,num>::begin()
{
	binTreeNode<K,T> * p;		
	pos = 0;
	for(pos=0;pos<num;pos++){
		if( (p = _t[pos].begin()) != NULL ) return p;
	}
	return NULL;
}

// 哈希表遍历函数                    
template <class K,class T,int num> 
binTreeNode<K,T> * hashTable<K,T,num>::next()
{
	binTreeNode<K,T> * p;
	if( (p=_t[pos].next()) != NULL ){
		return p;
	}
	else {
		while( ++pos < num ){
			if( (p = _t[pos].begin() ) != NULL ) return p;
		}
	}
	return NULL;
}

//wucj add 20090518
template<class K,class T,int num>
void hashTable<K,T,num>::clear()
{
	if(_t != NULL)
	{
		delete [] _t;
	} 
	_t = NULL;
	_t = new avlTree<K,T>[num];
};


/*
** 解决 string 类型比较占用空间问题
** 自定义 string 类型，用户储存帐单和总量
*/
// 构造函数
template<int len>
cString<len>::cString()
{
	memset(_s,0,sizeof(_s));
}

// 构造
template<int len>
cString<len>::cString(char * s)
{
	if( strlen(s) > len ) memcpy(_s,s,len);
	else strcpy(_s,s);
	_s[len] = '\0';
}

// 析构函数
template<int len>
cString<len>::~cString()
{
}

// 赋值操作符
template<int len>
cString<len> & cString<len>::operator = (const cString<len> & other)
{
	if( this == &other ) return *this;
	memset(_s,0,sizeof(_s));
	if( strlen(other._s) > len ) memcpy(_s,other._s,len);
	else strcpy(_s,other._s);
	_s[len] = '\0';
	return *this;
}

// 比较大小函数
template<int len>
int cString<len>::operator > (cString<len> & other)
{
	if( strcmp(_s,other._s) > 0 ) return 1;
	else return 0;
}

template<int len>
int cString<len>::operator < (cString<len> & other)
{
	if( strcmp(_s,other._s) < 0 ) return 1;
	else return 0;
}

template<int len>
int cString<len>::operator == (cString<len> & other)
{
	if( strcmp(_s,other._s) == 0 ) return 1;
	else return 0;
}




// 核心数据结构定义          
// 链表定义，类似于map
// 构造函数
template<class K,class T>
map2<K,T>::map2()
{
	_pt = NULL;	
	_st = NULL;
	_pw = &_pt;
	num =0;
	count = 0;
	pos = 0;
	max = 0;
	pthread_mutex_init(&mutex,NULL);
	return ;
}
/*
template<class K,class T>
map2<K,T>::map2(int nummax)
{
_pt = NULL;	
_st = NULL;
_pw = &_pt;
num =0;
max=nummax;
return ;
}
*/

// 析构函数
template<class K,class T>
map2<K,T>::~map2()
{
	typeMapNode * lp;
	typeMapNode * tp;
	for(tp=_pt;tp!=NULL;)
	{
		lp = tp->p;
		delete tp;
		tp = lp;
	}
	pthread_mutex_destroy(&mutex);
	return ;
}
/*
template<class K,class T>
map2<K,T>::init(int nummax)
{
_pt = NULL;	
_st = NULL;
_pw = &_pt;
num =0;
max= nummax;
return ;
}
*/
// 清空函数
template<class K,class T>
void map2<K,T>::clear()
{
	typeMapNode * lp;
	typeMapNode * tp;
	for(tp=_pt;tp!=NULL;)
	{
		lp = tp->p;
		delete tp;
		tp = lp;
	}
	
	_pt = NULL;	
	_st = NULL;
	_pw = &_pt;
	num =0;
	return ;
}

// 插入函数
template<class K,class T>
int map2<K,T>::insert(K k,T *t)
{
#ifdef DEBUG		
	printf("map2<K,T>::insert size()=[%d]\n",num);
#endif
	pthread_mutex_lock(&mutex);		
	(*_pw) = new typeMapNode;
	(*_pw)->k = k;
	(*_pw)->t = t;
	(*_pw)->p = NULL;
	_pw = &((*_pw)->p);
	num++;
	
	pthread_mutex_unlock(&mutex);
#ifdef DEBUG
	printf("map2<K,T>::insert 22 size()=[%d]\n",num);
#endif
	return 0;
}

// 插入函数
template<class K,class T>
T *map2<K,T>::search(K  k)
{	
	pthread_mutex_lock(&mutex);	
	typeMapNode * ptmp=NULL;
	//printf("map2<K,T>::search 11 size()=[%d] thread_id=[%d]\n",num,pthread_self());
	
	for(ptmp=_pt;ptmp!=NULL;ptmp = ptmp->p)
	{
		

		if(ptmp->k ==k)
		{
			
			pthread_mutex_unlock(&mutex);
			return ptmp->t;
		}
	} 
	pthread_mutex_unlock(&mutex);
	return NULL;   
}

// 插入函数
template<class K,class T>
T *map2<K,T>::delnode(K  k)
{	
	T *pt=NULL;
	typeMapNode * ptmp=NULL;
	typeMapNode * pp=NULL;
	int flag;
	pthread_mutex_lock(&mutex);	  
	for(ptmp=_pt;ptmp!=NULL;ptmp = ptmp->p)
	{
		if(ptmp->k ==k)
		{	
			break;
		}
		pp = ptmp;
	} 
	if(ptmp != NULL  && pp != NULL && ptmp->p == NULL)
	{
		pp->p=ptmp->p;
		if(ptmp->t != NULL)
		{
			pt =ptmp->t;
			ptmp->t=NULL;
		}
		delete ptmp;
		ptmp = NULL;
		
		pp->p=NULL;
		_pw = &(pp->p);
		num--; 	
	}
	else if(ptmp != NULL && pp != NULL)
	{
		pp->p=ptmp->p;
		if(ptmp->t != NULL)
		{
			pt =ptmp->t;
			ptmp->t=NULL;
		}
		delete ptmp;
		ptmp = NULL;
		num--; 
	}
	else if(ptmp != NULL)
	{
		if(ptmp->t != NULL)
			pt = ptmp->t;
		
		_pt= ptmp->p;
		delete ptmp;
		num--; 
	}
	if(num ==0)
	{
		_pt = NULL;	
		_st = NULL;
		_pw = &_pt;	
	}	    
	pthread_mutex_unlock(&mutex);
	return pt;
}

template<class K,class T>
int map2<K,T>::size()
{	
	return num;
}

// 定位于链表起始节点函数
template<class K,class T>
T *map2<K,T>::begin()
{
	_st = _pt;
	if( _st == NULL ) 
		return NULL;
	else 
		return _st->t;
}

// 定位于链表起始节点函数
template<class K,class T>
struct map2<K,T>::typeMapNode * map2<K,T>::start()
{
	return _pt;
}

// 定位于链表下一节点函数
template<class K,class T>
T  *map2<K,T>::next()
{
	if( _st == NULL ) 
		return NULL;
	_st = _st->p;
	if( _st == NULL ) 
		return NULL;
	else 
		return _st->t;
}

template<class K,class T>
map2<K,T> & map2<K,T>::operator=(map2<K,T> &other)
{
	if(this == &other) return *this;
	
	clear();
	
	T * tp;
	for(tp=other.begin();tp!=NULL;tp=other.next())
	{
		this->insert(*tp);
	}
	
	return *this;
}


// 核心数据结构定义          
// 链表定义，类似于deque
// 构造函数
template<class T>//wucj add lock
deque2<T>::deque2(int nummax)
{
	_pt = NULL;	
	_st = NULL;
	_lp = NULL;	
	_pw = &_pt;
	num =0;
	max =nummax;

	//liufei add[20100331]
	popModule = false;
	delaytime = 3; 

	pthread_mutex_init(&mutex,NULL);
	pthread_mutex_init(&push_lock,NULL);
	pthread_mutex_init(&pop_lock,NULL);
	pthread_mutex_init(&proc_lock,NULL);
	pthread_cond_init(&condIn,NULL);
	pthread_cond_init(&condOut,NULL);
}
template<class T>//wucj add lock
deque2<T>::deque2()
{
	_pt = NULL;	
	_st = NULL;
	_lp = NULL;	
	_pw = &_pt;
	num =0;
	max =500;
	//liufei add[20100331]
	popModule = false;
	delaytime = 3;

	pthread_mutex_init(&mutex,NULL);
	pthread_mutex_init(&push_lock,NULL);
	pthread_mutex_init(&pop_lock,NULL);
	pthread_mutex_init(&proc_lock,NULL);
	pthread_cond_init(&condIn,NULL);
	pthread_cond_init(&condOut,NULL);
	
	//printf(" in deque2() max=%d\n",max);
}

// 析构函数
template<class T>//wucj add lock
deque2<T>::~deque2()
{
	typeDequeNode * lp;
	typeDequeNode * tp;
	for(tp=_pt;tp!=NULL;){
		lp = tp->ap;
		delete tp->t;
		delete tp;
		tp = lp;
	}
	pthread_mutex_destroy(&mutex);
	pthread_mutex_destroy(&push_lock);
	pthread_mutex_destroy(&pop_lock);
	pthread_mutex_destroy(&proc_lock);
	pthread_cond_destroy(&condIn);
	pthread_cond_destroy(&condOut);
}

template<class T>
int deque2<T>::setMaxNum(int nummax)
{
	max = nummax;
	return 0;
}

template<class T>
int deque2<T>::isfullfill()
{
	if(num >= max)
		return 1;
	return 0;
}

// 清空函数
template<class T>//wucj add lock
void deque2<T>::clear()
{
	pthread_mutex_lock(&proc_lock);
	typeDequeNode * lp;
	typeDequeNode * tp;
	for(tp=_pt;tp!=NULL;){
		lp = tp->ap;
		delete tp->t;//wucj modify 内存泄露
		delete tp;
		tp = lp;
	}
	
	_pt = NULL;	
	_st = NULL;
	_pw = &_pt;
	_lp = NULL;
	num =0;
	pthread_mutex_unlock(&proc_lock);
}

//队首压入函数
template<class T>
int deque2<T>::push_top(T  *t)
{
	pthread_mutex_lock(&push_lock);
	pthread_mutex_lock(&mutex);
	//printf("push into queue before ....num=%d,max=%d  ------------------------- \n",num,max);
	while(num >= max)
	{
		pthread_cond_wait(&condIn,&mutex);
	}
	
	pthread_mutex_lock(&proc_lock);
	typeDequeNode *temp = NULL;
	temp = new typeDequeNode;
	if( NULL==temp )
	{
		//writelog( 4, "push subroutin new function failed,_pt is NULL !");
		pthread_mutex_unlock(&proc_lock);
		pthread_mutex_unlock(&mutex);
		pthread_mutex_unlock(&push_lock);
		return 0;
	}
	temp->t = t;	
	temp->ap = _pt;
	_pt = temp;
	
	num++;
	pthread_mutex_unlock(&proc_lock);
	
	pthread_cond_signal(&condOut);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&push_lock);
//	printf("push into queue after  num=%d,max=%d\n",num,max);
	return 1;
}

//队尾压入函数
template<class T>//wucj add lock
int deque2<T>::push_first(T  *t)
{
	pthread_mutex_lock(&push_lock);
	pthread_mutex_lock(&mutex);
//	printf("push into queue before....num=%d,max=%d  ------------------------- \n",num,max);
	while(num >= max)
	{
	/*
	struct timespec delay;
	delay.tv_sec  = time(NULL) + 1;
	delay.tv_nsec = 0;
	pthread_cond_timedwait(&condIn,&mutex,&delay);
		*/
		pthread_cond_wait(&condIn,&mutex);
	}
	
	pthread_mutex_lock(&proc_lock);
	if(NULL==((*_pw) = new typeDequeNode))
	{
		//writelog( 4, "push subroutin new function failed,_pt is NULL !");
		pthread_mutex_unlock(&proc_lock);
		pthread_mutex_unlock(&mutex);
		pthread_mutex_unlock(&push_lock);
		return 0;
	}
	(*_pw)->t = t;
	(*_pw)->bp = _lp;
	(*_pw)->ap = NULL;
	_lp = (*_pw);
	_pw = &((*_pw)->ap);
	num++;
	pthread_mutex_unlock(&proc_lock);
	
	pthread_cond_signal(&condOut);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&push_lock);
//	printf("push into queue after  num=%d,max=%d \n",num,max);
	return 1;
}

// 设置POP延时时长
template<class T> //liufei add[20100331]
void deque2<T>::setDelayTime(int ntime)
{
	popModule = true;
	delaytime = ntime;
}

// 取出函数
template<class T>//wucj add lock
T *deque2<T>::pop()
{
	pthread_mutex_lock(&pop_lock);
	pthread_mutex_lock(&mutex);
	
	//liufei delete[20100112]
	while(num == 0)
	{
		if(popModule)
		{
			struct timespec delay;
			delay.tv_sec  = time(NULL) + delaytime;
			delay.tv_nsec = 0;
			int ret = pthread_cond_timedwait(&condOut,&mutex,&delay);
			if( ETIMEDOUT == ret)
			{
				pthread_mutex_unlock(&pop_lock);
				pthread_mutex_unlock(&mutex);
				return NULL;
			}
		}
		else
		{
			pthread_cond_wait(&condOut,&mutex);
		}	  
	}	
	
	
	pthread_mutex_lock(&proc_lock);
	T *ptxt=NULL;
	typeDequeNode *tmp;	
	
	if(_pt == NULL )
	{
		//writeRunlog( 4, "pop there is wrong,_pt is NULL !");
		pthread_mutex_unlock(&proc_lock);
		pthread_mutex_unlock(&mutex);
		pthread_mutex_unlock(&pop_lock);
		return NULL;
	}
	
	
	if(num >1)
	{
		tmp = _pt;
		_pt = _pt->ap;
		ptxt = tmp->t;
		if(tmp) 
		{
			delete tmp;
			tmp = NULL;
		}
		num--;
	}
	else if(num == 1)
	{
		tmp = _pt;
		_pt = _pt->ap;	
		_st = NULL;
		_lp = NULL;	
		_pw = &_pt;
		if(tmp != NULL)
		{
			ptxt = tmp->t;
			delete tmp;
			tmp = NULL;
			while(_pt!=NULL)
			{
				tmp=_pt;
				_pt=_pt->ap;
				delete tmp;
				tmp = NULL;
				//writeRunlog( 4, "pop is wrong,_pt is not NULL !");	
			}
		}
		num--;
	}
	pthread_mutex_unlock(&proc_lock);
	
	pthread_cond_signal(&condIn);
	pthread_mutex_unlock(&mutex);
	pthread_mutex_unlock(&pop_lock);
	
	return ptxt;
}

template<class T>
int deque2<T>::sum()
{	
	return num;
}

// 定位于链表起始节点函数
template<class T>//wucj add lock
T * deque2<T>::begin()
{
	pthread_mutex_lock(&proc_lock);
	_st = _pt;
	if( _st == NULL ) 
	{
		pthread_mutex_unlock(&proc_lock);
		return NULL;
	}
	else 
	{
		pthread_mutex_unlock(&proc_lock);
		return _st->t;
	}	
}

// 定位于链表起始节点函数
template<class T>
struct deque2<T>::typeDequeNode * deque2<T>::start()
{
	return _pt;
}

// 定位于链表下一节点函数
template<class T>//wucj add lock
T * deque2<T>::next()
{
	pthread_mutex_lock(&proc_lock);
	_st = _st->ap;
	if( _st == NULL ) 
	{
		pthread_mutex_unlock(&proc_lock);
		return NULL;
	}
	else 
	{
		pthread_mutex_unlock(&proc_lock);
		return _st->t;
	}	
}

template <class T>
deque2<T> & deque2<T>::operator=(deque2<T> &other)
{
	if(this == &other) return *this;
	
	clear();
	
	T * tp;
	for(tp=other.begin();tp!=NULL;tp=other.next())
	{
		this->insert(*tp);
	}
	
	return *this;
}



/* 哈希表与map2的结合         */
// 构造函数
template<class K,class T,int num>
hashLink<K,T,num>::hashLink()
{
	_t = new map2<K,T>[num];
};

// 析构函数
template<class K,class T,int num>
hashLink<K,T,num>::~hashLink()
{
	if(_t != NULL) delete [] _t;
};

// 插入函数
template <class K,class T,int num> 
void hashLink< K,T,num>::insert(K k,T * t)
{
	int hashKey;
	
	/* 根据类型T定义的成员函数计算哈希值 */
	hashKey = k.getHashKey() % num;
#ifdef DEBUG
	printf("hashLink< K,T,num>::insert hashKey =[%d]\n",hashKey);
#endif
	if( hashKey < 0 ) hashKey = 0;
	
	/* 在哈希堆对应binTree里插入 */	
	_t[hashKey].insert(k,t);
	count++;
#ifdef DEBUG
	printf("hashLink< K,T,num>::insert count=[%d]\n",count);
#endif
};

// 查找函数
template <class K,class T,int num> 
T * hashLink< K,T,num>::search(K k)
{
	int hashKey;
	
	/* 根据类型T定义的静态函数计算哈希值 */
	hashKey = k.getHashKey() % num;
#ifdef DEBUG
	printf("T * hashLink< K,T,num>::search hashKey=[%d]\n",hashKey);
#endif
	/* 在binTree里查找key对应位置        */	
	return _t[hashKey].search(k);
};

// 删除函数
template <class K,class T,int num> 
T *hashLink< K,T,num>::delNode(K k)
{
	int hashKey;
	T *pt=NULL;
	
	/* 根据类型T定义的静态函数计算哈希值 */
	hashKey = k.getHashKey() % num;
#ifdef DEBUG
	printf("T * hashLink< K,T,num>::delNode hashKey=[%d]\n",hashKey);
#endif
	/* 在map里查找key,并删除        */	
	pt = _t[hashKey].delnode(k);
	if(pt != NULL)
		count--;
	
	//printf("hashLink< K,T,num>::delNode count=[%d]\n",count);
	return pt;
};

// 哈希表遍历函数                    
template <class K,class T,int num> 
T * hashLink<K,T,num>::begin()
{
	T  *p;		
	pos = 0;
	for(pos=0;pos<num;pos++){
		if( (p = _t[pos].begin()) != NULL ) return p;
	}
	return NULL;
}

// 哈希表遍历函数                    
template <class K,class T,int num> 
T * hashLink<K,T,num>::next()
{
	T  *p;
	if( (p=_t[pos].next()) != NULL ){
		return p;
	}
	else {
		while( ++pos < num ){
			if( (p = _t[pos].begin() ) != NULL ) return p;
		}
	}
	return NULL;
}

// 哈希表遍历函数                    
template <class K,class T,int num> 
int hashLink<K,T,num>::size()
{
	return count;
}

template <class K,class T,int num> 
int hashLink<K,T,num>::clear()
{
	int tmpos;
	for(tmpos =0;tmpos<num;tmpos++)
	{
		_t[tmpos].clear();
	}
	return 0;
}


template <class K,class T>
list2<K,T>::list2()
{
	m_root.next=NULL;
	pthread_mutex_init(&m_mutex,NULL);
}

template <class K,class T>
list2<K,T>::~list2()
{
	typeListNode *pNode=NULL;
	typeListNode *pLastNode=NULL;
	
	pLastNode=&(m_root);
	for(pNode=pLastNode->next;pNode != NULL; pNode=pLastNode->next)
	{
		pLastNode->next=pNode->next;
		delete pNode->t;
		delete pNode;
	}
	
	pthread_mutex_destroy(&m_mutex);
}

template <class K,class T>
void list2<K,T>::insert(K k,T *t,int sortFlag)
{
	typeListNode *pNode=NULL;
	typeListNode *pLastNode=NULL;
	
	pthread_mutex_lock(&m_mutex);
	
	#ifdef _DEBUG_INFO_
	cout<<"insert================"<<endl;
	t->prnObj();
	#endif
	
	if(m_root.next == NULL)
	{
		pNode=new typeListNode;
		pNode->t=t;
		pNode->k=k;
		pNode->next=NULL;
		m_root.next=pNode;
	}
	else if(sortFlag == 0)
	{
		pLastNode=m_root.next;
		for(pNode=m_root.next;pNode != NULL; pNode=pNode->next)
		{
			if(k < pNode->k )
			{
				typeListNode *pTmp=new typeListNode;
				pTmp->k=k;
				pTmp->t=t;
				pLastNode->next=pTmp;
				pTmp->next=pNode;
				break;
			}
			else
			{
				pLastNode=pNode;
			}
		}
		
		if(pNode == NULL);
		{
			pNode=new typeListNode;
			pNode->t=t;
			pNode->k=k;
			pNode->next=NULL;
			pLastNode->next=pNode;
		}
	}
	else
	{
		pLastNode=m_root.next;
		for(pNode=m_root.next;pNode != NULL; pNode=pNode->next)
		{
			if(k > pNode->k )
			{
				typeListNode *pTmp=new typeListNode;
				pTmp->k=k;
				pTmp->t=t;
				pLastNode->next=pTmp;
				pTmp->next=pNode;
				break;
			}
			else
			{
				pLastNode=pNode;
			}
		}
		
		if(pNode == NULL);
		{
			pNode=new typeListNode;
			pNode->t=t;
			pNode->k=k;
			pNode->next=NULL;
			pLastNode->next=pNode;
		}
	}
	
	pthread_mutex_unlock(&m_mutex);
}

template <class K,class T>
void list2<K,T>::deleteNode(T &t)
{
	typeListNode *pNode=NULL;
	typeListNode *pLastNode=NULL;
	
	pthread_mutex_lock(&m_mutex);
	
	pLastNode=&(m_root);
	for(pNode=pLastNode->next;pNode != NULL; pNode=pNode->next)
	{
		if(t == *(pNode->t) )
		{
			pLastNode->next=pNode->next;
			delete pNode->t;
			delete pNode;
			break;
		}
		else
		{
			pLastNode=pNode;
		}
	}
	
	pthread_mutex_unlock(&m_mutex);
}

template <class K,class T>
void list2<K,T>::deleteNodes(K k,int interval)
{
	typeListNode *pNode=NULL;
	typeListNode *pLastNode=NULL;
	
	pthread_mutex_lock(&m_mutex);
	
	pLastNode=&(m_root);
	for(pNode=pLastNode->next;pNode != NULL;pNode=pLastNode->next)
	{
		if(k - pNode->k > interval)
		{
			pLastNode->next=pNode->next;
			delete pNode->t;
			delete pNode;
		}
		else
		{
			break;
		}
	}
	
	pthread_mutex_unlock(&m_mutex);
}


template <class K,class T>
T* list2<K,T>::search(T &t)
{
	typeListNode *pNode=NULL;
	typeListNode *pLastNode=NULL;
	
	pthread_mutex_lock(&m_mutex);
	
	pLastNode=&(m_root);
	for(pNode=pLastNode->next;pNode != NULL; pNode=pNode->next)
	{
		if(t == *(pNode->t) )
		{
			break;
		}
		else
		{
			pLastNode=pNode;
		}
	}
	
	pthread_mutex_unlock(&m_mutex);
	
	if(pNode == NULL)
	{
		return NULL;
	}
	else
	{
		return pNode->t;
	}
}

template <class K,class T>
void list2<K,T>::prnObj()
{
	typeListNode *pNode=NULL;
	
	pthread_mutex_lock(&m_mutex);
	
	for(pNode=m_root.next;pNode != NULL;pNode=pNode->next)
	{
		(pNode->t)->prnObj();
	}
	
	pthread_mutex_unlock(&m_mutex);
}


#endif

