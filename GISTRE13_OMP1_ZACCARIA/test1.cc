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

#pragma omp parallel for schedule (auto)
   for (int i=0; i < Image.TellWidth (); i++)
   {
#pragma omp parallel for schedule (auto)
    for (int j=0; j < Image.TellHeight (); j++)
    {
     Image(i,j)->Red   = 255 - (Image(i,j)->Red);
     Image(i,j)->Green = 255 - (Image(i,j)->Green);
     Image(i,j)->Blue  = 255 - (Image(i,j)->Blue);
    }
   }

   Image.WriteToFile ("output.bmp");
  }
  else
    std::cerr << "Skipping (non-bmp) file " << argv[1] << " ...\n";
  return 0;
}
