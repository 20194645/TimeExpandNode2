#include "TimeExpandedNode.hpp"
#include "readfile.hpp"
using namespace std;
class Station :  public TimeExpandedNode{
    public:
    void makestation(TimeExpandedNode* temp, std::string name)
    {
       // this->srcs = temp->srcs;
        this->srcs.clear();
        for (auto& src : temp->srcs)
        {
            this->srcs.push_back(src);
        }
        this->name = name;
        //this->tgts = temp->tgts;
        for (auto& tgt : temp->tgts){
            this->tgts.push_back(tgt);
        }
        this->origin = temp->origin;
        this->time = temp->time;
        for(auto& pr : this->srcs){
            for(auto& pt : pr.first->tgts){
                        if(pt.first == temp){ 
                                 pt.first = this;
                        }
            }
         }
         for(auto& pr : this->tgts){
            if (pr.first!=nullptr)
            {
                for(auto& pt : pr.first->srcs){
                        if(pt.first == temp){ 
                                 pt.first = this;
                        }
                }
            }
        }

    }
};
class ArtificialShape : public PausingShape{
 public:
  ArtificialShape(double time) : PausingShape(time) {}
 //Phương thức getTime được ghi đè như sau
 double getWeight( ) override {
            return this->time;
  }
};
class ArtificialStation : public Station{
    public:
    double bestTime;
    double amplitude;
    double earliness, tardiness;
    ArtificialStation(std::string name, double bestTime, double amplitude) : Station( ){
         this->name = name;
         this->time = bestTime;
         this->amplitude = amplitude;
         this->earliness = bestTime - amplitude;
         this->tardiness = bestTime + amplitude;
    }
    void createConnection(TimeExpandedNode* node) override{
	if(instanceof<Station>(node) && !instanceof<ArtificialStation>(node)){
		if(node->name == this->name){
			//để gọi được hàm max cần dùng thư viện cmath
			auto penaltyT = max(this->earliness - node->time, 0.0 );
            penaltyT = max(penaltyT, node->time - this->tardiness); 
			auto aShape = new ArtificialShape(penaltyT);
			node->tgts.push_back(std::make_pair(this, aShape));
			this->srcs.push_back(std::make_pair(node, aShape));
		}
	 }
    }


};

void replaceStation (std::vector<std::vector<TimeExpandedNode*>> &graph,std::string stations){
         for(int i = 0; i < graph.size(); i++){
                  auto& v = graph.at(i);
                  for(int j = 0; j < v.size(); j++){
                           auto& temp = v.at(j);
                           if(temp->endOfLane()){
                                    auto name = temp->isStation(stations);
                                    if(name!=""){
                                            Station *tmp = new Station();

                                            tmp->makestation(temp,name);
                                            graph.at(i).at(j) = tmp;
                                    }    
                           }
                  }
        } 
         
}
std::map<std::string, std::vector<ArtificialStation*>> getTimeWindows(std::string fileName, double H, std::string &stations){
    std::map<std::string, std::vector<ArtificialStation*>> result; 
    FILE* f =fopen(fileName.c_str(),"r");
    char line[50];
    int i;
    while (!feof(f))
    {
        fscanf(f,"%[^\n]%*c",line);
        //printf("%s\n",line);
        int besttime =getBestTime(line);
        int Amplitude = getAmplitude(line);
        char name[10];
        getName(line,name);
        std::vector<ArtificialStation*> values ;
        int period=getPeriod(line);
        for(int i = 0; i < H; i += period){
		 values.push_back(new ArtificialStation(name, besttime + i, Amplitude));
	    }
        std::string namestr(name);
        if(values.size( ) > 0){
            result.insert({namestr, values});
            stations.append("$" + namestr + "$");
        }
        
    }
    fclose(f);
    return result;
}