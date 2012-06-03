#include <iostream>
using namespace std;

class prefixFeature {
	public:
          char firstC;
          char secondC;
          char thirdC;
          void set(char first, char second, char third){
            firstC=first;
            secondC=second;
            thirdC=third;
          }
          int prefixScore(prefixFeature& other){
            return this->firstC==other.firstC ? (1+(this->secondC==other.secondC ? ((this->thirdC==other.thirdC ? 3 : 0)+2) : -1)): -1;
          }
};

class substrFeature {
	public:
          char tokenArray[5][50];
          int tokenLen[5];
          char stringL[50];
          int len;
          int token_size;
	  void set(char* tokenS, int length){
            memcpy(stringL,tokenS,strlen(tokenS)+1);
            this->len=length;
            char* token = strtok(tokenS, " ");
            memcpy(tokenArray[0],token,strlen(token)+1);
            token_size=1;
            while (token) {
             token = strtok(NULL, " ");
             if(token){
                memcpy(tokenArray[token_size],token,strlen(token)+1);
                token_size++;
             }
            }
          }
	  int substrScore(substrFeature& other){
              int i=0;
	      //one string is a substring of the second string
              int sum=this->len>=other.len ? (strstr(this->stringL,other.stringL)==NULL ? -1 : 10) : (strstr(other.stringL,this->stringL)==NULL ? -1 : 10);
              for (i=0;i<this->token_size;i++){
              	sum+=this->tokenLen[i]>=other.len ?  (strstr(this->tokenArray[i],other.stringL)==NULL ? -1 : 10) : 
                                                     (strstr(other.stringL,this->tokenArray[i])==NULL ? -1 : 10);
              } 
              for (i=0;i<other.token_size;i++){
                sum+=this->len>=other.tokenLen[i] ?  (strstr(this->stringL,other.tokenArray[i])==NULL ? -1:10) : 
                                                     (strstr(other.tokenArray[i],this->stringL)==NULL? -1 : 10);
              } 
              return sum;
	  }
};

class lengthFeature {
       public: 
	int length;
        void set(int len){
          this->length=len;
        }
        int lengthScore(lengthFeature & other){
          return this->length==other.length ? 3 : 0;
        }
};

class mention {
      public:
        char stringL[50]; // The actual string
        int len;
        int doc; // The identifier for the document (could be a string)
        int para; // The number paragraph in the document
        int word; // The number word in the pargraph
        int pos; // Integer identifier for the part of speech
        int entityId; // entity the mention belongs to
        prefixFeature prefixf;
        substrFeature substrf;
        lengthFeature lengthf;
        void set(char*str, int length, int doc_id, int para_id, int word_id, int pos_id, int entity_id){
           memcpy(this->stringL,str,strlen(str)+1);
           this->len=length;
           this->doc=doc_id;
           this->para=para_id;
           this->word=word_id;
           this->pos=pos_id;
           this->entityId=entity_id;
           char temp[4];
           memcpy(temp,str,3);
           prefixf.set(temp[0],temp[1],temp[2]);
           substrf.set(str,length);
           lengthf.set(length);
        }
        int pairwiseScore(mention& other){
	  return prefixf.prefixScore(other.prefixf)+substrf.substrScore(other.substrf)+lengthf.lengthScore(other.lengthf);
        }
};
