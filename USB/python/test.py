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
intf = dev.get_active_configuration()[(0, 0)]

# find the first (and in this case only) OUT endpoint in our interface
epOut = usb.util.find_descriptor(
	intf,
	custom_match= \
	lambda e: \
	usb.util.endpoint_direction(e.bEndpointAddress) == \
	usb.util.ENDPOINT_OUT)

# find the first (and in this case only) IN endpoint in our interface
epIn = usb.util.find_descriptor(
	intf,
	custom_match= \
	lambda e: \
	usb.util.endpoint_direction(e.bEndpointAddress) == \
	usb.util.ENDPOINT_IN)

# make sure our endpoints were found
assert epOut is not None

value = 0
data_hist = []

csvfile = open('test.csv', 'w', newline='')
spamwriter = csv.writer(csvfile, delimiter=' ', quotechar='|', quoting=csv.QUOTE_MINIMAL)

assert epIn is not None
while 1:
	#print("Message: ")
	#t = input() # get the user input
	#i = len(t)
	#epOut.write(t) # send it
	message = epIn.read(64,5000)
	var_1 = message[0]
	var_2 = message[1] + 256*message[2]
	var_3 = message[3] + 256*message[4] + 256*256*message[5]
	currentDT = datetime.datetime.now()
	print(str(currentDT) + ' ' + str(var_1) + ' ' + str(var_2) + ' ' + str(var_3))
	spamwriter.writerow([var_1,var_2,var_3])
