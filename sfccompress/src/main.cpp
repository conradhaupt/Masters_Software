/**
 * @file main.cpp
 * @author Conrad Haupt (conrad@conradhaupt.co.za)
 * @brief
 * @version 1.0
 * @date 2019-10-20
 *
 * @copyright Copyright (c) 2020
 *
 */
#include "main.h"
#include <unistd.h>
#include <args.hxx>
#include <iostream>
#include <string>
#include "bittranspose.h"
#include "compress_handler.h"
#include "reorder.h"
#include "sfcc.h"

void _compress(args::Subparser& sp)
{
  args::MapFlag<std::string, sfc::main::sfcs::types> sfc(
      sp, "CURVE", sfc::main::desc::sfcurve, {'s', "sfc"},
      sfc::sfcc::sfc_string_t);

  args::MapFlag<std::string, sfc::main::compression::types> comp(
      sp, "COMP", sfc::main::desc::compression, {'c', "comp"},
      sfc::sfcc::compression_string_t);
  args::Positional<std::string> file(sp, "file",
                                     sfc::main::desc::compress_file);
  args::Positional<std::string> outfile(sp, "output file",
                                        sfc::main::desc::compress_out_file);
  args::Flag bittranspose(sp, "bittranspose", sfc::main::desc::bittranspose,
                          {'b', "bittranspose"});
  sp.Parse();
  if (!file || (!sfc && !comp)) std::cout << sp << std::endl;

  sfc::sfcc_file sfile(args::get(file));
  if (sfile.getHeader().compressiontype != sfc::sfcc::compression_t::NONE) {
    std::cerr << "File is already compressed: "
              << sfc::sfcc::tostring(sfile.getHeader().compressiontype) << " ["
              << int(std::uint8_t(sfile.getHeader().compressiontype)) << "]"
              << std::endl;
    return;
  }
  if (sfile.getHeader().sfctype != sfc::sfcc::sfc_t::ROW_MAJOR) {
    std::cerr << "File is already reordered: "
              << sfc::sfcc::tostring(sfile.getHeader().sfctype)
              << int(std::uint8_t(sfile.getHeader().sfctype)) << std::endl;
    return;
  }

  auto data_in = std::make_unique<std::uint8_t[]>(sfile.size());
  std::copy(sfile.getData(), sfile.getData() + sfile.size(), data_in.get());
  auto header = sfile.getHeader();
  auto dimlength = 1ULL << header.sidelength_k;

  // Reorder
  auto reorder = sfcc::make_reorderer(header.ndims, dimlength, header.sfctype,
                                      args::get(sfc));

  reorder->reorder_withtemporary_vardtype(
      data_in.get(), sfc::pow(dimlength, header.ndims) * header.dtype_nbytes,
      header.dtype_nbytes);

  // Bit-Transpose
  if (bittranspose) {
    if (::sfc::DEBUG)
      std::cout << "Transposing bits with data-type of "
                << int(header.dtype_nbytes) << " bytes" << std::endl;
    auto bittransposer = sfcc::BitTransposer();
    bittransposer.transpose(
        data_in.get(), sfc::pow(dimlength, header.ndims) * header.dtype_nbytes,
        header.dtype_nbytes);
    header.bittransposed = true;
  }

  header.sfctype = args::get(sfc);

  std::uint8_t* data;
  int length;
  std::unique_ptr<std::uint8_t[]> data_tmp;
  // Compress
  auto compressor = sfcc::make_compressor(args::get(comp), bittranspose);
  if (compressor) {
    auto [data_tmp, _length, _header] =
        compressor->compress(data_in, sfile.size(), header);
    data = data_tmp.release();
    length = _length;
    header = _header;
  } else {
    data = data_in.release();
    length = sfile.sizein_bytes();
    header.compressiontype = sfc::sfcc::compression_t::NONE;
  }
  std::ofstream outputfile;
  std::string outputFileName;
  if (outfile)
    outputFileName = args::get(outfile);
  else {
    outputFileName = args::get(file) + sfc::main::SFCToExt(args::get(sfc)) +
                     (bittranspose ? ".btr" : "") +
                     (compressor ? '.' + compressor->getFileExtension() : "");
  }
  outputfile = std::ofstream(outputFileName, std::ios::binary);
  auto header_ptr = sfc::HeaderToArray(header);
  if (::sfc::DEBUG) {
    std::cout << "Header: ";
    for (auto i = 0; i < header.size(); i++) {
      std::cout << std::hex << +header_ptr[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "First byte of data is " << std::hex << +data[0] << std::endl;
  }
  outputfile.write((char*)header_ptr.get(), header.size());
  outputfile.write((char*)data, length);
  outputfile.close();
  delete[] data;
}

void _decompress(args::Subparser& sp)
{
  args::Positional<std::string> file(sp, "file",
                                     sfc::main::desc::compress_file);
  args::Positional<std::string> outfile(sp, "output file",
                                        sfc::main::desc::compress_out_file);
  sp.Parse();

  sfc::sfcc_file sfile(args::get(file));
  // if (sfile.getHeader().compressiontype == sfc::sfcc::compression_t::NONE) {
  //   std::cerr << "File is already decompressed: "
  //             << sfc::sfcc::tostring(sfile.getHeader().compressiontype)
  //             << int(std::uint8_t(sfile.getHeader().compressiontype))
  //             << std::endl;
  //   return;
  // }
  // Decompress
  auto header = sfile.getHeader();
  bool bittransposed = header.bittransposed;
  if (::sfc::DEBUG)
    std::cout << "File is " << (bittransposed ? "" : "not ") << "bit-transposed"
              << std::endl;
  header.sfctype = sfc::main::sfcs::types::ROW_MAJOR;
  std::uint8_t* data;
  int length;
  std::unique_ptr<std::uint8_t[]> data_tmp;

  auto data_in = std::make_unique<std::uint8_t[]>(sfile.size());
  std::copy(sfile.getData(), sfile.getData() + sfile.size(), data_in.get());
  // Decompress
  auto compressor =
      sfcc::make_compressor(sfile.getHeader().compressiontype, bittransposed);
  if (compressor) {
    auto [data_tmp, _length, _header] =
        compressor->decompress(data_in, sfile.size(), header);
    data = data_tmp.release();
    length = _length;
    header = _header;
  } else {
    data = data_in.release();
    length = sfile.sizein_bytes();
    header.compressiontype = sfc::sfcc::compression_t::NONE;
  }
  data_in.reset();

  auto dimlength = 1ULL << sfile.getHeader().sidelength_k;

  // Bit-transpose if necessary
  if (bittransposed) {
    auto bittransposer = sfcc::BitTransposer();
    bittransposer.reverse_transpose(
        data, sfc::pow(dimlength, header.ndims) * header.dtype_nbytes,
        header.dtype_nbytes);
    if (::sfc::DEBUG) std::cout << "Transposed" << std::endl;
    header.bittransposed = false;
  }

  // Reorder
  auto reorder = sfcc::make_reorderer(sfile.getHeader().ndims, dimlength,
                                      sfile.getHeader().sfctype,
                                      sfc::main::sfcs::types::ROW_MAJOR);

  reorder->reorder_withtemporary_vardtype(
      data, sfc::pow(dimlength, header.ndims) * header.dtype_nbytes,
      header.dtype_nbytes);

  // Determine where to write to
  std::ostream* output;
  bool outputToStdOut = false;
  std::string outputFileName;
  if (outfile) {
    outputFileName = args::get(outfile);
    if (outputFileName == "-") outputToStdOut = true;
  } else {
    outputFileName = args::get(file);
    if (compressor) {
      auto nFilenameExt =
          outputFileName.rfind("." + compressor->getFileExtension());
      outputFileName.erase(nFilenameExt, outputFileName.size() - nFilenameExt);
    }
    if (bittransposed) {
      if (::sfc::DEBUG)
        std::cout << "Bit-Tranposed, removing extension" << std::endl;
      auto nFilenameExt = outputFileName.rfind(".btr");
      outputFileName.erase(nFilenameExt, outputFileName.size() - nFilenameExt);
    }
    auto nFilenameExt =
        outputFileName.rfind(sfc::main::SFCToExt(sfile.getHeader().sfctype));
    outputFileName.erase(nFilenameExt, outputFileName.size() - nFilenameExt);
  }

  if (!outputToStdOut)
    std::cout << "outputFileName=" << outputFileName << std::endl;
  if (!outputToStdOut)
    output = new std::ofstream(outputFileName, std::ios::binary);
  else
    output = &std::cout;
  auto header_ptr = sfc::HeaderToArray(header);
  if (::sfc::DEBUG) {
    std::cout << "Header: ";
    for (auto i = 0; i < header.size(); i++) {
      std::cout << std::hex << +header_ptr[i] << " ";
    }
    std::cout << std::endl;
    std::cout << "First byte of data is " << std::hex << +data[0] << std::endl;
  }
  output->write((char*)header_ptr.get(), header.size());
  output->write((char*)data, length);
  if (!outputToStdOut) delete output;
  delete[] data;
}

int main(int argc, char* argv[])
{
  // Argument configuration
  args::ArgumentParser parser(sfc::main::desc::program, "");
  args::Group arguments(parser, "arguments", args::Group::Validators::DontCare,
                        args::Options::Global);
  args::HelpFlag help(arguments, "help", sfc::main::desc::help, {'h', "help"});
  args::Group cmds(parser, "Commands");
  args::Command compress(cmds, "compress", sfc::main::desc::compress,
                         _compress);
  args::Command decompress(cmds, "decompress", sfc::main::desc::decompress,
                           _decompress);

  // Parse
  try {
    parser.ParseCLI(argc, argv);
  } catch (const args::Completion& eCompletion) {
    std::cout << eCompletion.what();
    exit(EXIT_SUCCESS);
  } catch (const args::Help&) {
    std::cout << parser;
    exit(EXIT_SUCCESS);
  } catch (const args::ParseError& eParse) {
    std::cerr << eParse.what() << std::endl;
    std::cerr << parser;
    exit(EXIT_FAILURE);
  }
  //  catch (...) {
  //   return -1;
  // }

  return 0;
}