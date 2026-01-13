#pragma once

#include <linux/module.h>
#include <linux/errno.h>
#include <linux/random.h>

#define DEV_NAME "keyboard-irq"
#define BUF_SIZE (1024)

extern char r_buffer[BUF_SIZE];
extern int irq_counter;

int     node_open(struct inode *inode, struct file *file);
int     node_release(struct inode *inode, struct file *file);
ssize_t node_read(struct file *file, char *buffer, size_t length, loff_t* offset);
