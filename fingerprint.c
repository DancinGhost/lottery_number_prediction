#include <linux/init.h>
#include <linux/errno.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/usb.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/interrupt.h>

#define USB_VENDOR_ID_BCM5880 0x0a5c
#define USB_PRODUCT_ID_BCM5880 0x5801

static struct usb_driver BCM5880_finger_driver;
static int major_number; // major number of the character device

/* table of devices that work with this driver */
static struct usb_device_id BCM5880_finger_driver_table [] = {
    { USB_DEVICE(USB_VENDOR_ID_BCM5880, USB_PRODUCT_ID_BCM5880) },
    { }                      /* Terminating entry */
};
MODULE_DEVICE_TABLE (usb, BCM5880_finger_driver_table);

static int usb_interrupt_in_endpoint(struct usb_interface *interface) {
    int i;
    struct usb_host_interface *iface_desc;
    struct usb_endpoint_descriptor *endpoint;

    // Get the interface descriptor
    iface_desc = interface->cur_altsetting;

    // Loop through all the endpoints and find an interrupt IN endpoint
    for (i = 0; i < iface_desc->desc.bNumEndpoints; i++) {
        endpoint = &iface_desc->endpoint[i].desc;

        if ((endpoint->bmAttributes & USB_ENDPOINT_XFERTYPE_MASK) == USB_ENDPOINT_XFER_INT &&
            usb_endpoint_dir_in(endpoint)) {
            return endpoint->bEndpointAddress;
        }
    }

    // No interrupt IN endpoint found
    return -1;
}

static irqreturn_t interrupt_handler(int irq, void *dev_id) {
    printk(KERN_INFO "Interrupt handled\n");
    return IRQ_HANDLED;
}

static int BCM5880_finger_driver_probe(struct usb_interface *interface, const struct usb_device_id *id){
    int irq_number;
    int result;

    printk(KERN_INFO "FINGERPRINT DRIVER HAS LOADED CORRECTLY");

    // Request an interrupt line
    irq_number = usb_interrupt_in_endpoint(interface);
    result = request_irq(irq_number, interrupt_handler, IRQF_SHARED, "BCM5880_finger_driver", &BCM5880_finger_driver);
    if (result) {
        printk(KERN_ERR "Could not register interrupt\n");
        return result;
    }

    return 0;
}

static void BCM5880_finger_driver_disconnect(struct usb_interface *interface) {
    int irq_number;

    printk(KERN_INFO "FINGERPRINT DRIVER HAS BEEN DISCONNECTED");

    // Free the interrupt line
    irq_number = usb_interrupt_in_endpoint(interface);
    free_irq(irq_number, &BCM5880_finger_driver);
}

static ssize_t device_read(struct file *filp, char __user *buffer, size_t length, loff_t *offset) {
    struct usb_device *dev;
    struct usb_interface *interface;
    unsigned char *bulk_buf;
    int interrupt_in_endpoint;
    int actual_length;
    int retval;

    dev = interface_to_usbdev(interface);
    bulk_buf = kmalloc(64, GFP_KERNEL);
    if (!bulk_buf) {
        printk(KERN_ERR "Failed to allocate bulk buffer: %d\n", -ENOMEM);
        return -ENOMEM;
    }

    interrupt_in_endpoint = usb_interrupt_in_endpoint(interface);
    if (interrupt_in_endpoint == -1) {
        printk(KERN_ERR "No interrupt IN endpoint found\n");
        return -1;
    }

    retval = usb_bulk_msg(dev, usb_rcvintpipe(dev, interrupt_in_endpoint), bulk_buf, 64, &actual_length, 1000);
    if (retval) {
        printk(KERN_ERR "Bulk message returned %d\n", retval);
        return retval;
    }

    if (copy_to_user(buffer, bulk_buf, actual_length)) {
        printk(KERN_ERR "Failed to copy data to user space: %d\n", -EFAULT);
        return -EFAULT;
    }

    printk(KERN_INFO "Read %d bytes from the device\n", actual_length);
    kfree(bulk_buf);

    return actual_length;
}

static struct file_operations fops = {
    .read = device_read,
};

static int finger_print_init(void)
{
    int local_error_code;

    /* register this driver with the USB subsystem */
    local_error_code = usb_register(&BCM5880_finger_driver);
    if (local_error_code < 0) {
        printk(KERN_INFO "Failed to register driver with USB subsystem: %d\n", local_error_code);
        return -1; 
    }

    /* register a character device */
    major_number = register_chrdev(0, "fingerprint", &fops);
    if (major_number < 0) {
        printk(KERN_ALERT "Failed to register a major number: %d\n", major_number);
        return major_number;
    }
    
    printk(KERN_INFO "Registered character device with major number %d\n", major_number);

    return 0;
}

static void finger_print_exit(void)
{
    usb_deregister(&BCM5880_finger_driver);
    
    /* unregister the character device */
    unregister_chrdev(major_number, "fingerprint");
}

static struct usb_driver BCM5880_finger_driver = {
    .name   =   "dell_fingerprint_driver",
    .probe  =   BCM5880_finger_driver_probe,
    .disconnect = BCM5880_finger_driver_disconnect,
    .id_table   =   BCM5880_finger_driver_table,
};

MODULE_LICENSE("GPL");
MODULE_VERSION("0.10");
MODULE_DESCRIPTION("fingerprint driver");
MODULE_AUTHOR("Julius");

module_init(finger_print_init);
module_exit(finger_print_exit);
