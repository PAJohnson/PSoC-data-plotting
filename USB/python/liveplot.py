import sys
import time
from pyqtgraph.Qt import QtCore, QtGui
import numpy as np
import pyqtgraph as pg
import usb.core
import usb.util
import array
import datetime
import csv
from collections import deque


class App(QtGui.QMainWindow):
	def __init__(self, parent=None):
		super(App, self).__init__(parent)
		#init USB
		self.usb = usb_data()
		
		#### Create Gui Elements ###########
		self.mainbox = QtGui.QWidget()
		self.setCentralWidget(self.mainbox)
		self.mainbox.setLayout(QtGui.QVBoxLayout())

		self.canvas = pg.GraphicsLayoutWidget()
		self.mainbox.layout().addWidget(self.canvas)

		self.label = QtGui.QLabel()
		self.mainbox.layout().addWidget(self.label)

		#  line plot 1
		self.otherplot = self.canvas.addPlot()
		self.h1 = self.otherplot.plot(pen='r')
		
		self.canvas.nextRow()
		
		#  line plot 2
		self.otherplot1 = self.canvas.addPlot()
		self.h2 = self.otherplot1.plot(pen='g')
		
		self.canvas.nextRow()
		
		#  line plot 3
		self.otherplot2 = self.canvas.addPlot()
		self.h3 = self.otherplot2.plot(pen='b')


		#### Set Data  #####################

		self.x = np.linspace(0,50., num=100)

		self.counter = 0
		self.fps = 0.
		self.lastupdate = time.time()
		
		self.data_top = deque(maxlen=200)
		self.data_mid = deque(maxlen=200)
		self.data_bot = deque(maxlen=20)

		#### Start  #####################
		self._update()

	def _update(self):
		
		self.data1, self.data2, self.data3 = usb_data.get_data(self.usb)
		self.data_top.append(self.data1)
		self.data_mid.append(self.data2)
		self.data_bot.append(self.data3)
		self.h1.setData(list(self.data_top))
		self.h2.setData(list(self.data_mid))
		self.h3.setData(list(self.data_bot))
		QtCore.QTimer.singleShot(1, self._update)
		self.counter += 1
		
class usb_data():
	def __init__(self, parent=None):
		# search for our device by product and vendor ID
		self.dev = usb.core.find(idVendor=0x4B4, idProduct=0x10)

		#raise error if device is not found
		if self.dev is None:
			raise ValueError('Device not found')
		# set the active configuration (basically, start the device)
		self.dev.set_configuration()

		# get interface 0, alternate setting 0
		self.intf = self.dev.get_active_configuration()[(0, 0)]

		# find the first (and in this case only) OUT endpoint in our interface
		self.epOut = usb.util.find_descriptor(
			self.intf,
			custom_match= \
			lambda e: \
			usb.util.endpoint_direction(e.bEndpointAddress) == \
			usb.util.ENDPOINT_OUT)

		# find the first (and in this case only) IN endpoint in our interface
		self.epIn = usb.util.find_descriptor(
			self.intf,
			custom_match= \
			lambda e: \
			usb.util.endpoint_direction(e.bEndpointAddress) == \
			usb.util.ENDPOINT_IN)

		# make sure our endpoints were found
		assert self.epOut is not None

		self.value = 0
		self.data_hist = []
		
	def get_data(self):
		self.message = self.epIn.read(64,5000)
		self.var_1 = self.message[0]
		self.var_2 = self.message[1] + 256*self.message[2]
		self.var_3 = self.message[3] + 256*self.message[4] + 256*256*self.message[5]
		return [self.var_1,self.var_2,self.var_3]

if __name__ == '__main__':
	#its about to get ugly!


	



	app = QtGui.QApplication(sys.argv)
	thisapp = App()
	thisapp.show()
	sys.exit(app.exec_())
