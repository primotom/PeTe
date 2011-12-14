no_processes = 15

def print_message(msg):
	print '\t<message>'+msg+'</message>'

def print_messages():
	print '<messages>'
	for i in range(no_processes):
		print_message('req_'+str(i))
		print_message('run_'+str(i))
		print_message('done_'+str(i))

	print '</messages>'
	
def print_state(st, state_type):
	if state_type != '':
		state_type= ' type="' + state_type +'"'
	print '\t<state' + state_type + '>'+st+'</state>'
	
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

def print_rule2(name, current1, current2, pre, next, post):
	print '<rule id="' + name + '">'
	if current1 != '' or current2 != '' or next != '':
		print '\t<pre>'
		if current1 != '':
			print '\t\t<current_state>'+ current1 + '</current_state>'
		if current2 != '':
			print '\t\t<current_state>'+ current2 + '</current_state>'
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
	
def print_process_states():
	print '<states>'
	
	print_state('noncritical','initial')
	print_state('critical', '')
	print_state('done','')

	print '</states>'

def print_process_rules(i):
	print_rule('dosendreq','noncritical','', 'noncritical', 'req_' + str(i))
	print_rule('dorun','noncritical','run_'+ str(i), 'critical', '')
	print_rule('dodone', 'critical', '', 'done', 'done_'+ str(i))
	print_rule('drop', 'noncritical', 'run_' + str(i), 'done', 'done_'+ str(i))
	print_rule('fail', 'noncritical', '', '', '')

def print_coordinator_states():
	print '<states>'
	print_state('Ready', 'initial')
	print_state('Limit', 'limited')
	for i in range(no_processes):
		print_state('running_p' + str(i),'')
	print '</states>'

def print_coordinator_rules():
	for i in range(no_processes):
		print_rule2('runp' + str(i),'Ready', 'Limit', 'req_'+str(i),'running_p' + str(i),'run_' + str(i))
		print_rule('donep' + str(i),'running_p' + str(i), 'done_'+str(i),'Ready','')

def print_coordinator(name):
	print '<role name="' + name + '">'
	print_coordinator_states()
	print_coordinator_rules()
	print '</role>'

def print_process(i):
	print '<role name="PROCESS_' + str(i) + '">'

	print_process_states()
	print_process_rules(i)
		
	print '</role>'

def print_protocol():
	print '<protocol name="MutalExclusion" medium="SET" capacity="6">'

	print_messages()
	print_coordinator('COORDINATOR')
	for i in range(no_processes):
		print_process(i)

	print '</protocol>'


print_protocol()
