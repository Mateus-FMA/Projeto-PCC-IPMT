#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <getopt.h>

#include "compression_type.h"
#include "index_type.h"
#include "huffman.h"
#include "utils.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cout << "Incorrect number of arguments." << std::endl;
    return EXIT_FAILURE; 
  }

  std::string mode = argv[1];
  ++optind;

  if (!mode.compare("index")) {
    // ## Processing index mode options.
    ipmt::Option long_options[] = {
      {"compression", required_argument, nullptr, 'c'},
      {"help", no_argument, nullptr, 'h'},
      {"indextype", required_argument, nullptr, 'i'}
    };

    int option_index = 0;
    int c = getopt_long(argc, argv, "chp", long_options, &option_index);

    ipmt::CompressionType compression_type = ipmt::CompressionType::kHuffman;
    ipmt::IndexType index_type = ipmt::IndexType::kSuffixArray;
    std::string option_arg;
    
    while (c != -1) {
      switch (c){
        case 'c':
          option_arg = optarg;

          if (!option_arg.compare("huffman")) {
            compression_type = ipmt::CompressionType::kHuffman;
          } else if (!option_arg.compare("lz78")) {
            compression_type = ipmt::CompressionType::kLZ78;
          } else {
            std::cout << "Unimplemented or invalid compression algorithm." << std::endl;
            return EXIT_FAILURE;
          }

          break;
        case 'h':
          ipmt::PrintIndexModeHelp();
          break;
        case 'i':
          option_arg = optarg;

          if (!option_arg.compare("sa")) {
            index_type = ipmt::IndexType::kSuffixArray;
          } else {
            std::cout << "Unimplemented or invalid index structure." << std::endl;
            return EXIT_FAILURE;
          }

          break;
        default:
          std::cout << "Invalid option argument." << std::endl;
          return EXIT_FAILURE;
      }

      c = getopt_long(argc, argv, "chp", long_options, &option_index);
    }

    if (optind >= argc) {
      std::cout << "Incorrect number of arguments (type ipmt --help for more details)."
                << std::endl;
      return EXIT_FAILURE;
    }

    // ## For each text file, build its respective index file.
    for (int i = optind; i < argc; ++i) {
      std::vector<std::string> filenames = ipmt::GetFilenames(argv[i]);
      
      for (size_t j = 0; j < filenames.size(); ++j) {
        std::ifstream ifs(filenames[j], std::ifstream::ate | std::ifstream::binary);
        if (!ifs) {
          std::cout << "Cannot open file(s) from argument list." << std::endl;
          return EXIT_FAILURE;
        }
         
        size_t file_size = ifs.tellg();
        ifs.seekg(0, ifs.beg);
        
        std::string text;
        text.resize(file_size);
        ifs.read(&text[0], text.size());

        // Build index.


        // Encode text.
        ipmt::DynamicBitset code;

        if (compression_type == ipmt::CompressionType::kHuffman) {
          ipmt::HuffmanHeapNode root;
          ipmt::HuffmanEncode(text, &code, &root);
        } else {
          // ipmt::LZ78Encode(text, &code);
        }

        // Write index and encoded text to index file.
        std::cout << code.ToString() << std::endl;
      }
    }
  } else if (!mode.compare("search")){
    // ## Processing search mode options.
    ipmt::Option long_options[] = {
      {"count", no_argument, nullptr, 'c'},      
      {"help", no_argument, nullptr, 'h'},
      {"pattern", no_argument, nullptr, 'p'}
    };

    int option_index = 0;
    int c = getopt_long(argc, argv, "chp", long_options, &option_index);

    bool print_num_occ_only = false;
    bool read_pattern_files = false;
    
    while (c != -1) {
      switch (c){
        case 'c':
          print_num_occ_only = true;
          break;
        case 'h':
          ipmt::PrintSearchModeHelp();
          break;
        case 'p':
          read_pattern_files = true;
          break;
        default:
          std::cout << "Invalid option argument." << std::endl;
          return EXIT_FAILURE;
      }

      c = getopt_long(argc, argv, "chp", long_options, &option_index);
    }

    if (optind >= argc) {
      std::cout << "Incorrect number of arguments (type ipmt --help for more details)."
                << std::endl;
      return EXIT_FAILURE;
    }

    // ## Read patterns from arguments.

    // ## For each index file, decode text and find patterns occurrences.

  } else if (!mode.compare("-h") || !mode.compare("--help")) {
    ipmt::PrintHelp();
  } else {
    std::cout << "Invalid option or mode." << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}

