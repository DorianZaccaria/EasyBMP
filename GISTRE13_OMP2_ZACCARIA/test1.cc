#include <iostream>
#include "EasyBMP.h"

int                     main (int               argc,
                              char**            argv)
{

  if (argc != 2)
  {
    std::cerr << "Error: number of arguments" << std::endl;
    return 1;
  }

  BMFH                  bmfh = GetBMFH (argv[1]);

  if( bmfh.bfType == 19778 )
  {
    std::cout << "Processing file " << argv[1] << " ...\n";

    BMP                  Image;

    Image.ReadFromFile (argv[1]);

#pragma omp parallel
{
      for (int i=0; i < Image.TellWidth (); i++)
      {

        for (int j=0; j < Image.TellHeight (); j++)
        {
#pragma omp sections nowait
  {
#pragma omp section
          Image(i,j)->Red   = 255 - (Image(i,j)->Red);
#pragma omp section
          Image(i,j)->Green = 255 - (Image(i,j)->Green);
#pragma omp section
          Image(i,j)->Blue  = 255 - (Image(i,j)->Blue);
  }
        }
      }

      for (int i=1; i < Image.TellWidth () - 1; i++)
      {
        for (int j=1; j < Image.TellHeight () - 1; j++)
        {
#pragma omp sections nowait
  {
#pragma omp section
    Image(i,j)->Red   =
      (Image(i - 1,j - 1)->Red + Image(i - 1,j)->Red + Image(i - 1,j + 1)->Red +
       Image(i,j - 1)->Red + Image(i,j)->Red + Image(i,j + 1)->Red +
       Image(i + 1,j - 1)->Red + Image(i + 1,j - 1)->Red + Image(i + 1,j + 1)->Red) / 9;
#pragma omp section
    Image(i,j)->Green   =
      (Image(i - 1,j - 1)->Green + Image(i - 1,j)->Green + Image(i - 1,j + 1)->Green +
       Image(i,j - 1)->Green + Image(i,j)->Green + Image(i,j + 1)->Green +
       Image(i + 1,j - 1)->Green + Image(i + 1,j - 1)->Green + Image(i + 1,j + 1)->Green) / 9;
#pragma omp section
    Image(i,j)->Blue   =
      (Image(i - 1,j - 1)->Blue + Image(i - 1,j)->Blue + Image(i - 1,j + 1)->Blue +
       Image(i,j - 1)->Blue + Image(i,j)->Blue + Image(i,j + 1)->Blue +
       Image(i + 1,j - 1)->Blue + Image(i + 1,j - 1)->Blue + Image(i + 1,j + 1)->Blue) / 9;

  }
        }
      }

      for (int i=1; i < Image.TellWidth () - 1; i++)
      {
        for (int j=1; j < Image.TellHeight () - 1; j++)
        {
#pragma omp sections nowait
  {
#pragma omp section
    if (Image(i,j)->Red + 70 < 256)
      Image(i,j)->Red += 70;
#pragma omp section
    if (Image(i,j)->Green + 70 < 256)
    Image(i,j)->Green += 70;
#pragma omp section
    if (Image(i,j)->Blue + 70 < 256)
      Image(i,j)->Blue += 70;
  }
        }
      }
}
      Image.WriteToFile ("output.bmp");
    }
    else
      std::cerr << "Skipping (non-bmp) file " << argv[1] << " ...\n";

  return 0;
}
