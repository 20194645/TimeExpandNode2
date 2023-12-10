#include "TimeExpandedNode.hpp"
using namespace std;
std::vector<std::vector<std::pair<int,int>>> connect;
class PausingShape : public Shape
{
public:
    PausingShape( ){ d = 0; name = "artificial"; time = 0;}
    PausingShape(double time) {
        this->time = time;
                name = "artificial"; 
    }
    PausingShape(Shape *s, std::string name){
        this->name = name;
        this->start = s->start;
        this->end = s->end;
        this->d = s->d;
        this->time = s->getWeight();
    }
    //Phương thức getTime được ghi đè như sau
    double getTime( ){
        return this->time;
    }

};
std::map<std::string,std::vector<std::pair<int,int>>> getchains(std::vector<std::vector<TimeExpandedNode*>> graph) {
    std::map<std::string,std::vector<std::pair<int,int>>> result;
    int i,j;
    for (i=0;i<graph.size();i++) {
        for (j =0;j<graph.at(i).size();j++) {
            TimeExpandedNode* TEN = graph.at(i).at(j);
            std::string coord = std::to_string(TEN->origin->x)+" "+std::to_string(TEN->origin->y);
            result[coord].push_back(make_pair(i,j)); // Corrected 'pushback' to 'push_back'
        }
    }
    return result;
}
std::vector<std::pair<double, TimeExpandedNode*>> makenewchains(std::vector<TimeExpandedNode*> chain, double H, double delta) {
    std::vector<std::pair<double, TimeExpandedNode*>> result;

    int i = 0;
    auto& root = chain.at(0);
    while (i < chain.size() - 1) {
        auto& pre = chain.at(i);
        auto& next = chain.at(i + 1);
        int steps = floor((next->time - pre->time) / delta);

        for (int k = 0; k < steps; ++k) {
            TimeExpandedNode* newNode = new TimeExpandedNode();
            newNode->setTENode(root->origin);
            newNode->time = pre->time + delta * (k + 1);
            //newNode->tgts = pre->tgts;
            for(auto& it : root->tgts){
                      newNode->tgts.push_back(make_pair(nullptr,it.second));
            }
            result.push_back(std::make_pair(newNode->time, newNode));
        }
        ++i;
    }

    auto& last = chain.back(); // Accessing the last element of 'chain'
    int steps = floor((H - last->time) / delta);
    for (int j = 0; j < steps; ++j) {
        TimeExpandedNode* newNode = new TimeExpandedNode();
        newNode->setTENode(root->origin);
        newNode->time = last->time + delta * (j + 1);
        //newNode->tgts = last->tgts;
        for(auto& it : root->tgts){
            newNode->tgts.push_back(make_pair(nullptr,it.second));
        }
        result.push_back(std::make_pair(newNode->time, newNode));
    }

    return result;
}

std::vector<std::pair<int, int>> insert2(std::vector<std::vector<TimeExpandedNode*>> &graph,std::vector<std::pair<double,TimeExpandedNode*>> newchains){
 std::vector<std::pair<int, int>> result;
 int i,k;
 for (auto& newchain : newchains){
  int size = graph.size();
  if (newchain.first > graph[size-1].at(0)->time)
  {
    std::vector<TimeExpandedNode*> newTEN;
    newTEN.push_back(newchain.second);
    graph.push_back(newTEN);
    result.push_back(make_pair(graph.size()-1,0));
  }
  else{
    int count = 0;
    for (i = 0;i<size;i++){
     if (newchain.first==graph.at(i).at(0)->time){
      graph.at(i).push_back(newchain.second);
      result.push_back(make_pair(i,graph.at(i).size()-1));
      count++;
     }
    }
    if (count == 0){
      i = 0;
      while(0==0){
        if(newchain.first<graph.at(i).at(0)->time){
          break;
        }
        i++;
      }
      vector<TimeExpandedNode*> newTEN;
      graph.insert(graph.begin()+i,1,newTEN);
    }
    
  }
 }
 return result;
} 
bool checkInsertion(std::vector<std::vector<TimeExpandedNode*>> graph, 
	std::vector<std::pair<double,TimeExpandedNode*>> newChains){
	for(auto& elem : newChains){
		if(isAvailable(graph,elem.second->origin, elem.first)!=nullptr){
			return true;
		}
	}
	return false;
}
bool checkresult(std::vector<std::vector<TimeExpandedNode*>> graph,std::vector<std::pair<int, int>> newpos,
std::vector<std::pair<double,TimeExpandedNode*>> newChains){
  int i;
  for ( i = 0; i < newpos.size() ; i++)
  {
    if (!graph.at(newpos.at(i).first).at(newpos.at(i).second)->origin->equals(newChains.at(i).second->origin)||
    graph.at(newpos.at(i).first).at(newpos.at(i).second)->time!=newChains.at(i).first)
    {
      return false;
    }
  }
  return true;
}
void spread2(std::vector<std::vector<TimeExpandedNode*>> &graph, int m, int n, double H){
    TimeExpandedNode* node = graph[m][n];
    std::queue<TimeExpandedNode*> Q;
    Q.push(node);
    while (!Q.empty()){
        TimeExpandedNode* temp = Q.front();
        Q.pop();
        for (auto& pair : temp->tgts) {
        Shape* s = pair.second;
        double time = temp->time + s->getWeight();
        if (time < H) {
            Point* origin = s->end;  TimeExpandedNode* n = pair.first;
            TimeExpandedNode* foundItem = isAvailable(graph, origin, time);
            if (foundItem == nullptr) {
                TimeExpandedNode* newNode = new TimeExpandedNode();
                newNode->setTENode(origin);
                newNode->time= time;
                if (n!= nullptr)
                {
                 for(auto& it : n->tgts){
                    newNode->tgts.push_back(make_pair(nullptr,it.second));
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
std::vector<std::pair<int,int>> merge(std::vector<std::pair<int,int>> oldchain,std::vector<std::pair<int, int>> newpos,
std::vector<std::vector<TimeExpandedNode*>> graph){
 std::vector<std::pair<int,int>> result;
 int oldsize = oldchain.size()-1;
 int newsize = newpos.size()-1;
 int i=0,j=0;
 while (i<oldsize&&j<newsize){
  if (graph[oldchain[i].first][oldchain[i].second]->time<graph[newpos[j].first][newpos[j].second]->time){
    result.push_back(make_pair(oldchain[i].first,oldchain[i].second));
    i++;
  }
  else if(graph[oldchain[i].first][oldchain[i].second]->time>graph[newpos[j].first][newpos[j].second]->time){
    result.push_back(make_pair(oldchain[i].first,oldchain[i].second));
    j++;
  }
 }
 while (i<=oldsize){
  result.push_back(make_pair(oldchain[i].first,oldchain[i].second));
  i++;
 }
 while (j<=newsize){
  result.push_back(make_pair(newpos[j].first,newpos[j].second));
  j++;
 }
 return result;
}
void connectChains(std::vector<std::vector<TimeExpandedNode*>> &graph,std::vector<std::pair<int, int>> newOrder){
  int i;
  for(i=0;i<newOrder.size()-1;i++){
    TimeExpandedNode *prev = graph.at(newOrder[i].first).at(newOrder[i].second);
    TimeExpandedNode *next = graph.at(newOrder[i+1].first).at(newOrder[i+1].second);
    PausingShape* s0 = new PausingShape(next->time-prev->time);
    s0->start = prev->origin;
    s0->end = next->origin;
    prev->tgts.push_back(make_pair(next,s0));
    next->srcs.push_back(make_pair(prev,s0));
  }
}
void connectAllChains(std::vector<std::vector<TimeExpandedNode*>> &graph,double H,double delta)
{
    map<string,vector<std::pair<int,int>>> oldchain = getchains(graph);
    
    for (auto& pair : oldchain){
      std::vector<TimeExpandedNode*> list;
      for (auto& it : pair.second){
       list.push_back(graph.at(it.first).at(it.second));
      }
      std::vector<std::pair<double, TimeExpandedNode*>> newchains = makenewchains(list,H,delta);
      std::vector<std::pair<int, int>> newpos = insert2(graph,newchains) ; 
      if(!newchains.empty()){
        assert(checkInsertion(graph,newchains));
      }
      assert(checkresult(graph,newpos,newchains));
      for(auto& pos : newpos){
        spread2(graph,pos.first,pos.second,H);
      } 
      std::vector<std::pair<int,int>> neworder = merge(pair.second,newpos,graph);
      connect.push_back(neworder); 
    }
    
}