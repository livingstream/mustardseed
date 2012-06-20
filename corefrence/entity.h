#include <unordered_set>
#include <assert.h>
#include "mention.h"
using namespace std;
#define clusterPrefixW (10)
#define clusterSameDocW (10)
#define maxtokenlen 50
#define group_size 4
mention mentionArray[Nmen];

class c_sameprefix_f {
public:
    bool same_prfeix;
    unordered_set<size_t>prefixSet;
    int prefix[256];
    c_sameprefix_f() {
        memset(prefix,0, sizeof(prefix) * sizeof(int));
    }
    void add(int char_ascii) {
        prefixSet.insert(char_ascii);
        prefix[char_ascii]++;
    }
    void remove(int char_ascii) {
        prefix[char_ascii]--;
        if(prefix[char_ascii]==0) prefixSet.erase(char_ascii);
    }
    int pre_add_score() {
        return 0;
        //return prefixSet.size()==1 ? clusterPrefixW : 0;
    }
    int pre_remove_score() {
        return 0;
        //return prefixSet.size()==1 ? clusterPrefixW : 0;
    }
};

class c_samedoc_f {
public:
    unordered_map<size_t,size_t> docMap;
    void add(int doc_id) {
        docMap[doc_id]=docMap[doc_id]+1;
    }
    void remove(size_t doc_id) {
        docMap[doc_id]=docMap[doc_id]-1;
        if(docMap[doc_id]==0) docMap.erase(doc_id);
    }
    int pre_add_score() {
        return 0;
        //return docMap.size()==1 ? clusterSameDocW : 0;
    }
    int pre_remove_score() {
        return 0;
    }
};

class c_samepos_f {
public:
    unordered_map<size_t,size_t> posMap;
    void add(int doc_id) {
        posMap[doc_id]=posMap[doc_id]+1;
    }
    void remove(size_t doc_id) {
        posMap[doc_id]=posMap[doc_id]-1;
        if(posMap[doc_id]==0) posMap.erase(doc_id);
    }
    int pre_add_score() {
        //return posMap.size()==1 ? clusterSameDocW : 0;
        return 0;
    }
    int pre_remove_score() {	
	return 0;
    }
};

struct token_count {
    char* token[maxtokenlen];
    size_t count;
};

class entity {
public:
    size_t id; // An unique identifier for the entity (in consequential)
    unordered_set<size_t> mentionSet; // all the mentions belong to the entity
    unordered_set<size_t> othersmentionSet; // all the mentions belong to the entity
    token_count token_freq[group_size];
    unordered_set<size_t> group_set[group_size];
    c_sameprefix_f prefixf;
    c_samedoc_f docf;
    c_samepos_f posf;
    void insert(int mentionId) {
        bool found=false;
        mentionSet.insert(mentionId);
        for(int i=0; i<group_size; i++) {
            if(token_freq[i].count==0) {
                token_freq[i].count=1;
                memset(token_freq[i].token,'\0',maxtokenlen);
                memcpy(token_freq[i].token,mentionArray[mentionId].stringL,mentionArray[mentionId].len);
                assert(group_set[i].size()==0);
                group_set[i].insert(mentionId);
                found=true;
                break;
            } else if (memcmp(mentionArray[mentionId].stringL,token_freq[i].token,maxtokenlen)==0) {
                token_freq[i].count++;
                group_set[i].insert(mentionId);
                found=true;
                break;
            }
        }
        if(!found) othersmentionSet.insert(mentionId);
    }
    int pre_add_score() {
        return prefixf.pre_add_score()+docf.pre_add_score()+posf.pre_add_score();
    }
    int pre_remove_score() {
        return prefixf.pre_remove_score()+docf.pre_remove_score()+posf.pre_remove_score();
    }
    entity() {
        id=0;
        for(int i=0; i<group_size; i++) {
            memset(token_freq[i].token,'\0',maxtokenlen);
            token_freq[i].count=0;
        }
    }
};
