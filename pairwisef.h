#include <assert.h>
#include <cstdlib>
#include <cstring>
#include "mcmclib.cpp"
using namespace std;

class PairwiseF {
public:
  int len;
  char *token; 
  PairwiseF(char*);
  int score(PairwiseF &);
};

PairwiseF::PairwiseF(char* token){
   token=token;
   len=strlen(token);
   assert (len<=49);//TODO delete it
}

int PairwiseF::score(PairwiseF & other){
  int sumAff=0;
  char s1[50]="",s2[50]="";
  strlcpy(s1,this->token,this->len+1);
  strlcpy(s2,other.token,other.len+1);

  s1=(string)s1;
  s2=(string)s2;
  bool firstMatch=false, secondMatch=false, thirdMatch=false;

  firstMatch = (s1[0]==s2[0]);
  secondMatch = (s1[1]==s2[1]);
  thirdMatch = (s1[2]==s2[2]);

  // match the the prefix with 1 character
  firstMatch ? sumAff+=1 : sumAff-=1;
  // match the the prefix with 2 character
  firstMatch&secondMatch ? sumAff+=2 : sumAff-=1;
  // match the the prefix with 3 character
  firstMatch&secondMatch&thirdMatch ? sumAff+=3 : sumAff-=0;
  //have the same length  
  this.len==other.len ? sumAff+=3 : sumAff-=0;

  //one string is a substring of the second string
  this.len<=other.len ? (s2.find(s1)!=string::npos ? sumAff+=10 : sumAff-=1) : 
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

