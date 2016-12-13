#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include <getopt.h>

#include "compression_type.h"
#include "index_type.h"
#include "huffman.h"
#include "lz78.h"
#include "sufarray.h"
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
    int c = getopt_long(argc, argv, "c:hi:", long_options, &option_index);

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
          return 0;

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

      c = getopt_long(argc, argv, "c:hi:", long_options, &option_index);
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
        
        std::vector<char> text(file_size);
        ifs.read(&text[0], text.size());

        // Build index and write index file. Since we only have suffix arrays right now, we will
        // not perform any type checking for the IndexType value.
        std::vector<int> suffix_array = ipmt::BuildSuffixArray(text.data());
        ipmt::WriteIndexFile(filenames[j], suffix_array, text.data(), compression_type);
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
          return 0;

        case 'p':
          read_pattern_files = true;
          break;

        default:
          std::cout << "Invalid option argument." << std::endl;
          return EXIT_FAILURE;
      }

      c = getopt_long(argc, argv, "chp", long_options, &option_index);
    }

    if (optind >= argc + 1) {
      std::cout << "Incorrect number of arguments (type ipmt --help for more details)."
                << std::endl;
      return EXIT_FAILURE;
    }

    // ## Read patterns from arguments.
    std::vector<std::string> patterns;

    if (read_pattern_files) {
      std::vector<std::string> pattern_files = ipmt::GetFilenames(argv[optind++]);
      for (size_t i = 0; i < pattern_files.size(); ++i) {
        std::ifstream ifs(pattern_files[i], std::ifstream::binary);
        if (!ifs) {
          std::cout << "Cannot open pattern file(s) from argument list." << std::endl;
          return EXIT_FAILURE;
        }

        std::string line;
        while (std::getline(ifs, line)) {
          patterns.push_back(line);
        }
      }
    } else {
      patterns.push_back(argv[optind++]);
    }

    // ## For each index file, decode text and find patterns occurrences.
    bool has_multiple_index_files = (argc - optind) > 1;
    for (int i = optind; i < argc; ++i) {
      std::vector<std::string> index_files = ipmt::GetFilenames(argv[i]);
      has_multiple_index_files |= index_files.size() > 1;

      for (size_t j = 0; j < index_files.size(); ++j) {
        std::vector<int> suffix_array;
        std::string text;
        int status = ipmt::ReadIndexFile(index_files[j], &text, &suffix_array);

        if (status == -1) {
          std::cout << "Cannot open index file " << index_files[j] << "." << std::endl;
          return EXIT_FAILURE;
        } else if (status == -2) {
          std::cout << "Invalid compression type on index file." << std::endl;
          return EXIT_FAILURE;
        } else {  // status == 0.
          std::vector<int> occurrences;
          size_t total = 0;

          for (size_t k = 0; k < patterns.size(); ++k) {
            occurrences = ipmt::GetOccurrences(patterns[k], text, suffix_array);
            if (!print_num_occ_only) {
              std::cout << ipmt::PrintOccurrences(occurrences, text, patterns[k].size());
            }

            total += occurrences.size();
          }

          if (print_num_occ_only && has_multiple_index_files) {
            std::cout << index_files[j] << ":" << total << std::endl;
          } else if (print_num_occ_only) {
            std::cout << total << std::endl;
          }
        }
      }
    }
  } else if (!mode.compare("-h") || !mode.compare("--help")) {
    ipmt::PrintHelp();
  } else {
    std::cout << "Invalid option or mode." << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}

