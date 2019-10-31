# Various ways to move files between attacker and host
## rdesktop
```bash
rdesktop $TARGET -u nedry -p $PASS -r disk:sharename=$(pwd)
```
