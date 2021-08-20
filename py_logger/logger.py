import serial
import datetime
import csv
import time
import struct
import numpy as np
import matplotlib.pyplot as plt
from drawnow import drawnow
#import string

pub_flag = 0
date = datetime.datetime.now()
date_string = date.strftime("%d_%m_%y_%H_%M")
file_name = "data_" + date_string + ".csv"
print("Saving to: ",end='')
print(file_name) 

ser = serial.Serial('COM4', baudrate=57600, bytesize=8, parity='N', stopbits=1, timeout=1.5, xonxoff=0, rtscts=0)
ser.close()
ser.open()
buffer = b'\x00'
time.sleep(1)
ser.write(b'\x73') #turn it on 
ser.read_until(b'\x01\x02') #discard it

def data_fig():
      ax1 = plt.subplot(211)
      plt.plot(time_data,vbus_data)
      plt.setp(ax1.get_xticklabels(), fontsize=6)     
      # share x only
      ax2 = plt.subplot(212, sharex=ax1)
      plt.plot(time_data,iloa_data)
      # make these tick labels invisible
      plt.setp(ax2.get_xticklabels(), visible=False)


plt.ion()
fig = plt.figure()

time_data = []
vbus_data = []
iloa_data = []

while True:
      ser.read_until(b'\x01\x02')
      buffer = ser.read(2)
      minutes = int.from_bytes(buffer, "big")
      buffer = ser.read(2)
      seconds = int.from_bytes(buffer, "big")
      buffer = ser.read(2)
      vbus = float(int.from_bytes(buffer, "big"))
      buffer = ser.read(2)
      ipva = float(int.from_bytes(buffer, "big"))
      buffer = ser.read(2)
      iloa = float(int.from_bytes(buffer, "big"))
      print("vbus = ",end=' ')
      print(vbus,end='\t')
      print("ipva = ",end=' ')
      print(ipva,end='\t')
      date = datetime.datetime.now()
      time_stamp = date.strftime("%d/%m/%y %H:%M:%S")
      #data.append([time_stamp, speed, status ,pub_speed])
      with open(file_name,'a+',newline='') as f:
            writer = csv.writer(f, dialect='excel')
            writer.writerow([time_stamp, vbus, ipva, iloa])
      ## PLOTS
      vbus_data.append(vbus)
      #vbus_data = vbus_data[-20:] 
      iloa_data.append(iloa)
      #iloa_data = iloa_data[-20:] 
      time_float = (minutes / 60) + (seconds)
      time_data.append(time_float)
      #time_data = time_data[-20:] 
      drawnow(data_fig)

ser.close()
