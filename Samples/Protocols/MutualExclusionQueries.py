for n in range(1,5):
	s = ''
	for i in range(0,n):
		for j in range(i+1,n):
			s += '(PROCESS_' + str(i) + '_critical > 0 and PROCESS_' + str(j) + '_critical > 0)'
			if i < n-2:
				s+= ' or '
	print s +'\n'
