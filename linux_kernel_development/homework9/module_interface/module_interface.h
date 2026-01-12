#pragma once

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/random.h>

#define DEV_NAME "module-interface"
#define BUF_SIZE (1024)

extern char r_buffer[BUF_SIZE];

int     node_open(struct inode *inode, struct file *file);
int     node_release(struct inode *inode, struct file *file);
ssize_t node_read(struct file *file, char *buffer, size_t length, loff_t* offset);
