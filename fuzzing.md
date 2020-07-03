# Fuzzing
How to fuzz stuff with various command line tools. These were tested against OWASP juice-shop
## burp intruder
1. Send the request (login for example) to the intruder
2. Specify the positions
    * Do one at a time when.
3. Set the paylod
    * Looking for injections
        *  fuzzdb (fuzzdb/attack/sql-injection/detect/xplatform.txt)
3. Unset payload encoding if JSON, leave if URL parameters
    * Only need this for URL parameters
    * For JSON payloads, need to be unset
4. Set `Grep-Match` to match failed condition to have it flagged in the result set (for filtering)
5. Set redirectings to `In-scope only` and `Process cookies in redirections`.
## ffuf
### JSON
Command line fuzzer which can be used on the OSCP
1. Capture the headers from the POST in question (browser, burp, wireshark, etc) and copy the command as a CuRL request. This is to get the headers to add to ffuf. They can be copied as is (e.g. all the -H's and passed directly to ffuf). **Do not interact with the website after this capture**.
2. Construct the POST command replacing the field to fuzz with **FUZZ**.
3. Fuzz and profit
Get the curl command from the browser
![Firefox CuRL](./img/copy_curl_ff.png)
Copy the headers
![CuRL Headers](./img/copy_headers_for_ffuf.png)
```bash
# Make the ffuf command (No Headers)
ffuf -w fuzzdb/attack/sql-injection/detect/xplatform.txt -X POST -d '{"email":"FUZZ","password":"ndnd"}' -u http://$TARGET:$PORT/rest/user/login -v -fc 401

ffuf -w fuzzdb/attack/sql-injection/detect/xplatform.txt -X POST -d '{"email":"FUZZ","password":"ndnd"}' -u $URL/rest/user/login -v -fc 401

ffuf -w fuzzdb/attack/sql-injection/detect/xplatform.txt -X POST -d '{"email":"FUZZ","password":"ndnd"}' -u $URLS/rest/user/login -v -fc 401

# Add the CuRL headers
ffuf -w fuzzdb/attack/sql-injection/detect/xplatform.txt -X POST -H 'User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:68.0) Gecko/20100101 Firefox/68.0' -H 'Accept: application/json, text/plain, */*' -H 'Accept-Language: en-US,en;q=0.5' --compressed -H 'Referer: http://localhost:3000/' -H 'Content-Type: application/json' -H 'Connection: keep-alive' -H 'Cookie: io=LISjtz-_92JzwZylAAnD; language=en; welcomebanner_status=dismiss; continueCode=zr8x6P25e1gNoBXnbMZjdEjUjTRi2kuzkuxoUWnG4KRWk9lm7vYJVDEOapqw' -d '{"email":"FUZZ","password":"ndnd"}' -u http://$TARGET:$PORT/rest/user/login -v -fc 401

ffuf -w fuzzdb/attack/sql-injection/detect/xplatform.txt -X POST -H 'User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:68.0) Gecko/20100101 Firefox/68.0' -H 'Accept: application/json, text/plain, */*' -H 'Accept-Language: en-US,en;q=0.5' --compressed -H 'Referer: http://localhost:3000/' -H 'Content-Type: application/json' -H 'Connection: keep-alive' -H 'Cookie: io=LISjtz-_92JzwZylAAnD; language=en; welcomebanner_status=dismiss; continueCode=zr8x6P25e1gNoBXnbMZjdEjUjTRi2kuzkuxoUWnG4KRWk9lm7vYJVDEOapqw' -d '{"email":"FUZZ","password":"ndnd"}' -u $URL/rest/user/login -v -fc 401

ffuf -w fuzzdb/attack/sql-injection/detect/xplatform.txt -X POST -H 'User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:68.0) Gecko/20100101 Firefox/68.0' -H 'Accept: application/json, text/plain, */*' -H 'Accept-Language: en-US,en;q=0.5' --compressed -H 'Referer: http://localhost:3000/' -H 'Content-Type: application/json' -H 'Connection: keep-alive' -H 'Cookie: io=LISjtz-_92JzwZylAAnD; language=en; welcomebanner_status=dismiss; continueCode=zr8x6P25e1gNoBXnbMZjdEjUjTRi2kuzkuxoUWnG4KRWk9lm7vYJVDEOapqw' -d '{"email":"FUZZ","password":"ndnd"}' -u $URLS/rest/user/login -v -fc 401
```
### URL Parameters
Same method as above for getting the headers from the browser
```bash
ffuf -w fuzzdb/attack/sql-injection/detect/xplatform.txt -X GET -u http://$TARGET:$PORT/rest/product/search?q=FUZZ -fc 401

ffuf -w fuzzdb/attack/sql-injection/detect/xplatform.txt -X GET -H 'User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.14; rv:68.0) Gecko/20100101 Firefox/68.0' -H 'Accept: application/json, text/plain, */*' -H 'Accept-Language: en-US,en;q=0.5' --compressed -H 'Referer: http://localhost:3001/' -H 'Connection: keep-alive' -H 'Cookie: io=1FTREwEtYeiDwMUQAAnL; language=en; welcomebanner_status=dismiss; continueCode=yo5LkY42egO813Dvrdv5UkTViqafgRSxOuzgUO7sEoAQzVbW9XRNmjPpwalZ' -H 'Pragma: no-cache' -H 'Cache-Control: no-cache' -u http://$TARGET:$PORT/rest/product/search?q=FUZZ -fc 401
```