#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <string>
#include <map>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <vector>
#include <stdio.h>
#include <exception>
#include "mcmc.h"
#include "pairwisef.h"
#include "clusterf.h"
#include "mcmclib.cpp"
/* strlcpy based on OpenBSDs strlcpy */
//#include <sys/types.h>
using namespace std;
#define Nmen 3000 // number of mentions
#define Niter 20000 // number of iterations
#define bias 0 // affinity score and replusion score bais
#define nytdatapath "/home/kun/Desktop/nytmentionspy.csv"
//size_t strlcpy(char *dst, const char *src, size_t siz);

int affinityArray[Nmen][Nmen];
mentions mentionArray[Nmen];
entity entityArray[Nmen];

int main ()
{
  int i=0,j=0,currentEntropy=1;
  map<string,int>literalMap;
  // read data from nyt dataset 
  ifstream namefile(nytdatapath);
  string input;
  if(!namefile.is_open()){
     cerr << "Error opening file";
     exit(EXIT_FAILURE);
  }
  int mentionInter=0;
  while(!namefile.eof() && mentionInter<Nmen){
     namefile >> input;
     string word;
     stringstream stream(input);
     i=0;
     while( getline(stream, word, ',') ){
        if(i==0)//extract the integer docid from string
           mentionArray[mentionInter].doc=atoi(word.substr(8,16)+word.substr(17,21));
        else if(i==1)
           mentionArray[mentionInter].para=atoi(word);
        else if(i==2)
           mentionArray[mentionInter].word=atoi(word);
        else if(i==3){
           transform(word.begin(),word.end(),word.begin(),::tolower);
           int word_len = word.size();
           mentionArray[mentionInter].token=new char[word_len+1];//assign the token
           mentionArray[mentionInter].length=word_len;//assgin length
           int dest_entity=0;
           if(literalMap.count(word)==0){
             dest_entity=literalMap.size();
             literalMap.insert(pair<string,int>(word,dest_entity)); 
           } else {
             dest_entity=literalMap.find(word)->second;
           }
           //update entity and mention
           mentionArray[mentionInter].entityId=dest_entity;
           entityArray[dest_entity].mentionSet.insert(mentionInter);
           strcpy(mentionArray[mentionInter].token,word.c_str());
        }
        i++;
     }
     mentionInter++;
  }

  namefile.close();

  // calcualte the affinity score 
  for(i=0;i<Nmen;i++){
    for(j=0;j<Nmen;j++){
      if(j>i) {
         int score=0;
         score=affinity(&mentionArray[i],&mentionArray[j]);
         affinityArray[i][j]=score;
         affinityArray[j][i]=score;
         //if(score>1){
         cout<<mentionArray[i].token<<"\t"<<mentionArray[j].token<<"\t"<<score<<endl;
         //   return 0;
         //}
      } else if (j==i){
         affinityArray[i][j]=0;
         //cout<<mentionArray[i].token<<"\t"<<mentionArray[j].token<<"\t"<<0<<endl;
      }
    }
  }
  //propose a change 
  int iter=0;
  int randomMention=0;
  int randomEntity=0;
  int accepted=0;
  int rejected=0;
  srand((unsigned)time(0));
  while(iter<Niter){
    iter=iter+1;
    randomMention=(rand()%Nmen);//random mention range from 0 to Nmen-1
    randomEntity=-1;
    if(entityArray[mentionArray[randomMention].entityId].mentionSet.size()==1||
       ((double)rand()/(double)RAND_MAX)<=0.8){
       randomEntity=rand()%Nmen;
    } else{ // place it in an empty or create a new entity TODO create a new entity
      vector<int>emptyEntityVector;
      for(i=0;i<Nmen;i++){
          if(entityArray[i].mentionSet.size()==0){
             emptyEntityVector.push_back(i);
          }
      }
      if(emptyEntityVector.size()>0){ 
         int pos=(rand())%(emptyEntityVector.size());
         randomEntity=emptyEntityVector.at(pos);
         emptyEntityVector.clear();
      }
    }
    if(randomEntity!=-1 && randomEntity!=mentionArray[randomMention].entityId){
       set<int>::iterator it;
       int loss=0;
       for(it=entityArray[mentionArray[randomMention].entityId].mentionSet.begin();it!=
           entityArray[mentionArray[randomMention].entityId].mentionSet.end();++it){
           loss+=affinityArray[randomMention][*it];
       }
       int gain=0;
       for(it=entityArray[randomEntity].mentionSet.begin();it!=
           entityArray[randomEntity].mentionSet.end();++it){
           gain+=affinityArray[randomMention][*it];
       }
       //accept or not
       if(gain>loss){// we should accept it
          accepted+=1;
          //remove the mention from old entity and place it into the new entity
          entityArray[mentionArray[randomMention].entityId].mentionSet.erase(randomMention);
          entityArray[randomEntity].mentionSet.insert(randomMention);
          mentionArray[randomMention].entityId=randomEntity;
          currentEntropy=currentEntropy+gain-loss;
       } else {// accept it with a probablity
               if(currentEntropy==0){cout<<"error! devided by 0"; return -1;}
               double ratio=exp(gain-loss);
               double p=((double)rand()/(double)RAND_MAX);
               if(ratio>p){// accept it
                  accepted+=1;
                  //remove the mention from old entity and place it into the new entity
                  entityArray[mentionArray[randomMention].entityId].mentionSet.erase(randomMention);
                  entityArray[randomEntity].mentionSet.insert(randomMention);
                  mentionArray[randomMention].entityId=randomEntity;
                  currentEntropy=currentEntropy+gain-loss;
               }
       }
    }
    cout<<"iteration "<<iter<<" score="<<currentEntropy<<endl;
  }
  cout<<"number of accepted proposals="<<"	"<<accepted<<endl;
  cout<<"number of rejected proposals="<<"	"<<Niter-accepted<<endl;
  
  //for(i=0; i<Nmen; i++)
  //   cout << "mention " <<i<< "	"<<mentionArray[i].entityId<<endl;
   
  for(i=0; i<Nmen; i++) delete[] mentionArray[i].token; 
  
  return 0;
  
}
