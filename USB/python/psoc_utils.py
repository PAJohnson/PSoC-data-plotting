import usb.core
import usb.util
import csv
import array

class psoc_usb:
	def __init__(self,vid,pid):
		#init usb, grab device info
		self.dev = usb.core.find(idVendor=vid, idProduct=pid)

		#raise error if device is not found
		if self.dev is None:
			raise ValueError('Device not found')
		# set the active configuration (basically, start the device)
		self.dev.set_configuration()

		# get interface 0, alternate setting 0
		self.cfg = self.dev[0]
		self.intf = self.cfg[(0,0)]
		self.epInStream = self.intf[0]
		self.epOutStream = self.intf[1]
		self.epInCmd = self.intf[2]
		self.epOutCmd = self.intf[3]
		
		
	def numvars(self):
		self.epOutCmd.write([0,0,0,1])
		message = self.epInCmd.read(64,1000)
		return int(message[0])
		
	def varsize(self,num):
		self.epOutCmd.write([0,0,0,2,int(num)])
		message = self.epInCmd.read(64,1000)
		return int(message[0])
		
	def varname(self,num):
		self.epOutCmd.write([0,0,0,3,int(num)])
		message = self.epInCmd.read(64,1000)
		return message
		
	def numpar(self):
		self.epOutCmd.write([0,0,0,11])
		message = self.epInCmd.read(64,1000)
		return int(message[0])
		
	def parsize(self,num):
		self.epOutCmd.write([0,0,0,12,int(num)])
		message = self.epInCmd.read(64,1000)
		return int(message[0])
		
	def parget(self,num):
		value = 0
		size = int(self.parsize(num)/8)
		self.epOutCmd.write([0,0,0,13,int(num)])
		message = self.epInCmd.read(64,1000)
		for i in range(int(size)):
			value = message[i]*256**i + value
		
		return value
		
	def parset(self,num,val):
		size = self.parsize(num)
		message = array.array('B',64 * [0])
		message[0] = 0
		message[1] = 0
		message[2] = 0
		message[3] = 14
		message[4] = int(num)
		valb = val.to_bytes(size, byteorder="little")
		for i in range(len(valb)):
			message[i+5] = valb[i]
				
		self.epOutCmd.write(message)
		
	def parsave(self):
		self.epOutCmd.write([0,0,0,15])
		

