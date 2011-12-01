no_processes = 25
can_fail = False

def print_message(msg):
	print '\t<message>'+msg+'</message>'

def print_messages():
	print '<messages>'
	for i in range(no_processes):
		for j in range(0,no_processes):
			print_message("msg_"+str(i)+ "_"+str(j))
	print '</messages>'
	
def print_state(st, state_type):
	if state_type != '':
		state_type= ' type="' + state_type +'"'
	print '\t<state' + state_type + '>'+st+'</state>'
	
def print_states():
	print '<states>'
	for i in range(no_processes):
		print_state('p' + str(i),'initial')
		if can_fail:
			print_state('p' + str(i) + '_failed', '')
	print_state('Leader','')
	print '</states>'

def print_rule(name, current, pre, next, post):
	print '<rule id="' + name + '">'
	if current != '' or next != '':
		print '\t<pre>'
		if current != '':
			print '\t\t<current_state>'+ current + '</current_state>'
		if pre != '':
			print '\t\t<received_message>' + pre + '</received_message>'
		print '\t</pre>'
	if next != '' or post != '':
		print '\t<post>'
		if next != '':
			print '\t\t<next_state>' + next + '</next_state>'
		if post != '':
			print '\t\t<send_message>' + post + '</send_message>'
		print '\t</post>'
	print '</rule>'
	

def print_role(name):
	print '<role name="' + name + '">'
	print_states()
	for i in range(no_processes):
		next_state = i+1
		if next_state == no_processes:
			next_state = 0

		#start
		print_rule('p' + str(i) + '_start','p' + str(i),'','p' + str(i),'msg_' + str(i) + '_' + str(next_state))
		
		#next
		#print_rule('p' + str(i) + '_next', 'p' + str(i), '', 'p' + str(next_state), '') 
		
		#elect
		print_rule('elect_p' + str(i), 'p' + str(i), 'msg_' + str(i) + '_' + str(i), 'Leader', '')
		
		#fail state
		if can_fail:
			print_rule('fail_p' + str(i), 'p' + str(i), '', 'p' + str(i) + '_failed', '')
		
		#receive smaller
		for s in range(i):
			print_rule('receive_smaller_p' + str(i) + '_' + str(s), 'p' + str(i), 'msg_' + str(s) + '_' + str(i), 'p' + str(i), 'msg_' + str(i) + '_' + str(next_state))
			
		#receive larger
		for l in range(i+1,no_processes):
			print_rule('receive_larger_p' + str(i) + '_' + str(l), 'p' + str(i), 'msg_' + str(l) + '_' + str(i), 'p' + str(i), 'msg_' + str(l) + '_' + str(next_state))
		
	print '</role>'

def print_protocol():
	print '<protocol name="LeaderElection" medium="SET" capacity="6">'

	print_messages()
	print_role('El')

	print '</protocol>'


print_protocol()
