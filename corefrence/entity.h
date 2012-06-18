#include <iostream>
#include <unordered_set>
using namespace std;
#define clusterPrefixW (10)
#define clusterSameDocW (10)
class clusterPrefixF {
	public:
          bool same_prfeix;
          unordered_set<int>prefixSet;
          int prefix[256];
          clusterPrefixF(){
             memset(prefix,0, sizeof(prefix) * sizeof(int));
          }
          void add(int char_ascii){
	  	prefixSet.insert(char_ascii);
		prefix[char_ascii]++;
          }
          void remove(int char_ascii){
		prefix[char_ascii]--;
                if(prefix[char_ascii]==0) prefixSet.erase(char_ascii);
          }
          int clusterPrefixScore(){
		return prefixSet.size()==1 ? clusterPrefixW : 0;	
          }
};

class clusterSameDocF {
	public:
	  unordered_map<int,int> docMap;
          void add(int doc_id){
	    docMap[doc_id]=docMap[doc_id]+1;
          }
	  void remove(int doc_id){
            docMap[doc_id]=docMap[doc_id]-1;
            if(docMap[doc_id]==0) docMap.erase(doc_id);
          }
          int clusterSameDocScore(){
		return docMap.size()==1 ? clusterSameDocW : 0;	
          }
};

class entity {
      public:
        int id; // An unique identifier for the entity (in consequential)
        unordered_set<int> mentionSet; // all the mentions belong to the entity   
        clusterPrefixF clusterPrefixf;
        clusterSameDocF clusterSameDocf;
        int clusterScore(){
	   return clusterPrefixf.clusterPrefixScore()+clusterSameDocf.clusterSameDocScore();         
        }
};
