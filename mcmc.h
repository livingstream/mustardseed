#include <iostream>
#include <set>
#include "pariwiseF.h"
#include "clusterF.h"
using namespace std;
struct mentions {
        char token[50]; // The actual string
        int length;
        int doc; // The identifier for the document (could be a string)
        int para; // The number paragraph in the document
        int word; // The number word in the pargraph
        int pos; // Integer identifier for the part of speech
        int entityId; // entity the mention belongs to
        PairwiseF pairwiseF;
};

struct entity {
        int id; // An unique identifier for the entity (in consequential)
        set<int> mentionSet; // all the mentions belong to the entity   
        ClusterF clusterF;
};

