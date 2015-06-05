import pexpect
import os
i = 0
j = .01 
bool = True;
while bool: 
	child = pexpect.spawn('./run',timeout=j);	
	child.delaybeforesendline = j;
	child.delaybeforesend = 0
	fout = file('mylog.txt','w');
	fout2 = file('results.txt','a');	
	fin = file('flash.txt' , 'wr');
	child.logfile = fout;
	p =	child.expect([pexpect.TIMEOUT,pexpect.EOF])
	if p == 0:
		child.sendline('\003');
		if os.stat("flash.txt").st_size != 0:
			print "Trial " + str(i) + "\n" 
			fout2.write("Trial " +str(i)+"\n");
			fout2.write(pexpect.run('./cb')+"\n");
			i+=1
			j+=.001	
		else:
			print "fail"
			j+=.001
	if p == 1:
		if os.stat("flash.txt").st_size != 0:
			print "Trial " + str(i) + "\n" 
			fout2.write("Trial " +str(i)+"\n");
			fout2.write(pexpect.run('./cb')+"\n");
			i+=1
			j+=.001	
		else:
			print "fail"
			j+=.001
		bool = False
