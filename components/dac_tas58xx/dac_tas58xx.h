#pragma once

#include "dac.h"

/**
 * TAS58xx (TAS5825M) DAC driver ops — register with dac_register() before
 * calling dac_init().
 */
extern const dac_ops_t dac_tas58xx_ops;

/** Sub level-trim limits (dB), relative to the master volume. */
#define TAS58XX_SUB_OFFSET_MIN_DB (-15.0f)
#define TAS58XX_SUB_OFFSET_MAX_DB (15.0f)

/**
 * Set the sub (PBTL mono) volume offset in dB, relative to the master volume.
 * Positive raises the bass level, negative lowers it. Clamped to
 * [TAS58XX_SUB_OFFSET_MIN_DB, TAS58XX_SUB_OFFSET_MAX_DB]. Safe to call before
 * dac_init(); the value is applied on the next volume update.
 */
void dac_tas58xx_set_sub_offset_db(float offset_db);

/** Get the current sub volume offset in dB. */
float dac_tas58xx_get_sub_offset_db(void);

/** Crossover frequency limits for the PBTL sub low-pass (Hz). */
#define TAS58XX_XOVER_MIN_HZ 40.0f
#define TAS58XX_XOVER_MAX_HZ 400.0f

/**
 * Set the low-pass crossover frequency applied to the PBTL mono sub, so it
 * only reproduces bass rather than the full-range feed. Anything below
 * TAS58XX_XOVER_MIN_HZ disables the filter; higher values are clamped to
 * TAS58XX_XOVER_MAX_HZ.
 */
void dac_tas58xx_set_sub_crossover_hz(float hz);

/** Get the sub crossover frequency in Hz (0 when full range). */
float dac_tas58xx_get_sub_crossover_hz(void);

/** Role of the second amplifier on dual-DAC boards. */
typedef enum {
  /** Secondary drives a bridged (PBTL) mono subwoofer; primary stays stereo. */
  TAS58XX_DUAL_SUB = 0,
  /** Primary carries the left channel, secondary the right — one speaker
   *  per amplifier, both of its outputs driven from the same channel. */
  TAS58XX_DUAL_BIAMP = 1,
} tas58xx_dual_mode_t;

/**
 * Number of TAS58xx chips found on the I2C bus. Returns 0 before dac_init();
 * >1 means the board is a dual-DAC variant.
 */
int dac_tas58xx_get_device_count(void);

/** Get the configured second-amplifier role. */
tas58xx_dual_mode_t dac_tas58xx_get_dual_mode(void);

/**
 * Set the second-amplifier role. PBTL is a control-port setting that can only
 * be changed while the output stage is idle, so the new role is stored and
 * applied by the next dac_init() — the caller must restart the device.
 */
void dac_tas58xx_set_dual_mode(tas58xx_dual_mode_t mode);
