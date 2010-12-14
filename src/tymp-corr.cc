// Copyright 2010 <Mark Washenberger>

#include <stdio.h>
#include <stdlib.h>

#include <sndfile.hh>

#include "SlidingWindow.h"

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

#define BUFSIZE 4096

int main(int argc, char **argv) {

  struct Args args = parse_args_or_die(argc, argv);
  SndfileHandle file(args.filepath);
  tympani::SlidingWindow window(args.width);
  int frames;
  int buf[BUFSIZE];
  int *corr = new int[args.width];
  while (1) {
    frames = file.readf(buf, BUFSIZE/file.channels());
    if (frames == 0)
      break;
    for (int i = 0; i < frames; i++) {
      window.add(buf[i * file.channels()], buf[i * file.channels() + 1]);
      window.correlations(corr);
    }
  }
  delete corr;

  return 0;

}
