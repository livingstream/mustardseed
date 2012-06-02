#include <iostream>
#include <set>
using namespace std;

class entity {
      public:
        int id; // An unique identifier for the entity (in consequential)
        set<int> mentionSet; // all the mentions belong to the entity   
        int clusterScore();
};
int entity::clusterScore(){

        return 0;
}

