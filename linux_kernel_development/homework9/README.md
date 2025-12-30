# Homework: two kernel modules - *device* and its *interface* for user space

## Guide for the *device*

Make sure you are in the *device* directory: `cd /module_device`

- Compile code: `make` or `sudo make`
- Insert module: `sudo insmod module_device.ko`
- Remove module: `sudo rmmod module_device.ko`
- Check if device was created `ls -l /dev/module-device`
- Write amount of randomly generated numbers: `echo 10 | sudo tee /dev/module-device`

## Guide for the *interface*

Make sure you are in the *interface* directory: `cd /module_interface`

**TBD**
