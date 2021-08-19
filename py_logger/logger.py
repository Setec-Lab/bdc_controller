import serial
import datetime
import csv
import time
import struct
import numpy as np
import matplotlib.pyplot as plt
import string

pub_flag = 0
date = datetime.datetime.now()
date_string = date.strftime("%d_%m_%y_%H_%M")
file_name = "data_" + date_string + ".csv"
print("Saving to: ",end='')
print(file_name) 

ser = serial.Serial('COM4', baudrate=57600, bytesize=8, parity='N', stopbits=1, timeout=1.5, xonxoff=0, rtscts=0)
ser.close()
ser.open()
pub_flag = 1
# start = b'\x00'
# prev = b'\x00'
# read = b'\x00'
buffer = b'\x00'
time.sleep(1)
ser.write(b'\x73') #turn it on 
ser.read_until(b'\x01\x02') #discard it
# print(ser.readline().decode(),end='')
# answer = input("")
# if answer == 'y':
#       pub_flag = 1
# ser.write(answer.encode())
# #ser.write(b'n')
data = []
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
      if pub_flag == 1:
            print(vbus,end='\t')
            print(ipva,end='\t')
      date = datetime.datetime.now()
      time_stamp = date.strftime("%d/%m/%y %H:%M:%S")
      #data.append([time_stamp, speed, status ,pub_speed])
      with open(file_name,'a+',newline='') as f:
            writer = csv.writer(f, dialect='excel')
            writer.writerow([time_stamp, vbus, ipva, iloa])


ser.close()
