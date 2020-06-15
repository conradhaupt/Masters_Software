/**
 * @file main.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-10-20
 *
 * @copyright Copyright (c) 2020
 *
 */
#ifndef SFCCOMPARE_MAIN_H
#define SFCCOMPARE_MAIN_H
#include <memory>
#include <string>
#include "libsfc/gray.h"
#include "libsfc/hilbert.h"
#include "libsfc/morton.h"
#include "libsfc/raster.h"
#include "libsfc/sfcdef.h"
#include "libsfc/sfcurve.h"
#include "sfcc.h"

namespace sfc::main {
namespace desc {
const std::string program =
    "This program manages compressing and decompressing SFC encoded files.";
const std::string help = "Display this menu";
const std::string compress = "Compress a file";
const std::string compress_file = "The file to compress";
const std::string compress_out_file =
    "The file to write the compressed data to";
const std::string decompress = "Decompress a file";
const std::string decompress_file = "The file to decompress";
const std::string decompress_out_file =
    "The file to write the decompressed data to";
const std::string sfcurve = "The curve with which to transform the data";
const std::string compression =
    "The compression scheme with which to compress the data";
const std::string bittranspose =
    "Reorder bits of the data so that they are grouped by significance";
const std::string nobittranspose =
    "Do not reorder bits of the data by significance";

}  // namespace desc
namespace sfcs {
using types = ::sfc::sfcc::sfc_t;
}  // namespace sfcs
namespace compression {
using types = ::sfc::sfcc::compression_t;
}

std::string SFCToExt(const sfc::main::sfcs::types& _sfc)
{
  switch (_sfc) {
    case sfc::main::sfcs::types::ROW_MAJOR:
      return "";
    case sfc::main::sfcs::types::MORTON:
      return ".mrtn";
    case sfc::main::sfcs::types::GRAY_CODE:
      return ".gry";
    case sfc::main::sfcs::types::HILBERT:
      return ".hbrt";
    default:
      return "";
  }
}

template <sfc::size_t NDims>
std::unique_ptr<sfc::sfcurve<NDims>> SFCToSFCurve(
    const sfc::main::sfcs::types& _sfc, sfc::size_t dimLength)
{
  switch (_sfc) {
    case sfc::main::sfcs::types::ROW_MAJOR:
      return std::make_unique<sfc::raster<NDims>>(dimLength);
    case sfc::main::sfcs::types::MORTON:
      return std::make_unique<sfc::morton<NDims>>(dimLength);
    case sfc::main::sfcs::types::GRAY_CODE:
      return std::make_unique<sfc::gray<NDims>>(dimLength);
    case sfc::main::sfcs::types::HILBERT:
      return std::make_unique<sfc::hilbert<NDims>>(dimLength);
    default:
      return nullptr;
  }
}
}  // namespace sfc::main
#endif