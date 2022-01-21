#include <linux/init.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>

#define USB_VENDOR_ID_BCM5880 0x0a5c
#define USB_PRODUCT_ID_BCM5880 0x5801

static struct usb_driver BCM5880_finger_driver;


/* table of devices that work with this driver */
static struct usb_device_id BCM5880_finger_driver_table [] = {
        { USB_DEVICE(USB_VENDOR_ID_BCM5880, USB_PRODUCT_ID_BCM5880) },
        { }                      /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, BCM5880_finger_driver_table);


static int BCM5880_finger_driver_probe(struct usb_interface *interface, const struct usb_device_id *id){
printk(KERN_INFO "FINGERPRINT DRIVER HAS LOADED CORRECTLY");
printk(KERN_INFO "FINGERPRINT DRIVER HAS LOADED CORRECTLY");
printk(KERN_INFO "FINGERPRINT DRIVER HAS LOADED CORRECTLY");
printk(KERN_INFO "FINGERPRINT DRIVER HAS LOADED CORRECTLY");
printk(KERN_INFO "FINGERPRINT DRIVER HAS LOADED CORRECTLY");
return 0;
}


static int finger_print_init(void)
{

	int local_error_code;

        /* register this driver with the USB subsystem */
        local_error_code = usb_register(&BCM5880_finger_driver);
        if (local_error_code < 0) {
                printk(KERN_INFO "FAILED TO LOAD DRIVER");
                return -1; }
                return 0;
}

static void finger_print_exit(void)
{
	usb_deregister(&BCM5880_finger_driver);

}


static struct usb_driver BCM5880_finger_driver = {
	.name	=	"dell_fingerprint_driver",
	.probe	=	BCM5880_finger_driver_probe,
	.id_table	=	BCM5880_finger_driver_table,
};

MODULE_LICENSE("GPL");
MODULE_VERSION("0.01");
MODULE_DESCRIPTION("fingerprint driver");

module_init(finger_print_init);
module_exit(finger_print_exit);
