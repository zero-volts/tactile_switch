#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>

struct gpio_btn {
    struct gpiod_line *line;
    struct gpiod_chip *chip;
};

struct gpio_btn *initialize_gpio(int pin)
{
    const char *chipname = "gpiochip0";
    struct gpio_btn *button = (struct gpio_btn *)malloc(sizeof(struct gpio_btn));

    // Open GPIO chip
    struct gpiod_chip *chip = gpiod_chip_open_by_name(chipname);
    printf("chip %s\n", chip);
    if (!chip) 
    {
        perror("Open chip failed");
        return NULL;
    }

    struct gpiod_line *line = gpiod_chip_get_line(chip, pin);
    if (!line) 
    {
        fprintf(stderr, "Cannot find line with pin %d\n", pin);
        gpiod_chip_close(chip);
        return NULL;
    }

    if (gpiod_line_request_input(line, "test_gpio") < 0) 
    {
        perror("Request line as output failed");
        gpiod_chip_close(chip);
        return NULL;
    }

    
    button->line = line;
    button->chip = chip;
   
    return button;
}

int main()
{   
    unsigned int TACTILE_PIN = 22;  // GPIO22 physic pin 15
    unsigned int TOUCH_PIN = 27;    // GPIO27 physic pin 13

    struct gpio_btn *button = initialize_gpio(TOUCH_PIN);
    
    printf ("waiting user to press the tactile button... \n");
    while(1)
    {
        int gpio_state = gpiod_line_get_value(button->line);
        printf ("gpio_state: %d \n", gpio_state);
        if (gpio_state == 1) {
            break;
        }
        
        usleep(100000);
    }

    printf ("tactile button pressed!\n");
    
    gpiod_line_release(button->line);
    gpiod_chip_close(button->chip);

    return 0;
}