/*
 * Copyright (C) 2024 Xiaomi Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _BRIGHTNESS_H
#define _BRIGHTNESS_H

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <stdint.h>
#include <uv.h>

#include "BrightnessServiceC.h"

#ifdef __cplusplus
extern "C" {
#endif

/****************************************************************************
 * Public Types
 ****************************************************************************/

enum {
    BRIGHTNESS_MONITOR_LEVEL = 0,
    BRIGHTNESS_MONITOR_MODE = 1,
};

typedef void(brightness_update_cb_t)(int type, intptr_t arg, void *user_data);
struct brightness_session_s;
typedef struct brightness_session_s brightness_session_t;

/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

int brightness_service_start(uv_loop_t *loop);
void brightness_service_stop(void);

/**
 * Create an instance to control brightness, and use it.
 * Normally the instance is bounded to a specific application, and when
 * application exits, system automatically takes over the control.
 *
 * @return the instance of brightness control
 */
brightness_session_t *brightness_create_session(void);

/**
 * Destroy the instance of brightness control.
 * After this call, system takes brightness control.
 * @param session the instance of brightness control
 */
void brightness_destroy_session(brightness_session_t *session);

/**
 * Get system brightness session.
 * @return the system brightness session
 */
brightness_session_t *brightness_get_system_session(void);

/**
 * Set/get brightness level.
 * When session is NULL, this API changes system level brightness.
 * @param session the brightness session instance
 * @param level the brightness level to set
 * @param ramp the brightness ramp speed in level per second, 0 to take effect
 *             immediately
 * @return 0 on success, negative on error
 */
int brightness_set_target(brightness_session_t *session, int level, int ramp);

/**
 * Get current brightness level. Current brightness may differ with set value
 * when smooth timer is used.
 * @param session the brightness session instance
 * @return the current brightness level, negative number on error.
 */
int brightness_get_target(brightness_session_t *session);

/**
 * Get current brightness level. Current brightness may differ with target
 * value when smooth timer is used.
 */
int brightness_get_current_level(void);

/**
 * Set brightness mode.
 * When session is NULL, this API changes system brightness mode.
 * @param session the brightness session instance
 * @param mode the brightness mode to set
 * @return 0 on success, negative on error
 */
int brightness_set_mode(brightness_session_t *session,
                        brightnessctl_mode_t mode);

/**
 * Get current brightness mode.
 * @param session the brightness session instance
 * @return the current brightness mode, negative number on error.
 */
brightnessctl_mode_t brightness_get_mode(brightness_session_t *session);

/**
 * Set brightness update callback. When display brightness changes, it will
 * call this callback.
 *
 * @param session the brightness session instance
 * @param cb the brightness update callback, set cb to NULL clears existing cb.
 * @param user_data the user data to pass to callback
 */
int brightness_set_update_cb(brightness_session_t *session,
                             brightness_update_cb_t *cb, void *user_data);

static inline int brightness_display_turn_off(brightness_session_t *session)
{
    int ret = brightness_set_mode(session, BRIGHTNESS_MODE_MANUAL);
    if (ret < 0)
        return ret;

    return brightness_set_target(session, BRIGHTNESS_LEVEL_OFF, 0);
}

static inline int brightness_display_full_power(brightness_session_t *session)
{
    return brightness_set_target(session, BRIGHTNESS_LEVEL_FULL, 0);
}

/**
 * Set the auto brightness user control point. Only valid when controller is
 * in auto mode.
 * @param session the brightness session instance
 * @param lux the lux value of the user control point
 * @param target the target brightness level of the user control point
 * @return 0 on success, negative on error
 */
int brightness_set_user_point(brightness_session_t *session, int lux,
                              int target);

/**
 * Get the auto brightness user control point.
 * @param session the brightness session instance
 * @param lux the lux value of the user control point
 * @param target the target brightness level of the user control point
 * @return 0 on success, negative on error
 */
int brightness_get_user_point(brightness_session_t *session, int *lux,
                              int *target);

#ifdef __cplusplus
} /*extern "C"*/
#endif

#endif
