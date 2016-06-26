#ifndef FBDHEADER_H
#define FBDHEADER_H

#include <linux/semaphore.h>

extern bool fbd_active;
extern struct semaphore sem;

#endif
