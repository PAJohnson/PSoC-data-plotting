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
import psoc_utils


class App(QtGui.QMainWindow):
	def __init__(self, parent=None):
		super(App, self).__init__(parent)
		#init USB
		self.usb = psoc_utils.psoc_usb(0x4b4,0x10)
		
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
		
		self.data_top = deque(maxlen=2000)
		self.data_mid = deque(maxlen=2000)
		self.data_bot = deque(maxlen=2000)
		
		self.usb.startStream()

		#### Start  #####################
		self._update()

	def _update(self):
		while self.usb.streamQueue.empty() != True:
			newData = self.usb.getStream()
			self.data_top.append(newData[0])
			self.data_mid.append(newData[1])
			self.data_bot.append(newData[2])
		
		self.h1.setData(list(self.data_top))
		self.h2.setData(list(self.data_mid))
		self.h3.setData(list(self.data_bot))
		QtCore.QTimer.singleShot(1, self._update)
		self.counter += 1
		


if __name__ == '__main__':
	#its about to get ugly!

	app = QtGui.QApplication(sys.argv)
	thisapp = App()
	thisapp.show()
	sys.exit(app.exec_())
