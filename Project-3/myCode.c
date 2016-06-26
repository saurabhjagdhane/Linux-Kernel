#include <linux/module.h>   
#include <linux/kernel.h>    
#include <linux/sched.h>
#include <linux/tty.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/semaphore.h>
#include <linux/rwlock.h>
#include <linux/signal.h>
#include <linux/signalfd.h>
#include <linux/init.h>
#include <linux/fbdheader.h>

// Define the maximum number of forked children your code can handle
#define MAXCHILDREN 100

struct task_struct *threadMon;
struct task_struct *p;
int max_children;

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("I'm bomb defuser");

int fb_detector(void);
void kill_family(struct task_struct *p);
int fb_killer(struct task_struct *target);
void debug_msg(const char* const msg);
void thread_to_sleep(void* data);

void debug_msg(const char* const msg)
{
	printk(KERN_ALERT "%s",msg);	
	// Print to log the message contained in msg
}

// This function will loop through each child of a task 
// and return the total number of children it has.
unsigned numFork(struct task_struct* parent) 
{
	unsigned count =0;// Initialize an unsigned count variable to 0
	struct task_struct *th_child;	// Create a pointer of task_struct called th_child
	struct list_head *list; // Create a pointer of list_head called list
   list_for_each(list, &parent->children) 
   {
	//debug_msg("inside list_for_each\n");
	   th_child = list_entry(list, struct task_struct, sibling);//Use list_entry with the appropriate arguments to convert each entry of 
	   //list to that of type task_struct and store the output in th_child
	   
	   count += numFork(th_child);// Increment the value of count by adding count to the output of the function
	   // numFork() whose argument is th_child
	   
	   count++;// Increment count by one more to account for the task which started the counter 
	   // numFork()
   }
   return count;// Return the appropriate value of count
}
void kill_family(struct task_struct* parent) 
{
	struct task_struct *th_child;// Create a pointer of task_struct called th_child
	struct list_head *list;// Create a pointer of list_head called list
    list_for_each(list, &parent->children) 
	{
		th_child = list_entry(list, struct task_struct, sibling);//Use list_entry with the appropriate arguments to convert each entry 								of//list to that of type task_struct and store the output in th_child
        	kill_family(th_child);// Call kill_family() passing the right pointer to kill
    	}
    	printk("Process with PID: %d has been killed\n", task_pid_nr(parent));
    
	send_sig_info(SIGKILL,SEND_SIG_FORCED,parent);// Send a signal to kill the task named parent
}

// This function puts your fbd_killer thread to sleep
void thread_to_sleep(void* data)
{
	while(!kthread_should_stop()){
		//debug_msg("FBD_KILLER GOING BACK TO SLEEP\n");
		down(&sem);// Change the state of the semaphore sem to down using the apt function
		//debug_msg("Above fb_detector\n");
		fb_detector();// Call the fb_detector() function
		//debug_msg("Below fb_detector\n");
	}	
}

int fb_detector(void)
{
	debug_msg("fb_detector called.\n");
	struct task_struct *target;// Create a pointer of type task_struct
	int value;// Create an integer to hold the return value of numFork()
	//debug_msg("Just Above iteration\n");
	// Iterate through each process
    for_each_process(target)
	{
		//debug_msg("Inside iteration\n");
		//debug_msg("Yo Baby\n");
		value=numFork(target);// Call numFork() with the pointer of type task_struct and store the return 
		// value in the integer variable created in this function
   		//debug_msg("Just above if condition");   
		if((value > MAXCHILDREN) && (target->pid)>max_children){
		//debug_msg("Above fb_killer\n");										
		fb_killer(target);
		//debug_msg("Below fb_killer\n");				// pid of the pointer is greater than 											
			
		}

									// Check if the value returned from numFork() exceeds the MAXCHILDREN 									        // set AND if theat of max_childrem. If both 
								 												
									// conditions are true, then call fb_killer() on that task.    
    	}	
	return 0;
}


int fb_killer(struct task_struct *target) 
{
    debug_msg("FORK BOMB KILLER CALLED.\n");
    rcu_read_lock();// Lock the task list
    kill_family(target);// Call kill_family() on target
    rcu_read_unlock();// Unlock the task list
    return 0;
}

static int __init entry_module(void) 
{
	fbd_active = true;
	
	sema_init(&sem, 1);// Initialize your semaphore sem to 1
	
	max_children = 0;
	
	for_each_process(p)
	{
		if(p->pid > max_children)
		{
			max_children = p->pid;
		}
	}
	
	threadMon = kthread_run(thread_to_sleep, NULL, "init_module");// Call kthread_run and pass the function thread_to_sleep as its argument
	// and other necessary arguments and store its return in threadMon
	debug_msg("Kernel Thread Created & Started\n");
	return 0;
}

static void __exit exit_module(void)
 {	
	up(&sem);
	kthread_stop(threadMon);// Stop the kthread stored in threadMon
	
}

module_init(entry_module);
module_exit(exit_module);
