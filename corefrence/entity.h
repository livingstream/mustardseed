#include <unordered_set>
using namespace std;
#define clusterPrefixW (10)
#define clusterSameDocW (10)
class clusterPrefixF {
	public:
          bool same_prfeix;
          unordered_set<size_t>prefixSet;
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
	  unordered_map<size_t,size_t> docMap;
          void add(int doc_id){
	    docMap[doc_id]=docMap[doc_id]+1;
          }
	  void remove(size_t doc_id){
            docMap[doc_id]=docMap[doc_id]-1;
            if(docMap[doc_id]==0) docMap.erase(doc_id);
          }
          int clusterSameDocScore(){
		return docMap.size()==1 ? clusterSameDocW : 0;	
          }
};

class entity {
      public:
        size_t id; // An unique identifier for the entity (in consequential)
        unordered_set<size_t> mentionSet; // all the mentions belong to the entity   
        clusterPrefixF clusterPrefixf;
        clusterSameDocF clusterSameDocf;
        int clusterScore(){
	   return clusterPrefixf.clusterPrefixScore()+clusterSameDocf.clusterSameDocScore();         
        }
};
