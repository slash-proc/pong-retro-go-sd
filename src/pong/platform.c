#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "gw_audio.h"
#include "gw_lcd.h"
#include "odroid_input.h"
#include "odroid_system.h"

#ifndef HOST_BUILD
#include "gw_core_bridge.h"
#else
#include "host_compat.h"
#endif

#include "gw_hardware/gw_buttons.h"
#include "platform.h"

static odroid_gamepad_state_t pad;
static uint32_t buttons;
static void *last_frame;
static unsigned long rng = 1;
static odroid_dialog_choice_t *options;

static uint32_t map_buttons(const odroid_gamepad_state_t *joy)
{
    uint32_t b = 0;

    if (joy->values[ODROID_INPUT_LEFT])   b |= B_Left;
    if (joy->values[ODROID_INPUT_UP])     b |= B_Up;
    if (joy->values[ODROID_INPUT_RIGHT])  b |= B_Right;
    if (joy->values[ODROID_INPUT_DOWN])   b |= B_Down;
    if (joy->values[ODROID_INPUT_A])      b |= B_A;
    if (joy->values[ODROID_INPUT_B])      b |= B_B;
    if (joy->values[ODROID_INPUT_SELECT]) b |= B_TIME | B_SELECT;
    if (joy->values[ODROID_INPUT_START])  b |= B_GAME | B_START;
    return b;
}

static void submit_silence(void)
{
    int16_t *buf;
    uint16_t len;

    if (common_emu_sound_loop_is_muted())
        return;

    buf = audio_get_active_buffer();
    len = audio_get_buffer_length();
    if (!buf || !len)
        return;
    memset(buf, 0, (size_t)len * sizeof(int16_t));
}

static void pong_repaint(void)
{
    /* Pause overlay composites over the last presented frame. */
}

void pong_platform_init(void)
{
    memset(&pad, 0, sizeof(pad));
    buttons = 0;
    last_frame = NULL;
    options = NULL;
}

void pong_set_options(odroid_dialog_choice_t *game_options)
{
    options = game_options;
}

void pong_poll(void)
{
    static odroid_dialog_choice_t none[] = {
        ODROID_DIALOG_CHOICE_LAST
    };

    wdog_refresh();
    (void)common_emu_frame_loop();
    odroid_input_read_gamepad(&pad);
    common_emu_input_loop(&pad, options ? options : none, &pong_repaint);
    common_emu_input_loop_handle_turbo(&pad);
    buttons = map_buttons(&pad);
}

void pong_present(void)
{
    common_ingame_overlay();
    last_frame = lcd_get_active_buffer();
    lcd_swap();
    submit_silence();
    common_emu_sound_sync(false);
    lcd_wait_for_vblank();
}

void pong_delay_ms(uint32_t ms)
{
    uint32_t frames = (ms * PONG_FPS + 999u) / 1000u;

    if (frames == 0)
        frames = 1;
    while (frames--) {
        pong_poll();
        submit_silence();
        common_emu_sound_sync(false);
        wdog_refresh();
    }
}

void pong_quit(void)
{
#ifdef HOST_BUILD
    exit(0);
#else
    odroid_system_switch_app(0);
    while (1) {
    }
#endif
}

void *pong_screenshot_buffer(void)
{
    return last_frame ? last_frame : lcd_get_active_buffer();
}

uint32_t buttons_get(void)
{
    return buttons;
}

void pong_srand(unsigned long seed)
{
    rng = seed ? seed : 1UL;
}

int pong_rand(void)
{
    rng = rng * 1103515245UL + 12345UL;
    return (int)((rng >> 16) & 0x7fff);
}
