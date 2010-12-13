// Copyright 2010 <Mark Washenberger>

//#include "SlidingWindow.h"
#include <stdio.h>
#include <stdlib.h>

struct Args {
  const char *filepath;
  int width;
};

struct Args parse_args_or_die(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <path> <width>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  struct Args args;
  args.filepath = argv[1];
  args.width = atoi(argv[2]);
  return args;
}

int main(int argc, char **argv) {

  struct Args args = parse_args_or_die(argc, argv);

  printf("filepath %s, width %d\n", args.filepath, args.width);

  return 0;

}
