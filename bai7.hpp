#include "TimeExpandedNode.hpp"
using namespace std;
ofstream myfile("Result.txt");
void assignKey(TimeExpandedNode* element, int* autoIncreament){
    if(element == nullptr)
	  {return;}
    if(element->key == -1){
        element->key = *autoIncreament;
        *autoIncreament = *autoIncreament + 1;
    }
    if(element->tgts.empty()){
      return;         
    }
    for(auto& e : element->tgts){
      if (e.first!=nullptr)
      {
       myfile<<e.first->time<<endl;
       assignKey(e.first, autoIncreament);
      }
    }
}
bool checkautoincreament(int autoincreament,int sumten)
{
    if((autoincreament - sumten)==0)
    {
        return true;
    }
    return false;
}
bool checkdup(std::vector<std::vector<TimeExpandedNode*>> graph,int sumten){
  set<int> count;
  for(auto& it : graph){
    for(auto& n : it){
      count.insert(n->key);
    }
  }
  if(count.size()==sumten) return true;
  return false;
}
void writeFile(std::vector<std::vector<TimeExpandedNode*>> graph,int sumten)
{
 int sumedge = 0;
 for(auto& it : graph)
 {
   for(auto& a : it){
    if (a->key!=-1)
    {
     for (auto& src : a->srcs)
     {
      if (src.first!=nullptr)
      {
        if (src.first->key!=-1) sumedge = sumedge +1;
        
        
      }
      
     }
     for (auto& tgt : a->tgts)
     {
      if (tgt.first!=nullptr){
        if (tgt.first->key!=-1)
         sumedge = sumedge+1;
      }
     }
    }
    
     
     
   }
 }
 myfile<<"p"<<" "<<"min"<<" "<<sumten<<" "<<sumedge<<endl;
 for(auto& it : graph){
    for(auto& a : it){
        if (a->name == "E0"&&a->key != -1)
        {
           myfile<<"n "<<a->key<<" 1"<<endl;
        }
        
        /*if (a->srcs.empty())
        {
          for (auto& b : a->tgts){
            if (b.second->name.find("E0")!= std::string::npos){
              //if (b.first!=nullptr && b.first->key!=-1) myfile<<"n "<<b.first->key<<" 1"<<endl;
              myfile<<"n "<<a->key<<" 1"<<endl;
            }
          }
        }*/
    }
 }
 for(auto& it : graph){
    for(auto& a : it){
      if (a->tgts.empty())
      {
       for (auto& c: a->srcs)
       {
         if (c.second->name == "E92")
         {
            //if (c.first!=nullptr) myfile<<"n "<<c.first->key<<" -1"<<endl;
            myfile<<"n "<<a->key<<" -1"<<endl;
         }
       }
      }
      
      
      
    }
 }
 
        
 for(auto& it : graph)
 {
    for(auto& a : it)
    {
      if (a->key!=-1)
      {
       for(auto& b : a->srcs)
       {
        if (b.first!=nullptr&&b.first->key!=-1) myfile<<"a "<<b.first->key<<" "<<a->key<<" 0 1 "<<(a->time - b.first->time)<<endl;
       }
       for(auto& c : a->tgts){
        if (c.first!=nullptr&&c.first->key!=-1) myfile<<"a "<<a->key<<" "<<c.first->key<<" "<<"0 1 "<<c.first->time - a->time<<endl;
       }
      }
    }
 }
}
void writefile2(std::vector<std::vector<TimeExpandedNode*>> graph){
  for(auto& it : graph)
  {
    for(auto& a : it)
    {
      for(auto& b : a->srcs)
      {
        if (b.first!=nullptr) myfile<<b.second->name<<",";
      }
      myfile<<"||";
      myfile<<a->name<<"||";
      for(auto& c : a->tgts){
        if (c.first!=nullptr) myfile<<c.second->name<<" ";
      }
      myfile<<endl;
    }
  }
}

