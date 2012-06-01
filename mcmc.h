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
        int pairwiseScore(mention &);
        
};
int mention::pairwiseScore(mention & other){
  int sumAff=0;
  char charArray1[50]="",charArray2[50]="";
  strlcpy(charArray1,this->token,this->len+1);
  strlcpy(charArray2,other.token,other.len+1);

  string s1=charArray1;
  string s2=charArray2;
  bool firstMatch=false, secondMatch=false, thirdMatch=false;

  firstMatch =  (this->token[0]==other.token[0]);
  secondMatch = (this->token[1]==other.token[1]);
  thirdMatch =  (this->token[2]==other.token[2]);

  // match the the prefix with 1 character
  firstMatch ? sumAff+=1 : sumAff-=1;
  // match the the prefix with 2 character
  firstMatch&secondMatch ? sumAff+=2 : sumAff-=1;
  // match the the prefix with 3 character
  firstMatch&secondMatch&thirdMatch ? sumAff+=3 : sumAff-=0;
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

