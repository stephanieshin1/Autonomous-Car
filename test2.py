import serial

s = serial.Serial('/dev/cu.HC-05-DevB', timeout=None)
if s.isOpen():
    print("Bluetooth detected!")

data = ""

while data != 'STOP/':
    data = s.read(s.read_until('/')).decode("ascii")
    print(data)

print("Program ended.")
