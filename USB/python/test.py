import usb.core
import usb.util
import array
import csv
import psoc_utils

psoc = psoc_utils.psoc_usb(0x4B4,0x10)

while 1:
	print("Command: ")
	cmd = input()
	if cmd == "NUMVARS":
		print(psoc.numvars())

	if cmd == "VARSIZE":
		arg = input()
		print(psoc.varsize(arg))

	if cmd == "VARNAME":
		arg = input()
		print(''.join(map(chr,list(psoc.varname(arg)))))
		
	if cmd == "START":
		epOutCmd.write([0,0,0,8])
		while 1:
			message = epInStream.read(64,1000)
			print(message[1]+message[2]*255)
			

	if cmd == "STOP":
		epOutCmd.write([0,0,0,9])

	if cmd == "NUMPAR":
		print(psoc.numpar())

	if cmd == "PARSIZE":
		arg = input()
		print(psoc.parsize(arg))


	if cmd == "PARGET":
		arg = input()
		print(psoc.parget(arg))

	if cmd == "PARSET":
		value = 0
		print("PARNUM?")
		parnum = input()
		print("VALUE?")
		val = int(input())
		psoc.parset(parnum,val)

		
	if cmd == "PARSAVE":
		print("CONFIRM? Y/N")
		confirm = input()
		if confirm == "Y":
			psoc.parsave()
			
		
	if cmd == "GETALLPARS":
		#get all parameters and save to a file
		print("File name?")
		filename = input()
		
		#get number of parameters
		number_pars = psoc.numpar()

		#open csv file
		#format for writing is par#,size,value\n
		csvfile = open(filename, 'w', newline='')
		csvwriter = csv.writer(csvfile, delimiter=' ', quotechar='|', quoting=csv.QUOTE_MINIMAL)
		
		for i in range(number_pars):
			#get size for each par, print line to csv
			size = psoc.parsize(i)
			#get value of par i
			value = psoc.parget(i)
			
			csvwriter.writerow([i,size,value])
		
		csvfile.close()
		
	if cmd == "SETALLPARS":
		#get parameter file name
		print("File name?")
		filename = input()
		
		#get number of parameters from device
		number_pars = psoc.numpar()
		
		#open csv file
		csvfile = open(filename, 'r', newline='')
		csvreader = csv.reader(csvfile, delimiter=' ',quotechar='|',quoting=csv.QUOTE_MINIMAL)
		
		parnum = []
		size = []
		value = []
		
		#write parameters from csv
		for row in csvreader:
			parnum.append(int(row[0]))
			size.append(int(row[1]))
			value.append(int(row[2]))
			
		if number_pars == len(parnum):
			#number of pars agrees with file
			for i in range(number_pars):
				newsize = psoc.parsize(i)
				if newsize == size[i]:
					psoc.parset(i,value[i])
					
			
		csvfile.close()
		
