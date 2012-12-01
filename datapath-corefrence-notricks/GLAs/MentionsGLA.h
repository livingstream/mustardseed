#ifndef _MENTIONS_GLA_H
#define _MENTIONS_GLA_H
// STL includes
#include <map>
#include <vector>
#include <unordered_map>
// DataPath library includes
#include "base/Types/INT.h"
/*  System Description of GLA
 *  GLA_DESC
 *      NAME(</MentionsGLA/>)
 * 	INPUTS(</(doc, INT), (para, INT), (num_word, INT), (mention, VARCHAR), (pos, VARCHAR), (ner, VARCHAR)/>)
 *  	OUTPUTS(</(stringL, HString), (len, INT), (doc, INT), (para, INT), (word, INT), (pos, HString), (ner, HString)/>)
 *      RESULT_TYPE(</multi/>)
 *  END_DESC
 */
#define maxtoken 10
#define maxtokenlen 50
#define pos_len 5
#define ner_len 15

using namespace std;

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
    unordered_map<string,int> tokenDict;
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
void set(char* tokenS, size_t length) {
    memcpy(stringL,tokenS,length);
    len=length;
    char* token = strtok(tokenS, " ");
    size_t wordindex=0;
    if(tokenDict.count((string)token)==0) {
        wordindex=tokenDict.size();
        tokenDict.insert(pair<string,size_t>(token,wordindex));
    } else {
        wordindex=tokenDict.find(token)->second;
    }
    tokenIntArray[0]=wordindex;
    memcpy(tokenArray[0],token,min(maxtokenlen-1,(int)strlen(token)));
    token_size=1;
    while (token_size<maxtoken && token) {
        token = strtok(NULL, " ");
        if(token) {
            int wordindex=0;
            if(tokenDict.count(token)==0) {
                wordindex=tokenDict.size();
                tokenDict.insert(pair<string,size_t>(token,wordindex));
            } else {
                wordindex=tokenDict.find(token)->second;
            }
            tokenIntArray[token_size]=wordindex;
            memcpy(tokenArray[token_size],token,min(maxtokenlen-1,(int)strlen(token)));
            token_size++;
        }
    }
}

int substrScore(substrFeature& other) {
    int sum=len>=other.len ? (strstr(stringL,other.stringL)==NULL ? -1 : 10) : (strstr(other.stringL,stringL)==NULL ? -1 : 10);
    for(int i=0; i<token_size; i++)
        for(int j=0; j<other.token_size; j++)
            sum+=(tokenIntArray[i]!=other.tokenIntArray[j] ? -1:10);
    return sum;
}
};

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


class Mention {
public:
    char stringL[maxtokenlen]; // The actual string
    int len;
    int doc; // The identifier for the document (could be a string)
    int para; // The number paragraph in the document
    int word; // The number word in the pargraph
    char pos[pos_len]; // Integer identifier for the part of speech
    char ner[ner_len];
    int entityId; // entity the mention belongs to
    prefixFeature prefixf;
    substrFeature substrf;
    lengthFeature lengthf;
    Mention() {
        memset(stringL,'\0',maxtokenlen);
        memset(pos,'\0',pos_len);
        memset(ner,'\0',ner_len);
        len=0;
        doc=0;
        para=0;
        word=0;
        entityId=0;
    }
    void set(const INT& doc, const INT& para, const INT& num_word, 
                 const VARCHAR& token, const VARCHAR& pos, const VARCHAR& ner) {
        len = min(50, (int)strlen(token.GetStr())); 
        memcpy(stringL,token.GetStr(),len);
        char temp[3]= {'\0'};
        memcpy(temp,token.GetStr(),min(3,(int)strlen(token.GetStr())));

        this->doc = doc;
        this->para = para;
        this->word = num_word;

        memcpy(this->pos,pos.GetStr(),strlen(pos.GetStr())+1);
        memcpy(this->ner,ner.GetStr(),strlen(ner.GetStr())+1);
        prefixf.set(temp[0],temp[1],temp[2]);
        substrf.set(stringL,len);
        lengthf.set(len);
    }

    int pairwiseScore(Mention& other) {
        return prefixf.prefixScore(other.prefixf)+substrf.substrScore(other.substrf)+lengthf.lengthScore(other.lengthf);
    }
};

class MentionsGLA {
    typedef vector<Mention> MentionVector;
    MentionVector mentionVector;


    MentionVector::iterator outIter;
public:
    MentionsGLA() { }
    ~MentionsGLA() { }
    void AddItem(const INT& doc, const INT& para, const INT& num_word, 
                 const VARCHAR& token, const VARCHAR& pos, const VARCHAR& ner) {
         Mention mention;
         mention.set(doc,para,num_word,token,pos,ner);
         mentionVector.push_back(mention);
    }
    void AddState( MentionsGLA& other ) {
        // Reference to the other map to improve the readibility of the code.
        for(int i=0; i<other.mentionVector.size();i++)
           mentionVector.push_back(other.mentionVector[i]);
    }
    vector<Mention> GetMentions(void) const {
	return mentionVector;
    }

    void Finalize( void ) {
	outIter = mentionVector.begin();
    }
    bool GetNextResult( HString& stringL, INT& len, INT& doc, INT& para, INT& word, HString& pos, HString& ner  ) {
	if( outIter != mentionVector.end() ) {
	    stringL = outIter->stringL;
	    len = outIter->len;
	    doc = outIter->doc;
	    para = outIter->para;
	    word = outIter->word;
            pos = outIter->pos;
            ner = outIter->ner;

	    ++outIter;
	    return true;
	} else {
	    return false;
	}
    }
};
#endif // _MENTIONS_GLA_H
