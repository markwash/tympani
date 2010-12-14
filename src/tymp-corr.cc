// Copyright 2010 <Mark Washenberger>

#include <stdio.h>
#include <stdlib.h>

#include <sndfile.hh>

#include "CorrelationsFile.h"
#include "SlidingWindow.h"

struct Args {
  const char *input_filepath;
  const char *output_filepath;
  int width;
};

struct Args parse_args_or_die(int argc, char **argv) {
  if (argc != 4) {
    fprintf(stderr, "Usage: %s <input file> <output file> <width>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  struct Args args;
  args.input_filepath = argv[1];
  args.output_filepath = argv[2];
  args.width = atoi(argv[3]);
  return args;
}

const int kBufSize = 4096;

int main(int argc, char **argv) {

  struct Args args = parse_args_or_die(argc, argv);
  SndfileHandle in_file(args.input_filepath);
  tympani::CorrelationsFile<int> out_file(args.width);
  out_file.open(args.output_filepath);
  tympani::SlidingWindow window(args.width);
  int frames;
  int buf[kBufSize];
  int *corr = new int[args.width];
  int left, right;
  while (1) {
    frames = in_file.readf(buf, kBufSize/in_file.channels());
    if (frames == 0)
      break;
    for (int i = 0; i < frames; i++) {
      left = buf[i * in_file.channels()];
      right = buf[i * in_file.channels() + 1];
      window.add(left, right);
      window.correlations(corr);
      out_file.write(corr);
    }
  }
  out_file.close();
  delete[] corr;

  return 0;

}
