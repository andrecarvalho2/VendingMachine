/**
 * @file main.c
 * @brief Main source file for the Vending Machine project.
 *
 * This file contains the main function and related functionalities to handle
 * the vending machine operations using a statechart-based approach.
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/printk.h>
#include "../src/sc_types.h"
#include "Statechart.h"
#include "Statechart_required.h"

#define SLEEP_TIME_MS   10*60*1000  ///< Define sleep time in milliseconds.

// GPIO Definitions using Device Tree Aliases
#define BUTTON1_NODE DT_ALIAS(sw0)  ///< Node for button 1.
#define BUTTON2_NODE DT_ALIAS(sw1)  ///< Node for button 2.
#define BROWSE_BUTTON_NODE DT_ALIAS(sw2)  ///< Node for the browse button.
#define ENTER_BUTTON_NODE DT_ALIAS(sw3)  ///< Node for the enter button.

#define LED1_NODE DT_ALIAS(led0)  ///< Node for LED 1.
#define LED2_NODE DT_ALIAS(led1)  ///< Node for LED 2.
#define LED3_NODE DT_ALIAS(led2)  ///< Node for LED 3.
#define LED4_NODE DT_ALIAS(led3)  ///< Node for LED 4.

// GPIO device specifications (Global Declarations)
static const struct gpio_dt_spec button1 = GPIO_DT_SPEC_GET(BUTTON1_NODE, gpios);  ///< GPIO spec for button 1.
static const struct gpio_dt_spec button2 = GPIO_DT_SPEC_GET(BUTTON2_NODE, gpios);  ///< GPIO spec for button 2.
static const struct gpio_dt_spec browseButton = GPIO_DT_SPEC_GET(BROWSE_BUTTON_NODE, gpios);  ///< GPIO spec for browse button.
static const struct gpio_dt_spec enterButton = GPIO_DT_SPEC_GET(ENTER_BUTTON_NODE, gpios);  ///< GPIO spec for enter button.
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(LED1_NODE, gpios);  ///< GPIO spec for LED 1.
static const struct gpio_dt_spec led2 = GPIO_DT_SPEC_GET(LED2_NODE, gpios);  ///< GPIO spec for LED 2.
static const struct gpio_dt_spec led3 = GPIO_DT_SPEC_GET(LED3_NODE, gpios);  ///< GPIO spec for LED 3.
static const struct gpio_dt_spec led4 = GPIO_DT_SPEC_GET(LED4_NODE, gpios);  ///< GPIO spec for LED 4.

Statechart statechart_instance;  ///< Instance of Statechart.

/**
 * @brief Prints the current credit amount.
 * @param handle Pointer to the statechart handling the logic.
 * @param credit Current credit amount.
 * @return Always returns 0 indicating success.
 */
sc_integer statechart_print_credit(Statechart* handle, const sc_integer credit) {
    printk("Credit: %d\n", credit);
    return 0;
}

/**
 * @brief Prints the selected product.
 * @param handle Pointer to the statechart handling the logic.
 * @param select Product selected.
 * @return Always returns 0 indicating success.
 */
sc_integer statechart_print_product_selected(Statechart* handle, const sc_integer select) {
    printk("Product selected: %d\n", select);
    printk("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    return 0;
}

/**
 * @brief Prints a message to insert more coins due to insufficient credit.
 * @param handle Pointer to the statechart handling the logic.
 * @return Always returns 0 indicating success.
 */
sc_integer statechart_print_no_credit_insert_more_coins(Statechart* handle) {
    printk("No credit. Please insert more coins.\n");
    return 0;
}

/**
 * @brief Prints that no credit is available.
 * @param handle Pointer to the statechart handling the logic.
 * @return Always returns 0 indicating success.
 */
sc_integer statechart_print_no_credit_available(Statechart* handle) {
    printk("No credit available.\n");
    return 0;
}

/**
 * @brief Prints that the product is being dispensed.
 * @param handle Pointer to the statechart handling the logic.
 * @return Always returns 0 indicating success.
 */
sc_integer statechart_print_dispensing_product(Statechart* handle) {
    printk("Dispensing product.\n");
    printk("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    return 0;
}

/**
 * @brief Prints a prompt to pick up the change.
 * @param handle Pointer to the statechart handling the logic.
 * @return Always returns 0 indicating success.
 */
sc_integer statechart_print_please_pick_your_change(Statechart* handle) {
    printk("Please pick your change.\n");
    printk("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    return 0;
}

/**
 * @brief Prints a prompt to press any button to proceed.
 * @param handle Pointer to the statechart handling the logic.
 * @return Always returns 0 indicating success.
 */
sc_integer statechart_print_press_any_button_to_proceed(Statechart* handle) {
    printk("Press any button to proceed.\n");
    printk("xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n");
    return 0;
}

// Function implementations for controlling LEDs based on statechart actions.
/**
 * @brief Turns on the credit indication LED.
 * 
 * This function activates the LED associated with the credit system,
 * signaling that credit has been added or is available.
 * 
 * @param handle Pointer to the statechart managing the state of the machine.
 * @return Always returns 0 to indicate successful execution.
 */
sc_integer statechart_led_credit_on(Statechart* handle) {
    gpio_pin_set(led4.port, led4.pin, 1);
    return 0;
}

/**
 * @brief Turns off the credit indication LED.
 * 
 * This function deactivates the LED associated with the credit system,
 * signaling that no credit is available.
 * 
 * @param handle Pointer to the statechart managing the state of the machine.
 * @return Always returns 0 to indicate successful execution.
 */
sc_integer statechart_led_credit_off(Statechart* handle) {
    gpio_pin_set(led4.port, led4.pin, 0);
    return 0;
}

/**
 * @brief Turns on the product dispensing LED.
 * 
 * Activates the LED that indicates a product is currently being dispensed.
 * 
 * @param handle Pointer to the statechart managing the state of the machine.
 * @return Always returns 0 to indicate successful execution.
 */
sc_integer statechart_led_dispense_on(Statechart* handle) {
    gpio_pin_set(led3.port, led3.pin, 1);
    return 0;
}

/**
 * @brief Turns off the product dispensing LED.
 * 
 * Deactivates the LED that indicates a product is currently being dispensed.
 * 
 * @param handle Pointer to the statechart managing the state of the machine.
 * @return Always returns 0 to indicate successful execution.
 */
sc_integer statechart_led_dispense_off(Statechart* handle) {
    gpio_pin_set(led3.port, led3.pin, 0);
    return 0;
}

/**
 * @brief Turns off the product selection LEDs.
 * 
 * Deactivates the LEDs that are used to indicate the current product selection.
 * 
 * @param handle Pointer to the statechart managing the state of the machine.
 * @return Always returns 0 to indicate successful execution.
 */
sc_integer statechart_led_product_selected_off(Statechart* handle) {
    gpio_pin_set(led1.port, led1.pin, 0);
    gpio_pin_set(led2.port, led2.pin, 0);
    printk("Turning off Product LEDs\n");
    return 0;
}

/**
 * @brief Updates the state of product selection LEDs based on the given input.
 * 
 * Adjusts the product selection LEDs based on the integer value provided, which
 * maps to specific combinations of LED states.
 * 
 * @param handle Pointer to the statechart managing the state of the machine.
 * @param a Integer representing the desired LED state combination.
 * @return Returns 0 for successful execution or -1 for unexpected selection values.
 */
sc_integer statechart_led_product_selected(Statechart* handle, const sc_integer a) {
    int led1_state = 0, led2_state = 0;
    switch (a) {
        case 0: // Both LEDs off
            led1_state = 0;
            led2_state = 0;
            break;
        case 1: // LED1 off, LED2 on
            led1_state = 0;
            led2_state = 1;
            break;
        case 2: // LED1 on, LED2 off
            led1_state = 1;
            led2_state = 0;
            break;
        case 3: // Both LEDs on
            led1_state = 1;
            led2_state = 1;
            break;
        default:
            printk("Unexpected selection value: %d\n", a);
            return -1;
    }

    gpio_pin_set(led1.port, led1.pin, led1_state);
    gpio_pin_set(led2.port, led2.pin, led2_state);
    return 0;
}


/**
 * @brief Callback function for button press events.
 * @param dev Device structure.
 * @param cb Callback structure.
 * @param pins Pins involved in the interrupt.
 */
void button_pressed(const struct device *dev, struct gpio_callback *cb, uint32_t pins) {
    if (pins & BIT(browseButton.pin)) {
        statechart_but_raise_browse(&statechart_instance);
    }
    if (pins & BIT(enterButton.pin)) {
        statechart_but_raise_enter(&statechart_instance);
    }
    if (pins & BIT(button1.pin)) {
        statechart_but_raise_coin1(&statechart_instance);
    }
    if (pins & BIT(button2.pin)) {
        statechart_but_raise_coin2(&statechart_instance);
    }
}

static struct gpio_callback button_cb_data;  ///< Callback data for button interrupts.

/**
 * @brief Main entry point for the Vending Machine application.
 * @return Returns 0 if successful, or an error code on failure.
 */
int main(void) {
    int ret;

    printk("\n xxxxxxxxxxxxxx   VendingMachine   xxxxxxxxxxxxxx \n");

    // Check if all devices are ready
    if (!device_is_ready(led1.port) || !device_is_ready(led2.port) ||
        !device_is_ready(button1.port) || !device_is_ready(button2.port) ||
        !device_is_ready(browseButton.port) || !device_is_ready(enterButton.port)) {
        printk("One or more devices are not ready\n");
        return -1;
    }

    // Configure GPIOs
    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT_INACTIVE);
    ret |= gpio_pin_configure_dt(&led2, GPIO_OUTPUT_INACTIVE);
    ret |= gpio_pin_configure_dt(&led3, GPIO_OUTPUT_INACTIVE);
    ret |= gpio_pin_configure_dt(&led4, GPIO_OUTPUT_INACTIVE);

    ret |= gpio_pin_configure_dt(&button1, GPIO_INPUT);
    ret |= gpio_pin_configure_dt(&button2, GPIO_INPUT);
    ret |= gpio_pin_configure_dt(&browseButton, GPIO_INPUT);
    ret |= gpio_pin_configure_dt(&enterButton, GPIO_INPUT);

    ret |= gpio_pin_interrupt_configure_dt(&button1, GPIO_INT_EDGE_TO_ACTIVE);
    ret |= gpio_pin_interrupt_configure_dt(&button2, GPIO_INT_EDGE_TO_ACTIVE);
    ret |= gpio_pin_interrupt_configure_dt(&browseButton, GPIO_INT_EDGE_TO_ACTIVE);
    ret |= gpio_pin_interrupt_configure_dt(&enterButton, GPIO_INT_EDGE_TO_ACTIVE);

    if (ret != 0) {
        printk("Failed to configure GPIOs\n");
        return ret;
    }

    gpio_init_callback(&button_cb_data, button_pressed, BIT(button1.pin) | BIT(button2.pin) | BIT(browseButton.pin) | BIT(enterButton.pin));
    gpio_add_callback(button1.port, &button_cb_data);

    statechart_init(&statechart_instance);
    statechart_enter(&statechart_instance);

    while (1) {
        k_msleep(SLEEP_TIME_MS);
    }
}
