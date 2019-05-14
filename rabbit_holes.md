# Rabbi Holes and Strategy
Some collection of notes people say about rabbit holes

## Rabbit Holes
```
Summarized: When it is too difficult, it is probably a rabbit hole.
```

```
Sometimes I realized I was in a rabbit hole, when I noticed that I would be typing the same commands I just typed 10 minutes ago, basically trying the same thing expecting different results. Remember that if you've truly enumerated everything and googled for potential exploits, but you still can't make any progress then you might be in a rabbit hole.
```

```
Rotate through machines every 3-4 hours.
```

```
The trick with the OSCP is to avoid so-called rabbit-holes, so don’t concentrate one the first finding and never let got. This costs time and the experience shows that Offensive Security does not want you to go in depth, but in the breadth. So the golden rule applies: “Enumerate, enumerate, enumerate!” indeed.
```

So to summerize, it would seem the best way to do this is to do the following
#### Fully enumerate the box.
Run all the enumeration scripts available for each service and profile the liklihood of each service being the exploitation vector. Use other services to enrich other service understanding .e.g. SNMP can help know something about the web server. 
#### Remove services which are "bullet proof"
Strong ssh, RDP with credentials, etc are unlikely to be an initial entry point and can be relagated to low priority
#### Start with the services with the most information about
Look for exploits in exploit-db, searchsploit and google. Google the whole thing, too, like "Ubuntu 9.09 Apache2" or "Debian 6 OpenSSH" since some vulnerabilities are tied to specific distros.
#### Eject on rabbit holes
- If little PoC or none exists, move on
- Any of the above rabbit hole indicators apply
    - Over 2 hours with no progress
    - Same commands over and over again
        - This could be running dirb again for example

## Enumeration
### Stuck
- enumerate more or google a way to enumerate the service differently
- how can you enumerated can chain together to get a shell or sensitive information
- have to piece together more information from various sources


## Strategy
### Target Selection
- Start BOF Box
- Start with High Point Boxes
### Time Management
- Plan Breaks with time, including dinner, lunch and breakfast.
    - Break every 3 hours (4/12 hour)
    - 12 hour shifts
- 2 Hours stuck before moving on
- Last 15/30 mins of exam, make sure all required material has been collected

![](https://i1.wp.com/infosecuritygeek.com/wp-content/uploads/2018/06/Screenshot-Mon-Jul-09-2018-122849-GMT0800-749x364.png?resize=749%2C364&ssl=1)
## Proofs
### Linux
- Low Priv
    - local.txt 
        - `find / -name "local.txt" 2>/dev/null`
    - ifconfig
- Root Priv
    - proof.txt
    - ifconfig
### Windows
- Low Priv
    - local.txt
        - `dir /s /b c:\local.txt`
    - ipconfig
- High Priv
    - Acceptable Users
        - Anyone in Administrator group
        - Administrator
        - SYSTEM
    - proof.txt
    - ipconfig
