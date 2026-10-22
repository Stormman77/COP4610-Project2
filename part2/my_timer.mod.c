#include <linux/module.h>
#include <linux/export-internal.h>
#include <linux/compiler.h>

MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x51357216, "proc_remove" },
	{ 0xbd03ed67, "__ref_stack_chk_guard" },
	{ 0x680628e7, "ktime_get_real_ts64" },
	{ 0x40a621c5, "snprintf" },
	{ 0x437e81c7, "simple_read_from_buffer" },
	{ 0xd272d446, "__stack_chk_fail" },
	{ 0xe4de56b4, "__ubsan_handle_load_invalid_value" },
	{ 0xd272d446, "__fentry__" },
	{ 0xdef8136e, "proc_create" },
	{ 0xd272d446, "__x86_return_thunk" },
	{ 0xe9196a28, "module_layout" },
};

static const u32 ____version_ext_crcs[]
__used __section("__version_ext_crcs") = {
	0x51357216,
	0xbd03ed67,
	0x680628e7,
	0x40a621c5,
	0x437e81c7,
	0xd272d446,
	0xe4de56b4,
	0xd272d446,
	0xdef8136e,
	0xd272d446,
	0xe9196a28,
};
static const char ____version_ext_names[]
__used __section("__version_ext_names") =
	"proc_remove\0"
	"__ref_stack_chk_guard\0"
	"ktime_get_real_ts64\0"
	"snprintf\0"
	"simple_read_from_buffer\0"
	"__stack_chk_fail\0"
	"__ubsan_handle_load_invalid_value\0"
	"__fentry__\0"
	"proc_create\0"
	"__x86_return_thunk\0"
	"module_layout\0"
;

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "FAF2140BFAC0936024D3F05");
