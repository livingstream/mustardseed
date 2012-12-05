import os
import nltk.data
path="/home/kun/nytdataset/nyt_eng"
output=open("/home/kun/mustardseed/nyttokenizer/nyt_eng_sentenced",'w')
pindicator=False
linecount=0
oneline=""
flag=False
listing = os.listdir(path)
for infile in listing:
        print "filename="+infile +" count="+str(linecount)
        if flag:
           break
        f=open(path+"/"+infile)
	for line in f:
	    line=line.lower().strip()
	    if line.find("</p>") != -1:
               tokenizer = nltk.data.load('tokenizers/punkt/english.pickle')
               senArray= tokenizer.tokenize(oneline)
	       pindicator = False
               linecount=linecount+len(senArray)
               for sen in senArray:
                   output.write(sen.strip()+"\n")
               if(linecount>8000050):
               #if(linecount>50):
                 flag=True
                 break
	    if pindicator == True:
	       oneline=oneline+" "+line.strip()
	    if line.find("<p>") != -1:
	       pindicator = True
               oneline=""
        f.close()
