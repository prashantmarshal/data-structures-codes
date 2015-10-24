/////////// Pre-Processor directives //////////////
#include <iostream>
#include <cstdlib>
#include <cstdio>
using namespace std;
 ///////////Class definition for parser////////////
class genlistnode
{
	public:
	int t;
	string s;
	genlistnode *down,*next;
};
///////////////////////////Class Declarations for Evaluator////////////
class value
{
	public:
	int tag;
	float val;
	genlistnode *link;
};
class env
{
	public:
	value *varlist[26];
	env *parent;
	env(){
		for(int i=0;i<=25;i++)
		varlist[i]=NULL;
		parent=NULL;
	}
};
//////////////Function declaration for evaluator ///////////////////////
void add(char x, value val_new, env &e); 
value find(char x, env& e);
value evalNode(genlistnode *p, env& e);
value evalList(genlistnode *p, env& e);
///////////// Function declaration for parser //////////////////////////
genlistnode *cmdparse(int &rb,int &lb);
//////////////////////////////////////////////////
void printlist(genlistnode *head);
/////////////////////////////////
int main(int argc, char **argv)
{
	int rb=0,lb=0;
	cout<<"Please start entering the commands\n \n";
	genlistnode *parsedlist=cmdparse(rb,lb);
	env e;
	value result=evalNode(parsedlist,e);
	cout<<"\n"<<result.val<<"\n"<<endl;
	
	
	
	return 0;
}
//////////////////Parser Function////////////////////////////
genlistnode* cmdparse(int &rb,int &lb)
{
	string token;
	cin>>token;
if (token=="(")
{
	rb=rb+1;
	if (rb==1)
	{
		genlistnode *head=new genlistnode;
		head->t=1;
		head->s="(";
		head->next=NULL;
		head->down=cmdparse(rb,lb);
		return head;
	}
	else
	{
		genlistnode *rbrack=new genlistnode;
		rbrack->t=1;
		rbrack->s="(";
		rbrack->down=cmdparse(rb,lb);
		if(rb!=lb)
		{
		rbrack->next= cmdparse(rb,lb);
	    }
	    return rbrack;
	}
}
else if(token==")")
{
	lb=lb+1;
	return NULL;
}
	else
	{
		genlistnode *keys= new genlistnode;
		keys->t=0;
		keys->s=token;
		keys->down=NULL;
		keys->next=cmdparse(rb,lb);
		return keys;
	}
	
}
////////////////
void printlist(genlistnode *head)
{
	cout<< head->down->next->down->s<<"\n";
	
	
}
//////////EVALUATOR FUNCTIONS//////////////////////////////////
void add(char x, value val_new, env &e) {
value *p = new value;
p->tag=val_new.tag;
p->link=val_new.link;
p->val=val_new.val;
e.varlist[x-'a']=p;
}
//////////////////////////////////////////
value find(char x, env& e) {
if (e.varlist[x-'a'] != NULL) {
return *(e.varlist[x-'a']);
}
else {
if (e.parent == NULL) {
cout << "Entry " << x << " not found in symbol table " << endl;
value v;
v.link=NULL;
v.val=0;
v.tag=0;
return v;
}
else {
//cout << " Looking in parent environment " << endl;
return find(x, *(e.parent));
}
}
}
////////////////////////////////////////
value evalNode(genlistnode *p, env& e)
{
	if (p->t==0)
	{
		string str=p->s;
		if (isalpha(str[0]))
		{
			return find(str[0],e);
		}
		if(isdigit(str[0]))
		{
			value *var_num=new value;
			var_num->tag=0;
			var_num->link=NULL;
			char var_val[(str.size())+1];
			for (int i=0;i<(str.size());i++)
			{
				var_val[i]=str[i];
			}
			var_num->val=atof(var_val);
			return *(var_num);
		}
	}
	else
	{
		return evalList(p->down,e);
	}
}
///////////////////////////////////////////////
value evalList(genlistnode *p,env& e)
{
	if (p->t==0)
	{
		if (p->s == "begin"||p->s=="Begin")  
		{
          p=p->next;
          while(p->next != NULL) 
          {
             evalNode(p, e);
             p = p->next;
          }
         return evalNode(p, e);
     }

	if(p->s=="define"||p->s=="Define")
		{
			char x=p->next->s[0];
			value dec_value=evalNode(p->next->next,e);
			add(x,dec_value,e);
			value v;
			v.val=dec_value.val;
			v.tag=0;
			v.link=NULL;
			return v;
	}
	if(p->s=="set"||p->s=="Set")
	{
		char x=p->next->s[0];
		value v=find(x,e);
		value c=evalNode(p->next->next,e);
		v.val=c.val;
		add(x,v,e);
	}
	if(p->s=="+")
	{
		value a=evalNode(p->next,e);
		value b=evalNode(p->next->next,e);
		value *f=new value;
		f->tag=0;
		f->val=a.val+b.val;
		f->link=NULL;
		return *(f);
	}
	if(p->s=="-")
	{
		value a=evalNode(p->next,e);
		value b=evalNode(p->next->next,e);
		value *f=new value;
		f->tag=0;
		f->val=a.val-b.val;
		f->link=NULL;
		return *(f);
	}
	if(p->s=="*")
	{
		value a=evalNode(p->next,e);
		value b=evalNode(p->next->next,e);
		value *f=new value;
		f->tag=0;
		f->val=a.val*b.val;
		f->link=NULL;
		return *(f);
	}
	if(p->s=="/")
	{
		value a=evalNode(p->next,e);
		value b=evalNode(p->next->next,e);
		value *f=new value;
		f->tag=0;
		f->val=a.val/b.val;
		f->link=NULL;
		return *(f);
	}
	if(p->s=="<")
	{
		value a=evalNode(p->next,e);
		value b=evalNode(p->next->next,e);
		value *f=new value;
		f->tag=0;
		if((a.val)<(b.val))
		{
		f->val=1;
	}
		else
		{
		f->val=0;
	}
		f->link=NULL;
		return *(f);
	}
	if(p->s==">")
	{
		value a=evalNode(p->next,e);
		value b=evalNode(p->next->next,e);
		value *f=new value;
		f->tag=0;
		if((a.val)>(b.val))
		{
		f->val=1;
	}
		else
		{
		f->val=0;
	}
		f->link=NULL;
		return *(f);
	}
	if(p->s=="<=")
	{
		value a=evalNode(p->next,e);
		value b=evalNode(p->next->next,e);
		value *f=new value;
		f->tag=0;
		if((a.val)<=(b.val))
		{

		f->val=1;
	}
		else
		{
		f->val=0;
	}
		f->link=NULL;
		return *(f);
	}
	if(p->s==">=")
	{
		value a=evalNode(p->next,e);
		value b=evalNode(p->next->next,e);
		value *f=new value;
		f->tag=0;
		if((a.val)>=(b.val))
		{
		f->val=1;
	}
		else
		{
		f->val=0;
	}
		f->link=NULL;
		return *(f);
	}
	if(p->s=="==")
	{
		value a=evalNode(p->next,e);
		value b=evalNode(p->next->next,e);
		value *f=new value;
		f->tag=0;
		if((a.val)==(b.val))
		{
		f->val=1;
	}
		else
		{
		f->val=0;
	}
		f->link=NULL;
		return *(f);
	}
	if(p->s=="%")
	{
		value a=evalNode(p->next,e);
		value b=evalNode(p->next->next,e);
		value *f=new value;
		f->tag=0;
		int x=int(a.val);
		int y=int(b.val);
		int c=x%y;
		f->val=float(c);
		f->link=NULL;
		return *(f);
	}
	if (p->s=="if"||p->s=="If")
	{
		value v=evalList(p->next->down,e);
		if (v.val==1)
		{
			value f=evalNode(p->next->next,e);
			return f;
		}
		else if (v.val==0)
		{
			value f=evalNode(p->next->next->next,e);
			return f;
		}
	}
	if (p->s=="while"||p->s=="While")
	{
		value v=evalNode(p->next,e);
		value iterate;
		while (v.val==1)
		{
			iterate=evalNode(p->next->next,e);
			v=evalNode(p->next,e);
		}
		return iterate;
	}
	if (p->s == "lambda") 
	{
      value new_val;
      new_val.tag=1;
      new_val.link=p;
      return new_val;
    }
if (p->s.size()==1 && isalpha(p->s[0]))
{
	char c=p->s[0];
	value v= find(c,e);
	env e1;
	e1.parent=&e;
	genlistnode* arg=v.link->next->down;
	while(arg!=NULL)
	{
		char var_in=arg->s[0];
		value in_arg=evalNode(p->next,e);
		add(var_in,in_arg,e1);
		arg=arg->next;
		p=p->next;
	}
	return evalNode(v.link->next->next,e1);
}
}
else if (p->t==1)
{
    env e1;
    e1.parent=&e;
    genlistnode* curr=p;
	genlistnode* q=p->down->next->down;
	while(q!=NULL)
	{
		char var_in=q->s[0];
		value in_arg=evalNode(p->next,e);
		add(var_in,in_arg,e1);
		q=q->next;
		p=p->next;
	}
	return evalNode(curr->down->next->next,e1);	
}
}
