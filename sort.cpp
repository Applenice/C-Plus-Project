#include <stdio.h>
/*同时找出最大最小值*/
void minmax (int *arr, int size, int *max, int *min)
{
  int *p = arr, *endp = arr + size;
  if (size % 2 != 0)
  {
    *max = *min = *p++;
  }
  else
  {
    if (*p > *(p + 1))
    {
      *max = *p;
      *min = *(p + 1);
    }
    else
    {
      *max = *(p + 1);
      *min = *p;
    }
    p += 2;
  }
  while (p != endp)
  {
    if (*p > *(p + 1))
    {
      if (*p > *max) *max = *p;
      if (*(p + 1) < *min) *min = *(p + 1);
    }
    else
    {
      if (*(p + 1) > *max) *max = *(p + 1);
      if (*(p) < *min) *min = *p;
    }
    p += 2;
  }
}

int main () {
  int a [] = {3, 4, 2, 6, 8, 1, 9, 12, 15, 11};
  int min, max;
  minmax (a, 10, &max, &min);
  printf ("Min : %d, Max: %d\n", min, max);
}