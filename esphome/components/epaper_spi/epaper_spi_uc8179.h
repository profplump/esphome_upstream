#pragma once
#include "epaper_spi.h"

namespace esphome::epaper_spi {

/**
 * Driver for e-paper displays using the Ultrachip UC8179 controller (also referred to as IL0371).
 * Used in Waveshare 5.83" V2 and similar large-format monochrome displays.
 *
 * The UC8179 requires the pixel buffer to be sent twice: once via command 0x10 (old frame)
 * and once via command 0x13 (new frame). The controller computes the delta and drives
 * the display accordingly.
 */
class EPaperUC8179 final : public EPaperBase {
 public:
  EPaperUC8179(const char *name, uint16_t width, uint16_t height, const uint8_t *init_sequence,
               size_t init_sequence_length)
      : EPaperBase(name, width, height, init_sequence, init_sequence_length) {
    this->buffer_length_ = (size_t) width * height / 8;
  }

 protected:
  bool transfer_data() override;
  void power_on() override {}  // powered on at end of init sequence; FSM waits for idle
  void power_off() override;
  void refresh_screen(bool partial) override;
  void deep_sleep() override;

  bool second_channel_{false};
};

}  // namespace esphome::epaper_spi
