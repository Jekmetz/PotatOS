#pragma once

int poll_input(char* buffer, int* length);

typedef struct control_sequence {
  char code[8];
  int id;
} ControlSequence;

enum KEYS {
  BASE = 1024,

  UP_ARROW,
  DOWN_ARROW,
  RIGHT_ARROW,
  LEFT_ARROW,

  HOME,
  INSERT,
  DELETE,
  END,
  PAGE_UP,
  PAGE_DOWN,

  F1,
  F2,
  F3,
  F4,
  F5,
  F6,
  F7,
  F8,
  F9,
  F10,
  F11,
  F12,
};
