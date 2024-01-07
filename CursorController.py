import pyautogui
import time 
import serial

arduinoSerial = serial.Serial(port = 'COM3', baudrate = 115200)
time.sleep(1)
pyautogui.FAILSAFE = False


while True:
   data = (arduinoSerial.readline().decode('utf8', errors='ignore')) # get serial port data
   axis = (data.split(",")) # read the axis data

   y = int(axis[0])

   x = int(axis[1])

   click = int(axis[2]) #if the ESP32 touch sensor is activated or not

   gest = int(axis[3].split("\r\n")[0]) #which gesture is detected by the ESP32

   pyautogui.moveTo(x,y)

   if click == 1:
      pyautogui.mouseDown()
   if click == 0:
      pyautogui.mouseUp()
   if gest == 0:
      pyautogui.hotkey('browserforward')
   if gest == 1:
      pyautogui.hotkey('browserback')
   if gest == 2:
      pyautogui.hotkey('win', 'm')
   if gest == 3:
      pyautogui.hotkey('win', 'up')

   pyautogui.PAUSE = 0