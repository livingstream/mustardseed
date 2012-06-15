#include <iostream>
#include <assert.h>
#include <algorithm>
#include <map>
#include <cstring>
using namespace std;
#define maxtoken 10
#define maxtokenlen 50
map<string,int>tokenDict;

class prefixFeature {
	public:
          char firstC;
          char secondC;
          char thirdC;
          prefixFeature(){
	    firstC='\0'; secondC='\0'; thirdC='\0';
          }
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
          char tokenArray[maxtoken][maxtokenlen];
          int tokenIntArray[maxtoken];
          char stringL[maxtokenlen];
          int len;
          int token_size;
          substrFeature(){
            int i=0,j=0;
            for(i=0;i<maxtoken;i++){
            	tokenIntArray[i]=0;
            	stringL[i]='\0';
  		for(j=0;j<maxtokenlen;j++)
		tokenArray[i][j]='\0';
            }
            len=0;
            token_size=0;
          }
	  void set(char* tokenS, int length){
            memcpy(stringL,tokenS,strlen(tokenS)+1);
            this->len=length;
            char* token = strtok(tokenS, " ");
            int wordindex=0;
            if(tokenDict.count(token)==0){
              wordindex=tokenDict.size();
              tokenDict.insert(pair<string,int>(token,wordindex));
            } else {
              wordindex=tokenDict.find(token)->second;
            }
            tokenIntArray[0]=wordindex; 
            memcpy(tokenArray[0],token,min(maxtokenlen,(int)strlen(token)+1));
            token_size=1;
            while (token_size<maxtoken && token) {
             token = strtok(NULL, " ");
             if(token){
                int wordindex=0;
                if(tokenDict.count(token)==0){
                   wordindex=tokenDict.size();
                   tokenDict.insert(pair<string,int>(token,wordindex));
                } else {
                   wordindex=tokenDict.find(token)->second;
                }
                tokenIntArray[token_size]=wordindex;

                memcpy(tokenArray[token_size],token,min(maxtokenlen,(int)strlen(token)+1));
                token_size++;
             }
            }
          }
	  int substrScore(substrFeature& other){
              int i=0;
	      //one string is a substring of the second string
              //int sum=this->len>=other.len ? (strstr(this->stringL,other.stringL)==NULL ? -1 : 10) : (strstr(other.stringL,this->stringL)==NULL ? -1 : 10);
              int sum=(strstr(this->stringL,other.stringL)==NULL ? -1 : 10);
              sum+=(strstr(other.stringL,this->stringL)==NULL ? -1 : 10);
              for (i=0;i<this->token_size;i++){
              	sum+=strstr(other.stringL,this->tokenArray[i])==NULL ? -1:10;
              } 
              for (i=0;i<other.token_size;i++){
                sum+=strstr(this->stringL,other.tokenArray[i])==NULL ? -1:10;
              } 
              return sum;
	  }
};

class lengthFeature {
       public: 
	int length;
        lengthFeature(){
	  length=0;
        }
        void set(int len){
          this->length=len;
        }
        int lengthScore(lengthFeature & other){
          return this->length==other.length ? 3 : 0;
        }
};

class mention {
      public:
        char stringL[maxtokenlen]; // The actual string
        int len;
        int doc; // The identifier for the document (could be a string)
        int para; // The number paragraph in the document
        int word; // The number word in the pargraph
        int pos; // Integer identifier for the part of speech
        int entityId; // entity the mention belongs to
        prefixFeature prefixf;
        substrFeature substrf;
        lengthFeature lengthf;
        mention(){
	  int i=0;
          for(i=0;i<maxtokenlen;i++)
	    stringL[i]='\0';
          len=0;doc=0;para=0;word=0;pos=0;entityId=0;
        }
        void set(char*str, int length, int doc_id, int para_id, int word_id, int pos_id, int entity_id){
           memcpy(this->stringL,str,min(maxtokenlen,(int)strlen(str)+1));
           this->len=length;
           this->doc=doc_id;
           this->para=para_id;
           this->word=word_id;
           this->pos=pos_id;
           this->entityId=entity_id;
           char temp[4]={'\0'};
           memcpy(temp,str,3);
           prefixf.set(temp[0],temp[1],temp[2]);
           substrf.set(str,length);
           lengthf.set(length);
        }
        int pairwiseScore(mention& other){
	  return prefixf.prefixScore(other.prefixf)+substrf.substrScore(other.substrf)+lengthf.lengthScore(other.lengthf);
        }
};
