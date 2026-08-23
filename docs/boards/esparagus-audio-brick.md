# Esparagus Audio Brick

The [Esparagus Audio Brick](https://github.com/sonocotta/esparagus-media-center/?tab=readme-ov-file#esparagus-audio-brick-prototype)
is an ESP32 board built around a TI **TAS5825M** Class-D DAC and amplifier. Like the
SqueezeAMP, it needs no external DAC — connect speakers directly.

## Features

- TAS5825M with on-chip DSP and a 15-band parametric EQ (25 Hz – 16 kHz)
- Hardware volume control with a configurable maximum level
- Speaker fault detection with automatic mute and recovery
- Automatic power state management (deep sleep / standby / play) driven by AirPlay session state
- 8 MB flash
- Optional [Bluetooth A2DP](../features/bluetooth.md)
- Optional [W5500 SPI Ethernet](../features/ethernet.md) with automatic WiFi failover

## Flashing

=== "Browser"

    Use the Esparagus Audio Brick installer on the
    [flashing page](../getting-started/flashing.md). The published binary is the
    Bluetooth + Ethernet build.

=== "PlatformIO"

    ```bash
    # AirPlay only
    pio run -e esparagus-audio-brick -t upload
    pio run -e esparagus-audio-brick -t uploadfs

    # AirPlay + Bluetooth + Ethernet
    pio run -e esparagus-audio-brick-bt -t upload
    pio run -e esparagus-audio-brick-bt -t uploadfs

    # Serial monitor
    pio run -e esparagus-audio-brick -t monitor
    ```

=== "ESP-IDF"

    ```bash
    idf.py set-target esp32
    idf.py -DSDKCONFIG_DEFAULTS="config/sdkconfig.defaults;config/sdkconfig.defaults.esparagus-audio-brick" build
    idf.py -p /dev/ttyUSB0 flash
    ```

## Default GPIO assignments

| Function | GPIO | Notes |
| --- | --- | --- |
| I2S BCK | 26 | Bit clock |
| I2S WS | 25 | Word select (LRCLK) |
| I2S DO | 22 | Serial audio data |
| I2C SDA | 21 | DAC control (TAS5825M) |
| I2C SCL | 27 | DAC control (TAS5825M) |
| Jack detect | 34 | Headphone jack insertion, input |
| DAC warning | 36 | TAS5825M warning output, input |
| Speaker fault | 39 | TAS5825M fault output, input |

!!! note "GPIOs 34–39 are input-only"

    On the ESP32, GPIOs 34–39 are input-only and have no internal pull-up. The board
    provides external pull-ups on the fault and warning lines.

The build selects the TAS58xx driver automatically (`CONFIG_DAC_TAS58XX`). The driver
auto-detects the TAS5825M I2C address in the range 0x4C–0x4F at startup.

## Equaliser

TAS5825M boards expose a 15-band parametric EQ through the device's web interface at
`/eq.html`. Changes are applied to the DAC's on-chip DSP in real time and persisted to
NVS.

## Variants

| Environment | Chip | Notes |
| --- | --- | --- |
| `esparagus-audio-brick` | ESP32 | AirPlay only |
| `esparagus-audio-brick-bt` | ESP32 | Bluetooth + Ethernet, prebuilt binary published |
| `esparagus-audio-brick-s3` | ESP32-S3 | S3-based revision, no Bluetooth |
| `esparagus-audio-brick-dual-dac` | ESP32-S3 | Rev D with two TAS5825M: stereo at 0x4C, PBTL mono subwoofer at 0x4D |

### Esparagus Louder

The Esparagus Louder is the same TAS5825M design with additional gain.

| Environment | Chip | Bluetooth | Prebuilt |
| --- | --- | :-: | :-: |
| `esparagus-louder` | ESP32 | — | — |
| `esparagus-louder-bt` | ESP32 | yes | — |
| `esparagus-louder-s3` | ESP32-S3 | — | yes |

```bash
pio run -e esparagus-louder-s3 -t upload
pio run -e esparagus-louder-s3 -t uploadfs
```

## Related

- [Bluetooth A2DP](../features/bluetooth.md)
- [Ethernet (W5500)](../features/ethernet.md)
- [Build environments](../reference/build-environments.md)
