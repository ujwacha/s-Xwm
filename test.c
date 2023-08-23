#include <stdio.h>

int matrix[3][3] = {0};

void main()
{
  int(*array)[3];
  array = &matrix[1];
  for (int i = 0; i < 3; i++)
  {
    scanf("%d", &(*array)[i]);
    printf("%d\n", i);
  }
  for (int i = 0; i < 3; i++)
  {
    for (int j = 0; j < 3; j++)
    {
      printf("%d  ", matrix[i][j]);
    }
    printf("\n");
  }
}