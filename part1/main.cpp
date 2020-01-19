#include "helper.h"
#include <unistd.h>

int main(int argc, char** argv) {
  Sys* sys = new Sys();
  char *fvalue = const_cast<char *>("");
  int cli_opt;

  while ((cli_opt = getopt(argc, argv, "f:")) != -1) {
    switch (cli_opt)
    {
    case 'f':
      fvalue = optarg;
      break;
    default:
      abort();
    }
  }

  sys->pln(fvalue);
  
}