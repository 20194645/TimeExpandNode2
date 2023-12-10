#include <iostream>
#include <vector>
#include <utility>
#include <fstream>
#include <regex>
#include <set>
#include "reader.hpp"
#include "shape.hpp"
#include "TimeExpandedNode.hpp"
#include "bai3.hpp"
#include "bai45.hpp"
#include "bai6.hpp"
#include "bai7.hpp"
using namespace std;
std::vector<part> allpart;
ofstream myfile1("Result2.txt");
int main(){
    vector<Shape *> S;
    vector<vector<TimeExpandedNode *>> allTENs;
    vector<TimeExpandedNode *> tempTENs;
    allTENs.push_back(tempTENs);
    vector<Point*> P;
    //ifstream input("/mnt/d/MinGW64/AllParts.txt");
    //input.is_open();
    int counter = 0;
    makeallpart();
    set<string> pointstr;
    map<string,vector<Shape*>> startshape;
    map<string,vector<Shape*>> endshape;
    map<string,TimeExpandedNode*> mapTEN;
    set<string> shapestr;
    for(auto& a : allpart)
    {
        Shape *shape = new Shape();
        //shape->setName(a.name);
        Point *firstPoint = new Point();
        a.Point1.first = std::round(a.Point1.first * 100.0)/ 100.0;
        a.Point1.second = std::round(a.Point1.second * 100.0)/ 100.0;
        firstPoint->setPoint(a.Point1.first,a.Point1.second);
        Point *lastPoint = new Point();
        a.Point2.first = std::round(a.Point2.first * 100.0)/ 100.0;
        a.Point2.second =  std::round(a.Point2.second * 100.0)/ 100.0;
        lastPoint->setPoint(a.Point2.first,a.Point2.second);
        if(!firstPoint->equals(lastPoint)){
          shape->setPoint(firstPoint, lastPoint);
          shape->setName(a.name);
          string coord1 = to_string(firstPoint->x)+" "+to_string(firstPoint->y);
          string coord2 = to_string(lastPoint->x)+" "+to_string(lastPoint->y);
          
          if (pointstr.find(coord1)==pointstr.end()){
            P.push_back(firstPoint);
            pointstr.insert(coord1);
            TimeExpandedNode *n1 = new TimeExpandedNode();
            n1->setTENode(firstPoint);
            allTENs.at(0).push_back(n1);
          }
          if (pointstr.find(coord2)==pointstr.end()){
            P.push_back(lastPoint);
            pointstr.insert(coord2);
            TimeExpandedNode *n1 = new TimeExpandedNode();
            n1->setTENode(lastPoint);
            allTENs.at(0).push_back(n1);
          }
          string shapecoord = coord1+"_"+coord2;
          if (shapestr.find(shapecoord)==shapestr.end()){
            S.push_back(shape);
          }
        }
    }
    for(auto& s : S){
      string firstcoord = to_string(s->start->x)+" "+to_string(s->start->y);
      string lastcoord = to_string(s->end->x)+" "+to_string(s->end->y);
      if (pointstr.find(firstcoord)!=pointstr.end()){
        startshape[firstcoord].push_back(s);
      }
      if(pointstr.find(lastcoord)!=pointstr.end()){
        endshape[lastcoord].push_back(s);
      }
    }
    for (TimeExpandedNode *n : allTENs.at(0))
    {
        string coord = to_string(n->origin->x)+" "+to_string(n->origin->y);
        auto it  = startshape.find(coord);
        if (it!=startshape.end()){
          for (auto& a : it->second){
            n->insertTarget(a);
          }
        }
        auto it1 = endshape.find(coord);
        if (it1!=endshape.end()){
          for (auto& a : it1->second){
            n->insertSource(a);
          }
        }
        mapTEN[coord] = n;
    }
    for (TimeExpandedNode *n : allTENs.at(0))
    {
      for (auto& a : n->srcs){
        string coord = to_string(a.second->start->x)+" "+to_string(a.second->start->y);
        TimeExpandedNode * it = mapTEN[coord];
        a.first = it;
      }
      for (auto& a : n->tgts){
        string coord = to_string(a.second->end->x)+" "+to_string(a.second->end->y);
        TimeExpandedNode * it = mapTEN[coord];
         a.first = it;
      }
      
    }
    map<string,int> checkpointdup;
    for (auto &pair : checkpointdup) {
        pair.second = 0;
    }
    for (TimeExpandedNode *n : allTENs.at(0)){
      string coord =  to_string(n->origin->x)+" "+to_string(n->origin->y);
      assert(n->time == 0);
      checkpointdup[coord]++;
    }
    for (auto &pair : checkpointdup) {
      assert(pair.second == 1);
    }
    assert(checkpointdup.size()==P.size());
    map<string,int> checkshapedup;
    for (auto &pair : checkshapedup) {
        pair.second = 0;
    }
    for (TimeExpandedNode *n : allTENs.at(0)){
      for (pair<TimeExpandedNode *, Shape *> x : n->srcs){
         assert(x.first != nullptr && x.second != nullptr);
         string coord1 = to_string(x.second->start->x)+" "+to_string(x.second->start->y);
         string coord2 = to_string(x.second->end->x)+" "+to_string(x.second->end->y);
         string coord = coord1 + coord2;
         checkshapedup[coord]++;
      }
      for (pair<TimeExpandedNode *, Shape *> x : n->tgts){
        if(x.first == nullptr || x.second == nullptr){
			   cout<<"DEBUYG"<<endl;
		    }
        assert(x.first != nullptr && x.second != nullptr);
        string coord1 = to_string(x.second->start->x)+" "+to_string(x.second->start->y);
        string coord2 = to_string(x.second->end->x)+" "+to_string(x.second->end->y);
        string coord = coord1 + coord2;
        checkshapedup[coord]++;        
      }
    }
    for (auto &pair : checkshapedup) {
        assert(pair.second == 2);
    }
    assert(checkshapedup.size()==S.size());
    map<string,int> checksum;
    for(auto& n : allTENs.at(0)){
      for (pair<TimeExpandedNode *, Shape *> x : n->srcs){
       string coord = to_string(x.first->origin->x)+" "+to_string(x.first->origin->y);
       checksum[coord]++;
      }
      for (pair<TimeExpandedNode *, Shape *> x : n->tgts){
       string coord = to_string(x.first->origin->x)+" "+to_string(x.first->origin->y);
       checksum[coord]++;
      }
    }
    for(auto& pair : checksum){
      TimeExpandedNode * tmp = mapTEN[pair.first];
      int sum = tmp->srcs.size()+ tmp->tgts.size();
      assert(sum == pair.second);
    }
    assert(checksum.size()== allTENs.at(0).size());
    int count = 0;
    int sumTEN = 0;
    vector<int>  initializations = getStartedNodes(allTENs);
    for(int index : initializations){
		  spread(allTENs, 0, index,100);
    }

    //cout<<allTENs.size()<<endl;
    //cout<<newNode->tgts.size()<<endl;
    std::vector<std::pair<int, int>> redundants = filter(allTENs);
    remove2(redundants,allTENs);
    //cout<<redundants.size()<<endl;
    for(auto& it: allTENs)
    {
     assertTime(it,1);
    }
     for(auto& it : allTENs)
    {
     sumTEN = sumTEN+it.size();
    }
    cout<<sumTEN<<endl;
    vector<TimeExpandedNode*> starnode;
    for(auto& it : allTENs)
    {
        for(auto& a: it)
        {
          if(a->srcs.empty())
          {
            starnode.push_back(a);
          }
        }
    }
    for(auto& element : starnode){
	    assignKey(element);
    }
    cout<<counter<<endl;
    cout<<"-------------------------------------------------------"<<endl;
    sumTEN = 0;
    connectAllChains(allTENs,100,15);
     for(auto& it : allTENs)
    {
     sumTEN = sumTEN+it.size();
    }
    cout<<sumTEN<<endl;
    int sum = 0;
    for (auto& it : connect){
      connectChains(allTENs,it);
    }
    //connectAllChains(allTENs,Points,2);
    //(allTENs,P,90);
    cout<<"-------------------------------------------------------"<<endl;
    std:: string filename = "intinerary.txt";
    std::string station="";
    std::map<std::string, std::vector<ArtificialStation*>> mapArtificialStations = getTimeWindows(filename,1,station);
    
    replaceStation(allTENs,station);
    int i,k;
    for( i = 0; i < allTENs.size( ) ; i++){
	  auto& v = allTENs.at(i);
	  for(int j = 0; j < v.size( ); j++){
		auto& temp = v.at(j);
		if(instanceof<Station>(temp)){
			std::string name = temp->name;
			auto foundit = mapArtificialStations.find(name);
			if(foundit != mapArtificialStations.end()){
				auto& allArStations = mapArtificialStations[name];
				for(auto& elem : allArStations){
					elem->createConnection(temp);
				}
			}
		}
	 }
    }
    for (const auto& el : mapArtificialStations){ 
     auto& v = el.second;
	 for(auto& elem : v){
		insert(allTENs, elem);
	 }
    }
    redundants = filter(allTENs);
    remove2(redundants,allTENs);
    
    for(auto& it: allTENs)
    {
     assertTime(it,1);
    }
}