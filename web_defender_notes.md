# Dev 522 Notes
## Web Basics
### HTTP Request
```
GET /index.html HTTP/1.1
Host: www.sans.org
User-Agent: Mozilla/5.0
... more headers

BODY if POST
```
### HTTP Response
```
HTTP/1.1 200 Ok
... headers ...

BODY
```
### HTTP Methods
* GET
    * Pull web page. Can take query parameters
* POST
    * Submit data for processing in the request body
* HEAD
    * Get just the headers in a `GET` request
* OPTIONS
    * Get methods supported by the server
* DELETE
    * delete specified resource
* PUT
    * Store data in body at specified resources
* TRACE
    * echo request back to sender
* CONNECT
    * Create tunnel for TLS
* PATCH
    * RESTful update of resource

Use `POST` over `GET` when parameters are involved

### Response Codes
* 1XX
    * Informational
* 2XX
    * Success
* 3XX
    * Redirection
* 4XX
    * Client Errors
* 5XX
    * Server errors

### Headers
* `Referrer` is not secure and should not be used for security descisions
    * That said, can still add security to prevent data leakage
    * Header: `Referrer-Policy: Origin`
 * `User-Agent`
    * Spoofable

### Conections
* HTTP 1.1 Supports persistent TCP connections

### Authentication
#### Basic Auth
`Authorization: Basic $(echo 'username:passsword' | base64)`
#### Digest Auth
* Challenge response based
* Uses MD5
* RFC2617
* Response has the password in the form
    * `echo 'username:realm:password:nonce:cnonce' | md5sum`
#### Certificate Auth
* Verifys identify of both sides via CA signed certificates
#### Form Based Auth
* Data sent in as a `POST` in message body
### HTTP State
* HTTP is stateless
* Cookies used to maintain state
* Client side sessions with JWTs
### Cookies
* Set with  `Set-Cookie` header
* HttpOnly
    * Only HTTP requesets can access the cookies
    * JS cant touch them
* Secure
    * Only sent over HTTPS
* SameSite
    * Strict or Lax
        * Strick prevents sending with X-Origin requests
## WebApp Architecture
* 3 tiers
* * Presentation
* * * Web Server e.g. Nginx
* * Application
* * * Actual WebApp Code (Flask, Django) - Server Side / not SPA
* * Persistence
* * * Databases
### Reference Architecture
inet -> IPS/IDS -> LB -> Presentation -> FW -> App Servers -> FW -> DBs
## Web Proxies
* Add header `X-Forwareded-For`
# Authentication
## Testing
* Source Code
    * Look for references to IP and refer
* Runtime
    * Manipulate form fields and observe the affect
* Test Password Reset
## Prevention
* Set for autocomplete to off
    * `<from ... AUTOCOMPLETE=off>`
## Account Lockout
* Rate limiting loging requests
* Straight up lockout
* Exponential backoff
## Best Practices
* Use ReAuth for sensitive functions or force 2FA for them
* Dont use Digest or Basic auth
* Use origin base proctection
    * does this person log in from here?
    * should this person log in from here
* Use timing proctection
    * Detect automated attacks
* Use behavior protections
    * detect unusual login times
* Sign transactions
## MFA
* Time Based
    * TOTP
        * Time based
    * HOTP
        * HMAC Based
    * OATH
        * [T|H]TOP
    * OTPAUTH
        * Google
        * TOTP
    * Seeded with Secret key from site using MFA
* Per Use Token
* OOB
* Challenge-Response
## Authorization
* Do checks server side
    * Dont rely on client submitted data
        * unless signed by server
* URL Based
    * Use URL along with critera to make decisions
## Static Content
* Put all content in one directory struture
    * Have WAF block all other downloads
### Testing
* Gobuster
### Path Traversal
* Use WAF to block relative paths
    * Also, just dont allow it

# Common Web Vulnerabilities and Mitigations
## TLS
* Non-Usage of TLS in signin page
    * Make whole site TLS via URL Rewrite or 301 Moved to HTTPS version of site
* For TLS 1.2, make sure using secure cipher suites
    * SSL Labs
    * Cipher suites controlled via the CSR request
    * AES GCM mode is best (and it 1.3)
        * Any none authenticated encryption schemes are vulnerable to a padding oracle attack
* Use `HSTS` header to force TLS
### Perfect Forward Secrets
* Diffie-Hellman Cipher Suites - DHE
* Ellipic Curve ECDHE
* Resistant to stolen TLS private key
### Certificates
* Use ECC certs
* Multi-Domain
    * Wildcards
        * Dangerous
    * Multiple SANs
    * Server Name Indication (SNI)
        * One IP many Sites
        * SNI is encrypted in TLS1.3 preventing MITM proxying
* OCSP Stapling
    * Cert status is fetched from CA and presented with request
    * Status is signed by CA
* Cert Pining
    * Look for specific certs
    * Can shoot yourself in the foot it app is client side
    * `pin-sha256="sjkdgsdfslkfgsdlkfsldkjflksdjfldsjds=`
## Passwords
### Key Management
* Database Encrytion
    * Monitor keys and log key accesses
    * Store in key vault
    * Only exchange over secure channels
* Whitelisted Password Hashes
    * Bcrypt
    * PBKDF2
    * Scrypt
    * Argon2
    * Need a 'slow' hash function
* Limit password sizes to prevent a DoS from sending in 1MB sized passwords and killing the hash function
## Reasonable Encryption Technologies
From book, but use [this](https://latacora.micro.blog/2018/04/03/cryptographic-right-answers.html) instead.
### Hashing
* SHA-256/512
* SHA-3
* RIPEMD-1690
### Symmetric Key
* AES
* Blowfish
* Twofish
* Serpent
### Asymmetric Key
* RSA
* ECDSA
## Sessions
* Stateful vs Stateless
    * Stateful is maintained by the server
    * Stateless is the client presenting the state via something like a JWT or Cookie
* Best Practices (stateful)
    * Dont use URL session IDs
    * Check for user agent changes
    * Check for IP address changes
    * Make session IDs not predicatable
    * Change session ID with each request
        * Prevents replay attacks
        * Prevent session fixation attacks
    * Use random user IDs
    * All session information should be stored on the server
    * Sessions should timeout
    * Logout event should immediatly invalidate the session
 ## CSRF
 * Rides existing sessions to make requests and use victims session information stored in the browswer
 * Drive by style attacks
 * mitigations
    * Session Timeouts
    * Check referrer headers
    * Captcha
    * State flow controls
    * CSRF Tokens in Forms
        * Random on each payload and stored and validated server side
        * XSS can be used to bypass these controls by grabbing the CSRF tokens out of the page and using them in the request
    * SameSite=strict cookie 
    
## Business Logic
* Look at edge cases and race conditions
    * e.g. quantity = -1
    * e.g. does adding something to the cart remove it from inventory?
        * DoS Conditions
# Common Vulnerabilities
## SQL Injection
* Blind injection gives no feedback as to the result of the command
    * Usually used with the timing side channel to exfil data
        * Guess and see if a wait occurs
* SQLMap
* Migitation
    * Input filtering
    * Input validation
    * Use ORM's rather than raw SQL
    * Use libraries to escape special characters
    * Use Parameterized queries
    * Use stored procedures
        * Dont use dynamic SQL in procedure or you get the same issues
    * Use correctly scoped permissions for webapp access to DB to limit lateral movement
    * Surpress framework SQL error messages
## Reponse Splitting
* Attacker injects headers into the response headers
    * Attacks the client and browser
    * Vulnerable if user data ends up in the response headers
    * Proxy cache posioning
    * Prevent with input validation, specifically looking for CR/LF
## XSS
* Stored
    * Scripts get into persistant storage and recalled later
* Reflected
    * Scripty input is rendered on the page
* DOM
    * Script only gets executed locally and does not rely on the server at all
* Mitigation
    * Filter input for Script characters
    * Output encoding
    * Set specific character set
    * [OWASP XSS Prevention Checklist](https://cheatsheetseries.owasp.org/cheatsheets/Cross_Site_Scripting_Prevention_Cheat_Sheet.html)
    * Dont put user input into JavScript
    * [https://ha.ckers.org/xss.html](https://ha.ckers.org/xss.html) for test strings
## Input Validation
All of WebApps is basically about input validation, so here are the key parts for input validation
* Avoid using user input
    * map values to things like enums
* Validate the source of the data
    * Cookies
    * Referer
    * Other headers
* Canonicalization of Data
    * Set encoding and enforce throughout of the system
    * Always convert to this character set
    * Whitelist characters
    * Apply whitelist to the canonicalized input
    * Use blacklists too incase somehow the whitelist fails
## File Uploads
* Specify size limits
* Check MIME Type
* Check file type once on disk
* check filename
* Always rename file to inert format
    * e.g. .upload or something that a webserver will not render
* Save outside webdirectories
* Run virus scans and stuff like that
* Do format appropriate checking
    * image
        * resize image
            * will rewrite headers and bits
            * kills stego channels
    * PDF
        * use forensic tools to look for maliciousness
        * [Analyzing Malicious Documents](https://www.sans.org/security-resources/posters/dfir/)
    * Office
        * Same as PDF
## Configuration
* Disable directory listings
* Make sure SCM is not deployed with web content
* Cross Site Tracing
    * Combined with XSS to relay message headers to attacker to harvest creds potentially
    * Disable TRACE except in dev environments
* Dont run multiple sites on the same host
    * lateral movement potential
    * If same host is required, run multiple instances of the webserver in different webroots
* Admin interface isolation
    * Require admin creds so no force browsing vuln
## Logging and Errors
* Log everything
    * Be able to construct a forensic timeline
        * Who did what action to what object when
* Surpress framework error messages
* Catch all exceptions
* Dont put user input into log files
    * Always encode user input that MUST go into logs
        * Prevent log poisoning
    * Bracket the user input 
        * prevent log forging
        * e.g. `fmt.logf("%s submitted <<<<%s>>>>",user.Name, user.Data)
        * Can detected if additional log entries were added
## IR
* Identification
    * Helpdesk
    * Read / Parse / Alert on Logs
    * Declare Incident
* Containment
    * Prevent from spreading
    * Preserve evidence
    * Leverage firewalls
    * suspend user accounts
    * Take down site
* Eradication
    * Validate intrusion has been eleminated
    * record all activites
    * use WAF to block attack from re-occuring
    * Determine Root Cause
    * Analyze for similar problems
        * might lead to discovery of additional compromises
* Recovery
    * Ensure the system is back to normal
    * Validate all functionalities of the system
        * hopefully we have goot UAT tests
    * Create alerts for compromise and monitor
    * Enable WAF rules
* Lessions learned
    * Refine IR procedures
    * Prevent issue from happening again in source code
    * new libs etc
## Automation Stoppers
* CAPTCHA
* Robots.txt
* Rate Limiting
## HoneyPots
* HoneyToken
    * Random variables that mean nothing to detect manipulation
        * Someone is probing
    * Fake cookie values
## Safe Redirection
* Throw warning for Interal -> External redirections
* Sign redirect
* Check referer
* Use robots.txt to prevent indexinbg of a redirect URL
* Tokens use to validate that the server generated the redirect initially
    * Similar to CSRF tokens
# AJAX / Web Services
* SOAP
    * XML Based Messages
    * Follows WSDLs for Message Formats
        * Dont public to public endpoints to prevent enumeration
## Web Services Defenses
* Always do schema validation of incoming messages
* Dont use DTD
### XXE
* Basically RCE in the DTD section of an XML message
* Migations
    * Disable DTD's in the XML processor
    * [OWASP Cheetsheet](https://cheatsheetseries.owasp.org/cheatsheets/XML_External_Entity_Prevention_Cheat_Sheet.html)
### XML Attacks
* Attack the network traffic
    * DoS
    * Manipulate messages to:
* Attack the XML parser
    * parsing bugs
    * oversized payloads
* Attack the XML processor
    * How the data gets loaded into the XML responses
    * XXE is this type of attack
    * XPath injection
## SAML
* Assertion from SAML Authority
* Assertion is the claim by the SAML authority on the identiy of the subject
* Service providers can authorize a user based on the claim by the SAML authority
* Assertion contains
    * Time Limit
    * Permissions
    * Identity Providers Signature
## AJAX
* Client will Async send request via XML to the server to populate dynamic data
* Technologies
    * HTML
    * JavaScript
    * DHTML
    * DOM
### Requests
* XMLHTTPRequest is the request that is sent from the client to the server
* Can transfer XML, JSON, HTML
* Also called XHR
* Requests initiated in JavaScript
### SOP
* Prevents scripts from on other websites from being loaded loaded on the current site
    * breaks functionalility
    * Conditional third party scripts able to be laoded via a CORs policy

## Cross Domain AJAX
* SOP prevents this
* AJAX bridging is using a proxy on the server to forward the request
    * Vulnerable to SSRF
* JSONP can get aroud SOP
    * Json with padding
    * Hard to get right
### Preflight Requests
* An initial XHR request is sent to the third party resource service asking about its CORs policy
* Origin header is used which holds the root address
    * referer holds the full address
Request
```
OPTIONS HTTP request
Access-Control-Request-Method: POST
Access-Control-Request-Headers: X-Ping
```
Postive Response
```
OPTIONS HTTP request
Access-Control-Allow-Origin: https://original-website.com
Access-Control-Alloe-Method: POST, GET
OPTIONS
Access-Control-Request-Headers: X-Ping
```
## AJAX Defense Summary
* AJAX vulns tend to be amplified
* Clearly defined seperation between client and server components
* Input validation
* Schema validate JSON payloads same as XML
* Make sure CORs policies are good
    * Policy is an allow list on the resource based on the origin header

## REST APIs
* Methods
    * POST
        * Create Resource
    * GET
        * Retrieve resource
    * PUT
        * Update the resource (replace)
    * PATCH
        * Update the resource (modify)
    * Delete
        * Remove the resource
## Security
* Sign requests based on credentials and pass in headers
    * S3 approach
* Vulnerable to CSRF
* Dont pass credentials via query parameteres
    * same as web apps
* Deny non-used methods
* Content type validation
* Rate limiting
## Content Security Policy (CSP)
* Whitelist for what content and from where can be loaded onto the site
* Essentiually CORS for this site
* Defaults
    * No inline javascript
    * Code will not be created from strings
    * No data in URI
    * No XBL bindings
* Key Values
    * script-src
        * Where the site can load scripts from
        * CORS for the current site
    * frame-src
        * locations where IFrames can be loaded from
## Mime-Sniffing
* Browswers will attempt to read the file type and bypass the MIME type to render it
* Can be used for RCE
* Mitigation
    * set `X-Content-Type-Options: nosniff`