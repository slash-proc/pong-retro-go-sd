#ifndef PONG_PLATFORM_H_
#define PONG_PLATFORM_H_

#include <stdint.h>
#include <stdbool.h>
#include "odroid_overlay.h"

#define PONG_FPS         60
#define PONG_SAMPLE_RATE 16000
#define PONG_AUDIO_LEN   (PONG_SAMPLE_RATE / PONG_FPS)

void pong_platform_init(void);
void pong_set_options(odroid_dialog_choice_t *game_options);

/* Pump watchdog, firmware pause menu, and cached pad. Call every loop. */
void pong_poll(void);

/* True when this tick should blit + lcd_swap (same gate as tgb-dual:
 * common_emu_frame_loop() && !lcd_is_swap_pending()). */
bool pong_can_draw(void);

/* Overlay + conditional swap, then audio pacing via common_emu_sound_sync. */
void pong_present(void);

/* Busy-wait that still feeds the watchdog and SAI. */
void pong_delay_ms(uint32_t ms);

/* Return to the Retro-Go launcher (host: exit). */
void pong_quit(void) __attribute__((noreturn));

void *pong_screenshot_buffer(void);

void pong_srand(unsigned long seed);
int pong_rand(void);

#define HAL_NVIC_SystemReset() pong_quit()
#define HAL_Delay(ms)          pong_delay_ms(ms)

#endif
