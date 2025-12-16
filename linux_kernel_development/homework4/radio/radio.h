#pragma once

#include <linux/module.h>
#include <linux/errno.h>

#define DEV_NAME "radio"

int radio_open(struct inode*, struct file*);
int radio_release(struct inode*, struct file*);

ssize_t radio_write(struct file *file, const char __user *buf, size_t length, loff_t* offset);
