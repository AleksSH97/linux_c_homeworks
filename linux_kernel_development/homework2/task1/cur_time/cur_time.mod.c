#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.arch = MODULE_ARCH_INIT,
};

KSYMTAB_FUNC(cur_time, "", "");

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "66EF9D5AAA8EB774CF2DD5C");
