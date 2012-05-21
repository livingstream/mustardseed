#include <iostream>
#include <sstream>
#include <set>
#include <ctime>
#include <cstdlib>
#include <math.h>
#include <vector>
using namespace std;
#define Nmen 29 // number of mentions
#define Niter 20000 // number of iterations
struct mentions {
	char* token; // The actual string
	int doc; // The identifier for the document (could be a string)
	int para; // The number paragraph in the document
	int word; // The number word in the pargraph
	int pos; // Integer identifier for the part of speech
        int entityId; // entity the mention belongs to
};
struct entity {
	int id; // An unique identifier for the entity (in consequential)
        set<int>mentions; // all the mentions belong to the entity   
};

// affinity factor or features
int affinity(mentions* mention1, mentions* mention2){
  int sumAff=0;
  char str1[50]="";
  char str2[50]="";
  int str1Len=strlen(mention1->token);
  int str2Len=strlen(mention2->token);
  strlcpy(str1,mention1->token,str1Len);
  strlcpy(str2,mention2->token,str2Len);
  string s1=str1;
  string s2=str2;
  bool firstMatch = (str1[0]==str2[0]);
  bool secondMatch = (str1[1]==str2[1]);
  bool thirdMatch = (str1[2]==str2[2]);
  
  // match the the prefix with 1 character
  firstMatch ? sumAff+=1 : sumAff-=1;
  // match the the prefix with 2 character
  firstMatch&&secondMatch ? sumAff+=2 : sumAff-=1;
  // match the the prefix with 3 character
  firstMatch&&secondMatch&&thirdMatch ? sumAff+=3 : sumAff-=1; 

  //have the same length  
  str1Len==str2Len ? sumAff+=3 : sumAff-=0;
  
  //one string is a substring of the second string
  str1Len<=str2Len ? (s2.find(s1)!=string::npos ? sumAff+=10 : sumAff-=1) : (s1.find(s2)!=string::npos ? sumAff+=10 : sumAff-=1);
  
  const char* split=" ";
  char *saveptr1=NULL;
  char *saveptr2=NULL;
  char *p;
  
  //split string into tokens
  p=strtok_r(str1,split,&saveptr1);
  string sp=p;
  s2.find(sp)!=string::npos ? sumAff+=4 : sumAff-=0;
  while(p!=NULL){
     p=strtok_r(NULL,split,&saveptr1);
     if(p!=NULL){
        s2.find(p)!=string::npos ? sumAff+=4 : sumAff-=0;
     }
  }
   
  //split string into tokens
  p=strtok_r(str2,split,&saveptr2);
  sp=p;
  s1.find(sp)!=string::npos ? sumAff+=4 : sumAff-=0;
  while(p!=NULL){
     p=strtok_r(NULL,split,&saveptr2);
     if(p!=NULL){
        s1.find(p)!=string::npos ? sumAff+=4 : sumAff-=0;
     }
  }

  return sumAff;
}


int main ()
{
  int i=0,j=0; int currentEntropy=1;
  int affinityArray[Nmen][Nmen];
  mentions* mentionArray = new mentions[Nmen];
  entity* entityArray = new entity[Nmen];
  // Andrew MaCallum
  mentionArray[0].token=strdup("Andrew McCallum");
  mentionArray[1].token=strdup("Andrew MacCallum");
  mentionArray[2].token=strdup("Angrew McCallum");
  mentionArray[3].token=strdup("McCallum");
  mentionArray[4].token=strdup("A. McCallum");
  // Michael Wick
  mentionArray[5].token=strdup("Michael Wick");
  mentionArray[6].token=strdup("Mike Wick");
  mentionArray[7].token=strdup("Michael Andrew Wick");
  mentionArray[8].token=strdup("Wick");
  mentionArray[9].token=strdup("Wick");
  // Khashayar Rohanemanesh
  mentionArray[10].token=strdup("Khashayar Rohanemanesh");
  mentionArray[11].token=strdup("Khash R.");
  mentionArray[12].token=strdup("Kesh Rohanemanesh");
  // Aron Culotta
  mentionArray[13].token=strdup("Aron Culotta");
  mentionArray[14].token=strdup("Andrew Culotta");
  mentionArray[15].token=strdup("A. Culotta");
  mentionArray[16].token=strdup("Culotta McCallum");
  mentionArray[17].token=strdup("Culotta");
  mentionArray[18].token=strdup("Culotta");
  // Charles Sutton
  mentionArray[19].token=strdup("Charles Sutton");
  mentionArray[20].token=strdup("Charles A. Sutton");
  mentionArray[21].token=strdup("Sutton");
  mentionArray[22].token=strdup("Sutton");
  // Nicola Cancceda
  mentionArray[23].token=strdup("Nicola Cancceda");
  mentionArray[24].token=strdup("Nicola Canceda");
  mentionArray[25].token=strdup("Nicolla Cancceda");
  mentionArray[26].token=strdup("Nicol Cancheta");
  mentionArray[27].token=strdup("Canceda");
  mentionArray[28].token=strdup("Cancceda");

  for(i=0;i<Nmen;i++){
      mentionArray[i].entityId=i;
      entityArray[i].mentions.insert(i);
  }
  // calcualte the affinity score 
  for(i=0;i<Nmen;i++){
    for(j=0;j<Nmen;j++){
      if(j>i) {
         cout<<mentionArray[i].token<<"\t"<<mentionArray[j].token<<"\t";
         int score=affinity(&mentionArray[i],&mentionArray[j]);
         affinityArray[i][j]=score;
         affinityArray[j][i]=score;
         cout<<score<<endl;
      } else if (j==i){
         affinityArray[i][j]=0;
         cout<<mentionArray[i].token<<"\t"<<mentionArray[j].token<<"\t"<<0<<endl;
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
    if(entityArray[mentionArray[randomMention].entityId].mentions.size()==1||
       ((double)rand()/(double)RAND_MAX)<=0.8){
       randomEntity=rand()%Nmen;
    } else{ // place it in an empty or create a new entity TODO create a new entity
      vector<int>emptyEntityVector;
      for(i=0;i<Nmen;i++){
          if(entityArray[i].mentions.size()==0){
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
       for(it=entityArray[mentionArray[randomMention].entityId].mentions.begin();it!=
           entityArray[mentionArray[randomMention].entityId].mentions.end();++it){
           loss+=affinityArray[randomMention][*it];
       }
       int gain=0;
       for(it=entityArray[randomEntity].mentions.begin();it!=
           entityArray[randomEntity].mentions.end();++it){
           gain+=affinityArray[randomMention][*it];
       }
       //accept or not
       if(gain>loss){// we should accept it
          accepted+=1;
          //remove the mention from old entity and place it into the new entity
          entityArray[mentionArray[randomMention].entityId].mentions.erase(randomMention);
          entityArray[randomEntity].mentions.insert(randomMention);
          mentionArray[randomMention].entityId=randomEntity;
          currentEntropy=currentEntropy+gain-loss;
       } else {// accept it with a probablity
               if(currentEntropy==0){cout<<"error! devided by 0"; return -1;}
               double ratio=exp(gain-loss);
               double p=((double)rand()/(double)RAND_MAX);
               if(ratio>p){// accept it
                  accepted+=1;
                  //remove the mention from old entity and place it into the new entity
                  entityArray[mentionArray[randomMention].entityId].mentions.erase(randomMention);
                  entityArray[randomEntity].mentions.insert(randomMention);
                  mentionArray[randomMention].entityId=randomEntity;
                  currentEntropy=currentEntropy+gain-loss;
               }
       }
    }
    //cout<<"iteration "<<iter<<" score="<<currentEntropy<<endl;
  }
  cout<<"number of accepted proposals="<<"	"<<accepted<<endl;
  cout<<"number of rejected proposals="<<"	"<<Niter-accepted<<endl;
  
  for(i=0; i<Nmen; i++)
     cout << "mention " <<i<< "	"<<mentionArray[i].entityId<<endl;
  
  return 0;
}
