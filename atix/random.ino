void randomizeList()
{
  unsigned char chosen[NUMOFNUMBERS];
  unsigned char index, i2;
  for (index = 0; index < NUMOFNUMBERS; index++)
    chosen[index] = 0;
  for (index = 0; index < NUMOFNUMBERS; index++)
  {
    int r = random(NUMOFNUMBERS-index);
    for (i2 = 0; i2 <= r; i2++)
    {
      r += chosen[i2];
    }
    chosen[r] = 1;
    numbers[index] = r+1;
  }
} 
