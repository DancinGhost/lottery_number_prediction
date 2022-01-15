#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_AUTHOR("Julius Victorian and R");
MODULE_LICENSE("GPL");
MODULE_VERSION("0.01");
MODULE_DESCRIPTION("fingerprint driver for dell xfr 6420fingerprint reader");

static int finger_print_init(void)
{
        pr_debug("Hi\n");
        return 0;
}

static void finger_print_exit(void)
{
        pr_debug("Bye\n");
}

module_init(finger_print_init);
module_exit(finger_print_exit);
