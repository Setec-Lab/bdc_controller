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

##Principal
ser = serial.Serial('COM5', baudrate=57600, bytesize=8, parity='N', stopbits=1, timeout=1.5, xonxoff=0, rtscts=0)
ser.close()
ser.open()
buffer = b'\x00'
time.sleep(1)
ser.write(b'\x73') #turn it on
ser.read_until(b'\x01\x02') #discard it

##bateria1
ser1 = serial.Serial('COM6', baudrate=57600, bytesize=8, parity='N', stopbits=1, timeout=1.5, xonxoff=0, rtscts=0)
ser1.close()
ser1.open()
buffer1 = b'\x00'
time.sleep(1)
# ser1.write(b'\x73') #turn it on
ser1.read_until(b'\x01\x02') #discard it

##bateria2
ser2 = serial.Serial('COM7', baudrate=57600, bytesize=8, parity='N', stopbits=1, timeout=1.5, xonxoff=0, rtscts=0)
ser2.close()
ser2.open()
buffer2 = b'\x00'
time.sleep(1)
# ser1.write(b'\x73') #turn it on
ser2.read_until(b'\x01\x02') #discard it



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
vbat1_data = []
ibat1_data = []
vbat2_data = []
ibat2_data = []

while True:
      ##Principal
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

      ##Bateria 1
      ser1.read_until(b'\x01\x02')
      buffer1 = ser1.read(2)
      minutes = int.from_bytes(buffer1, "big")
      buffer1 = ser1.read(2)
      seconds = int.from_bytes(buffer1, "big")
      buffer1 = ser1.read(2)
      vbus1 = float(int.from_bytes(buffer1, "big"))
      buffer1 = ser1.read(2)
      vbat1 = float(int.from_bytes(buffer1, "big"))
      buffer1 = ser1.read(2)
      ibat1=0
      if float(int.from_bytes(buffer1, "big")) > 60000:
            ibat1 = 65535-float(int.from_bytes(buffer1, "big"))
      else :
            ibat1 = float(int.from_bytes(buffer1, "big"))
      print("vbat1 = ",end=' ')
      print(vbat1,end='\t')
      print("ibat1 = ",end=' ')
      print(ibat1,end='\t')
      date = datetime.datetime.now()
      time_stamp = date.strftime("%d/%m/%y %H:%M:%S")

      ##Bateria 2
      ser2.read_until(b'\x01\x02')
      buffer2 = ser2.read(2)
      minutes = int.from_bytes(buffer2, "big")
      buffer2 = ser2.read(2)
      seconds = int.from_bytes(buffer2, "big")
      buffer2 = ser2.read(2)
      vbus2 = float(int.from_bytes(buffer2, "big"))
      buffer2 = ser2.read(2)
      vbat2 = float(int.from_bytes(buffer2, "big"))
      buffer2 = ser2.read(2)
      ibat2=0
      if float(int.from_bytes(buffer2, "big")) > 60000:
            ibat2 = 65535-float(int.from_bytes(buffer2, "big"))
      else:
            float(int.from_bytes(buffer2, "big"))
      print("vbat2 = ",end=' ')
      print(vbat2,end='\t')
      print("ibat2 = ",end=' ')
      print(ibat2,end='\t')
      date = datetime.datetime.now()
      time_stamp = date.strftime("%d/%m/%y %H:%M:%S")
      
      #data.append([time_stamp, speed, status ,pub_speed])
      with open(file_name,'a+',newline='') as f:
            writer = csv.writer(f, dialect='excel')
            writer.writerow([time_stamp, vbus, ipva, iloa, vbat1, ibat1, vbat2, ibat2])
## PLOTS Principal
      vbus_data.append(vbus)
      #vbus_data = vbus_data[-20:] 
      iloa_data.append(iloa)
      #iloa_data = iloa_data[-20:] 
      time_float = minutes + (seconds / 60)
      time_data.append(time_float)
      #time_data = time_data[-20:] 
      drawnow(data_fig)
      
## PLOTS bateria 1
      vbat1_data.append(vbat1)
      #vbus_data = vbus_data[-20:] 
      ibat1_data.append(ibat1)
      #iloa_data = iloa_data[-20:] 
      time_float = minutes + (seconds / 60)
   #   time_data.append(time_float)
      #time_data = time_data[-20:] 
      #drawnow(data_fig)

## PLOTS bateria 2
      vbat2_data.append(vbat2)
      #vbus_data = vbus_data[-20:] 
      ibat2_data.append(ibat2)
      #iloa_data = iloa_data[-20:] 
      time_float = minutes + (seconds / 60)
    #  time_data.append(time_float)
      #time_data = time_data[-20:] 
      #drawnow(data_fig)
#65535-FFFF
ser.close()
ser1.close()
ser2.close()
