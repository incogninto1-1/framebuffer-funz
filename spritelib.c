#include "spritelib.h"

Object genObj(int xSiz, int ySiz){
    Object temp;
    temp.xSize = xSiz;
    temp.ySize = ySiz;
    temp.xPos = 0;
    temp.yPos = 0;
    return temp;
}