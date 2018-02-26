/** matrix.h 
 * Date: Oct 19, 2015
 * Author: cplus.shen@gmail.com
 * Description: header file for struc definitions
 *
 */

#ifndef __MATRIX_H__
#define __MATRIX_H__

/* move to matrix.h */
enum {
  TYPE_STAND = 0,
  TYPE_HOR,
  TYPE_VER
};

enum {
  DIR_UP = 0,
  DIR_RIGHT,
  DIR_DOWN,
  DIR_LEFT
};

struct point {
  int8_t x;
  int8_t y;
};

struct loc {
  uint8_t type;
  struct point points[2]; 
  uint8_t is_valid; /* used to keep valid node */
  uint8_t dir; /* used to keey valid dir */
};

#endif /* __MATRIX_H__ */

