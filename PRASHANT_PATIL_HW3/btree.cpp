#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<cstring>
using namespace std;

#define MAX_DEG 1000
#define MAX_TREES 10000

int MAX=4;
int MIN=2;
int T;

class BTreeNode
{ 
	friend class BTree;
	public:
	int total_n,cnt;
	int keys[2001] ;
	BTreeNode *ptr[2002] ;
	BTreeNode *parent;
	
	BTreeNode( )
	{
		parent=NULL;
		for(int i=0;i<2002;i++)
			ptr[i]=NULL;
		cnt=0;
		total_n=0;
	}
};

class BTree 
{	 
	public :
	BTreeNode *root ;
	BTree( ) ; 
	void insert ( int val ) ; 
	int set ( int val, BTreeNode *n, int *p, BTreeNode **c ) ; 
	BTreeNode * search ( int val, BTreeNode *root, int *pos ) ; 
	int searchnode ( int val, BTreeNode *n, int *pos ) ;
	void nodefill ( int val, BTreeNode *c, BTreeNode *n, int k ) ; 
	void split ( int val, BTreeNode *c, BTreeNode *n, int k, int *y, BTreeNode **newnode ) ;
	void del ( int val ) ;
	int recurdel ( int val, BTreeNode *root ) ; 
	void clear ( BTreeNode *root, int k ) ; 
	void traverse ( BTreeNode *root, int i ) ; 
	void restore ( BTreeNode *root, int i ) ;  
	void rshift ( int k, BTreeNode *T ) ; 
	void lshift ( int k, BTreeNode *T ) ; 
	void mergenode ( int k, BTreeNode *T ) ; 
	int counter (BTreeNode *tempnode);
	int select(int k, BTreeNode *tempnode);
	void join (BTreeNode *r1, int key , BTreeNode *r2);
	void insjoin (BTreeNode *tempnode);
	void splitroot1( BTreeNode *r1, int key );
	void splitroot2( BTreeNode *r2, int key );
};

BTree :: BTree( ) 							
{
	root = NULL ; 
}

void BTree :: insert ( int val ) 
{
	int i ;
	BTreeNode *c, *n ;
	int tem ;
	tem = set ( val, root, &i, &c ) ;
	if ( tem )
	{
		n = new BTreeNode ;
		n -> total_n = 1 ;
		n -> keys[1] = i ;
		n -> ptr[0] = root ;
		n -> ptr[1] = c ;
		root = n ;
	}
}

int BTree :: set ( int val, BTreeNode *n, int *p, BTreeNode **c )
{
	int k ;
	if ( n == NULL )
	{
		*p = val ;
		*c = NULL ;
		return 1 ;
	}
	else
	{
		if ( searchnode ( val, n, &k ) );
		
		if ( set ( val, n -> ptr[k], p, c ) )
		{
			if ( n -> total_n < MAX )
			{
				nodefill ( *p, *c, n, k ) ;
				return 0 ;
			}
			else
			{
				split( *p, *c, n, k, p, c );
				return 1 ;
			}
		}
		return 0 ;
	}
}

BTreeNode * BTree :: search ( int val, BTreeNode *root, int *pos )
{
	if ( root == NULL ) return NULL ; 
	else
	{
		if ( searchnode ( val, root, pos ) ) return root ;
		else return search ( val, root -> ptr[*pos], pos ) ;
	}
}

int BTree :: searchnode ( int val, BTreeNode *n, int *pos )
{
	if ( val < n -> keys[1] )
	{
		*pos = 0 ;
		return 0 ;
	}
	else
	{
		*pos = n -> total_n ;
		while ( ( val < n -> keys[*pos] ) && *pos > 1 )
		( *pos )-- ;
		if ( val == n -> keys[*pos] ) return 1 ;
		else return 0 ;
	}
} 

void BTree :: nodefill ( int val, BTreeNode *c, BTreeNode *n, int k )
{
	int i ;
	for ( i = n ->total_n ; i > k ; i-- )
	{
		n -> keys[i + 1] = n -> keys[i] ;
		n -> ptr[i + 1] = n -> ptr[i] ;
	}
	n -> keys[k + 1] = val ;
	n -> ptr[k + 1] = c ;
	n -> total_n++ ;
} 

void BTree :: join (BTreeNode *r1, int key , BTreeNode *r2)
{
	insjoin(r1);
	insert(key);
	insjoin(r2);
}

void BTree :: insjoin (BTreeNode *tempnode)
{
	bool leaf=1;
	for(int i=0;i<= tempnode->total_n ; i++)
	if (tempnode->ptr[i]!=NULL)
	{
		leaf=0;
		break;
	}	
	
	if (leaf)
	{
		for(int i=1;i<= tempnode->total_n ; i++)
			insert(tempnode -> keys[i]);
	}
	else
	{
		insjoin(tempnode -> ptr[0]);
		for(int i=1;i<= tempnode->total_n ; i++)
		{
			insert(tempnode -> keys[i]);
			insjoin(tempnode -> ptr[i]);
		}	
	}
}

int BTree :: select(int k, BTreeNode *tempnode)
{	
	bool leaf=1;
	for(int i=0;i<= tempnode->total_n ; i++)
	if (tempnode->ptr[i]!=NULL)
	{
		leaf=0;
		break;
	}	
	
	if (leaf)
		return tempnode -> keys[k] ;
	else
	{
		for(int i=0 ; i<= tempnode -> total_n ; i++)
		if(tempnode -> ptr[i] -> cnt < k )
		{
			k-=tempnode -> ptr[i] -> cnt;
			if(k==1 && i< tempnode -> total_n )
			{	
				return tempnode -> keys[i+1];
			}
			else k--;
		}
		else
		{
			return select(k, tempnode -> ptr[i]);
		}
	}
}

int BTree ::  counter (BTreeNode *tempnode)
{
	bool leaf=1;
	for(int i=0;i<= tempnode->total_n ; i++)
	if (tempnode->ptr[i]!=NULL)
	{
		leaf=0;
		break;
	}	
	
	if (leaf)
		tempnode->cnt=tempnode->total_n;
	else
	{
		int temp=0;
		for(int i=0; i<= tempnode->total_n ; i++)
			temp+=counter(tempnode->ptr[i]);
		temp+=tempnode->total_n;
		tempnode->cnt=temp;
	}
	
	return tempnode->cnt;
}

void BTree :: split ( int val, BTreeNode *c, BTreeNode *n, int k, int *y, BTreeNode **newnode )
{
	int i, mid ;
	if ( k <= MIN ) mid = MIN ;
	else mid = MIN + 1 ;
	*newnode = new BTreeNode ;
	for ( i = mid + 1 ; i <= MAX ; i++ )
	{
		( *newnode ) -> keys[i - mid] = n -> keys[i] ;
		( *newnode ) -> ptr[i - mid] = n -> ptr[i] ;
	}

	( *newnode ) -> total_n = MAX - mid ;
	n -> total_n = mid ;

	if ( k <= MIN ) nodefill ( val, c, n, k ) ;
	else nodefill ( val, c, *newnode, k - mid ) ;

	*y = n -> keys[n -> total_n] ;
	( *newnode ) -> ptr[0] = n -> ptr[n -> total_n] ;
	n -> total_n-- ;
} 

void BTree :: del ( int val )
{
	BTreeNode * temp ;
	if ( ! recurdel ( val, root ) ) ;

	else
	{
		if ( root -> total_n == 0 ) 
		{
			temp = root ;
			root = root -> ptr[0] ;
			delete temp ;
		}
	}
}

int BTree :: recurdel ( int val, BTreeNode *tempnode )
{
	if(tempnode == NULL) return 0;
	
	int i ;
	int tem ;
	
	if ( tempnode == NULL ) return 0 ;
	else
	{
		tem = searchnode ( val, tempnode, &i ) ;
		if ( tem )
		{
			if ( tempnode -> ptr[i - 1] )
			{
				traverse ( tempnode, i ) ;
				tem = recurdel ( tempnode -> keys[i], tempnode -> ptr[i] ) ;
				if ( !tem );
			}
			else clear ( tempnode, i ) ;
		}
		else tem = recurdel ( val, tempnode -> ptr[i] ) ;

		if ( tempnode -> ptr[i]!= NULL )
		{
			if ( tempnode -> ptr[i] -> total_n < MIN )
			restore ( tempnode, i ) ;
		}
		return tem ;
	}
}
 
void BTree :: clear ( BTreeNode *tempnode, int k )
{
	int i ;
	for ( i = k + 1 ; i <= tempnode -> total_n ; i++ )
	{
		tempnode -> keys[i - 1] = tempnode -> keys[i] ;
		tempnode -> ptr[i - 1] = tempnode -> ptr[i] ;
	}
	tempnode -> total_n-- ;
}
 
void BTree :: traverse ( BTreeNode *tempnode, int i )
{
	BTreeNode *temp = tempnode -> ptr[i] ;
	while ( temp -> ptr[0] ) temp = temp -> ptr[0] ;
	tempnode -> keys[i] = temp -> keys[1] ;
} 

void BTree :: restore ( BTreeNode *tempnode, int i )
{
	if ( i == 0 )
	{
		if ( tempnode -> ptr [1] -> total_n > MIN )
		lshift ( 1 , tempnode ) ;
		else mergenode ( 1 , tempnode) ;
	}
	else
	{
		if ( i == tempnode -> total_n )
		{
			if ( tempnode -> ptr[i - 1] -> total_n > MIN )
			rshift ( i , tempnode ) ; 
			else mergenode ( i , tempnode ) ; 
		}
		else 
		{ 
			if ( tempnode -> ptr[i - 1] -> total_n > MIN ) 
				rshift ( i , tempnode ) ; 
			else 
			{ 
				if ( tempnode -> ptr[i + 1] -> total_n > MIN ) 
				lshift ( i + 1 , tempnode ) ; 
				else mergenode ( i, tempnode ) ; 
			} 
		}
	}
} 

void BTree::lshift( int k, BTreeNode * T)
{
	BTreeNode * temp;
	
	temp = T -> ptr[k-1];
	temp->total_n++;
	temp->keys[temp->total_n] = T -> keys[k];
	temp->ptr[temp->total_n] = T -> ptr[k] -> ptr[0];
	temp= T->ptr[k];
	
	T->keys[k] = temp -> keys[1];
	temp->ptr[0] = temp-> ptr[1];
	temp->total_n--;
	
	for (int i=1; i <= temp->total_n ; i++)
	{
		temp->keys[i] = temp->keys[i+1];
		temp->ptr[i] = temp->ptr[i+1];
	}
}

void BTree::rshift( int i, BTreeNode * T) 
{
	BTreeNode *temp, *temp2;
	
	temp = T -> ptr[i];
	temp2 = T -> ptr[i-1];
	
	for (int j=temp->total_n; j >0; j--)
	{
		temp->keys[j+1] = temp->keys[j];
		temp->ptr[j+1] = temp->ptr[j];
	}
	
	temp->ptr[1] = temp-> ptr[0];
	temp->total_n++;
	temp->keys[1] = T->keys[i];
	T->keys[i] = temp2 -> keys[temp2->total_n];
	T->ptr[i]->ptr[0] = temp2 -> ptr[temp2->total_n];
	temp2->total_n--;
}

void BTree::mergenode(int i, BTreeNode * T)
{
	BTreeNode *temp1, *temp2;
	
	temp1 = T -> ptr[i];
	temp2 = T ->ptr[i-1];
	
	temp2 -> total_n++;
	
	temp2 ->keys[temp2->total_n] = T->keys[i];
	temp2 ->ptr[temp2->total_n] = temp1->ptr[0];
	
	for (int j = 1; j <= temp1->total_n; j++ ) 
	{
		(temp2 -> total_n)++ ;
		temp2-> keys[temp2->total_n] = temp1 -> keys[j];
		temp2-> ptr[temp2->total_n] = temp1 -> ptr[j];
	}
	for (int j = i; j <= T->total_n ; j++ ) 
	{
		T-> keys[j] = T -> keys[j+1];
		T-> ptr[j] = T -> ptr[j+1];
	}
	
	T->total_n--;
	delete temp1;
}

void BTree :: splitroot1( BTreeNode *tempnode, int key )
{
	bool leaf=1;
	for(int i=0;i<= tempnode->total_n ; i++)
	if (tempnode->ptr[i]!=NULL)
	{
		leaf=0;
		break;
	}
	
	if (leaf)
	{
		for(int i=1;i<= tempnode->total_n ; i++)
		{
			if(tempnode -> keys[i] < key)
			insert(tempnode -> keys[i]);
			else return;
		}
	}
	else
	{
		splitroot1(tempnode -> ptr[0], key );
		for(int i=1;i<= tempnode->total_n ; i++)
		{
			if(tempnode -> keys[i] < key )
			{
				insert(tempnode -> keys[i]);
				splitroot1(tempnode -> ptr[i] , key );
			}
			else return;
		}	
	}
}

void BTree :: splitroot2( BTreeNode *tempnode, int key )
{
	bool leaf=1;
	for(int i=0;i<= tempnode->total_n ; i++)
	if (tempnode->ptr[i]!=NULL)
	{
		leaf=0;
		break;
	}	

	if (leaf)
	{
		for(int i=tempnode->total_n ; i>= 1 ; i--)
		{
			
			if(tempnode -> keys[i] > key)
			insert(tempnode -> keys[i]);
			else return;
		}
	}
	else
	{
		splitroot2(tempnode -> ptr[tempnode ->total_n], key );
		for(int i=tempnode->total_n ; i>= 1 ; i--)
		{
			if(tempnode -> keys[i] > key )
			{
				insert(tempnode -> keys[i]);
				splitroot2(tempnode -> ptr[i-1] , key );
			}
			else return;
		}	
	}
}


int main( ) 
{	
	BTree tr_array[MAX_TREES];
	int n,i,j,n1,treeno;
	char s[10];
		
	cin>>T>>n;
	
	MAX=2*T-1;
	MIN=T-1;
	
	i=0;
	while(i<n)
	{
		scanf("%s",s);	
		if(!strcmp(s,"insert"))
		{
			int treeno,n1;
			
			cin>>treeno>>n1;
			int nos[n1];
			for(j=0;j<n1;j++)
			{
				cin>>nos[j];
				tr_array[treeno].insert(nos[j]);
			}
		}
		else if(!strcmp(s,"select"))
		{
			int treeno,rank,ans;
			cin>>treeno>>rank;
			
			tr_array[treeno].counter(tr_array[treeno].root);
			ans=tr_array[treeno].select(rank, tr_array[treeno].root);
			
			cout<<ans<<endl;
		}
		else if(!strcmp(s,"delete"))
		{
			int treeno,n1;
			cin>>treeno>>n1;
			int nos[n1];
			for(j=0;j<n1;j++)
			{
				cin>>nos[j];
				tr_array[treeno].del(nos[j]);
			}		
		}
		else if(!strcmp(s,"split"))
		{
			int tsplit,T1,T2,key;
			cin>>tsplit>>key;
			cin>>T1>>T2;
			
			tr_array[T1].splitroot1( tr_array[tsplit].root, key );
			
			tr_array[T2].splitroot2( tr_array[tsplit].root, key );
		}		
		else if(!strcmp(s,"join"))
		{
			int tjoin,T1,T2,key;
			cin>>T1>>key>>T2;
			cin>>tjoin;
			
			tr_array[tjoin].join( tr_array[T1].root, key , tr_array[T2].root);
		}
		
		
		i++;
	}
	
	
return 0;
}
