import usb.core
import usb.util
import csv
import array
import threading
import queue

#add classes for automated parsing
#use the numvars, varsize primitives to build a list

class psoc_var:
	def __init__(self):
		#the following fields define a streaming variable
		#pretty similar to a DBC file signal description
		self.name = str()
		self.size = int(0)
		self.start = int(0)
		self.mult = int(1)
		self.div = int(1)
		self.offset = int(0)
		
	def parse(self, message):
		return int.from_bytes(message[self.start:self.start+self.size], byteorder='little') * self.mult / self.div + self.offset

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
		
		#set up primitives for threading
		#look at producer/consumer model
		#max size / 1000 = memory in seconds
		self.streamQueue = queue.Queue(maxsize=10000)
		self.streamGo = False
		
		#variable list
		self.varList = []
		self.varsPop = False

		
	def _startStream(self):
		while self.streamGo == True:
			message = self.epInStream.read(64,1000)
			varTemp = []
			for i in range(len(self.varList)):
				varTemp.append(self.varList[i].parse(message))
			
			self.streamQueue.put(varTemp)
			
	def startStream(self):
		#creates thread for data
		#check that vars have been populated
		if len(self.varList) == 0:
			self.populateVars()
		
		if len(self.varList) != 0:
			self.epOutCmd.write([0,0,0,8])
			self.streamGo = True
			self.streamThread = threading.Thread(target=self._startStream)
			self.streamThread.start()
			
	def getStream(self):
		return self.streamQueue.get()
		
	def stopStream(self):
		self.streamGo = False
		self.streamThread.join()
		self.epOutCmd.write([0,0,0,9])
		
	def log(self):
		while self.streamGo == True:
			self.csvwriter.writerow([str(self.getStream())])
		
	def startLog(self):
		self.csvfile = open('test2.csv', 'w', newline='')
		self.csvwriter = csv.writer(self.csvfile, delimiter=' ',quotechar='|',quoting=csv.QUOTE_MINIMAL)
		self.logThread = threading.Thread(target=self.log)
		self.logThread.start()
		
	def stopLog(self):
		self.csvfile.close()
		self.logThread.join()
			
	def populateVars(self):
		start = 0
		if self.varsPop == False:
			self.numVars = self.numvars()
			for i in range(self.numVars):
				self.varList.append(psoc_var())
				self.varList[i].name = self.varname(i)
				self.varList[i].size = self.varsize(i)
				for j in range(0,i):
					start += self.varList[j].size
					
				self.varList[i].start = start
				start = 0
				
			self.varsPop = True
			
		
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
		

