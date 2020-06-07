/**
 * @file sfcc.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-10-20
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "sfcc.h"

namespace sfc {
namespace sfcc {
std::unordered_map<std::string, sfc_t> sfc_string_t{
    {"ROW_MAJOR", sfc_t::ROW_MAJOR},
    {"SNAKE_SCAN", sfc_t::SNAKE_SCAN},
    {"MORTON", sfc_t::MORTON},
    {"GRAY_CODE", sfc_t::GRAY_CODE},
    {"HILBERT", sfc_t::HILBERT}};
std::unordered_map<std::string, compression_t> compression_string_t{
    {"NONE", compression_t::NONE},
    {"RLE", compression_t::RLE},
    {"HUFFMAN", compression_t::HUFFMAN},
    {"LZ77", compression_t::LZ77},
    {"LZ78", compression_t::LZ78},
    {"LZW", compression_t::LZW},
    // {"DEFLATE", compression_t::DEFLATE},
    {"BZIP_LZ77", compression_t::BZIP_LZ77},
    {"BZIP_LZW", compression_t::BZIP_LZW},
    {"BWT", compression_t::BWT}};

const std::string magicword = "SFCC";
const std::uint8_t magicword_uint8_t[] = {'S', 'F', 'C', 'C'};
const std::uint8_t bittranspose_mask = 0b10000000;
}  // namespace sfcc

sfcc_file::sfcc_file(const std::string& filename) : _data(nullptr)
{
  // Open file
  std::ifstream file(filename, std::ios::binary | std::ios::in);
  if (::sfc::DEBUG) std::cout << "Opening " << filename << std::endl;
  if (!file.good() || !file.is_open())
    throw std::runtime_error("Opening file failed");
  // Make the file throw an exception if any error occurs
  file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  // Load header
  // std::uint8_t* magic_header = new std::uint8_t[4];
  auto magic_header = std::make_unique<std::uint8_t[]>(4);
  file.read((char*)magic_header.get(), 4);
  for (auto i = 0; i < 4; i++)
    if (magic_header[i] != sfcc::magicword[i]) {
      std::cerr << "ERROR: File not correct: ";
      std::copy((char*)(magic_header.get()), (char*)(magic_header.get()) + 4,
                std::ostreambuf_iterator<char>(std::cerr));
      std::cerr << " vs ";
      std::copy((char*)(sfcc::magicword_uint8_t),
                (char*)(sfcc::magicword_uint8_t) + 4,
                std::ostreambuf_iterator<char>(std::cerr));
      std::cerr << std::endl;
      exit(EXIT_FAILURE);
    }
  sfc::sfcc_header header;
  file.get((char&)header.ndims);
  file.get((char&)header.sidelength_k);
  file.get((char&)header.dtype_nbytes);
  std::uint8_t temp;
  file.get((char&)temp);

  // Extract bittranspose
  auto bittranspose_int = temp & sfcc::bittranspose_mask;
  // Reset bit in sfc type int
  temp &= ~sfcc::bittranspose_mask;
  header.sfctype = sfcc::sfc_t(temp);
  header.bittransposed = bittranspose_int != 0;

  if (::sfc::DEBUG)
    std::cout << "sfctype=" << int(temp)
              << ", bit-transposed=" << (header.bittransposed ? "yes" : "no")
              << std::endl;

  file.get((char&)temp);
  header.compressiontype = sfcc::compression_t(temp);
  if (sfc::sfcc_header::CompressionRequiresPaddingBits(
          header.compressiontype)) {
    std::uint8_t temp;
    file.get((char&)temp);
    header.npaddingbits = temp;
  }

  _header = header;

  // Calculate file size
  auto file_size = std::filesystem::file_size(filename);  // File size in bytes
  file_size -= _header.size();
  _data_size = file_size;

  _data = std::make_unique<std::uint8_t[]>(_data_size);
  auto i = 1ULL;
  file.read((char*)_data.get(), _data_size);
  auto [min, max] =
      std::minmax_element((std::uint16_t*)_data.get(),
                          (std::uint16_t*)_data.get() + (_data_size / 2));
  // delete[] magic_header;
  if (::sfc::DEBUG) std::cout << "Finished opening " << filename << std::endl;
}

// sfcc_file::~sfcc_file() {}

std::unique_ptr<std::uint8_t[]> HeaderToArray(const sfcc_header& header)
{
  auto ptr = std::make_unique<std::uint8_t[]>(header.size());
  unsigned int index = 4;
  std::copy(std::begin(sfcc::magicword), std::end(sfcc::magicword), ptr.get());
  ptr[index++] = header.ndims;
  ptr[index++] = header.sidelength_k;
  ptr[index++] = header.dtype_nbytes;
  // Encode bittransposed into sfctype
  auto sfctype_int = std::uint8_t(header.sfctype);
  if (header.bittransposed) sfctype_int |= sfcc::bittranspose_mask;

  ptr[index++] = sfctype_int;
  ptr[index++] = std::uint8_t(header.compressiontype);
  if (header.npaddingbits && sfc::sfcc_header::CompressionRequiresPaddingBits(
                                 header.compressiontype)) {
    if (::sfc::DEBUG)
      std::cout << "npaddingbits = "
                << std::uint16_t(header.npaddingbits.value()) << std::endl;
    ptr[index++] = std::uint8_t(header.npaddingbits.value());
  }
  return std::move(ptr);
}
};  // namespace sfc