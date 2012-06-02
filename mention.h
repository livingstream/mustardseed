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
        substrFeature substrf;
        lengthFeature lengthf;
        mention(char[] stringL, int length, int doc_id, int para_id, int word_id, int pos_id, int entity_id){
           this->token=stringL;
           this->len=length;
           this->doc=doc_id;
           this->para=para_id;
           this->word=word_id;
           this->pos=pos_id;
           this->entityId=entity_id;
        }
};
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

class substrFeature {
	public:
          char tokenArray[5][50];
          char stringL[50];
          int len;
          int token_size;
	  substrFeature(char* stringL, char** token, int size){
            this->stringL=stringL;
	    this->tokenArray=token;
	    this->token_size=size;
          }
	  int substrScore(substrFeature& other){
              int i=0;
	      //one string is a substring of the second string
              int sum=this->len>=other.len ? (strstr(*stringL,other.stringL)==NULL ? -1 : 10) : (strstr(other.stringL,*stringL)==NULL ? -1 : 10);
              for (i=0;i<this->token_size;i++){
              	sum+=tokenArray[i]->len>=other.len ? (strstr(*tokenArray[i],other.stringL)==NULL ? -1 : 10) : (strstr(other.stringL,*tokenArray[i])==NULL ? -1 : 10);
              } 
              for (i=0;i<other.token_size;i++){
              sum+=this->len>=other.tokenArray[i] ? (strstr(this->stringL,other.tokenArray[i])==NULL?-1:10) : (strstr(other.tokenArray[i],this->stringL)==NULL? -1 : 10);
              } 
              return sum;
	  }
}

class lengthFeature {
       public: 
	int length;
        lengthFeature(int len){
          this->length=len;
        }
        int lengthScore(lengthFeature & other){
          return this->length==other.length ? sumAff+=3 : sumAff-=0;
        }
      
}
