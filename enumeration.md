# Source
https://guif.re/networkpentest
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

# SMTP 25
```bash
# find users
smtp-user-enum -M VRFY -U /usr/share/seclists/Usernames/top_shortlist.txt -t 10.11.1.115 -p 25
```

# 53 DNS Lookup Range
```bash
dig.sh <ips.txt>

for ip in $(cat ips.txt); do nslookup $ip <nameserver>; done
```

# TFTP UDP 69
```bash
nmap -n -vvv -d -sU -p69 10.11.1.226 --script tftp-enum.nse --script-args tftp-enum.filelist=/usr/share/wordlists/metasploit/tftp.txt
```

# 88 Kerberos
```bash
nmap -p88 --script=krb5-enum-users --script-args krb5-enum-users.realm="$DOMAIN.local",userdb=/usr/share/seclists/Usernames/Names/names.txt $TARGET

nmap -n -v -p 88 --script krb5-enum-users -sV 10.11.1.201 -d --script-args krb5-enum-users.realm='corp',userdb=cirt-default-usernames.txt
```

# 139/445 SMB
```bash
nmap -n -v -p 135-139,445 -T4 -PN -sVC --script= --script-args=unsafe=1 $TARGET
nmap -n -v -p 135-139,445 -T4 -PN -sVC --script --script-args=unsafe=1 $TARGET
nmblookup -A $TARGET
smbclient //MOUNT/share -I $TARGET N
smbclient -L //$TARGET
enum4linux -a $TARGET
rpcclient -U "" $TARGET
```

# 389 LDAP
```bash
nmap -n -v -p 389 --script ldap-rootdse.nse -sV 10.11.1.201
```

# 80/443 - HTTP Servers
```bash
nmap -n -v -p 80,443,8080 -T4 -PN -sVC --script http-vuln* --script-args=unsafe=1 $TARGET
nikto -h http://$TARGET -p 80 -output 80_nikto.txt
curl -v -i $TARGET:80
w3m -dump $TARGET/robots.txt | tee 80_robots.txt
VHostScan -t $TARGET -oN 80_vhosts.txt
```

## 80/443 - HTTP (Discovery)
```bash
dirb http://$TARGET:80/ -o 80_dirb.txt
dirbuster -H -u http://$TARGET:80/ -l /usr/share/wordlists/dirbuster/directory-list-lowercase-2.3-medium.txt -t 20 -s / -v -r 80_dirbuster_medium.txt
gobuster -w /usr/share/seclists/Discovery/Web-Content/common.txt -u http://$TARGET:80/ -s '200,204,301,302,307,403,500' -e | tee '80_gobuster_common.txt'
gobuster -w /usr/share/seclists/Discovery/Web-Content/RobotsDisallowed-Top1000.txt -u http://$TARGET:80/ -s '200,204,301,302,307,403,500' -e | tee '80_gobuster_toprobots.txt'
gobuster -w /usr/share/seclists/Discovery/Web-Content/CGIs.txt -u http://$TARGET:80/ -s '200,204,301,307,403,500' -e  | tee '80_gobuster_cgis.txt'
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
nmap -vv -sV -Pn -p 27900 --script=ms-sql-info,ms-sql-config,ms-sql-dump-hashes --script-args=mssql.instance-port=27900,mssql.username=sa,mssql.password=sa $TARGET

 nmap -n -v -p 1433 10.11.1.31 -sTV -PN --script ms-sql-xp-cmdshell --script-args mssql.username=sa,mssql.password=poiuytrewq,ms-sql-xp-cmdshell.cmd="c:\ralph.exe"

 nmap -n -v -p 27900 10.11.1.227 -sV --script ms-sql-query.nse --script-args mssql.username=sa,mssql.password=password,ms-sql-query.query="SELECT * FROM tblCustomers",mssql.database=tblCustomers

```

## 8080/various Proxies (squid, etc)
```bash
#Connect to the proxy and scan whats available behind it:
#htc -F <local-port> <address>:<proxy-port>
htc -F 80 10.10.11.31:3128
curl -vv http://127.0.0.1:80

https://tools.kali.org/maintaining-access/httptunnel
```

## 5900 VNC
```bash
# vuln
nmap -n -v -p 5900 -sV -T4 -PN --script realvnc*,vnc-info*,vnc-title* $TARGET
# Find public exploits
searchsploit vnc
# Bruteforcing
crowbar -b vnckey -s 10.11.1.73/32 -p IP -k PASS_FILE
```
