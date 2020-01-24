#pragma once

int poll_input(char* buffer, int* length);

typedef struct EscapeCode_s {
  char code[8];
  int id;
} EscapeCode;

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
