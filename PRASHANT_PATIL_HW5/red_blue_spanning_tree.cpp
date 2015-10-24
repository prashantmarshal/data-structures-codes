#include <iostream>
#include <cstdio>
#include <vector>
using namespace std;

typedef pair<int,int> span;
vector<span> red_edges;
vector<span> blue_edges;
vector<span> span_tree;
vector<int> span_root;
int k,n,num_edges;

void init_tree()
{
 num_edges = 0;
 for(int i=1;i<=n;i++)
  span_root[i]=i;
}

int find_root(int idx)
{
 if(span_root[idx]==idx)
  return idx;
 return span_root[idx] = find_root(span_root[idx]);
}

void set_tree(int val1, int val2)
{
 span_root[find_root(val1)] = find_root(val2);
}

void set_red()
{
 for(int i=0;num_edges<n-1 && i<red_edges.size();i++)
 {
  if(find_root(red_edges[i].first) != find_root(red_edges[i].second))
  {
   set_tree(red_edges[i].first,red_edges[i].second);
   num_edges++;
  }
 }
}

void set_blue()
{
 for(int i=0;num_edges<n-1 && i<blue_edges.size();i++)
 {
  if(find_root(blue_edges[i].first) != find_root(blue_edges[i].second))
  {
   set_tree(blue_edges[i].first,blue_edges[i].second);
   num_edges++;
  }
 }
}

void set_edge()
{
 for(int i=0;num_edges<k && i<red_edges.size();i++)
 {
  if(find_root(red_edges[i].first) != find_root(red_edges[i].second))
  {
   set_tree(red_edges[i].first,red_edges[i].second);
   span_tree.push_back(make_pair(red_edges[i].first,red_edges[i].second));
   num_edges++;
  }
 }

 for(int i=0;num_edges<n-1 &&i<blue_edges.size();i++)
 {
  if(find_root(blue_edges[i].first) != find_root(blue_edges[i].second))
  {
   set_tree(blue_edges[i].first,blue_edges[i].second);
   span_tree.push_back(make_pair(blue_edges[i].first,blue_edges[i].second));
   num_edges++;
  }
 }
}

int main()
{
 cin >> k >> n;
 red_edges.clear();
 blue_edges.clear();
 span_root.resize(n+1);
 for(int i=1;i<=n;i++)
 {
  int vertex,colour;
  while(true)
  {
   cin >> vertex;
   if(vertex==-1)
    break;
   cin >> colour;
   if(i<vertex)
   {
    if(colour==0)
     red_edges.push_back(make_pair(i,vertex));
    else
     blue_edges.push_back(make_pair(i,vertex));
   }
  }
 }
 
 init_tree();
 set_blue();
 int min_k = n-1-num_edges;
 set_red();
 if(num_edges != n-1)
 {
  cout << "NO" << endl;
  return 0;
 }
 init_tree();
 set_red();
 int max_k = num_edges;
 set_blue();
 if(num_edges != n-1)
 {
  cout << "NO" << endl;
  return 0;
 }

 if(k>=min_k && k<=max_k)
  cout << "YES" << endl;
 else
 {
  cout << "NO" << endl;
  return 0;
 }

 while(span_tree.size()!=n-1)
 {
  span_tree.clear();
  init_tree();
  set_edge();
  if(red_edges.size()>k)
   red_edges.erase(red_edges.begin());
  else
   break;
 }
 
 for(int i=0; i<span_tree.size(); i++)
  cout << span_tree[i].first << " " << span_tree[i].second << endl;

 return 0;
}
