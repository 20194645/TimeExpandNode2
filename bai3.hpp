#include "TimeExpandedNode.hpp"
using namespace std;
TimeExpandedNode* isAvailable(std::vector<std::vector<TimeExpandedNode*>> graph, Point* origin, double time){
    for(auto& vec : graph){
      if (vec.at(0)->time == time){
        for (auto& it : vec){
          if(it->origin->equals(origin)) return it;
        }
      }
    }
    return nullptr;
}
bool isavailable(std::vector<std::vector<TimeExpandedNode*>> graph, double time, int index){
   int count = 0;
   for(auto& it : graph.at(index))
   {
    if(it->time == time) count++;
   }     
   if(count == graph.at(index).size()) {return true;}
   else{
       index = index + 1;
       return false;
   }
}
void insert(std::vector<std::vector<TimeExpandedNode*>> &graph, TimeExpandedNode *ten)
{
     int count = 0;
     for(auto& it :graph)
     {
       if (it.at(0)->time == ten->time)
        {
            it.push_back(ten);
            return;
        }
     }
    vector<TimeExpandedNode *> tempTEN;
    tempTEN.push_back(ten);
    graph.push_back(tempTEN);
     
} 
void spread(std::vector<std::vector<TimeExpandedNode*>> &graph, int m, int n, double H) {
    map<string,int> checkTENdup;
    for (auto &pair : checkTENdup) {
        pair.second = 0;
    }
    TimeExpandedNode* node = graph[m][n];
    std::queue<TimeExpandedNode*> Q;
    Q.push(node);
    
    while (!Q.empty()) {
        TimeExpandedNode* temp = Q.front();
        Q.pop();
        
        for (auto& pair : temp->tgts) {
            string coord = to_string(pair.first->origin->x)+" "+to_string(pair.first->origin->y);
            if (checkTENdup[coord]!=1){
                checkTENdup[coord]=1;
                Shape* s = pair.second;
                double time = temp->time + s->getWeight();
                if (time < H) {
                Point* origin = s->end;  TimeExpandedNode* n = pair.first;
                TimeExpandedNode* foundItem = isAvailable(graph, origin, time);
                if (foundItem == nullptr) {
                    TimeExpandedNode* newNode = new TimeExpandedNode();
                    newNode->setTENode(origin);
                    newNode->time= time;
                    if (n!= nullptr){
                     for(auto& it : n->tgts){
                      newNode->tgts.push_back(make_pair(it.first,it.second));
                     }
                    }
                    pair.first = newNode;
                    foundItem = newNode;
                 }
                 int index = foundItem->indexInSources(s);
                 if (index != -1) {
                    foundItem->srcs[index].first = temp;
                 }
                 else {
                    foundItem->srcs.push_back(std::make_pair(temp, s));
                 }
                 insert(graph, foundItem); // Gọi hàm đã làm ở câu (d)
                 Q.push(foundItem);
                }
            }
           
        }
    }
    map<string, int>().swap(checkTENdup);
}

std::vector<std::pair<int, int>> filter(std::vector<std::vector<TimeExpandedNode*>> &graph){
    vector<std::pair<int,int>> a;
    int i,k;
    for(i = 0;i<graph.size();i++)
    {
        for(k=0;k<graph[i].size();k++)
        {
           int count  = 0;
           for(auto& it : graph.at(i).at(k)->srcs)
           {
            if(it.first->time+it.second->getWeight() > graph.at(i).at(k)->time){
              count++;
              //a.push_back(make_pair(i,k));
            }
            
           }
           if(count > 0)
            {
             a.push_back(make_pair(i,k));
            }  
        }
    }
    return a;
}  
void remove(vector<pair<int, int>> filters,
            vector<vector<TimeExpandedNode *>> &graph)
{
    int length = filters.size();
    for (int i = length - 1; i >= 0; i--)
    {
        pair<int, int> p = filters.at(i);
        // cout << graph.at(0).at(p.second)->origin->x << " - "
        //      << graph.at(0).at(p.second)->origin->y << " --- " << p.second << endl;

        graph.at(0).erase(graph.at(0).begin() + p.second);
    }
}
void remove2(vector<pair<int, int>> filters,
            vector<vector<TimeExpandedNode *>> &graph){
        int length = filters.size();
        for (int i = length - 1; i >= 0; i--){
            pair<int, int> p = filters.at(i);
            graph.at(p.first).erase(graph.at(p.first).begin()+p.second);
        }
}
std::vector<int> getStartedNodes(std::vector<std::vector<TimeExpandedNode*>> graph)
{
    std::vector<int> a;
    int k ;
    for(auto& it : graph)
    {
        for(k = 0;k<it.size();k++)
        {
            if(it[k]->srcs.empty())
            {
                if(it[k]->time==0)
                {
                    a.push_back(k);
                }
            }
        }
    }
    return a;
}
void assertTime(std::vector<TimeExpandedNode*> graph, double v){
    int stage  = 0;
    for(auto& it : graph)
    {
        int count = 0;
        for(auto& a : it->srcs)
        {
            if(a.first->time + a.second->getWeight() !=  it->time) count++;
        }
        if(count != 0) { //cout<<count<<endl;
            stage =1;}
    }
    if(stage == 1) cout<<"Error assertTime"<<endl;
}