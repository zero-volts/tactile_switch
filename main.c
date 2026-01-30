#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "gpio_buttons.h"

int main()
{   
    unsigned int TACTILE_GPIO_1 = 21;   //40 pin fisico
    unsigned int TOUCH_GPIO = 26;       //37 pin fisico

    struct gpio_btn *button1 = gpio_btn_init(TACTILE_GPIO_1);
    struct gpio_btn *button2 = gpio_btn_init(TOUCH_GPIO);
    if (!button1 || !button2 ) {
        if (button1) {
            gpio_btn_close(button1);
        }

        if (button2) {
            gpio_btn_close(button2);
        }

        return 1;
    }
    
    printf ("waiting user to press the tactile button (pull-down: press => 1)... \n");
    while(1)
    {
        int gpio_state = gpio_btn_get(button1);
        printf ("gpio_state: %d \n", gpio_state);
        if (gpio_state < 0) {
            perror("Read line 19 failed");
            break;
        }
        
        if (gpio_state == 0) {
            break;
        }

        int gpio_state2 = gpio_btn_get(button2);
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
    
    gpio_btn_close(button1);
    gpio_btn_close(button2);

    return 0;
}
