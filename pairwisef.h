class PairwiseF {
public:
  char *token;
  int score(PairwiseF & other){
  int sumAff=0;
  char str1[50]="";
  char str2[50]="";
  int str1Len=strlen(mention1->token);
  int str2Len=strlen(mention2->token);
  if(str1Len>49||str2Len>49){cout<<"exceed 50 chars, exit!<<endl";cin.get();}
  strlcpy(str1,mention1->token,str1Len+1);
  strlcpy(str2,mention2->token,str2Len+1);
  string s1=str1;
  string s2=str2;

  bool firstMatch=false;
  bool secondMatch=false;
  bool thirdMatch=false;

  firstMatch = (str1[0]==str2[0]);
  if(str1Len>=2&&str2Len>=2) secondMatch = (str1[1]==str2[1]);
  if(str1Len>=3&&str2Len>=3) thirdMatch = (str1[2]==str2[2]);

  // match the the prefix with 1 character
  firstMatch ? sumAff+=1 : sumAff-=1;
  // match the the prefix with 2 character
  firstMatch&&secondMatch ? sumAff+=2 : sumAff-=1;
  // match the the prefix with 3 character
  firstMatch&&secondMatch&&thirdMatch ? sumAff+=3 : sumAff-=0;

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
  //string sp=p;
  s2.find(p)!=string::npos ? sumAff+=4 : sumAff-=0;
  while(p!=NULL){
     p=strtok_r(NULL,split,&saveptr1);
     if(p!=NULL){
        s2.find(p)!=string::npos ? sumAff+=4 : sumAff-=0;
     }
  }

  //split string into tokens
  p=strtok_r(str2,split,&saveptr2);
  //sp=p;
  s1.find(p)!=string::npos ? sumAff+=4 : sumAff-=0;
  while(p!=NULL){
     p=strtok_r(NULL,split,&saveptr2);
     if(p!=NULL){
        s1.find(p)!=string::npos ? sumAff+=4 : sumAff-=0;
     }
  }
  return sumAff;

};
