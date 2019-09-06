import usb.core
import usb.util
import array
import datetime
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import time
import csv

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

csvfile = open('test.csv', 'w', newline='')
spamwriter = csv.writer(csvfile, delimiter=' ', quotechar='|', quoting=csv.QUOTE_MINIMAL)

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
		epOutCmd.write([0,0,0,1])
		message = epInCmd.read(64,1000)
		print(message[0])

	if cmd == "VARSIZE":
		arg = input()
		epOutCmd.write([0,0,0,2,int(arg)])
		message = epInCmd.read(64,1000)
		print(message[0])

	if cmd == "VARNAME":
		arg = input()
		epOutCmd.write([0,0,0,3,int(arg)])
		message = epInCmd.read(64,1000)
		print(''.join(map(chr,list(message))))
		
	if cmd == "START":
		epOutCmd.write([0,0,0,8])
		while 1:
			message = epInStream.read(64,1000)
			print(message[1]+message[2]*255)
			
	if cmd == "STOP":
		epOutCmd.write([0,0,0,9])
		

