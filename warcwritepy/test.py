import warc

fileob = open("test.header","r")
http="http"
start=0
while True:
	ch=fileob.read(1)
	if ch=='h':
		break;
	start=start+1
position=fileob.seek(start)
#print fileob.read()
#word=data.split()
#print word[3].index(http)
data=fileob.readlines()

for line in data:
	words=line.split()
	if(len(words)>1):
		if words[0]=='Content-Type:':
			print words[1]
			break
fileob.seek(start)
#print fileob.read()
fo = open("test.txt", "w")
for lines in fileob.readlines():
	fo.write(lines)
data=open("test.data","r")
for lines in data.readlines():
	fo.write(lines)
fileob.close()
fo.close()
fo=open("test.txt","r")
f = warc.open("test.warc.gz","w")
header = warc.WARCHeader({"WARC-Type": "response"}, defaults=True)
linet=''
for lines in fo.readlines():
	linet+=lines
record1 = warc.WARCRecord(header,linet)
fo.close()


fileobimage = open("image.header","r")
start=0
while True:
	ch=fileobimage.read(1)
	if ch=='h':
		break;
	start=start+1
position=fileobimage.seek(start)
data=fileobimage.readlines()

for line in data:
	words=line.split()
	if(len(words)>1):
		if words[0]=='Content-Type:':
			print words[1]
			break
fileobimage.seek(start)
fo = open("testimage.txt", "w")
for lines in fileobimage.readlines():
	fo.write(lines)
image=open("image.data","r")
for lines in image.readlines():
	fo.write(lines)
fileobimage.close()
fo.close()
fo=open("testimage.txt","r")
f = warc.open("test.warc.gz","w")
headerimage = warc.WARCHeader({"WARC-Type": "response"}, defaults=True)
lineimage=''
for lines in fo.readlines():
	lineimage+=lines
record2 = warc.WARCRecord(headerimage,lineimage)




f.write_record(record1)
f.write_record(record2)
f.close()
fo.close()
