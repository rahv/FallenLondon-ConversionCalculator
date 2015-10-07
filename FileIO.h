#ifndef FILEIO_H
#define FILEIO_H

#include <vector>

#include "Conversion.h"

namespace FileIO
{

/** Reads all input files and populates the data structures */
int readInputFiles(std::vector<ItemCategory> &categories,
                   std::vector<Conversion> &conversions);

};

#endif //FILEIO_H
