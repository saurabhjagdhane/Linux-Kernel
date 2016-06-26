/*---Start of syscall.c----*/

#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/slab.h>
#include <linux/semaphore.h>
#include <linux/tty.h>
#include <linux/kthread.h>
#include <linux/list.h>
#include <linux/jiffies.h>
#include <linux/linkage.h>


//The System call
asmlinkage long sys_my_syscall(int var, char *dataBuffer)
{
	char buff[800];
	int id, ctu;
	cputime_t utime, stime, jiff;
	char taskName[64];
	char cmd[64];
	int secs, min, hrs;
	char time[9];
	int counter = 0;
	int buffLength = 0;

	struct task_struct *task;	// Pointer for structure struct_task
	
	
	for_each_process(task){
		if (counter == var){
			//PID
			id = task->pid;
			
			//TIME
			utime = 0;
			stime = 0;
			thread_group_cputime_adjusted(task, &utime, &stime);
			jiff = utime + stime;
			secs = jiff / HZ;
			hrs = secs / 3600;
			secs = secs - hrs * 3600;
			min = secs / 60;
			secs = secs - min * 60;
			snprintf(time, sizeof(time), "%02d:%02d:%02d", (int)hrs, (int)min, (int)secs);

			//CMD
			strcpy(cmd, task->comm);
			
			//TTY
			if (task->signal->tty == NULL){
				strcpy(taskName, "?");
			}
			else{
				strcpy(taskName, task->signal->tty->name);
			}
			
			snprintf(buff, sizeof(buff),"%5d %-8s %8s %s", id, taskName, time, cmd); // stores the values in the buffer
			buffLength = strlen(buff); // gets the size of the array
			if (buffLength > 800)
				buffLength = 800;
			}
		counter++;
		

		}
	
	
		
	
	ctu = copy_to_user(dataBuffer, buff, buffLength); // use copy_to_user to actually copy kernel info to user space
	
	return counter; 
}

 
/*---End of syscall.c------*/
