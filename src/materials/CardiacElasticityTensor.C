#include "CardiacElasticityTensor.h"

  /// table for converting minor indices to major block indices
  unsigned int const CardiacElasticityTensor::tbl_minor[3][3] = { { 0, 3, 5 },
                                                                  { 3, 1, 4 },
                                                                  { 5, 4, 2 } };

  /// table for converting major block to linear indices
  unsigned int const CardiacElasticityTensor::tbl_major[6][6] = { {  0,  6, 11, 15, 18, 20 },
                                                                  {  6,  1,  7, 12, 16, 19 },
                                                                  { 11,  7,  2,  8, 13, 17 },
                                                                  { 15, 12,  8,  3,  9, 14 },
                                                                  { 18, 16, 13,  9,  4, 10 },
                                                                  { 20, 19, 17, 14, 10,  5 } };

  /// table for converting major block indices to minor indices
  unsigned int const CardiacElasticityTensor::tbl_minor_inv[6][2] = { { 0, 0 }, { 1, 1 }, { 2, 2},
                                                                      { 0, 1 }, { 1, 2 },
                                                                      { 0, 2 } };

  /// table for converting linear indices to major block indices
  unsigned int const CardiacElasticityTensor::tbl_major_inv[21][2] = { { 0, 0 }, { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 },
                                                                       { 0, 1 }, { 1, 2 }, { 2, 3 }, { 3, 4 }, { 4, 5 },
                                                                       { 0, 2 }, { 1, 3 }, { 2, 4 }, { 3, 5 },
                                                                       { 0, 3 }, { 1, 4 }, { 2, 5 },
                                                                       { 0, 4 }, { 1, 5 },
                                                                       { 0, 5 } };

std::ostream&
operator<<(std::ostream & stream, const CardiacElasticityTensor & obj)
{
  stream << "CardiacElasticityTensor:\n"
         << std::setprecision(6)
         << std::setw(13) << obj._val[ 0] << "\t"
         << std::setw(13) << obj._val[ 6] << "\t"
         << std::setw(13) << obj._val[11] << "\t"
         << std::setw(13) << obj._val[15] << "\t"
         << std::setw(13) << obj._val[18] << "\t"
         << std::setw(13) << obj._val[20] << "\n"
         //
         << "\t\t"
         << std::setw(13) << obj._val[ 1] << "\t"
         << std::setw(13) << obj._val[ 7] << "\t"
         << std::setw(13) << obj._val[12] << "\t"
         << std::setw(13) << obj._val[16] << "\t"
         << std::setw(13) << obj._val[19] << "\n"
         //
         << "\t\t\t\t"
         << std::setw(13) << obj._val[ 2] << "\t"
         << std::setw(13) << obj._val[ 8] << "\t"
         << std::setw(13) << obj._val[13] << "\t"
         << std::setw(13) << obj._val[17] << "\n"
         //
         << "\t\t\t\t\t\t"
         << std::setw(13) << obj._val[ 3] << "\t"
         << std::setw(13) << obj._val[ 9] << "\t"
         << std::setw(13) << obj._val[14] << "\n"
         //
         << "\t\t\t\t\t\t\t\t"
         << std::setw(13) << obj._val[ 4] << "\t"
         << std::setw(13) << obj._val[10] << "\n"
         //
         << "\t\t\t\t\t\t\t\t\t\t"
         << std::setw(13) << obj._val[15] << std::endl;
  return stream;
}
