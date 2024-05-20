#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
/* STEP 6 - Include the header file of printk */
#include <zephyr/sys/printk.h>

/* STEP 8.1 - Define the macro MAX_NUMBER_FACT that represents the maximum number to calculate its factorial  */
#define MAX_Select 10

#define SLEEP_TIME_MS   10*60*1000

//#define SW0_NODE	DT_ALIAS(sw0)
//static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW0_NODE, gpios);

//#define LED0_NODE DT_ALIAS(led0)
//static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);

#define LED1_NODE DT_ALIAS(led1)
#define LED2_NODE DT_ALIAS(led2)
#define SW3_NODE  DT_ALIAS(sw3)  // Assuming button 3 is aliased as 'sw3'

static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET(SW3_NODE, gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);




static int select = 0;

void set_leds_based_on_selection(int selection) {
    int led1_state = 0; // Initialize to 0 (Off)
    int led2_state = 0; // Initialize to 0 (Off)

    switch (selection) {
        case 0: // Off-Off
            led1_state = 0;
            led2_state = 0;
            break;
        case 1: // Off-On
            led1_state = 0;
            led2_state = 1;
            break;
        case 2: // On-Off
            led1_state = 1;
            led2_state = 0;
            break;
        case 3: // On-On
            led1_state = 1;
            led2_state = 1;

            break;
        default:
            // Optionally handle unexpected values
            printk("Unexpected selection value: %d\n", selection);
            return; // Early return if the value is out of expected range
    }

    gpio_pin_set(led1.port, led1.pin, led1_state);
    gpio_pin_set(led2.port, led2.pin, led2_state);
}







/* STEP 8.2 - Replace the button callback function */
void button_pressed(const struct device *dev, struct gpio_callback *cb,
		    uint32_t pins)
{
  if (select == 3) {
        select = 0;
    } else {
        select++;
    }
	// Set LEDs based on the new value of select
  set_leds_based_on_selection(select);
    // Print the current value of select
    printk("Current select value: %d\n", select);
  /*Important note! 
  Code in ISR runs at a high priority, therefore, it should be written with timing in mind.
  Too lengthy or too complex tasks should not be performed by an ISR, they should be deferred to a thread. 
  */
}

static struct gpio_callback button_cb_data;

int main(void)
{
    int ret;
    printk("Testing button 3 with LEDs 1 and 2\n");

    if (!device_is_ready(led1.port) || !device_is_ready(led2.port) || !device_is_ready(button.port)) {
        printk("One or more devices are not ready\n");
        return -1;
    }

    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        printk("Failed to configure LED1\n");
        return -1;
    }

    ret = gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE);
    if (ret < 0) {
        printk("Failed to configure LED2\n");
        return -1;
    }

    ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
    if (ret < 0) {
        printk("Failed to configure button 3\n");
        return -1;
    }

    ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
    if (ret < 0) {
        printk("Failed to enable button 3 interrupt\n");
        return -1;
    }

    gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));    
    gpio_add_callback(button.port, &button_cb_data);    

    while (1) {
        k_msleep(SLEEP_TIME_MS); 
    }
}
