#ifndef INPUT_H
#define INPUT_H

struct Input
{
  Input()
    : forward(false),
      backward(false),
      left(false),
      right(false),
      use(false)
  {}

  bool forward, backward;
  bool left, right;
  bool use;
};

#endif /* INPUT_H */
