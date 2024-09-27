#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <libusb-1.0/libusb.h>

// USB Parameters
#define VENDOR_ID  0x0b05
#define PRODUCT_ID 0x1b2c
#define REPORT_ID  0x5A
#define WVALUE     0x035A
#define WINDEX     4
#define WLENGTH    16

// Backlight Levels
typedef enum {
    OFF = 0,
    LOW = 1,
    MID = 2,
    HIGH = 3
} BacklightLevel;

// Function to display usage
void usage(const char *prog_name) {
    fprintf(stderr, "Usage: %s [off|low|mid|high]\n", prog_name);
    exit(EXIT_FAILURE);
}

// Function to re-execute the program with sudo
void rerun_with_sudo(int argc, char *argv[]) {
    char *new_argv[argc + 2];
    new_argv[0] = "sudo";
    for (int i = 0; i < argc; i++) {
        new_argv[i + 1] = argv[i];
    }
    new_argv[argc + 1] = NULL;

    execvp("sudo", new_argv);
    // If execvp fails
    perror("Failed to rerun with sudo");
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
    // Check if running as root
    if (getuid() != 0) {
        fprintf(stderr, "Error: This program requires sudo privileges to run.\n");
        rerun_with_sudo(argc, argv);
    }

    if (argc != 2) {
        fprintf(stderr, "Error: Missing backlight level argument.\n");
        usage(argv[0]);
    }

    // Determine the desired backlight level
    BacklightLevel level;
    if (strcmp(argv[1], "off") == 0) {
        level = OFF;
    } else if (strcmp(argv[1], "low") == 0) {
        level = LOW;
    } else if (strcmp(argv[1], "mid") == 0) {
        level = MID;
    } else if (strcmp(argv[1], "high") == 0) {
        level = HIGH;
    } else {
        fprintf(stderr, "Error: Invalid backlight level '%s'.\n", argv[1]);
        usage(argv[0]);
    }

    printf("Setting backlight level to '%s' (Level %d).\n", argv[1], level);

    // Prepare the data packet
    unsigned char data[WLENGTH] = {0};
    data[0] = REPORT_ID;
    data[1] = 0xBA;
    data[2] = 0xC5;
    data[3] = 0xC4;
    data[4] = (unsigned char)level;
    // The rest are already initialized to 0

    libusb_device_handle *handle = NULL;
    int r;

    // Initialize libusb
    r = libusb_init(NULL);
    if (r < 0) {
        fprintf(stderr, "Failed to initialize libusb: %s\n", libusb_error_name(r));
        return EXIT_FAILURE;
    }

    // Open the device
    handle = libusb_open_device_with_vid_pid(NULL, VENDOR_ID, PRODUCT_ID);
    if (!handle) {
        fprintf(stderr, "Could not open device (Vendor ID: 0x%04X, Product ID: 0x%04X)\n", VENDOR_ID, PRODUCT_ID);
        libusb_exit(NULL);
        return EXIT_FAILURE;
    }

    // Detach kernel driver if necessary
    if (libusb_kernel_driver_active(handle, WINDEX)) {
        r = libusb_detach_kernel_driver(handle, WINDEX);
        if (r != 0) {
            fprintf(stderr, "Failed to detach kernel driver: %s\n", libusb_error_name(r));
            libusb_close(handle);
            libusb_exit(NULL);
            return EXIT_FAILURE;
        }
    }

    // Claim the interface
    r = libusb_claim_interface(handle, WINDEX);
    if (r < 0) {
        fprintf(stderr, "Failed to claim interface %d: %s\n", WINDEX, libusb_error_name(r));
        libusb_close(handle);
        libusb_exit(NULL);
        return EXIT_FAILURE;
    }

    // Send the control transfer
    r = libusb_control_transfer(handle,
                                0x21,        // bmRequestType: Host to Device | Class | Interface
                                0x09,        // bRequest: SET_REPORT
                                WVALUE,      // wValue: 0x035A
                                WINDEX,      // wIndex: Interface number
                                data,        // Data buffer
                                WLENGTH,     // wLength: 16 bytes
                                1000);       // Timeout in ms

    if (r < 0) {
        fprintf(stderr, "Control Transfer failed: %s\n", libusb_error_name(r));
        libusb_release_interface(handle, WINDEX);
        libusb_close(handle);
        libusb_exit(NULL);
        return EXIT_FAILURE;
    } else if (r != WLENGTH) {
        fprintf(stderr, "Warning: Only %d bytes were sent out of %d.\n", r, WLENGTH);
    } else {
        printf("Data packet sent successfully.\n");
    }

    // Release the interface and close the device
    libusb_release_interface(handle, WINDEX);
    libusb_close(handle);
    libusb_exit(NULL);

    return EXIT_SUCCESS;
}
