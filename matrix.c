/**
 * Date: Oct 16, 2015
 * Author: cplus.shen@gmail.com
 * Description: To solve matrix problem
 *
 *
 * LONPOS.CC quiz 022
 * - List possible steps
 * - List boundary
 * - Recursive with depth n
 * - Added black point
 *
 * The matrix map for x-axis and y-axis definition
 *      0  1 2 3 4 5 6(x)
 *   0 0,0           6,0
 *   1
 *   2
 *   3
 *   4
 *   5
 *   6
 *   7 0,7           6,7
 *  (y)
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include "matrix.h"

#define MATRIX_WIDTH 7
#define MATRIX_HEIGHT 8

#define MAX_STEP 20000
#define MAX_DEPTH 10

#define MAX_HISTORY 2000

#define DEBUG 0

#if DEBUG
#define dprintf(fmt,args...) printf (fmt ,##args)
#else
#define dprintf
#endif

static struct loc loc_history[MAX_HISTORY];
static uint32_t loc_history_ptr = 0;
static struct point black_point[] = {
  { 1, 6},
  { 6, 1},
};

static void dump_matrix(struct loc* lo)
{
#if DEBUG
  uint8_t i;
  uint8_t j;
  
  printf(" ");
  for (i=0;i<MATRIX_WIDTH;i++) {
    printf("%1d",i);
  }
  printf("\n");

  for (j=0;j<MATRIX_HEIGHT;j++) {
    printf("%1d",j);
    for (i=0;i<MATRIX_WIDTH;i++) {
      if ((i==(lo->points[0].x)) && (j==(lo->points[0].y))) {
        printf("0"); 
      } else if ( (lo->type != TYPE_STAND) && (i==(lo->points[1].x)) && (j==(lo->points[1].y))) {
        printf("x"); 
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
#endif
}

static void add_history(struct loc* now) 
{
  memcpy(loc_history+loc_history_ptr,now,sizeof(struct loc));
  loc_history_ptr++;
  
  if (loc_history_ptr >= MAX_HISTORY) {
    printf("** exceed history size %d\n",MAX_HISTORY);
    exit(1);
  }
}

static void dump_history()
{
  int i;

  printf("ID TY X  Y  X  Y  D\n");
  printf("----------------------\n");
  for(i=(loc_history_ptr-1);i>=0;i--) {
    printf("%-3d%-3d%-3d%-3d%-3d%-3d",(loc_history_ptr - i),loc_history[i].type,
      loc_history[i].points[0].x,loc_history[i].points[0].y,
      loc_history[i].points[1].x,loc_history[i].points[1].y);
    if (i==0) {
      printf("x\n");
    } else {
      switch (loc_history[i].dir) {
      case DIR_UP:
        printf("U\n");
        break;
      case DIR_RIGHT:
        printf("R\n");
        break;
      case DIR_DOWN:
        printf("D\n");
        break;
      case DIR_LEFT:
        printf("L\n");
        break;
      }
    }
  } 
}

/**
 * is_loc_equ() - check if the src and dest point is equal or not
 * @src: source point
 * @dest: destination point
 *
 * return 1 if equal
 * return 0 if no equal
 */
static int is_loc_equ(struct loc* src, struct loc* dest)
{

  if (src->type != dest->type) {
    return 0;
  }

  switch(src->type) {
  case TYPE_STAND:
     if ( (src->points[0].x == dest->points[0].x) &&
          (src->points[0].y == dest->points[0].y)) {
       return 1;
     }
    break;
  case TYPE_VER:
  case TYPE_HOR:
     if ( (src->points[0].x == dest->points[0].x) &&
          (src->points[0].y == dest->points[0].y) &&
          (src->points[1].x == dest->points[1].x) &&
          (src->points[1].y == dest->points[1].y)) {
       return 1;
     }

     if ( (src->points[1].x == dest->points[0].x) &&
          (src->points[1].y == dest->points[0].y) &&
          (src->points[0].x == dest->points[1].x) &&
          (src->points[0].y == dest->points[1].y)) {
       return 1;
     }
    break;
  }
  return 0;

}

/**
 * is_invalid_loc() - check the loc is over the boundary
 * @next:  the next location
 *
 * check the next location is over the boundary or not
 * return 1 if over or failed
 * return 0 if inside the boundary or normal
 */
static int is_invalid_loc(struct loc* next) 
{
  uint32_t i;

  switch(next->type) {
  case TYPE_STAND:
    if (next->points[0].x >= MATRIX_WIDTH) {
      return 1;
    }
    if (next->points[0].y >= MATRIX_HEIGHT) {
      return 1;
    }
    if (next->points[0].x < 0) {
      return 1;
    }
    if (next->points[0].y < 0) {
      return 1;
    }
    break;
  case TYPE_HOR:
  case TYPE_VER:
    if (next->points[0].x >= MATRIX_WIDTH) {
      return 1;
    }
    if (next->points[0].y >= MATRIX_HEIGHT) {
      return 1;
    }
    if (next->points[1].x >= MATRIX_WIDTH) {
      return 1;
    }
    if (next->points[1].y >= MATRIX_HEIGHT) {
      return 1;
    }
    if (next->points[0].x < 0) {
      return 1;
    }
    if (next->points[0].y < 0) {
      return 1;
    }
    if (next->points[1].x < 0) {
      return 1;
    }
    if (next->points[1].y < 0) {
      return 1;
    }
    break;
  }

  for(i=0;i<loc_history_ptr;i++) {
    if (is_loc_equ(&loc_history[i],next)) {
      return 1;
    }
  } 

  /* black point */
  for (i=0; i< (sizeof(black_point)/sizeof(struct point)); i++ ) {
    switch(next->type) {
    case TYPE_STAND:
      if ( (next->points[0].x == black_point[i].x) && (next->points[0].y == black_point[i].y) )
        return 1;
      break;
    case TYPE_VER:
    case TYPE_HOR:
      if ( (next->points[0].x == black_point[i].x) && (next->points[0].y == black_point[i].y) )
        return 1;
      if ( (next->points[1].x == black_point[i].x) && (next->points[1].y == black_point[i].y) )
        return 1;
      break;
    }
  }
  return 0;
}

/**
 * next_step() 
 * @now: current locaiton
 * @dir: 0: UP, 1: RIGHT, 2: DOWN, 3: LEFT
 * @next: next location
 * 
 * provide the next location according current location and turn direction
 * return 0 if have next step
 * return 1 if no next step
 */

static int next_step(struct loc* now,uint8_t dir,struct loc* next)
{
  uint32_t ret = 0;

  switch (now->type) {
  case TYPE_STAND:
    dprintf("%s() type stand,now (%d,%d), dir %d --> ",__FUNCTION__,now->points[0].x,now->points[0].y,dir);
    switch (dir) {
    case DIR_UP:
      next->type = TYPE_VER;
      next->points[0].x = now->points[0].x;
      next->points[0].y = now->points[0].y - 1;
      next->points[1].x = now->points[0].x;
      next->points[1].y = now->points[0].y - 2;
      dprintf("UP,type %d, next (%d,%d) (%d,%d)\n",
        next->type,next->points[0].x,next->points[0].y,next->points[1].x,next->points[1].y);
      return is_invalid_loc(next);
    case DIR_RIGHT:
      next->type = TYPE_HOR;
      next->points[0].x = now->points[0].x + 1;
      next->points[0].y = now->points[0].y;
      next->points[1].x = now->points[0].x + 2;
      next->points[1].y = now->points[0].y;
      dprintf("RIGHT,type %d, next (%d,%d) (%d,%d)\n",
        next->type,next->points[0].x,next->points[0].y,next->points[1].x,next->points[1].y);
      return is_invalid_loc(next);
    case DIR_DOWN:
      next->type = TYPE_VER;
      next->points[0].x = now->points[0].x;
      next->points[0].y = now->points[0].y + 1;
      next->points[1].x = now->points[0].x;
      next->points[1].y = now->points[0].y + 2;
      dprintf("DOWN,type %d, next (%d,%d) (%d,%d)\n",
        next->type,next->points[0].x,next->points[0].y,next->points[1].x,next->points[1].y);
      return is_invalid_loc(next);
    case DIR_LEFT:
      next->type = TYPE_HOR;
      next->points[0].x = now->points[0].x - 1;
      next->points[0].y = now->points[0].y;
      next->points[1].x = now->points[0].x - 2;
      next->points[1].y = now->points[0].y;
      dprintf("LEFT,type %d, next (%d,%d) (%d,%d)\n",
        next->type,next->points[0].x,next->points[0].y,next->points[1].x,next->points[1].y);
      return is_invalid_loc(next);
    default:
      printf("dir %d undefined\n",dir);
      exit(1);
    } 
    break;

  case TYPE_HOR:
    dprintf("%s() type horizontal, curr (%d,%d) (%d,%d),dir %d --> ",__FUNCTION__,
      now->points[0].x,now->points[0].y,now->points[1].x,now->points[1].y, dir);
    switch (dir) {
    case DIR_UP:
      next->type = TYPE_HOR;
      next->points[0].x = now->points[0].x;
      next->points[0].y = now->points[0].y - 1;
      next->points[1].x = now->points[1].x;
      next->points[1].y = now->points[1].y - 1;
      dprintf("UP, type %d, next (%d,%d) (%d,%d)\n",
        next->type,next->points[0].x,next->points[0].y,next->points[1].x,next->points[1].y);
      return is_invalid_loc(next);
    case DIR_RIGHT:
      next->type = TYPE_STAND;
      if (now->points[1].x > now->points[0].x ) {
        next->points[0].x = now->points[1].x + 1;
        next->points[0].y = now->points[1].y;
        next->points[1].x = 0;
        next->points[1].y = 0;
      } else {
        next->points[0].x = now->points[0].x + 1;
        next->points[0].y = now->points[0].y;
        next->points[1].x = 0;
        next->points[1].y = 0;
      }
      dprintf("RIGHT,type %d, next (%d,%d)\n",next->type,next->points[0].x,next->points[0].y);
      return is_invalid_loc(next);
    case DIR_DOWN:
      next->type = TYPE_HOR;
      next->points[0].x = now->points[0].x;
      next->points[0].y = now->points[0].y + 1;
      next->points[1].x = now->points[1].x;
      next->points[1].y = now->points[1].y + 1;
      dprintf("DOWN,type %d, next (%d,%d) (%d,%d)\n",
        next->type,next->points[0].x,next->points[0].y,next->points[1].x,next->points[1].y);
      return is_invalid_loc(next);
    case DIR_LEFT:
      next->type = TYPE_STAND;
      if (now->points[0].x < now->points[1].x ) {
        next->points[0].x = now->points[0].x - 1;
        next->points[0].y = now->points[1].y;
        next->points[1].x = 0;
        next->points[1].y = 0;
      } else {
        next->points[0].x = now->points[1].x - 1;
        next->points[0].y = now->points[0].y;
        next->points[1].x = 0;
        next->points[1].y = 0;
      }
      dprintf("LEFT,type %d, next (%d,%d)\n",next->type,next->points[0].x,next->points[0].y);
      return is_invalid_loc(next);
    default:
      printf("dir %d undefined\n",dir);
      exit(1);
    } 
    break;
  case TYPE_VER:
    dprintf("%s() type vertical,curr (%d,%d) (%d,%d),dir %d --> ",__FUNCTION__,
      now->points[0].x,now->points[0].y,now->points[1].x,now->points[1].y,dir);
    switch (dir) {
    case DIR_UP:
      next->type = TYPE_STAND;
      next->points[0].x = now->points[0].x;
      if (now->points[1].y < now->points[0].y) {
        next->points[0].y = now->points[1].y - 1;
      } else {
        next->points[0].y = now->points[0].y - 1;
      }
      next->points[1].x = 0;
      next->points[1].y = 0;
      dprintf("UP,type %d, next (%d,%d)\n",next->type, next->points[0].x,next->points[0].y);
      return is_invalid_loc(next);
    case DIR_RIGHT:
      next->type = TYPE_VER;
      next->points[0].x = now->points[0].x + 1;
      next->points[0].y = now->points[0].y;
      next->points[1].x = now->points[1].x + 1;
      next->points[1].y = now->points[1].y;
      dprintf("RIGHT,type %d, next (%d,%d) (%d,%d)\n",
        next->type,next->points[0].x,next->points[0].y,next->points[1].x,next->points[1].y);
      return is_invalid_loc(next);
    case DIR_DOWN:
      next->type = TYPE_STAND;
      next->points[0].x = now->points[0].x;
      if (now->points[0].y > now->points[1].y) {
        next->points[0].y = now->points[0].y + 1;
      } else {
        next->points[0].y = now->points[1].y + 1;
      }
      next->points[1].x = 0;
      next->points[1].y = 0;
      dprintf("DOWN,type %d, next (%d,%d)\n",next->type, next->points[0].x,next->points[0].y);
      return is_invalid_loc(next);
    case DIR_LEFT:
      next->type = TYPE_VER;
      next->points[0].x = now->points[0].x - 1;
      next->points[0].y = now->points[0].y;
      next->points[1].x = now->points[1].x - 1;
      next->points[1].y = now->points[1].y;
      dprintf("LEFT,type %d, next (%d,%d) (%d,%d)\n",
        next->type,next->points[0].x,next->points[0].y,next->points[1].x,next->points[1].y);
      return is_invalid_loc(next);
    default:
      printf("dir %d undefined\n",dir);
      exit(1);
    } 
    break;
  default:
    printf("type %d is undefined\n",now->type);
    exit(1);
  }
  return 0;
}


static int search_loc(struct loc* now, struct loc* dest,uint8_t depth,uint8_t max_depth)
{
  struct loc next;
  uint8_t dir;
  static uint32_t max_step = 0;
 
#if DEBUG
  switch(now->type) {
  case TYPE_STAND:
    dprintf("%s(),now type %d (%d,%d), dest type %d, (%d,%d)(%d,%d), depth %d -->\n",__FUNCTION__,
      now->type,now->points[0].x,now->points[0].y,
      dest->type, dest->points[0].x,dest->points[0].y,dest->points[1].x, dest->points[1].y,
      depth);
    break;
  case TYPE_VER:
  case TYPE_HOR:
    dprintf("%s(),now type %d (%d,%d)(%d,%d), dest type %d, (%d,%d)(%d,%d), depth %d -->\n",__FUNCTION__,
      now->type,now->points[0].x,now->points[0].y,now->points[1].x,now->points[1].y,
      dest->type, dest->points[0].x,dest->points[0].y,dest->points[1].x, dest->points[1].y,
      depth);
    break;
  }
#endif
  bzero(&next, sizeof(next));

  /* exit criteria: find the location */
  if (is_loc_equ(now,dest)) {
    add_history(now);
    return 0;
  }

  /* exit criteria: over max step */
  if ((max_step++) > MAX_STEP) {
    printf("exceed max_step %d\n",MAX_STEP);
    exit (1);
  }  

  /* exit criteria: over the stack depth */
  if (depth >= max_depth) {
    dprintf("exceed max_depth %d\n",max_depth);
    return 1;
  }

  /* main recursive part */
  for (dir=DIR_UP; dir<=DIR_LEFT; dir++) {
    if (next_step(now, dir, &next) == 0) {
      dump_matrix(&next);
      if (search_loc(&next,dest,depth+1,max_depth)){
        dprintf("%s() ret = 1\n",__FUNCTION__);
      } else {
        now->is_valid = 1;
        now->dir = dir;
        add_history(now);
        return 0;
      }
    } else {
      dprintf("%s() no next_step for dir %d\n",__FUNCTION__,dir);
      continue;
    }
  }
  return 1;

}

int main(int argc,char* argv[])
{
  struct loc now;
  struct loc next;
  struct loc dest;
  uint8_t dir;
  uint32_t i;
  uint8_t depth = 0;
  uint8_t max_depth = MAX_DEPTH;

  bzero(&now,sizeof(now));
  bzero(&next,sizeof(next));
  bzero(&dest,sizeof(dest));

  printf("max_depth = %d\n", max_depth);
  printf("Usage: %s <source_type> <source_x> <source_y> <dest_type> <dest_x> <dest_y>\n", argv[0]);
  printf("Type 0: STAND, Type 1: HOR, Type 2: VER\n");
  printf(
"(y)\n"
" 0 0,0                     6,0 \n"
" 1                             \n"
" 2                             \n"
" 3                             \n"
" 4                             \n"
" 5                             \n"
" 6                             \n"
" 7 0,7 1,7 2,7 3,7 4,7 5,7 6,7 \n"
"    0   1   2   3   4   5   6  (x)\n\n");

  now.type = strtoul(argv[1],NULL,0);
  switch(now.type) {
  case TYPE_STAND:
    now.points[0].x=strtoul(argv[2],NULL,0);
    now.points[0].y=strtoul(argv[3],NULL,0);

    dest.type = strtoul(argv[6],NULL,0);
    switch(dest.type) {
    case TYPE_STAND:
      dest.points[0].x=strtoul(argv[7],NULL,0);
      dest.points[0].y=strtoul(argv[8],NULL,0);
      break;
    case TYPE_HOR:
    case TYPE_VER:
      dest.points[0].x=strtoul(argv[7],NULL,0);
      dest.points[0].y=strtoul(argv[8],NULL,0);
      dest.points[1].x=strtoul(argv[9],NULL,0);
      dest.points[1].y=strtoul(argv[10],NULL,0);
      break;
    }
 
    break;
  case TYPE_HOR:
  case TYPE_VER:
    now.points[0].x=strtoul(argv[2],NULL,0);
    now.points[0].y=strtoul(argv[3],NULL,0);
    now.points[1].x=strtoul(argv[4],NULL,0);
    now.points[1].y=strtoul(argv[5],NULL,0);
    dest.type = strtoul(argv[6],NULL,0);
    switch(dest.type) {
    case TYPE_STAND:
      dest.points[0].x=strtoul(argv[7],NULL,0);
      dest.points[0].y=strtoul(argv[8],NULL,0);
      break;
    case TYPE_HOR:
    case TYPE_VER:
      dest.points[0].x=strtoul(argv[7],NULL,0);
      dest.points[0].y=strtoul(argv[8],NULL,0);
      dest.points[1].x=strtoul(argv[9],NULL,0);
      dest.points[1].y=strtoul(argv[10],NULL,0);
      break;
    }
    break;
  default:
    printf("type %d is undefined\n",now.type);
    exit(1);
  }

  dprintf("now (%d,%d) (%d,%d) -> dest (%d,%d) (%d,%d), max_depth %d, max_step %d\n",
    now.points[0].x,now.points[0].y,now.points[1].x,now.points[1].y, 
    dest.points[0].x,dest.points[0].y,dest.points[1].x,dest.points[1].y,max_depth,MAX_STEP); 
  dprintf("-------------------------------------\n");

//  dump_matrix(&now);

  if (search_loc(&now,&dest,depth,max_depth) == 0) {
    dump_history();
    return 0;
  } else {
    printf ("*** NOT FOUND **\n");
    return 1;
  }

  return 0;
}

