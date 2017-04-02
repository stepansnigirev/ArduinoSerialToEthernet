import serial
import os
import sys

port = "/dev/tty.usbserial-FTYID1HW"

settings = {
	"baudrate": 9600,
	"parity": 'N',
	"bytesize": 8, # wordlength in arduino settings
	"stopbits": 1,
	"timeout": 0.1,
}

# # wierd settings for testing
# settings = {
# 	"baudrate": 38400,
# 	"parity": 'O',
# 	"bytesize": 7, # wordlength in arduino settings
# 	"stopbits": 2,
# 	"timeout": 0.1,
# }

s = serial.Serial(port, **settings)
print(s)
while True:
	try:
		c = s.read(1)
		os.write(1,c)
		if c==b'?':
			s.write(b"YEYYYYY!!!!\r\n")
	except KeyboardInterrupt:
		s.close()
		print("\nSerial port closed")
		sys.exit()