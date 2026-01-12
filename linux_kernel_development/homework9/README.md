# Homework: two kernel modules - *device* and its *interface* for user space

Use `sudo dmesg --follow` to track logs from both kernel modules

## Guide for the *device*

Make sure you are in the *device* directory: `cd /module_device`

- Compile code: `make` or `sudo make`
- Insert module: `sudo insmod module_device.ko`
- Remove module: `sudo rmmod module_device.ko`
- Check if device was created `ls -l /dev/module-device`
- Write amount of randomly generated numbers: `echo 10 | sudo tee /dev/module-device`

## Guide for the *interface*

Make sure you are in the *interface* directory: `cd /module_interface`

- Compile code: `make` or `sudo make`
- Insert module: `sudo insmod module_interface.ko` (**You should insert module_device module first**)
- Remove module: `sudo rmmod module_interface.ko`
- Check if device was created `ls -l /dev/module-interface`
- Get data from *module-device* using *module-interface*: `sudo cat /dev/module-interface`

Example of output:

```
sudo cat /dev/module-interface 
-1853413306
-1061506511
-971385491
-1860736733
-1380979854
-435287497
652857134
-1881414699
313951745
1472536933
```
