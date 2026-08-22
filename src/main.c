/*
 * Pong — Retro-Go SD GWHB homebrew port of
 * https://github.com/Jopo12321/game-and-watch-pong
 *
 * Original game by Jopo12321 (BSD-3-Clause). Hardware init / DMA2D / reset
 * paths are replaced by the firmware ABI (LCD, pad, audio, settings).
 */

#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "common.h"
#include "gw_lcd.h"
#include "gw_audio.h"
#include "odroid_system.h"
#include "odroid_overlay.h"
#include "odroid_settings.h"
#include "odroid_audio.h"

#ifndef HOST_BUILD
#include "gw_core_bridge.h"
#include "gw_core_i18n.h"
#else
#include "host_compat.h"
#include "gw_core_i18n.h"
#include <stm32h7xx_hal.h>
#endif

#include "platform.h"
#include "main_menu/main_menu.h"
#include "main_menu/settings.h"

#if defined(PROJECT_KIND_HOMEBREW)
#define APP_ID  14
#else
#define APP_ID  100
#endif

static const gw_i18n_entry_t i18n_ingame[] = {
    { "en", "In-game menu (GAME)" },
    { "fr", "Menu en jeu (GAME)" },
    { "es", "Menú en partida (GAME)" },
    { "de", "Spielmenü (GAME)" },
    GW_I18N_END
};

static char ingame_hint[8] = "GAME";

static bool hint_update_cb(odroid_dialog_choice_t *option,
                           odroid_dialog_event_t event, uint32_t repeat)
{
    (void)repeat;
    strncpy(option->value, ingame_hint, 7);
    option->value[7] = '\0';
    return event == ODROID_DIALOG_ENTER;
}

static bool LoadState(const char *savePathName)
{
    (void)savePathName;
    return false;
}

static bool SaveState(const char *savePathName)
{
    (void)savePathName;
    return false;
}

static void *Screenshot(void)
{
    lcd_wait_for_vblank();
    return pong_screenshot_buffer();
}

static void Shutdown(void)
{
    pong_settings_save();
}

static void SleepWake(void)
{
    odroid_audio_init(PONG_SAMPLE_RATE);
    audio_clear_buffers();
    audio_start_playing(PONG_AUDIO_LEN);
}

static void SramSave(void)
{
}

void app_main(uint8_t load_state, uint8_t start_paused, int8_t save_slot)
{
    odroid_dialog_choice_t options[2];

    gw_core_bridge_init();
    pong_platform_init();
    pong_srand((unsigned long)HAL_GetTick() ^ 0xA5A5u);

    if (start_paused) {
        common_emu_state.pause_after_frames = 2;
        odroid_audio_mute(true);
    } else {
        common_emu_state.pause_after_frames = 0;
    }
    common_emu_state.frame_time_10us = (uint16_t)(100000 / PONG_FPS + 0.5f);
    lcd_set_refresh_rate(PONG_FPS);

    odroid_system_init(APP_ID, PONG_SAMPLE_RATE);
    odroid_system_emu_init(&LoadState, &SaveState, &Screenshot,
                           &Shutdown, &SleepWake, &SramSave, NULL);

    pong_settings_load();

    options[0].id = 100;
    options[0].label = gw_i18n(i18n_ingame);
    options[0].value = ingame_hint;
    options[0].enabled = 0;
    options[0].update_cb = &hint_update_cb;
    options[1] = (odroid_dialog_choice_t)ODROID_DIALOG_CHOICE_LAST;
    pong_set_options(options);

    audio_start_playing(PONG_AUDIO_LEN);

    if (load_state)
        odroid_system_emu_load_state(save_slot);
    else
        lcd_clear_buffers();

    while (1)
        main_menu_init();
}
