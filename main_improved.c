#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

int main(void)
{
    const char *chipname = "gpiochip0";
    const unsigned int PIN = 22; // BCM GPIO22 (physical pin 15)

    struct gpiod_chip *chip = gpiod_chip_open_by_name(chipname);
    if (!chip) {
        perror("Open chip failed");
        return 1;
    }

    printf("Opened chip %s\n", chipname);

    struct gpiod_line *tactile_btn = gpiod_chip_get_line(chip, PIN);
    if (!tactile_btn) {
        fprintf(stderr, "Cannot find line with offset %u\n", PIN);
        gpiod_chip_close(chip);
        return 1;
    }

    if (gpiod_line_request_input(tactile_btn, "tactile_button") < 0) {
        perror("Request line as input failed");
        gpiod_chip_close(chip);
        return 1;
    }

    printf("GPIO%u input ready. Press button to exit loop.\n", PIN);
    printf("Note: with pull-up, idle=1 and press=0.\n");

    while (1) {
        int value = gpiod_line_get_value(tactile_btn);
        if (value < 0) {
            perror("Read line failed");
            break;
        }

        printf("gpio_state: %d\n", value);

        // Exit on press (active-low) when button connects GPIO to GND
        if (value == 0) {
            printf("Button pressed, exiting.\n");
            break;
        }

        usleep(100000);
    }

    gpiod_line_release(tactile_btn);
    gpiod_chip_close(chip);
    return 0;
}
