#pragma once

#include <linux/module.h>
#include <linux/errno.h>

#define DEV_NAME         "random_generator"
#define BUF_SIZE         (1024)
#define RAND_NUMS_AMOUNT (5)

extern char r_buffer[BUF_SIZE];
extern long rand_nums_amount;

int node_open(struct inode*, struct file*);
int node_release(struct inode*, struct file*);

ssize_t node_read(struct file*, char*, size_t, loff_t*);
ssize_t node_write(struct file *file, const char *buffer, size_t length, loff_t* offset);
