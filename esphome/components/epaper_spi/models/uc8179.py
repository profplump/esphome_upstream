"""UC8179 e-paper controller driver.

Supported models:
- waveshare-5.83in-v2: 648x480 pixels (Waveshare 5.83" V2 display)
  https://www.waveshare.com/w/upload/3/37/5.83inch_e-Paper_V2_Specification.pdf
  https://github.com/waveshare/e-Paper/blob/master/Arduino/epd5in83_V2/epd5in83_V2.cpp
"""

from . import EpaperModel

# fmt: off
EpaperModel(
    "waveshare-5.83in-v2",
    "EPaperUC8179",
    width=648,
    height=480,
    minimum_update_interval="5s",
    initsequence=(
        (0x01, 0x07, 0x07, 0x3F, 0x3F),        # Power Setting
        (0x00, 0x1F),                            # Panel Setting
        (0x61, 0x02, 0x88, 0x01, 0xE0),         # Resolution Setting: 648x480
        (0x15, 0x00),                            # Dual SPI
        (0x60, 0x22),                            # TCON Setting
        (0x30, 0x3C),                            # PLL Control
        (0x04,),                                 # Power On - last; FSM waits for idle before data transfer
    ),
)
