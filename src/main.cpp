#include <iostream>
#include <string>

#include <getopt.h>

#include "utils.h"

int main(int argc, char *argv[]) {
  if (argc == 1) {
    std::cout << "Incorrect number of arguments." << std::endl;
    return EXIT_FAILURE; 
  }

  std::string mode = argv[1];
  ++optind;

  if (!mode.compare("index")) {
    ipmt::Option long_options[] = {
      {"compression", required_argument, nullptr, 'c'},
      {"help", no_argument, nullptr, 'h'},
      {"indextype", required_argument, nullptr, 'i'}
    };

    // TODO(Mateus/Valdemir): implementar as chamadas das funções de indexação.
    ipmt::PrintIndexModeHelp();
  } else if (!mode.compare("search")){
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
  } else if (!mode.compare("-h") || !mode.compare("--help")) {
    ipmt::PrintHelp();
  } else {
    std::cout << "Invalid option or mode." << std::endl;
    return EXIT_FAILURE;
  }

  return 0;
}
