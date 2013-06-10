#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
 .name = KBUILD_MODNAME,
 .init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
 .exit = cleanup_module,
#endif
 .arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__attribute_used__
__attribute__((section("__versions"))) = {
	{ 0xf6337a3a, "struct_module" },
	{ 0x4c3af445, "__request_region" },
	{ 0xc4149ada, "cdev_del" },
	{ 0x8f39f971, "cdev_init" },
	{ 0xdc3eaf70, "iomem_resource" },
	{ 0x35224760, "malloc_sizes" },
	{ 0xa03d6a57, "__get_user_4" },
	{ 0xeae3dfd6, "__const_udelay" },
	{ 0x7485e15e, "unregister_chrdev_region" },
	{ 0x1b7d4074, "printk" },
	{ 0x2da418b5, "copy_to_user" },
	{ 0x9eac042a, "__ioremap" },
	{ 0x7ee30be3, "cdev_add" },
	{ 0xa097339a, "kmem_cache_alloc" },
	{ 0xb2fd5ceb, "__put_user_4" },
	{ 0x8bb33e7d, "__release_region" },
	{ 0xd1cd32a9, "pci_unregister_driver" },
	{ 0x37a0cba, "kfree" },
	{ 0xedc03953, "iounmap" },
	{ 0x93ae567f, "__pci_register_driver" },
	{ 0x172de8ee, "pci_enable_device" },
	{ 0xf2a644fb, "copy_from_user" },
	{ 0x29537c9e, "alloc_chrdev_region" },
};

static const char __module_depends[]
__attribute_used__
__attribute__((section(".modinfo"))) =
"depends=";

