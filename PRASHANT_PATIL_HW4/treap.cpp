
#include <iostream>
#include<cstdlib>
#include<cstring>
#include<ctime>
using namespace std;

class TreeNode
{
public:
TreeNode *par,*left,*right;   
int n;   
float p;    
int noofleft;   

TreeNode();   
TreeNode* search(int k);   
void putintotreap(int k,float pr);
void deleteelement(int k);
void searchmthsmall(int m);  
};

class Treapclass
{
public:
TreeNode *treaproot;
Treapclass();
TreeNode* search(int k);    
void putintotreap(int k,float pr); 
void deleteelement(int k);    
void searchmthsmallest(int m);    
};

Treapclass::Treapclass()
{
treaproot=NULL;
}
TreeNode::TreeNode()
{
par=NULL;
left=NULL;
right=NULL;
n=0;
p=0.0;
noofleft=0;
}

TreeNode* TreeNode::search(int num)
{
if(this==NULL)
return NULL;
if(n==num)
return this;
else if(n>num)
return (this->left)->search(num);
else if(n<num)
return (this->right)->search(num);
}


TreeNode* Treapclass::search(int k)
{
return treaproot->search(k);
}




void changeleftcount(TreeNode* node,int i)
{
while(node!=NULL)
{
if(node->par!=NULL)
if(node->par->left==node)
node->par->noofleft+=i;
node=node->par;
}

}

void left_rotation(TreeNode* ptr1,TreeNode* ptr2)
{

ptr2->par=ptr1->par;
if(ptr1->par!=NULL)
{
if(ptr1->par->left==ptr1)
ptr1->par->left=ptr2;
else if(ptr1->par->right==ptr1)
ptr1->par->right=ptr2;
}
ptr1->par=ptr2;
if(ptr2->left!=NULL)
ptr2->left->par=ptr1;
ptr1->right=ptr2->left;
ptr2->left=ptr1;
ptr2->noofleft+=(1+ptr1->noofleft);

}


void right_rotation(TreeNode* ptr1,TreeNode* ptr2)
{
ptr2->par=ptr1->par;
if(ptr1->par!=NULL)
{
if(ptr1->par->left==ptr1)
ptr1->par->left=ptr2;
else if(ptr1->par->right==ptr1)
ptr1->par->right=ptr2;
}
ptr1->par=ptr2;
if(ptr2->right!=NULL)
ptr2->right->par=ptr1;
ptr1->left=ptr2->right;
ptr2->right=ptr1;
ptr1->noofleft-=(1+ptr2->noofleft);
}

void insertrotate(TreeNode* par,TreeNode* chi)
{
if(par==NULL)
return;
if(par->p>chi->p)
{
if(par->right==chi)
left_rotation(par,chi);
else if(par->left==chi)
right_rotation(par,chi);

insertrotate(chi->par,chi);
}


}

TreeNode* searchpar(TreeNode* node,int k)
{
if(node==NULL)
return NULL;
if((node->left==NULL)&&(node->right==NULL))
return node;
else if(node->n>=k)
{
if(node->left!=NULL)
return searchpar(node->left,k);
else
return node;
}
else if(node->n<k)
{
if(node->right!=NULL)
return searchpar(node->right,k);   
else
return node;

}
}

void TreeNode::putintotreap(int k,float pr)
{

TreeNode* temp=new TreeNode;
temp->n=k;
temp->p=pr;
for(int l=0;l<1000;l++)
{}
TreeNode* temptr1=new TreeNode;

temptr1=searchpar(this,k);

temp->par=temptr1;
if(k<=temptr1->n)
temptr1->left=temp;
else if(k>temptr1->n)
temptr1->right=temp;
changeleftcount(temp,1);
insertrotate(temptr1,temp);


}

void Treapclass::putintotreap(int k,float pr)
{
if(treaproot==NULL)
{
TreeNode* temp=new TreeNode;
temp->n=k;
temp->p=pr;
treaproot=temp;
}
else
treaproot->putintotreap(k,pr);

TreeNode* t=new TreeNode;
t=treaproot;
while(t->par!=NULL)
t=t->par;
treaproot=t;

}


void balancefordel(TreeNode* node)
{
if((node->left==NULL)&&(node->right==NULL))
return;

if(node->left!=NULL)
if(node->p>node->left->p)
if(node->right!=NULL)
{
if(node->left->p<node->right->p)
{right_rotation(node,node->left);
balancefordel(node);
}
}
else if(node->right==NULL)
{right_rotation(node,node->left);
balancefordel(node);
}

if(node->right!=NULL)
if(node->p>node->right->p)
if(node->left!=NULL)
{
if(node->right->p<node->left->p)
{left_rotation(node,node->right);
balancefordel(node);
}
}
else if(node->left==NULL)
{left_rotation(node,node->right);
balancefordel(node);
}


}



void deleteelementreapnode(TreeNode* temp)
{


if((temp->left==NULL)&&(temp->right==NULL))
{
changeleftcount(temp,-1);
if(temp->par!=NULL)
{
if(temp->par->left==temp)
temp->par->left=NULL;
else if(temp->par->right==temp)
temp->par->right=NULL;
}
temp->par=NULL;
delete temp;
}


else if((temp->left!=NULL)&&(temp->right==NULL))
{
changeleftcount(temp,-1);
temp->left->par=temp->par;
if(temp->par!=NULL)
{
if(temp->par->left==temp)
temp->par->left=temp->left;
else if(temp->par->right==temp)
temp->par->right=temp->left;
}
temp->par=NULL;
temp->left=NULL;
delete temp;

}

else if((temp->left==NULL)&&(temp->right!=NULL))
{
changeleftcount(temp,-1);
temp->right->par=temp->par;
if(temp->par!=NULL)
{
if(temp->par->left==temp)
temp->par->left=temp->right;
else if(temp->par->right==temp)
temp->par->right=temp->right;
}
temp->par=NULL;
temp->right=NULL;
delete temp;


}

else if((temp->left!=NULL)&&(temp->right!=NULL))
{
TreeNode* suc=new TreeNode;
suc=temp->left;
while(suc->right!=NULL)
suc=suc->right;
temp->n=suc->n;
temp->p=suc->p;
deleteelementreapnode(suc);
balancefordel(temp);
}
}


void TreeNode::deleteelement(int k)
{
TreeNode* temp=this->search(k);
deleteelementreapnode(temp);
}


void Treapclass::deleteelement(int k)
{
//If key not present
if(treaproot->search(k)==NULL)
{
cout<<"Key not present\n";
return;
}

//If Treapclass has only single element
if((treaproot->n==k)&&((treaproot->left==NULL)&&(treaproot->right==NULL)))
{
treaproot=NULL;
return;
}

//If treaproot has the element
if(treaproot->n==k)
{
if((treaproot->left!=NULL)&&(treaproot->right==NULL))
{
treaproot->left->par=NULL;
treaproot=treaproot->left;
return;
}
}
if(treaproot->n==k)
{
if((treaproot->left==NULL)&&(treaproot->right!=NULL))
{
treaproot->right->par=NULL;
treaproot=treaproot->right;
return;
}
}

if((treaproot->n==k)&&(treaproot->left!=NULL)&&(treaproot->right!=NULL))
{
TreeNode* suc=new TreeNode;
suc=treaproot->left;
while(suc->right!=NULL)
suc=suc->right;
treaproot->n=suc->n;
treaproot->p=suc->p;
deleteelementreapnode(suc);
balancefordel(treaproot);
TreeNode* t=new TreeNode;
t=treaproot;
while(t->par!=NULL)
t=t->par;
treaproot=t;
return;
}
//Otherwise
treaproot->deleteelement(k);
}

//Find mth smallest node in the subtree treaprooted at this node
void TreeNode::searchmthsmall(int m)
{
if(m<=noofleft)
left->searchmthsmall(m);
else if(m==noofleft+1)
cout<<n<<"\n";
else if(m>noofleft)
right->searchmthsmall(m-(1+noofleft));
}

//To find mth smallest node in this Treapclass
void Treapclass::searchmthsmallest(int m)
{
treaproot->searchmthsmall(m);
}

//Function to generate random number
int randnum(int minnum, int maxnum)
        {
            int result=0,lownum=0,hinum=0;
            if(minnum<maxnum)
            {
                lownum=minnum;
                hinum=maxnum+1;
            }else{
                lownum=maxnum+1;
                hinum=minnum;
            }
            srand(time(NULL));
            result = (rand()%(hinum-lownum))+lownum;
            return result;
        }

//A function to putintotreap all elements of 1 Treapclass into another
void joinallele( Treapclass &t,TreeNode* ro ){
    if( ro == NULL ) return;
    joinallele( t,ro->left );
    t.putintotreap(ro->n,ro->p);
    joinallele( t,ro->right );
}

//Three way join
Treapclass joinerfunc(Treapclass t1,int k,Treapclass t2)
{
int prior=randnum(1,32000);
t1.putintotreap(k,prior);
joinallele(t1,t2.treaproot);
return t1;
}

void splitintotwo(Treapclass t1,int k,Treapclass &t2,Treapclass &t3)
{
t1.putintotreap(k,-32000);
t2.treaproot=t1.treaproot->left;
t2.treaproot->par->left=NULL;
t2.treaproot->par=NULL;
t3.treaproot=t1.treaproot->right;
t3.treaproot->par->right=NULL;
t3.treaproot->par=NULL;
}





int main() {

int treapno,cho,number;
cin>>cho;
Treapclass treaps[1001];
char choice[10];
int i,ele,m;

for(int l=0;l<1000;l++)
{}
while(cho--)
{
cin>>choice;
if(strcmp(choice,"insert")==0)
{

cin>>treapno>>number;
for(i=0;i<number;i++)
{
cin>>ele;
int prior=randnum(1,32000);
treaps[treapno].putintotreap(ele,prior);
}

}

if(strcmp(choice,"delete")==0)
{
cin>>treapno>>number;
for(i=0;i<number;i++)
{
cin>>ele;
treaps[treapno].deleteelement(ele);
}
}

if(strcmp(choice,"select")==0)
{
cin>>treapno>>m;
treaps[treapno].searchmthsmallest(m);
}


if(strcmp(choice,"join")==0)
{
int tr1,tr2,tr3,k;
cin>>tr1>>k>>tr2>>tr3;
treaps[tr3]=joinerfunc(treaps[tr1],k,treaps[tr2]);

}

if(strcmp(choice,"split")==0)
{
int tr1,tr2,tr3,k;
cin>>tr1>>k>>tr2>>tr3;
splitintotwo(treaps[tr1],k,treaps[tr2],treaps[tr3]);

}
}
return 0;
}
