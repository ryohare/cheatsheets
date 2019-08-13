#incluce <stddio.h>

unsigned char random[] = "some random string from /dev/random ... cat /dev/random | tr -dc A-Z-a-z-0-9 | head -c512";

unsigned char shellcode[] = "msfvenom shell code";

int main(void)
{
  // cast to a void function pointer (void (*)() ) then invoke (funcptr)()
  ( (void (*)() )shellcode)();
}
