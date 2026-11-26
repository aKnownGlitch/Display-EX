#pragma once

#include <stdint.h>

constexpr char LAYOUT_NAME[] = "Weinheimer-RR";
constexpr uint16_t LOCO_ID = 3;
constexpr uint16_t TURNOUT_IDS[] = {101, 102, 103};

constexpr unsigned long SERIAL_BAUD = 115200;
constexpr unsigned long UPDATE_INTERVAL = 1000;

static_assert(LOCO_ID >= 1 && LOCO_ID <= 32767, "LOCO_ID must be between 1 and 32767");
static_assert(TURNOUT_IDS[0] >= 1 && TURNOUT_IDS[0] <= 32767 &&
              TURNOUT_IDS[1] >= 1 && TURNOUT_IDS[1] <= 32767 &&
              TURNOUT_IDS[2] >= 1 && TURNOUT_IDS[2] <= 32767,
              "Turnout IDs must be between 1 and 32767");
static_assert(TURNOUT_IDS[0] != TURNOUT_IDS[1] &&
              TURNOUT_IDS[0] != TURNOUT_IDS[2] &&
              TURNOUT_IDS[1] != TURNOUT_IDS[2],
              "Turnout IDs must be unique");
