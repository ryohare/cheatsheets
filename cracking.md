# Cracking Help
# Wordlists
* [Seclist](https://github.com/danielmiessler/SecLists)
* [fuzzdb](https://github.com/fuzzdb-project/fuzzdb)
* [Skull Security](https://wiki.skullsecurity.org/Passwords)
* [Combiner Scripts](https://github.com/laconicwolf/Password-Scripts)
# John
## JWT
Based on example [here](https://gist.github.com/pich4ya/f76280b7a6af67a9adf740f3ee547689). Need to compile John with JWT support.
```bash
# converter script
git clone https://github.com/Sjord/jwtcrack
cd jwtcrack
virtualenv venv -p python3
source venv/bin/activate
python jwt2john.py eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9.eyJpc3MiOiJPbmxpbmUgSldUIEJ1aWxkZXIiLCJpYXQiOjE1OTM5Njc1NzYsImV4cCI6MTYyNTUwMzU3NiwiYXVkIjoid3d3LmV4YW1wbGUuY29tIiwic3ViIjoianJvY2tldEBleGFtcGxlLmNvbSIsIkdpdmVuTmFtZSI6IkpvaG5ueSIsIlN1cm5hbWUiOiJSb2NrZXQiLCJFbWFpbCI6Impyb2NrZXRAZXhhbXBsZS5jb20iLCJSb2xlIjpbIk1hbmFnZXIiLCJQcm9qZWN0IEFkbWluaXN0cmF0b3IiXX0.y-YZkJflIiISFj52okgTRqGJbQp5D-Ny5tNVa0aXsJs > /tmp/jwt.john
deactivate
cd ..
brew install openssl
git clone https://github.com/magnumripper/JohnTheRipper
cd JohnTheRipper/src
./configure CPPFLAGS="-I/usr/local/opt/openssl/include" LDFLAGS="-L/usr/local/opt/openssl/lib" --disable-pkg-config
$ make clean && make
$ make install
$ ../run/john /tmp/jwt.john --wordlist /path/to/wordlist.txt
```
# Hashcat
## JWT
HS256 Only
```bash
hashcat -a0 -m 16500 hash2.txt wordlist.txt -r /usr/local/Cellar/hashcat/5.1.0/share/doc/hashcat/rules/best64.rule
```
# Other
## JWT
### c-jwt-cracker
Brute force cracker for RS256
```bash
brew install openssl
export LDFLAGS="-L/usr/local/opt/openssl/lib" export CFLAGS="-I/usr/local/opt/openssl/include"
git clone https://github.com/brendan-rius/c-jwt-cracker
cd c-jwt*
make
./jwtcrack eyJ0eXAiOiJKV1QiLCJhbGciOiJSUzI1NiJ9.eyJzdGF0dXMiOiJzdWNjZXNzIiwiZGF0YSI6eyJpZCI6MTksInVzZXJuYW1lIjoiIiwiZW1haWwiOiJuZWRyeUBpbmdlbi5jb20iLCJwYXNzd29yZCI6ImViMTBhYWQ3M2M1NTBjOWQ4MzcwZDNhZmM1NDE3NWQ5Iiwicm9sZSI6ImN1c3RvbWVyIiwiZGVsdXhlVG9rZW4iOiIiLCJsYXN0TG9naW5JcCI6InVuZGVmaW5lZCIsInByb2ZpbGVJbWFnZSI6Ii9hc3NldHMvcHVibGljL2ltYWdlcy91cGxvYWRzL2RlZmF1bHQuc3ZnIiwidG90cFNlY3JldCI6IiIsImlzQWN0aXZlIjp0cnVlLCJjcmVhdGVkQXQiOiIyMDIwLTA3LTAyIDIwOjAzOjQyLjI3NyArMDA6MDAiLCJ1cGRhdGVkQXQiOiIyMDIwLTA3LTAyIDIwOjA3OjE0Ljg5MSArMDA6MDAiLCJkZWxldGVkQXQiOm51bGx9LCJpYXQiOjE1OTM5NjY3MzksImV4cCI6MTU5Mzk4NDczOX0.hL4ccn9_CeEB1ilFdQ_PF7XH-y-eYcfwJVzkQDtFY9zNhOsgrzcohFWpo75EXxZupBKrrqfXd6HbaN5nY3tEFtqn1eSkxYRlPkzcE0JnjRNjne0iY_GmX0yWcWM59-NpN0P9Iw5Zwr-uez1GMLFW4nAua6IcHQs0k_GlTfXFRgM
```bash