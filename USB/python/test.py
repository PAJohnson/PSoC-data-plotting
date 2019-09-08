import usb.core
import usb.util
import array
import datetime
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import csv

def numvars(epIn,epOut):
	epOut.write([0,0,0,1])
	message = epIn.read(64,1000)
	return message[0]
	
def varsize(epIn,epOut,num):
	epOut.write([0,0,0,2,int(num)])
	message = epIn.read(64,1000)
	return message[0]
	
def varname(epIn,epOut,num):
	epOut.write([0,0,0,3,int(num)])
	message = epIn.read(64,1000)
	return message
	
def numpar(epIn,epOut):
	epOut.write([0,0,0,11])
	message = epIn.read(64,1000)
	return message[0]
	
def parsize(epIn,epOut,num):
	epOut.write([0,0,0,12,int(num)])
	message = epIn.read(64,1000)
	return message[0]
	
def parget(epIn,epOut,num):
	value = 0
	size = int(parsize(epIn,epOut,num)/8)
	epOut.write([0,0,0,13,int(num)])
	message = epIn.read(64,1000)
	for i in range(int(size)):
		value = message[i]*256**i + value
	
	return value
	
def parset(epIn,epOut,num,val):
	size = parsize(epInCmd,epOutCmd,num)
	message = array.array('B',64 * [0])
	message[0] = 0
	message[1] = 0
	message[2] = 0
	message[3] = 14
	message[4] = int(num)
	valb = val.to_bytes(size, byteorder="little")
	for i in range(len(valb)):
		message[i+5] = valb[i]
			
	epOutCmd.write(message)
	
def parsave(epIn,epOut):
	epOutCmd.write([0,0,0,15])
	


# search for our device by product and vendor ID
dev = usb.core.find(idVendor=0x4B4, idProduct=0x10)

#raise error if device is not found
if dev is None:
	raise ValueError('Device not found')
# set the active configuration (basically, start the device)
dev.set_configuration()

# get interface 0, alternate setting 0
cfg = dev[0]
intf = cfg[(0,0)]
epInStream = intf[0]
epOutStream = intf[1]
epInCmd = intf[2]
epOutCmd = intf[3]



# make sure our endpoints were found
assert epOutStream is not None
assert epOutCmd is not None

value = 0
data_hist = []

assert epInStream is not None
assert epInCmd is not None
while 1:
	#print("Message: ")
	#t = input() # get the user input
	#i = len(t)
	#epOut.write(t) # send it
	#message = epInStream.read(64,5000)
	print("Command: ")
	cmd = input()
	if cmd == "NUMVARS":
		print(numvars(epInCmd,epOutCmd))

	if cmd == "VARSIZE":
		arg = input()
		print(varsize(epInCmd,epOutCmd,arg))

	if cmd == "VARNAME":
		arg = input()
		print(''.join(map(chr,list(varname(epInCmd,epOutCmd,arg)))))
		
	if cmd == "START":
		epOutCmd.write([0,0,0,8])
		while 1:
			message = epInStream.read(64,1000)
			print(message[1]+message[2]*255)
			
		
	

	if cmd == "STOP":
		epOutCmd.write([0,0,0,9])

	if cmd == "NUMPAR":
		print(numpar(epInCmd,epOutCmd))

	if cmd == "PARSIZE":
		arg = input()
		print(parsize(epInCmd,epOutCmd,arg))


	if cmd == "PARGET":
		arg = input()
		print(parget(epInCmd,epOutCmd,arg))

	if cmd == "PARSET":
		value = 0
		print("PARNUM?")
		parnum = input()
		print("VALUE?")
		val = int(input())
		parset(epInCmd,epOutCmd,parnum,val)

		
	if cmd == "PARSAVE":
		print("CONFIRM? Y/N")
		confirm = input()
		if confirm == "Y":
			parsave(epInCmd,epOutCmd)
			
		
	if cmd == "GETALLPARS":
		#get all parameters and save to a file
		print("File name?")
		filename = input()
		
		#get number of parameters
		number_pars = int(numpar(epInCmd,epOutCmd))

		#open csv file
		#format for writing is par#,size,value\n
		csvfile = open(filename, 'w', newline='')
		csvwriter = csv.writer(csvfile, delimiter=' ', quotechar='|', quoting=csv.QUOTE_MINIMAL)
		
		for i in range(number_pars):
			#get size for each par, print line to csv
			size = int(parsize(epInCmd,epOutCmd,i))
			#get value of par i
			value = int(parget(epInCmd,epOutCmd,i))
			
			csvwriter.writerow([i,size,value])
		
		csvfile.close()
		
