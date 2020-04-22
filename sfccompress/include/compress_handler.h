/**
 * @file compress_handler.h
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-08-18
 *
 * @copyright Copyright (c) 2020
 *
 */

#ifndef SFCC_COMPRESS_HANDLER_H
#define SFCC_COMPRESS_HANDLER_H

#include "compressor.h"
#include "sfcc.h"
#include "sfccexcept.h"
namespace sfc {
namespace sfcc {
class CompressHandler
{
 private:
  const compression_t _comp;
  sfcc_header _orig_header;
  std::unique_ptr<::sfcc::compressor> _compressor;

 public:
  CompressHandler(const sfcc_header& header,
                  const compression_t& compression_type)
      : _orig_header(header), _comp(compression_type)
  {
    if (_orig_header.compressiontype != compression_t::NONE)
      throw DataAlreadyCompressed(_orig_header.compressiontype);
  }
};
}  // namespace sfcc
}  // namespace sfc

#endif