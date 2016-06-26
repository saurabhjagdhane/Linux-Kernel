/*---Start of syscall.c----*/

#include<linux/kernel.h>
#include<linux/syscalls.h>

//The System call
asmlinkage long sys_my_syscall(const char *test)
{
printk (KERN_ALERT "This is the new system call [Saurabh Jagdhane] implemented.\n");
//0 for successful operation
return 0;
}
 
/*---End of syscall.c------*/
