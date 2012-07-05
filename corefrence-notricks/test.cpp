#include <iostream>
#include <sstream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include <string>
#include <unordered_map>
#include <list>
#include <ctime>
#include <time.h>
#include <cstdlib>
#include <math.h>
#include <stdio.h>
#include <exception>
#include <assert.h>
using namespace std;

int main ()
{
  unordered_map<string,int> myset;
  char* x="abc";
  if(myset.count(x)==0)
    cout<<"no";
  return 0;
  
}
