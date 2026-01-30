#include <gpiod.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

struct gpio_btn {
    struct gpiod_line *line;
    struct gpiod_chip *chip;
};

struct gpio_btn *initialize_gpio(int pin)
{
    const char *chipname = "gpiochip0";
    struct gpio_btn *button = (struct gpio_btn *)malloc(sizeof(struct gpio_btn));
    if (!button) {
        perror("malloc failed");
        return NULL;
    }

    // Open GPIO chip
    struct gpiod_chip *chip = gpiod_chip_open_by_name(chipname);
    printf("chip %s\n", chipname);
    if (!chip) 
    {
        perror("Open chip failed");
        free(button);
        return NULL;
    }

    struct gpiod_line *line = gpiod_chip_get_line(chip, pin);
    if (!line) 
    {
        fprintf(stderr, "Cannot find line with pin %d\n", pin);
        gpiod_chip_close(chip);
        free(button);
        return NULL;
    }

    if (gpiod_line_request_input(line, "test_gpio") < 0) 
    {
        perror("Request line as input failed");
        gpiod_chip_close(chip);
        free(button);
        return NULL;
    }

    button->line = line;
    button->chip = chip;
   
    return button;
}

int main()
{   
    unsigned int TACTILE_GPIO_1 = 21;   //40 pin fisico
    unsigned int TOUCH_GPIO = 26;       //37 pin fisico

    struct gpio_btn *button1 = initialize_gpio(TACTILE_GPIO_1);
    struct gpio_btn *button2 = initialize_gpio(TOUCH_GPIO);
    if (!button1 || !button2 ) {
        if (button1) {
            gpiod_line_release(button1->line);
            gpiod_chip_close(button1->chip);
            free(button1);
        }

        if (button2) {
            gpiod_line_release(button2->line);
            gpiod_chip_close(button2->chip);
            free(button2);
        }

        return 1;
    }
    
    printf ("waiting user to press the tactile button (pull-down: press => 1)... \n");
    while(1)
    {
        int gpio_state = gpiod_line_get_value(button1->line);
        printf ("gpio_state: %d \n", gpio_state);
        if (gpio_state < 0) {
            perror("Read line 19 failed");
            break;
        }
        
        if (gpio_state == 0) {
            break;
        }

        int gpio_state2 = gpiod_line_get_value(button2->line);
        printf ("gpio_state2: %d \n", gpio_state2);
        if (gpio_state2 < 0) {
            perror("Read line 16 failed");
            break;
        }

        if (gpio_state2 == 0) {
            break;
        }
        
        usleep(100000);
    }

    printf ("tactile button pressed!\n");
    
    gpiod_line_release(button1->line);
    gpiod_chip_close(button1->chip);
    free (button1);

    gpiod_line_release(button2->line);
    gpiod_chip_close(button2->chip);
    free (button2);

    return 0;
}