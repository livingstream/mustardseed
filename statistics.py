import os
from collections import Counter
pospath = 'pos/'
negpath = 'neg/'
no_files=1000
topk_token=10000
dic=[]
cnt = Counter()
# to store all the training and testing files
pos_files=[]
neg_files=[]

# positive files
inter=0
listing = (os.listdir(pospath))
for infile in listing:
    pos_files.append(pospath + infile)
    if inter<800:
       f=open(pospath + infile);
       filecontent = f.read()
       for word in filecontent.split():
           cnt[word] += 1
    inter += 1
if inter != no_files:
   while True: print "error" + str(inter)

# negative files
inter=0
listing = (os.listdir(negpath))
for infile in listing:
    neg_files.append(negpath + infile)
    if inter<800:
       f=open(negpath + infile);
       filecontent = f.read()
       for word in filecontent.split():
           cnt[word] += 1
    inter += 1
if inter != no_files:
   while True: print "error" + str(inter)
#print len(postrain_files)
#print len(postest_files)
#print len(negtrain_files)
#print len(negtest_files)

topk_token=cnt.most_common(topk_token)
#print toptoken
total=0
for item in topk_token:
       dic.append(item[0])
    #if cnt[word]>1100:
    #   print word + " " + str(cnt[word])

trainingf=open("training",'w');
testingf=open("testing",'w');
feature=[[ "0" for col in range(len(topk_token))] for row in range(2000)]
fileinter=0

#feature extraction positive training sentences
for infile in pos_files:
    cnt = Counter()
    f=open(infile);
    filecontent = f.read()
    for word in filecontent.split():
        cnt[word] += 1
    #wordarray=filecontent.split()
    wordinter=0
    for dicword in dic:
        if dicword in cnt:
           feature[fileinter][wordinter]="1"
        else:
              feature[fileinter][wordinter]="0"
        wordinter +=1
    fileinter += 1 
for i in range(0,1000):
    if i<800:
             trainingf.write(str(i+1) + "\t1\t{" + ",".join(feature[i]) + "}\n")
    else:
             testingf.write(str(i+1-800) + "\t1\t{" + ",".join(feature[i]) + "}\n")

#negative training sentences
fileinter=1000
for infile in neg_files:
    cnt = Counter()
    f=open(infile);
    filecontent = f.read()
    for word in filecontent.split():
        cnt[word] += 1
    #wordarray=filecontent.split()
    wordinter=0
    for dicword in dic:
        if dicword in cnt:
           feature[fileinter][wordinter]="1"
        else:
              feature[fileinter][wordinter]="0"
        wordinter +=1
    fileinter += 1 
for i in range(1000,2000):
    if i<1800:
             trainingf.write(str(i+1-200) + "\t2\t{" + ",".join(feature[i]) + "}\n");
    else:
             testingf.write(str(i+1-1800+200) + "\t2\t{" + ",".join(feature[i]) + "}\n");
