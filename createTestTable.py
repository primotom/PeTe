f = open("SuperNewTests.csv")




curCurry = ""
model = ""
method = ""
sat = ""

i = 0
for line in f:
	if(i > 0):
		if (line.find("PeTe error") >= 0):
			continue
		
		line.strip("\t")
		lSp = line.split(",")
		if (curCurry != lSp[1] or method != lSp[2]):
			#begin new table	
			curCurry = lSp[1]
			method = lSp[2]
			if (i > 1):
				print "\\end{tabular}"
			print ""
			print "\\begin{tabular}{|l|l|c|c|c|c|c|c|c|}\hline"
			print "\multicolumn{9}{|c|}{" + lSp[1] + " with " + lSp[2] +  " } \\\\\\hline"
			print " Model & Satisfied & Time  & Expanded & Explored & Trace & Transitions Fired & Removed & Memory \\\\\\hline"
		sat = "N/A"

		if( lSp[3].find("Not satisfiable") >= 0):
			sat = "NO"
			print lSp[0].strip(".pet") + " & " + sat + " & " + lSp[4]+ " & " + lSp[5] + " & " + lSp[6] + " & " + lSp[7] + " & " + lSp[8]  + " & " + lSp[9]  + " & " + lSp[10].strip("\n")  + "\\\\\\hline"
		
		if( lSp[3].find("Satisfied") >= 0):
			sat = "YES"
			print lSp[0].strip(".pet") + " & " + sat + " & " + lSp[4]+ " & " + lSp[5] + " & " + lSp[6] + " & " + lSp[7] + " & " + lSp[8]  + " & " + lSp[9]  + " & " + lSp[10].strip("\n")  + "\\\\\\hline"
		
		if( lSp[3].find("OutOfTime") >= 0):
			sat = "Out Of Time"
			print lSp[0].strip(".pet") + " & " + sat + " & - & - & - & - & - & - & " + lSp[8].strip("\n")  + "\\\\\\hline"

		
		
	i += 1
print "\\end{tabular}"
