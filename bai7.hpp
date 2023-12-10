#include "TimeExpandedNode.hpp"
using namespace std;
ofstream myfile("Result.txt");
int counter = 0;
void assignKey(TimeExpandedNode* element){
   std::queue<TimeExpandedNode*> Q;
   Q.push(element);
   while(!Q.empty()){
      TimeExpandedNode* temp = Q.front();
      Q.pop();
      if (temp!=nullptr && temp->key == -1)
      {
         temp->key = counter;
         counter = counter+1;
      }
      for (auto& pair : temp->tgts){
         Q.push(pair.first);
       }
   }
}
bool checkautoincreament(int autoincreament,int sumten)
{
    if((autoincreament - sumten)==1)
    {
        return true;
    }
    return false;
}
/*bool checkduplicate(std::vector<std::vector<TimeExpandedNode*>> graph,int sumten){
  x = new int[sumten];
  memset(x,0,sizeof(int) * sumten);
  int i;
  int count = 0;
  for(auto& it : graph){
    for(auto& a : it){
        if(x[a->key]==0){
           x[a->key] = 1;
        }
        else
        {
            cout<<a->key<<endl;
        }
        
    }
  }
  /*for ( i = 0; i < sumten; i++){
    if(x[i]!=1){
        return false;
        //count++;
    } 
  }*/
  //return true;
//}
void writeFile(std::vector<std::vector<TimeExpandedNode*>> graph,int sumten)
{
 int sumedge = 0;
 for(auto& it : graph)
 {
   for(auto& a : it){
     sumedge = a->srcs.size()+sumedge+a->tgts.size();
   }
 }
 myfile<<"p"<<" "<<"min"<<" "<<sumten<<" "<<sumedge<<endl;
 for(auto& it : graph){
    for(auto& a : it){
        for (auto& b : a->srcs){
            if (b.second->name.find("E0")!= std::string::npos){
                myfile<<"n "<<b.first->key<<" 1"<<endl;
            }
        }
    }
 }
 for(auto& it : graph){
    for(auto& a : it){
      for (auto& c: a->tgts)
      {
         if (c.second->name.find("E92")!= std::string::npos)
         {
              myfile<<"n "<<c.first->key<<" -1"<<endl;
         }
      }
    }
 }
 
        
 for(auto& it : graph)
 {
    for(auto& a : it)
    {
      for(auto& b : a->srcs)
      {
        myfile<<"a "<<b.first->key<<" "<<a->key<<" 0 1 "<<100*a->time<<endl;
      }
      for(auto& c : a->tgts){
        myfile<<"a "<<a->key<<" "<<c.first->key<<" "<<"0 1 "<<100*c.first->time<<endl;
      }
    }
 }
}
void writefile2(std::vector<std::vector<TimeExpandedNode*>> graph){
   for(auto& it : graph)
   {
    for(auto& a : it)
    {
      for(auto& b : a->srcs){
        myfile<<b.second->name<<" ";
      }
      myfile<<"||";
      myfile<<a->origin->x<<" "<<a->origin->y<<"||";
      if (a->tgts.empty())
      {
        myfile<<endl;
      }
      
      for(auto& c : a->tgts){
        myfile<<c.second->name<<" ";
      }
      myfile<<endl;
    }
   }
}

