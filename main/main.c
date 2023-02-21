/* Reaction time game */
#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"

#define LED_GPIO    GPIO_NUM_2
#define BTN_GPIO    GPIO_NUM_4

#define NUM_LEDS sizeof(ledArray)
uint8_t ledArray[] = 
{
  GPIO_NUM_32,  // Leftmost LED
  GPIO_NUM_33,
  GPIO_NUM_25,
  GPIO_NUM_26,
  GPIO_NUM_27,
  GPIO_NUM_14,
  GPIO_NUM_12   //Rightmost LED
};

uint8_t btnArray[] = 
{
  GPIO_NUM_19,  // Left Btn
  GPIO_NUM_18   // Right Btn
};

enum
{
  eGoingLeft = -1,
  eGoingRight = 1
};

#define LEFT_BTN        GPIO_NUM_19
#define RIGHT_BTN       GPIO_NUM_18
#define LED_ON_DELAY    300
#define LED_OFF_DELAY   50
#define SPEED_UP        20

// Enumerations
typedef enum ButtonState_tag
{
    eBtnUndefined = 0,
    eBtnShortPressed,
    eBtnLongPressed
} eButtonState_t;

typedef enum PlayerInputState_tag
{
    eWaiting = 0,
    eCorrect,
    eIncorrect
} ePlayerInputState_t;

typedef enum ButtonId_tag
{
    eLeftButton = 0, // This enum must follow btnArray
    eRightButton,
    eMaxButtonId
} eButtonId_t;

typedef enum GameState_tag
{
    eGameRestart = 0,
    eWaitForStart,
    eOngoingGame,
    eEnd
} eGameState_t;

// Global variable
uint32_t _millis;
uint8_t score;

static void initIO(void)
{
    // FIXME:
    // Replace the following code and insert
    // code to initialize all IO pins for the assigment
    gpio_reset_pin(LED_GPIO);
    gpio_reset_pin(BTN_GPIO);
    /* Set LED GPIO as a push/pull output */
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    /* Set LED GPIO as a push/pull output */
    gpio_set_direction(BTN_GPIO, GPIO_MODE_INPUT);
    gpio_pullup_en(BTN_GPIO);
}

void playSequence(eGameState_t gameState)
{
    // FIXME:
    // Playback the corresponding animation of the gameState parameter
    gpio_set_level(LED_GPIO, !gpio_get_level(BTN_GPIO)); 
}

eButtonState_t checkButtons(eButtonId_t *buttonNumber)
{
    // FIXME:
    // Polls each button and returns the press state and the corresponding button
    *buttonNumber = eMaxButtonId;
    return eBtnUndefined;
}

ePlayerInputState_t checkPlayerInput(eButtonState_t buttonState, eButtonId_t buttonId)
{
    // FIXME:
    // Waits for player input and verifies that it is matching the pattern
    return eWaiting;
}

void delayMs(uint16_t ms)
{
    vTaskDelay(ms / portTICK_PERIOD_MS);
}

int app_main(void)
{
    eGameState_t  currentGameState = eGameRestart;
    ePlayerInputState_t playerInputState;
    eButtonId_t buttonId;
    eButtonState_t buttonState;

    initIO();

    while(1)
    {   
        buttonState = checkButtons(&buttonId);
        
        if (buttonState == eBtnLongPressed)
            currentGameState = eGameRestart;

        switch(currentGameState)
        {
            case eGameRestart:
                //Init game variables
                currentGameState++;
                break;

            case eWaitForStart:
                playSequence(eWaitForStart);
                if (buttonState == eBtnShortPressed)
                    currentGameState++;
                break;

            case eOngoingGame:
                playSequence(eOngoingGame);
                playerInputState = checkPlayerInput(buttonState, buttonId);
                if (playerInputState == eCorrect)
                {
                    score++;
                }
                else if (playerInputState == eIncorrect)
                {
                    currentGameState = eEnd;
                }
                break;
            case eEnd:
                playSequence(eEnd);
                break;
        }
        delayMs(1);
        _millis++;
    }
}