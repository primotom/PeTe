for n in range(5,25+5,5):
	s = '<query name="Too Many in Critical" strategy="Ordered DFS" jit="false">'
	q = '<query name="One Done" strategy="Ordered DFS" jit="false">'
	for i in range(0,n):
		q += 'PROCESS_' + str(i) + '_done &gt; 0'
		for j in range(i+1,n):
			s += '(PROCESS_' + str(i) + '_critical &gt; 0 and PROCESS_' + str(j) + '_critical &gt; 0)'
			if i < n-2:
				s+= ' or '
		if i < n-1:
			q+= ' or '
	s += '</query>'
	q += '</query>'
	print '<queries>\n\t' + s + '\n\t' + q +'\n</queries>\n'
