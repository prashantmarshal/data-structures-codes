#include<stdio.h>
#include<iostream>
#include <stdlib.h>
#include <math.h>
#include<stdio.h>
#include<iostream>
using namespace std;
class inoutque;
class inque;
class outque;
class heap;
outque *out;
inque *in;
inoutque *inout;
heap *h;
int n;
int m;
float acc=0;
float tim=0;
int ct=0;
float generate_exponential(float lambda) {
float p =(1.0*(rand())/(RAND_MAX));
return (1/lambda) * log(1/(1-p));
}
class customer
{
public:
int id;
float entrytime;
float exittime;
customer* next;      
};
customer* cinter;
class links
{
  public:
  float p;
  int queue;
};
class event
{
  public:
  int qno;
  float time;
  int id;
};
class heap
{
    public:
    int count;
    event *ar;
      heap()
      {
        count=0;
        ar=(event*)malloc(2000*sizeof(event));
      };
       void insert()
     {
      event temp;
     int flag=0;
     int par=(count-1)/2;
     int ptr=count;
     while(ar[ptr].time<=ar[par].time)
     {
        if(par==0)
        flag=1;
        temp=ar[ptr];
        ar[ptr]=ar[par];
        ar[par]=temp;
        ptr=par;
        par=(ptr-1)/2;
        if(flag==1)
        break;
     }
     count++;
   }
   void deletenode()
{
   int ptr=0,left=1,right=2;
   event last=ar[count-1];
   count--;
   while(left<=count-1 || right<=count-1)
   {
      if(last.time<=ar[left].time && last.time<=ar[right].time)
      {ar[ptr]=last;break;}
      if(ar[left].time<ar[right].time)
      {
        ar[ptr]=ar[left];
        ptr=left;
      }
      else 
      {
        ar[ptr]=ar[right];
        ptr=right;
      }
      left=2*ptr+1;
      right=left+1;
   }
   ar[ptr]=last;
   int i;
   
}  
         void add(int qno,float t,int id)
         {
                ar[count].time=t;
                ar[count].qno=qno;
                ar[count].id=id;
                insert();
          }
         int minheap()
         {
            return ar[0].qno;
         }
         float minheaptime()
         {
            return ar[0].time;
         }
         
         void display()
         {
              int i;
              for(i=0;i<count;i++)
              {
                 printf(" %f",ar[i].time);
              }
         }
};
class outque
{
      
      public:
      void add(customer* c,float t)
      {   acc+=c->exittime-c->entrytime;
          
      }
};
class inoutque{
public:
int qno;
customer *c1;
float dt;
int flag;
customer* l;
float u;
customer *curr;
links *lk;
int nlinks;
float lasttime;
inoutque()
{
   l=NULL;
   curr=NULL;
}
void add(customer* c,float t)
{
     c1=new customer;
     c1->id=c->id;
     c1->entrytime=c->entrytime;
   if(curr==NULL)
   {
     curr=c1;
     dt=generate_exponential(u);
     lasttime=tim+dt;
     curr->exittime=lasttime;
     h->add(qno,t+dt,curr->id);
   }
   else
   {
   addtolist(c1);
   }
}
void finish()
{ 
  int d;
  int i=0;
  float sum=0;
  customer *cou;
  cou=new customer;
  cou->id=curr->id;
  cou->entrytime=curr->entrytime;
  cou->exittime=curr->exittime;
  float f=(1.0*rand())/(RAND_MAX);
  for(i=0;i<nlinks;i++)
  {
    sum+=lk[i].p;
    if(f<=sum)
    {
        d=lk[i].queue;
        break;
    }
  }
  if(l==NULL)
  curr=NULL;  
  else
  {
     dt=generate_exponential(u);
     curr->next->exittime=curr->exittime+dt;
     h->add(qno,curr->next->exittime,curr->next->id);
     lasttime=curr->next->exittime;
     curr=l;
     if(l->next!=NULL)
     l=l->next;
     else
     l=NULL;
  }
  
    if(d==(n-1))
    {
      out[0].add(cou,tim);
    }
    else
    {
        inout[d].add(cou,tim);
    }    
    
}
void addtolist(customer* c)
{ 
  float T;
  if(l==NULL)
  {
  l=c;
  curr->next=l;
  l->next=NULL;
  }
  else
  {
    c1=l;
    flag=0;
    while(1)
    {
      if(c1->next==NULL)
      {
          c1->next=c;
          c->next=NULL;
          flag=1;
      }
      if(flag==1)
      break;
      c1=c1->next;
    }
  }
}
};
class inque
{
   public:
   links *lk;
   int nlinks;
   float lambda;
   customer *c;
   customer *c1;
   int qno; 
   void finish()
   {
      int d;
      int i=0;
      float sum=0;
      float f=(1.0*rand())/(RAND_MAX);
      for(i=0;i<nlinks;i++)
     {
      sum+=lk[i].p;
     if(f<=sum)
    {
        d=lk[i].queue;
        break;
    }
  }
     inout[d].add(c,c->entrytime);
     c1=addcustomer();
     h->add(qno,c1->entrytime,c1->id);
   }   
   customer* addcustomer()
   {
      float dt;
      dt=generate_exponential(lambda);
      c=new customer;
      c->entrytime=tim+dt;
      c->id=++ct;
      return c; 
   }
   
   
};
int main()
{    
     out=new outque;
     float simtim;
     cin>>simtim;
     cin>>n;
     cin>>m;
     int *lambda;
     lambda=(int*)malloc((n-1)*sizeof(int));
     int i;
     for(i=0;i<=n-2;i++)
     {
       cin>>lambda[i];
     }
     int nlinks;
     links *lk;
     int j;
     in=(inque*)malloc(1000*sizeof(inque));
     inout=(inoutque*)malloc(1000*sizeof(inoutque));
     out=(outque*)malloc(sizeof(outque));
     for(i=0;i<=(n-2);i++)
     {
          
        cin>>nlinks;                   
        if(i<=(m-1))
        {
          in[i].nlinks=nlinks;
          lk=(links*)malloc(nlinks*sizeof(links));
          for(j=0;j<nlinks;j++)
          {
            cin>>lk[j].queue;
            cin>>lk[j].p;
          }
          in[i].lk=lk;
          in[i].qno=i;
        }
        if(i>=m && i<=(n-2))
        {
          inout[i].nlinks=nlinks;
          lk=(links*)malloc(nlinks*sizeof(links));
          for(j=0;j<nlinks;j++)
          {
            cin>>lk[j].queue;
            cin>>lk[j].p;
          }
          inout[i].lk=lk;
          inout[i].qno=i;
        }
     }
     for(i=0;i<=n-2;i++)
     {
        if(i<m)
        {  
           in[i].lambda=lambda[i];
        }
        if(i>=m && i<=(n-2))
        {
           inout[i].u=lambda[i];
        }
     }
    int qno;
    h=new heap;
    customer *c;
    for(i=0;i<=(m-1);i++)
    {
    c=in[i].addcustomer();
    h->add(i,c->entrytime,c->id);
    }
    tim=h->minheaptime();
    while(tim<=simtim)
    {
       tim=h->minheaptime();
       qno=h->minheap();
       h->deletenode();
         if(qno>=0 && qno<=(m-1))
          {
             in[qno].finish();
          }
          if(qno>=m && qno<=(n-2))
          {
            inout[qno].finish();
          }
    }
    cout<<endl;
    printf("%f",acc/ct);
    return 0;
}
