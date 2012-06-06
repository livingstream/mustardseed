#include <iostream>
#include <set>
using namespace std;
size_t strlcpy(char *dst, const char *src, size_t siz);

class mention {
      public:
        char token[50]; // The actual string
        int len;
        int doc; // The identifier for the document (could be a string)
        int para; // The number paragraph in the document
        int word; // The number word in the pargraph
        int pos; // Integer identifier for the part of speech
        int entityId; // entity the mention belongs to
        prefixFeature prefixf;
        mention(char[],int,int,int,int,int,int);
        int pairwiseScore(mention &);
        
};
mention::mention(char[] _token, int length, int doc_id, int para_id, int word_id, int pos_id, int entity_id){
        this->token=s_token;
        this->len=length;
        this->doc=doc_id;
        this->para=para_id;
        this->word=word_id;
        this->pos=pos_id;
        this->entityId=entity_id;
}
class prefixFeature {
	public:
          char firstC;
          char secondC;
          char thirdC;
          prefixFeature(char first, char second, char third){
            firstC=first;
            secondC=second;
            thirdC=third;
          }
          int prefixScore(prefixFeature& other){
            return this->firstC==other.firstC ? (1+(this->secondC==other.secondC ? ((this->thirdC==other.thirdC ? 3 : 0)+2) : -1)): -1
          }
}
int mention::pairwiseScore(mention & other){
  int sumAff=0;
  char charArray1[50]="",charArray2[50]="";
  strlcpy(charArray1,this->token,this->len+1);
  strlcpy(charArray2,other.token,other.len+1);

  string s1=charArray1;
  string s2=charArray2;
  //have the same length  
  this->len==other.len ? sumAff+=3 : sumAff-=0;

  //one string is a substring of the second string
  this->len<=other.len ? (s2.find(s1)!=string::npos ? sumAff+=10 : sumAff-=1) : 
                        (s1.find(s2)!=string::npos ? sumAff+=10 : sumAff-=1);
  const char* split=" ";
  char *saveptr1=NULL,*saveptr2=NULL,*p;

  //split string into tokens
  p=strtok_r(s1,split,&saveptr1);
  //string sp=p;
  s2.find(p)!=string::npos ? sumAff+=4 : sumAff-=0;
  while(p!=NULL){
     p=strtok_r(NULL,split,&saveptr1);
     if(p!=NULL){
        s2.find(p)!=string::npos ? sumAff+=4 : sumAff-=0;
     }
  }

  //split string into tokens
  p=strtok_r(s2,split,&saveptr2);
  //sp=p;
  s1.find(p)!=string::npos ? sumAff+=4 : sumAff-=0;
  while(p!=NULL){
     p=strtok_r(NULL,split,&saveptr2);
     if(p!=NULL){
        s1.find(p)!=string::npos ? sumAff+=4 : sumAff-=0;
     }
  }
  return sumAff;
}


class entity {
      public:
        int id; // An unique identifier for the entity (in consequential)
        set<int> mentionSet; // all the mentions belong to the entity   
        int clusterScore();
};
int entity::clusterScore(){

	return 0;
}

