#ifndef INPUT_H
#define INPUT_H

struct Input
{
  bool forward = false;
  bool backward = false;
  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;

  bool toggle_cursor = false;
};

#endif /* INPUT_H */
