#pragma once

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/random.h>

#define DEV_NAME "module-device"
#define BUF_SIZE (1024)

extern char w_buffer[BUF_SIZE];
extern long rand_nums_amount;

void lock_writing_mutex(void);
void unlock_writing_mutex(void);

int     node_open(struct inode *inode, struct file *file);
int     node_release(struct inode *inode, struct file *file);
ssize_t node_write(struct file *file, const char *buffer, size_t length, loff_t* offset);
