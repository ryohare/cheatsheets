# JWT Hacking Techniques
## Tools
* BURP Json Web Token Attacker (B-App) (Joseph)
## Private Data
* Check JWT for sensitive data
## PrivEsc
* Check for None signing type
    * Check all varions of `none` e.g. `None`,`NONE`, etc
        * Use `Joseph` in burp to test all these
    * Delete the signature when sending int the JWT
    * Cookie Token vs Bearer Token
        * Apply `none` type to cookie JWT and test
        * Apply `none` type of bearer JWT and test
        * Use valid JWT and test
        * Compare responses (Sizes will work in burp)
* Hashcat can crack JWTs
    * Often times HS256 keys are weak and/or default ones
    * Brute force is the only way for RS258 signing method
        * [Use This](https://github.com/brendan-rius/c-jwt-cracker)
    * [Example](https://gist.github.com/pich4ya/f76280b7a6af67a9adf740f3ee547689)
```bash
# Wordlist
hashcat -a0 -m 16500 text.hash [dict]

# Example with wordlist
hashcat -a0 -m 16500 hash2.txt wordlist.txt -r /usr/local/Cellar/hashcat/5.1.0/share/doc/hashcat/rules/best64.rule

# Brute force
hashcat -a3 -m 16500 text.hash
```