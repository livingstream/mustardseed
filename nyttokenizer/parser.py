f=open("/home/kun/nytdataset/nyt_eng/nyt_eng_199407")
output=open("/home/kun/mustardseed/nyttokenizer/nyt_eng_tokenized",'w')
pindicator=False
linecount=0
oneline=""
for line in f:
    line=line.lower().strip()
    if line.find("</p>") != -1:
       linecount=linecount+1
       output.write(oneline.strip()+"\n");
       print oneline
       pindicator = False
    if pindicator == True:
       oneline=oneline+" "+line
    if line.find("<p>") != -1:
       pindicator = True
       oneline=""
print linecount
