#include "epaper_spi_uc8179.h"

#include "esphome/core/log.h"

namespace esphome::epaper_spi {

static const char *const TAG = "epaper_spi.uc8179";

bool EPaperUC8179::transfer_data() {
  const uint32_t start_time = App.get_loop_component_start_time();
  const size_t buffer_length = this->buffer_length_;

  if (this->current_data_index_ == 0) {
    if (!this->second_channel_) {
      this->cmd_data(0x82, {0x12});  // VCM DC setting
      this->cmd_data(0x50, {0x97});  // VCOM and Data Interval Setting
      this->command(0x10);           // Data Start Transmission 1 (old frame)
    } else {
      this->command(0x13);           // Data Start Transmission 2 (new frame)
    }
  }

  uint8_t bytes_to_send[MAX_TRANSFER_SIZE];
  size_t buf_idx = 0;
  while (this->current_data_index_ < buffer_length) {
    bytes_to_send[buf_idx++] = this->buffer_[this->current_data_index_++];
    if (buf_idx == sizeof(bytes_to_send)) {
      this->start_data_();
      this->write_array(bytes_to_send, buf_idx);
      this->disable();
      buf_idx = 0;
      if (millis() - start_time > MAX_TRANSFER_TIME) {
        return false;
      }
    }
  }
  if (buf_idx != 0) {
    this->start_data_();
    this->write_array(bytes_to_send, buf_idx);
    this->disable();
  }

  this->current_data_index_ = 0;

  if (!this->second_channel_) {
    this->second_channel_ = true;
    return false;
  }

  this->second_channel_ = false;
  return true;
}

void EPaperUC8179::refresh_screen(bool partial) {
  ESP_LOGV(TAG, "Refresh");
  this->command(0x12);
}

void EPaperUC8179::power_off() {
  ESP_LOGV(TAG, "Power off");
  this->command(0x02);
}

void EPaperUC8179::deep_sleep() {
  ESP_LOGV(TAG, "Deep sleep");
  this->cmd_data(0x07, {0xA5});
}

}  // namespace esphome::epaper_spi
