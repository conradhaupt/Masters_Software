/**
 * @file compressor.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-10-14
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "compressor.h"

namespace sfcc {
// HUFFMAN
huffman::huffman() {}
huffman::~huffman() {}
std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
           sfc::sfcc_header>
huffman::compress(const std::unique_ptr<std::uint8_t[]>& data,
                  const unsigned long long& length,
                  const sfc::sfcc_header& header)
{
  // Construct temporary data
  std::uint8_t* tmp_data;
  int output_length;
  int output_length_bits;

  // Compress using Huffman encoding
  // void easyEncode(const std::uint8_t *uncompressed,
  // const int uncompressedSizeBytes, std::uint8_t **compressed,
  // int *compressedSizeBytes, int *compressedSizeBits)

  ::huffman::easyEncode(data.get(), length, &tmp_data, &output_length,
                        &output_length_bits);

  // Create appropriately sized unique_ptr
  auto output_data = std::make_unique<std::uint8_t[]>(output_length);

  // Move tmp data to unique_ptr
  std::move(tmp_data, tmp_data + output_length, output_data.get());
  delete[] tmp_data;
  auto head = header;
  head.compressiontype = ::sfc::sfcc::compression_t::HUFFMAN;
  head.npaddingbits = output_length_bits % 8;
  // Move-return unique_ptr
  return {std::move(output_data), output_length, head};
}

std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
           sfc::sfcc_header>
huffman::decompress(const std::unique_ptr<std::uint8_t[]>& data,
                    const unsigned long long& length,
                    const sfc::sfcc_header& header)
{
  // Construct temporary data
  auto compressed_size_bits =
      header.npaddingbits.value() == 0
          ? length * 8
          : (length - 1) * 8 + header.npaddingbits.value();
  auto uncompressed_size_bytes =
      (1ULL << (header.sidelength_k * header.ndims)) * header.dtype_nbytes;
  std::uint8_t* tmp_data = new std::uint8_t[uncompressed_size_bytes];

  // Compress using Huffman encoding
  // void easyEncode(const std::uint8_t *uncompressed,
  // const int uncompressedSizeBytes, std::uint8_t **compressed,
  // int *compressedSizeBytes, int *compressedSizeBits)

  ::huffman::easyDecode(data.get(), length,
                        // ((length - 1) * 8) + header.npaddingbits.value() + 1,
                        length * 8, tmp_data, uncompressed_size_bytes);

  // Create appropriately sized unique_ptr
  auto output_data = std::make_unique<std::uint8_t[]>(uncompressed_size_bytes);
  std::move(tmp_data, tmp_data + uncompressed_size_bytes, output_data.get());
  delete[] tmp_data;
  auto head = header;
  head.compressiontype = ::sfc::sfcc::compression_t::NONE;
  head.sfctype = ::sfc::sfcc::sfc_t::ROW_MAJOR;
  // Move-return unique_ptr
  return {std::move(output_data), uncompressed_size_bytes, head};
}

std::string huffman::getFileExtension() const { return "hff"; }
sfc::sfcc::compression_t huffman::getCompressionType() const
{
  return sfc::sfcc::compression_t::HUFFMAN;
}

// LZ77
lz77::lz77() {}
lz77::~lz77() {}
std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
           sfc::sfcc_header>
lz77::compress(const std::unique_ptr<std::uint8_t[]>& data,
               const unsigned long long& length, const sfc::sfcc_header& header)
{
  ::lz77::compress_t lz77comp;
  auto compressed_data = lz77comp.feed(data.get(), data.get() + length);
  int output_length = compressed_data.size();
  // Move tmp data to unique_ptr
  auto head = header;
  head.compressiontype = ::sfc::sfcc::compression_t::LZ77;
  // Create appropriately sized unique_ptr
  auto output_data = std::make_unique<std::uint8_t[]>(output_length);

  // Copy the compressed data into an array to be returned using move semantics
  std::copy(std::begin(compressed_data), std::end(compressed_data),
            output_data.get());
  // Move-return unique_ptr
  return {std::move(output_data), output_length, head};
}

std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
           sfc::sfcc_header>
lz77::decompress(const std::unique_ptr<std::uint8_t[]>& data,
                 const unsigned long long& length,
                 const sfc::sfcc_header& header)
{
  ::lz77::decompress_t lz77decomp;
  std::string remaining;

  lz77decomp.feed(data.get(), data.get() + length, remaining);
  auto decompressed_data = lz77decomp.result();
  int output_length = decompressed_data.size();
  // auto expected_output_length =
  //     (1ULL << (header.sidelength_k * header.ndims)) * header.dtype_nbytes;

  // NOT COMPATIBLE WITH BZIP<> COMPRESSOR
  // if (output_length != expected_output_length)
  //   throw std::logic_error("Decompressed size was not as expected" +
  //                          std::to_string(output_length) + " instead of " +
  //                          std::to_string(expected_output_length));
  // Move tmp data to unique_ptr
  auto head = header;
  head.compressiontype = ::sfc::sfcc::compression_t::NONE;
  // Create appropriately sized unique_ptr
  auto output_data = std::make_unique<std::uint8_t[]>(output_length);

  // Copy the compressed data into an array to be returned using move semantics
  std::copy(std::begin(decompressed_data), std::end(decompressed_data),
            output_data.get());
  // Move-return unique_ptr
  return {std::move(output_data), output_length, head};
}

std::string lz77::getFileExtension() const { return "lz77"; }
sfc::sfcc::compression_t lz77::getCompressionType() const
{
  return sfc::sfcc::compression_t::LZ77;
}

// LZW
lzw::lzw() {}
lzw::~lzw() {}
std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
           sfc::sfcc_header>
lzw::compress(const std::unique_ptr<std::uint8_t[]>& data,
              const unsigned long long& length, const sfc::sfcc_header& header)
{
  // Construct temporary data
  std::uint8_t* tmp_data;
  int output_length;
  int output_length_bits;

  // Compress using Huffman encoding
  // void easyEncode(const std::uint8_t *uncompressed,
  // const int uncompressedSizeBytes, std::uint8_t **compressed,
  // int *compressedSizeBytes, int *compressedSizeBits)

  ::lzw::easyEncode(data.get(), length, &tmp_data, &output_length,
                    &output_length_bits);
  // std::cout << "LZQ: output length=" << output_length
  //           << ", output_length_bits=" << output_length_bits << std::endl;

  // Create appropriately sized unique_ptr
  auto output_data = std::make_unique<std::uint8_t[]>(output_length);
  // std::cout << int(tmp_data[0]) << ", " << int(tmp_data[1]) << ", "
  //           << int(tmp_data[2]) << ", " << int(tmp_data[3]) << ", "
  //           << int(tmp_data[4]) << ", " << int(tmp_data[5]) << ", "
  //           << std::endl;
  // Move tmp data to unique_ptr
  std::move(tmp_data, tmp_data + output_length, output_data.get());
  delete[] tmp_data;
  auto head = header;
  head.compressiontype = ::sfc::sfcc::compression_t::LZW;
  head.npaddingbits = output_length_bits % 8;
  return {std::move(output_data), output_length, head};
}
std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
           sfc::sfcc_header>
lzw::decompress(const std::unique_ptr<std::uint8_t[]>& data,
                const unsigned long long& length,
                const sfc::sfcc_header& header)
{
  // Construct temporary data
  unsigned long long output_length;
  if (header.output_block_length) {
    output_length = header.output_block_length.value();
  } else {
    output_length =
        (1ULL << (header.sidelength_k * header.ndims)) * header.dtype_nbytes;
  }
  std::uint8_t* tmp_data = new std::uint8_t[output_length];

  auto length_bits = header.npaddingbits.value() == 0
                         ? length * 8
                         : ((length - 1) * 8) + header.npaddingbits.value();

  ::lzw::easyDecode(data.get(), length, length_bits, tmp_data, output_length);

  // Create appropriately sized unique_ptr
  auto output_data = std::make_unique<std::uint8_t[]>(output_length);

  // Move tmp data to unique_ptr
  std::move(tmp_data, tmp_data + output_length, output_data.get());
  delete[] tmp_data;
  auto head = header;
  head.compressiontype = ::sfc::sfcc::compression_t::NONE;
  // Move-return unique_ptr
  return {std::move(output_data), output_length, head};
}

std::string lzw::getFileExtension() const { return "lzw"; }
sfc::sfcc::compression_t lzw::getCompressionType() const
{
  return sfc::sfcc::compression_t::LZW;
}

// RLE
rle::rle() {}
rle::~rle() {}
std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
           sfc::sfcc_header>
rle::compress(const std::unique_ptr<std::uint8_t[]>& data,
              const unsigned long long& length, const sfc::sfcc_header& header)
{
  // Construct temporary data
  const auto MaxOutputSize = (length << 2);
  std::uint8_t* tmp_data = new std::uint8_t[MaxOutputSize];

  // Compress using Huffman encoding
  // void easyEncode(const std::uint8_t *uncompressed,
  // const int uncompressedSizeBytes, std::uint8_t **compressed,
  // int *compressedSizeBytes, int *compressedSizeBits)

  int output_length = -1;
  if (header.dtype_nbytes == 2) {
    auto ptr = reinterpret_cast<std::uint16_t*>(data.get());
    output_length = ::rle::easyEncode<std::uint16_t>(
        ptr, length / sizeof(std::uint16_t), tmp_data, MaxOutputSize);
  } else if (header.dtype_nbytes == 4) {
    auto ptr = reinterpret_cast<std::uint32_t*>(data.get());
    output_length = ::rle::easyEncode<std::uint32_t>(
        ptr, length / sizeof(std::uint32_t), tmp_data, MaxOutputSize);
  } else {
    delete[] tmp_data;
    throw std::logic_error("Unexpected data-type size of " +
                           std::to_string(header.dtype_nbytes) + " bytes");
  }
  if (output_length == -1)
    throw std::runtime_error("Could not complete RLE process");

  // Create appropriately sized unique_ptr
  auto output_data = std::make_unique<std::uint8_t[]>(output_length);

  std::move(tmp_data, tmp_data + output_length, output_data.get());
  delete[] tmp_data;
  auto head = header;
  head.compressiontype = ::sfc::sfcc::compression_t::RLE;

  std::cout << "RLE: output_length=" << output_length << std::endl;
  // Move-return unique_ptr
  return {std::move(output_data), output_length, head};
}

std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
           sfc::sfcc_header>
rle::decompress(const std::unique_ptr<std::uint8_t[]>& data,
                const unsigned long long& length,
                const sfc::sfcc_header& header)
{
  // Construct temporary data
  const auto OutputSize =
      (1ULL << (header.sidelength_k * header.ndims)) * header.dtype_nbytes;
  std::cout << "RLE: Received size=" << length << std::endl;
  std::uint8_t* tmp_data = new std::uint8_t[OutputSize];

  // Compress using Huffman encoding
  // void easyEncode(const std::uint8_t *uncompressed,
  // const int uncompressedSizeBytes, std::uint8_t **compressed,
  // int *compressedSizeBytes, int *compressedSizeBits)
  int output_length = -1;
  std::cout << "RLE: expected OutputSize=" << OutputSize << std::endl;
  if (header.dtype_nbytes == 2) {
    output_length = ::rle::easyDecode<std::uint16_t>(
        data.get(), length, (std::uint16_t*)tmp_data, OutputSize);
  } else if (header.dtype_nbytes == 4) {
    output_length = ::rle::easyDecode<std::uint32_t>(
        data.get(), length, (std::uint32_t*)tmp_data, OutputSize);
  } else {
    delete[] tmp_data;
    throw std::logic_error("Unexpected data-type size of " +
                           std::to_string(header.dtype_nbytes) + " bytes");
  }

  if (output_length == -1)
    throw std::runtime_error("Could not complete RLE process");

  std::cout << "RLE: Actual output size=" << output_length << std::endl;

  // Create appropriately sized unique_ptr
  auto output_data = std::make_unique<std::uint8_t[]>(output_length);

  std::move((std::uint8_t*)tmp_data, (std::uint8_t*)tmp_data + (output_length),
            output_data.get());
  delete[] tmp_data;
  auto head = header;
  head.compressiontype = ::sfc::sfcc::compression_t::NONE;
  // Move-return unique_ptr
  return {std::move(output_data), output_length, head};
}
std::string rle::getFileExtension() const { return "rle"; }
sfc::sfcc::compression_t rle::getCompressionType() const
{
  return sfc::sfcc::compression_t::RLE;
}

// // DEFLATE
// deflate::deflate() {}
// deflate::~deflate() {}
// std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
//            sfc::sfcc_header>
// deflate::compress(const std::unique_ptr<std::uint8_t[]>& data,
//                   const unsigned long long& length,
//                   const sfc::sfcc_header& header)
// {
//   auto lz77comp = std::make_unique<lz77>();
//   auto huffcomp = std::make_unique<huffman>();
//   auto [temp_data, temp_length, temp_head] =
//       lz77comp->compress(data, length, header);
//   // std::cout << "LZ77 length=" << temp_length << std::endl;
//   auto [output_data, output_length, head] =
//       huffcomp->compress(temp_data, temp_length, temp_head);
//   // std::cout << "Huffman length=" << output_length << std::endl;
//   // std::cout << "\tnpaddingbits=" << int(head.npaddingbits.value()) <<
//   // std::endl;
//   head.compressiontype = sfc::sfcc::compression_t::DEFLATE;
//   return {std::move(output_data), output_length, head};
// }
// std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
//            sfc::sfcc_header>
// deflate::decompress(const std::unique_ptr<std::uint8_t[]>& data,
//                     const unsigned long long& length,
//                     const sfc::sfcc_header& header)
// {
//   std::cout << "npaddingbits=" << int(header.npaddingbits.value()) <<
//   std::endl; auto lz77comp = std::make_unique<lz77>(); auto huffcomp =
//   std::make_unique<huffman>(); std::cout << "Compressed length=" << length <<
//   std::endl; auto [temp_data, temp_length, temp_head] =
//       huffcomp->decompress(data, length, header);
//   auto [output_data, output_length, head] =
//       lz77comp->decompress(temp_data, temp_length, temp_head);
//   head.compressiontype = sfc::sfcc::compression_t::NONE;
//   return {std::move(output_data), output_length, head};
// }
// std::string deflate::getFileExtension() const { return "dflt"; }
// sfc::sfcc::compression_t deflate::getCompressionType() const
// {
//   return sfc::sfcc::compression_t::DEFLATE;
// }

// BZIP
template <class postcompressor>
bzip<postcompressor>::bzip(bool forceByteCompression)
    : _forceByteCompression{forceByteCompression}
{
  std::cout << "Forcing byte compression" << std::endl;
}
template <class postcompressor>
bzip<postcompressor>::~bzip()
{}

template <class postcompressor>
std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
           sfc::sfcc_header>
bzip<postcompressor>::compress(const std::unique_ptr<std::uint8_t[]>& data,
                               const unsigned long long& length,
                               const sfc::sfcc_header& header)
{
  // Block Size in Bytes
  const auto blockSize = MAX_BLOCK_SIZE_BYTES;
  // Number of blocks
  const auto nblocks = length / blockSize;

  // Generate block data containers
  auto inputBlocks = std::make_unique<Block[]>(nblocks);
  auto outputBlocks = std::make_unique<Block[]>(nblocks);

  for (auto i = 0; i < nblocks; i++) {
    inputBlocks[i].data = data.get() + (MAX_BLOCK_SIZE_BYTES * i);
    inputBlocks[i].length = MAX_BLOCK_SIZE_BYTES;
  }

  // Compress each block
  for (auto iBlock = 0; iBlock < nblocks; iBlock++) {
    std::cout << std::endl
              << "Compressing block " << iBlock << "/" << nblocks << std::endl;
    // Get block pointers etc
    auto inBlock = inputBlocks.get()[iBlock];
    auto& outBlock = outputBlocks[iBlock];

    // BWT the block
    // BWT block length is the original + the primary index. Primary index is of
    // type std::int32_t.
    auto bwt_length = inBlock.data_length();
    // Create BWT temporary values
    auto bwt = std::make_unique<std::uint8_t[]>(bwt_length);
    std::int32_t primary_index =
        divbwt((const sauchar_t*)inBlock.data, (sauchar_t*)bwt.get(), nullptr,
               inBlock.data_length());

    // Handle BWT errors
    if (primary_index < 0) {
      throw std::runtime_error("BWT failed on block " + std::to_string(iBlock) +
                               " with error code " +
                               std::to_string(primary_index));
    }

    // BWT was successful, store primary index
    outBlock.bwt_primary_index = primary_index;

    // MTF blocksize is the original size. The min and max values are managed by
    // outBlock
    unsigned long long mtf_length = inBlock.data_length();

    // MTF on a byte level
    std::uint8_t min;
    std::uint8_t max;
    auto mtf = std::make_unique<std::uint8_t[]>(mtf_length);
    auto [mtf_temp, min_temp, max_temp] =
        ::sfcc::easyMTFAnyuType<std::uint8_t, std::uint8_t>(
            (std::uint8_t*)bwt.get(), mtf_length);
    std::move((std::uint8_t*)mtf_temp.get(),
              (std::uint8_t*)(mtf_temp.get() + mtf_length), mtf.get());
    min = *(std::uint8_t*)&min_temp;
    max = *(std::uint8_t*)&max_temp;

    bwt.reset();

    // assign to outBlock the MTF values
    outBlock.min_value = min;
    outBlock.max_value = max;

    std::cout << "Boundaries of MTF Data:" << std::endl;
    for (auto i = 0; i < 5; i++) {
      std::cout << std::hex << (int)mtf[i] << " ";
    }
    std::cout << std::endl << "..." << std::endl;
    for (auto i = mtf_length - 5; i < mtf_length; i++) {
      std::cout << std::hex << (int)mtf[i] << " ";
    }
    std::cout << std::endl << std::dec;

    // Postcompressor
    auto postcomp = std::make_unique<postcompressor>();
    std::cout << "Compressing MTF data of length " << mtf_length << std::endl;
    auto [outBlock_data, outBlock_length, outblock_head] =
        postcomp->compress(mtf, mtf_length, header);
    std::cout << "Postcompression is complete: output length="
              << outBlock_length << std::endl;
    // MTF is complete, free up memory
    mtf.reset();

    std::cout << "Boundaries of Compressed Data:" << std::endl;
    for (auto i = 0; i < 5; i++) {
      std::cout << std::hex << (int)outBlock_data[i] << " ";
    }
    std::cout << std::endl << "..." << std::endl;
    for (auto i = outBlock_length - 5; i < outBlock_length; i++) {
      std::cout << std::hex << (int)outBlock_data[i] << " ";
    }
    std::cout << std::endl << std::dec;

    // Assign to outblock the postcompressor values
    if (::sfc::sfcc_header::CompressionRequiresPaddingBits(
            outblock_head.compressiontype))
      outBlock.npaddingbits = outblock_head.npaddingbits;
    outBlock.length = outBlock_length;
    outBlock.data = outBlock_data.release();
    outBlock.encoded = true;
  }

  // Consolidate outputblocks
  auto blocksize_sum = 0ULL;
  // Get total size: sum of block headers and block data lengths
  std::for_each(
      outputBlocks.get(), outputBlocks.get() + nblocks,
      [&blocksize_sum](auto& block) { blocksize_sum += block.size(); });

  // Move data across
  std::cout << "Allocating output memory: output summed size=" << blocksize_sum
            << std::endl;
  auto output_data = std::make_unique<std::uint8_t[]>(blocksize_sum);
  auto output_data_ptr = output_data.get();
  for (auto iBlock = 0; iBlock < nblocks; iBlock++) {
    auto& block = outputBlocks[iBlock];
    auto block_size = block.size();
    auto block_data = block.toArray();
    std::move(block_data.get(), block_data.get() + block_size, output_data_ptr);
    output_data_ptr += block_size;
    std::cout << "Block [" << iBlock << "]: " << block_size << std::endl;
  }

  // Update file header
  auto output_header = header;
  output_header.compressiontype = getCompressionType();
  output_header.npaddingbits = 0;
  // std::cout << output_header.size() << std::endl;
  return {std::move(output_data), blocksize_sum, output_header};
}

template <class postcompressor>
std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
           sfc::sfcc_header>
bzip<postcompressor>::decompress(const std::unique_ptr<std::uint8_t[]>& data,
                                 const unsigned long long& length,
                                 const sfc::sfcc_header& header)
{
  // Does compression use npaddingbits?
  const auto usesNPaddingBits =
      sfc::sfcc_header::CompressionRequiresPaddingBits(header.compressiontype);
  if (usesNPaddingBits) std::cout << "Using padding bits" << std::endl;

  auto nblocks = 0;
  // Calculate the number of blocks needed
  for (auto it = data.get(); it < data.get() + length;) {
    // Current blocksize
    try {
      auto blocksize = *((std::uint32_t*)it);
      it += sizeof(std::uint32_t);
      if (usesNPaddingBits) it += sizeof(std::int8_t);
      it += sizeof(std::int8_t);    // Min Value
      it += sizeof(std::int8_t);    // Max Value
      it += sizeof(std::uint32_t);  // BWT Primary Index
      it += blocksize;
      nblocks++;
    } catch (std::exception& e) {
      throw std::runtime_error("Data length is incorrect, block " +
                               std::to_string(nblocks + 1) +
                               " exceeds data size");
    }
  }

  // Populate input blocks
  auto inputBlocks = std::make_unique<Block[]>(nblocks);
  auto outputBlocks = std::make_unique<Block[]>(nblocks);
  auto data_ptr = data.get();
  for (auto iBlock = 0; iBlock < nblocks; iBlock++) {
    inputBlocks[iBlock].length = *((std::uint32_t*)data_ptr);
    data_ptr += sizeof(std::uint32_t);

    if (usesNPaddingBits) {
      inputBlocks[iBlock].npaddingbits = *((std::uint8_t*)data_ptr);
      data_ptr += sizeof(std::uint8_t);
    }

    inputBlocks[iBlock].min_value = *((std::uint8_t*)data_ptr);
    data_ptr += sizeof(std::uint8_t);

    inputBlocks[iBlock].max_value = *((std::uint8_t*)data_ptr);
    data_ptr += sizeof(std::uint8_t);

    inputBlocks[iBlock].bwt_primary_index = *((std::uint32_t*)data_ptr);
    data_ptr += sizeof(std::uint32_t);

    inputBlocks[iBlock].data = data_ptr;
    data_ptr += inputBlocks[iBlock].length;

    std::cout << "Read block of size " << inputBlocks[iBlock].length
              << std::endl;
  }

  // Decompress each block
  for (auto iBlock = 0; iBlock < nblocks; iBlock++) {
    // Get block pointers etc
    auto inBlock = inputBlocks.get()[iBlock];
    auto& outBlock = outputBlocks[iBlock];

    std::cout << std::endl
              << "Decompressing block " << iBlock << "/" << nblocks
              << " with length " << inBlock.length << std::endl;

    std::cout << "Boundaries of Compressed Data:" << std::endl;
    for (auto i = 0; i < 5; i++) {
      std::cout << std::hex << (int)inBlock.data[i] << " ";
    }
    std::cout << std::endl << "..." << std::endl;
    for (auto i = inBlock.length - 5; i < inBlock.length; i++) {
      std::cout << std::hex << (int)inBlock.data[i] << " ";
    }
    std::cout << std::endl << std::dec;

    // Postcompressor
    auto postcomp = std::make_unique<postcompressor>();
    auto compressed_data = std::make_unique<std::uint8_t[]>(length);
    std::copy(inBlock.data, inBlock.data + length, compressed_data.get());

    std::cout << "Decompressing compressed data of length " << inBlock.length
              << std::endl;
    auto decompress_header = header;

    if (usesNPaddingBits)
      decompress_header.npaddingbits = inBlock.npaddingbits.value();
    // Required for LZW
    decompress_header.output_block_length = MAX_BLOCK_SIZE_BYTES;
    auto [mtf_data, mtf_length, mtf_head] = postcomp->decompress(
        compressed_data, inBlock.length, decompress_header);
    std::cout << "Postcompression is complete: output length=" << mtf_length
              << std::endl;
    // Decompression is complete, free up memory
    compressed_data.reset();

    std::cout << "Boundaries of MTF Data:" << std::endl;
    for (auto i = 0; i < 5; i++) {
      std::cout << std::hex << (int)mtf_data[i] << " ";
    }
    std::cout << std::endl << "..." << std::endl;
    for (auto i = mtf_length - 5; i < mtf_length; i++) {
      std::cout << std::hex << (int)mtf_data[i] << " ";
    }
    std::cout << std::endl << std::dec;

    // Decode MTF data
    auto bwt_data = easyMTFDecodeAnyuType<std::uint8_t, std::uint8_t>(
        mtf_data.get(), mtf_length, inBlock.min_value.value(),
        inBlock.max_value.value());
    mtf_data.reset();

    // Decode BWT data
    std::cout << "Decoding BWT data of length " << std::to_string(mtf_length)
              << std::endl;
    auto bwt_length = mtf_length;
    // Create BWT temporary values
    auto bwt = std::make_unique<std::uint8_t[]>(bwt_length);
    inverse_bw_transform((const sauchar_t*)bwt_data.get(), bwt.get(), nullptr,
                         bwt_length, inBlock.bwt_primary_index.value());
    std::cout << "Decoded BWT data" << std::endl;

    outBlock.length = bwt_length;
    outBlock.data = bwt.release();
    outBlock.npaddingbits.reset();
    outBlock.min_value.reset();
    outBlock.max_value.reset();
    outBlock.bwt_primary_index.reset();
    outBlock.encoded = false;
  }

  // Consolidate outputblocks
  auto blocksize_sum = 0ULL;
  // Get total size: sum of block headers and block data lengths
  std::for_each(
      outputBlocks.get(), outputBlocks.get() + nblocks,
      [&blocksize_sum](auto& block) { blocksize_sum += block.size(); });

  // Move data across
  std::cout << "Allocating output memory: output summed size=" << blocksize_sum
            << std::endl;
  auto output_data = std::make_unique<std::uint8_t[]>(blocksize_sum);
  auto output_data_ptr = output_data.get();
  for (auto iBlock = 0; iBlock < nblocks; iBlock++) {
    auto& block = outputBlocks[iBlock];
    auto block_size = block.size();
    auto block_data = block.toArray();
    std::move(block_data.get(), block_data.get() + block_size, output_data_ptr);
    output_data_ptr += block_size;
    std::cout << "Block [" << iBlock << "]: " << block_size << std::endl;
  }

  // Update file header
  auto output_header = header;
  output_header.compressiontype = sfc::sfcc::compression_t::NONE;
  output_header.npaddingbits.reset();
  return {std::move(output_data), blocksize_sum, output_header};
}

template <class postcompressor>
std::string bzip<postcompressor>::getFileExtension() const
{
  return "bwt";
}

template <>
std::string bzip<lz77>::getFileExtension() const
{
  return "bwt.lz77";
}

template <>
std::string bzip<lzw>::getFileExtension() const
{
  return "bwt.lzw";
}

template <>
std::string bzip<no_compressor>::getFileExtension() const
{
  return "bwt";
}

template <>
sfc::sfcc::compression_t bzip<lz77>::getCompressionType() const
{
  return sfc::sfcc::compression_t::BZIP_LZ77;
}

template <>
sfc::sfcc::compression_t bzip<lzw>::getCompressionType() const
{
  return sfc::sfcc::compression_t::BZIP_LZW;
}

template <>
sfc::sfcc::compression_t bzip<no_compressor>::getCompressionType() const
{
  return sfc::sfcc::compression_t::BWT;
}

// NO_COMPRESSOR
no_compressor::no_compressor() {}
no_compressor::~no_compressor() {}
std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
           sfc::sfcc_header>
no_compressor::compress(const std::unique_ptr<std::uint8_t[]>& data,
                        const unsigned long long& length,
                        const sfc::sfcc_header& header)
{
  auto output_data = std::make_unique<std::uint8_t[]>(length);
  std::copy(data.get(), data.get() + length, output_data.get());
  auto output_header = header;
  output_header.compressiontype = sfc::sfcc::compression_t::NONE;
  return {std::move(output_data), length, output_header};
}

std::tuple<std::unique_ptr<std::uint8_t[]>, unsigned long long,
           sfc::sfcc_header>
no_compressor::decompress(const std::unique_ptr<std::uint8_t[]>& data,
                          const unsigned long long& length,
                          const sfc::sfcc_header& header)
{
  auto output_data = std::make_unique<std::uint8_t[]>(length);
  std::copy(data.get(), data.get() + length, output_data.get());
  auto output_header = header;
  output_header.compressiontype = sfc::sfcc::compression_t::NONE;
  return {std::move(output_data), length, output_header};
}

std::string no_compressor::getFileExtension() const { return "raw"; }
sfc::sfcc::compression_t no_compressor::getCompressionType() const
{
  return sfc::sfcc::compression_t::BWT;
}

std::unique_ptr<compressor> make_compressor(
    const ::sfc::sfcc::compression_t& comp, const bool& bitTransposed)
{
  using ::sfc::sfcc::compression_t;
  switch (comp) {
    case compression_t::HUFFMAN:
      return std::make_unique<huffman>();
    case compression_t::LZ77:
      return std::make_unique<lz77>();
    case compression_t::LZW:
      return std::make_unique<lzw>();
    case compression_t::RLE:
      return std::make_unique<rle>();
    // case compression_t::DEFLATE:
    //   return std::make_unique<deflate>();
    case compression_t::BZIP_LZ77:
      return std::make_unique<bzip<lz77>>(bitTransposed);
    case compression_t::BZIP_LZW:
      return std::make_unique<bzip<lzw>>(bitTransposed);
    case compression_t::BWT:
      return std::make_unique<bzip<no_compressor>>(bitTransposed);
    default:
      return nullptr;
  }
}
}  // namespace sfcc
