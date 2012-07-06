#include <assert.h>
#include <algorithm>
#include <unordered_map>
#include <cstring>
using namespace std;
#define maxtoken 10
#define maxtokenlen 50
#define Nmen 60 // number of mentions
#define Niter 2000000 // number of iterations

class prefixFeature {
public:
    char firstC;
    char secondC;
    char thirdC;
    prefixFeature() {
        firstC='\0';
        secondC='\0';
        thirdC='\0';
    }
    void set(char first, char second, char third) {
        firstC=first;
        secondC=second;
        thirdC=third;
    }
    int prefixScore(prefixFeature& other) {
        return firstC==other.firstC ? (1+(secondC==other.secondC ? ((thirdC==other.thirdC ? 3 : 0)+2) : -1)): -1;
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
    substrFeature() {
        memset(stringL,'\0',maxtokenlen);
        for(int i=0; i<maxtoken; i++) {
            tokenIntArray[i]=0;
            memset(tokenArray[i],'\0',maxtokenlen);
        }
        len=0;
        token_size=0;
    }
    void set(char* tokenS, size_t length);
    int substrScore(substrFeature& other);
};

unordered_map<string,int> substrFeature::tokenDict( {{string("NULL"),0}, {string("NULL"),0}}); // Try to initialize it.

void substrFeature::set(char* tokenS, size_t length) {
    memcpy(stringL,tokenS,length);
    len=length;
    char* token = strtok(tokenS, " ");
    size_t wordindex=0;
    if(tokenDict.count((string)token)==0) {
        wordindex=substrFeature::tokenDict.size();
        substrFeature::tokenDict.insert(pair<string,size_t>(token,wordindex));
    } else {
        wordindex=substrFeature::tokenDict.find(token)->second;
    }
    tokenIntArray[0]=wordindex;
    memcpy(tokenArray[0],token,min(maxtokenlen-1,(int)strlen(token)));
    token_size=1;
    while (token_size<maxtoken && token) {
        token = strtok(NULL, " ");
        if(token) {
            int wordindex=0;
            if(substrFeature::tokenDict.count(token)==0) {
                wordindex=substrFeature::tokenDict.size();
                substrFeature::tokenDict.insert(pair<string,size_t>(token,wordindex));
            } else {
                wordindex=substrFeature::tokenDict.find(token)->second;
            }
            tokenIntArray[token_size]=wordindex;
            memcpy(substrFeature::tokenArray[token_size],token,min(maxtokenlen-1,(int)strlen(token)));
            token_size++;
        }
    }
}

int substrFeature::substrScore(substrFeature& other) {
    int sum=len>=other.len ? (strstr(stringL,other.stringL)==NULL ? -1 : 10) : (strstr(other.stringL,stringL)==NULL ? -1 : 10);
    for(int i=0; i<token_size; i++)
        for(int j=0; j<other.token_size; j++)
            sum+=(tokenIntArray[i]!=other.tokenIntArray[j] ? -1:10);
    return sum;
}

class lengthFeature {
public:
    size_t length;
    lengthFeature() {
        length=0;
    }
    void set(size_t len) {
        length=len;
    }
    int lengthScore(lengthFeature & other) {
        return length==other.length ? 3 : 0;
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
    mention() {
        memset(stringL,'\0',maxtokenlen);
        len=0;
        doc=0;
        para=0;
        word=0;
        pos=0;
        entityId=0;
    }
    void set(char*str, int length, int doc_id, int para_id, int word_id, int pos_id, int entity_id) {
        memcpy(stringL,str,length);
        len=length; doc=doc_id; para=para_id; word=word_id; pos=pos_id; entityId=entity_id;
        char temp[3]= {'\0'};
        memcpy(temp,str,min(3,length));
        prefixf.set(temp[0],temp[1],temp[2]);
        substrf.set(str,length);
        lengthf.set(length);
    }
    int pairwiseScore(mention& other) {
        return prefixf.prefixScore(other.prefixf)+substrf.substrScore(other.substrf)+lengthf.lengthScore(other.lengthf);
    }
};
