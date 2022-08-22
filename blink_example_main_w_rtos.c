/* Blink Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "led_strip.h"
#include "sdkconfig.h"

static const char *TAG = "example";

/* Use project configuration menu (idf.py menuconfig) to choose the GPIO to blink,
   or you can edit the following line and set a number here.
*/
    #define Green_LED 32
    #define Red_LED 33

static uint8_t s_led_state = 0;
static uint8_t s_led_state_red = 0;

#ifdef CONFIG_BLINK_LED_RMT
static led_strip_t *pStrip_a;
static led_strip_t *pStrip_b;
static void blink_led_g(void)
{
    /* If the addressable LED is enabled */
    if (s_led_state) {
        /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        pStrip_a->set_pixel(pStrip_a, 0, 16, 16, 16);
        /* Refresh the strip to send data */
        pStrip_a->refresh(pStrip_a, 100);
    } else {
        /* Set all LED off to clear all pixels */
        pStrip_a->clear(pStrip_a, 50);
    }
}
static void blink_led_r(void)
{
    /* If the addressable LED is enabled */
    if (s_led_state_red) {
        /* Set the LED pixel using RGB from 0 (0%) to 255 (100%) for each color */
        pStrip_b->set_pixel(pStrip_b, 0, 16, 16, 16);
        /* Refresh the strip to send data */
        pStrip_b->refresh(pStrip_b, 100);
    } else {
        /* Set all LED off to clear all pixels */
        pStrip_b->clear(pStrip_b, 50);
    }
}

static void configure_green_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink addressable LED!");
    /* LED strip initialization with the GPIO and pixels number*/
    pStrip_a = led_strip_init(CONFIG_BLINK_LED_RMT_CHANNEL, Green_LED, 1);
   
    
    /* Set all LED off to clear all pixels */
    pStrip_a->clear(pStrip_a, 50);
   
}
static void configure_red_led(void)
{
    pStrip_b = led_strip_init(CONFIG_BLINK_LED_RMT_CHANNEL, Red_LED, 1);
    pStrip_b->clear(pStrip_b,50);
}

#elif CONFIG_BLINK_LED_GPIO

static void blink_led_g(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(Green_LED, s_led_state);
}
static void blink_led_r(void)
{
    /* Set the GPIO level according to the state (LOW or HIGH)*/
    gpio_set_level(Red_LED, s_led_state_red);
}


static void configure_green_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(Green_LED);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(Green_LED, GPIO_MODE_OUTPUT);
}
static void configure_red_led(void)
{
    ESP_LOGI(TAG, "Example configured to blink GPIO LED!");
    gpio_reset_pin(Red_LED);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(Red_LED, GPIO_MODE_OUTPUT);
}

#endif
void task_green(void)
{
    configure_green_led();

    while (1) {
        ESP_LOGI(TAG, "Turning the Green LED %s!", s_led_state == true ? "ON" : "OFF");
        blink_led_g();
        /* Toggle the LED state */
        s_led_state = !s_led_state;
        vTaskDelay(1000/ portTICK_PERIOD_MS);
    }
}
void task_red(void)
{
    configure_red_led();
    
    while (1) {
        ESP_LOGI(TAG, "Turning the RED LED %s!", s_led_state_red == true ? "ON" : "OFF");
        blink_led_r();
        /* Toggle the LED state */
        s_led_state_red = !s_led_state_red;
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
void app_main(void)
{

   xTaskCreate(&task_green, "Green Blinking", 2048, "task green", 1, NULL);
   xTaskCreate(&task_red, "Red Blinking", 2048, "task RED", 2, NULL);

}
