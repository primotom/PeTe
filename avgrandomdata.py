

def printthis(count,model,curcurry,method,sat,time,Expanded,Explored,Path,fired,Removed):
	print model + "," + curCurry + "," + method + "," + sat + "," + str(time / count) + "," + str(Expanded / count) + "," + str(Explored / count) + "," + str(Path/count) + "," + str(fired / count) + "," + str(Removed / count)

f = open("MoreRandomTests.csv")


i = 0


curCurry = ""
method = ""
model = ""
count = 0
time = 0.0
Expanded= 0.0
Explored =0.0
Path = 0.0
fired = 0.0
Removed = 0.0

for line in f:
	if(i > 0):
		
		line.strip()
		lSp = line.split(",")
		if (curCurry != lSp[1] or method != lSp[2] or model != lSp[0]):
			#begin new table	

			if (i>1 and count > 0):
				printthis(count,model,curCurry,method,sat,time,Expanded,Explored,Path,fired,Removed)
			
			
			time = 0.0
			Expanded= 0.0
			Explored =0.0
			Path = 0.0
			fired = 0.0
			Removed = 0.0
			count = 0


			curCurry = lSp[1]
			method = lSp[2]
			model = lSp[0]



			
		if(lSp[4].find("-") == -1):
			time = time + float(lSp[4])
			Expanded = Expanded + float(lSp[5])
			Explored = Explored + float(lSp[6])
			Path = Path + float(lSp[7])
			fired = fired + float(lSp[8])
			Removed = Removed + float(lSp[9])


			count += 1
		sat = lSp[3]

		
		
	i += 1
printthis(count,model,curCurry,method,sat,time,Expanded,Explored,Path,fired,Removed)


