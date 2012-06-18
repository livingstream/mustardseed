#include <iostream>
#include <assert.h>
#include <algorithm>
#include <unordered_map>
#include <cstring>
using namespace std;
#define maxtoken 10
#define maxtokenlen 50

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
          static unordered_map<string,int> tokenDict;
          char tokenArray[maxtoken][maxtokenlen];
          int tokenIntArray[maxtoken];
          char stringL[maxtokenlen];
          int len;
          int token_size;
          substrFeature(){
            int i=0,j=0;
            memset(stringL,'\0',maxtokenlen);
            for(i=0;i<maxtoken;i++){
            	tokenIntArray[i]=0;
                memset(tokenArray[i],'\0',maxtokenlen);
            }
            len=0;
            token_size=0;
          }
	  void set(char* tokenS, int length);
	  int substrScore(substrFeature& other);
};

unordered_map<string,int> substrFeature::tokenDict;

void substrFeature::set(char* tokenS, int length){
	memcpy(stringL,tokenS,strlen(tokenS)+1);
	this->len=length;
	char* token = strtok(tokenS, " ");
	cout<<token<<endl;
	cout<<"size"<<tokenDict.size()<<endl;
	int wordindex=0;
	if(tokenDict.count((string)token)==0){
		cout<<"set21"<<endl;
		wordindex=substrFeature::tokenDict.size();
		substrFeature::tokenDict.insert(pair<string,int>(token,wordindex));
	} else {
		wordindex=substrFeature::tokenDict.find(token)->second;
	}
	tokenIntArray[0]=wordindex;
	memcpy(tokenArray[0],token,min(maxtokenlen,(int)strlen(token)+1));
	token_size=1;
	while (token_size<maxtoken && token) {
		token = strtok(NULL, " ");
		if(token){
			int wordindex=0;
			if(substrFeature::tokenDict.count(token)==0){
				wordindex=substrFeature::tokenDict.size();
				substrFeature::tokenDict.insert(pair<string,int>(token,wordindex));
			} else {
				wordindex=substrFeature::tokenDict.find(token)->second;
			}
			tokenIntArray[token_size]=wordindex;

			memcpy(substrFeature::tokenArray[token_size],token,min(maxtokenlen,(int)strlen(token)+1));
			token_size++;
		}
	}
}

int substrFeature::substrScore(substrFeature& other){
	int i=0,j=0;
	//one string is a substring of the second string
	int sum=this->len>=other.len ? (strstr(this->stringL,other.stringL)==NULL ? -1 : 10) : (strstr(other.stringL,this->stringL)==NULL ? -1 : 10);
	for(i=0;i<this->token_size;i++)
		for(j=0;j<other.token_size;j++)
			sum+=(this->tokenIntArray[i]!=other.tokenIntArray[j] ? -1:10);
	return sum;
}

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
           char temp[4]={'\0','\0','\0','\0'};
           prefixf.set(temp[0],temp[1],temp[2]);
           substrf.set(str,length);
           lengthf.set(length);
        }
        int pairwiseScore(mention& other){
	  return prefixf.prefixScore(other.prefixf)+substrf.substrScore(other.substrf)+lengthf.lengthScore(other.lengthf);
        }
};
