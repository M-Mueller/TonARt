#include "util.h"

double round(double x)
{
   if (x >= 0.0)
      return floor(x + 0.5);
   else
      return ceil(x - 0.5);
}
