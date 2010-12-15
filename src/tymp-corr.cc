// Copyright 2010 <Mark Washenberger>

#include <stdio.h>
#include <stdlib.h>

#include <sndfile.hh>

#include "AveragingSlidingWindow.h"
#include "CorrelationsFile.h"
#include "SlidingWindow.h"

const int kBufSize = 4096;

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

struct LoopContext {
  SndfileHandle *in_file;
  tympani::CorrelationsFile<int> *out_file;
  tympani::SlidingWindow *sliding_window;
  tympani::AveragingSlidingWindow *avg_window;
};

struct LoopContext initializeContext(struct Args args) {
  struct LoopContext context;
  context.in_file = new SndfileHandle(args.input_filepath);
  context.out_file = new tympani::CorrelationsFile<int>(args.width);
  context.out_file->open(args.output_filepath);
  context.sliding_window = new tympani::SlidingWindow(args.width);
  context.avg_window = new tympani::AveragingSlidingWindow(
        context.sliding_window, 11025);
  return context;
}

void runLoop(struct LoopContext context) {
  int frames, left, right;
  int buf[kBufSize];
  int *corr = new int[context.sliding_window->width()];
  int frames_to_read = kBufSize/context.in_file->channels();
  int samples_per_write = context.in_file->samplerate() / 30;

  int count = 0;
  while (1) {
    frames = context.in_file->readf(buf, frames_to_read);
    if (frames == 0)
      break;
    for (int i = 0; i < frames; i++) {
      count++;
      left = buf[i * context.in_file->channels()];
      right = buf[i * context.in_file->channels() + 1];
      context.avg_window->add(left, right);
      if (count % samples_per_write == 0) {
        context.avg_window->correlations(corr);
        context.out_file->write(corr);
      }
    }
  }
  delete[] corr;
}

void destroyContext(struct LoopContext context) {
  context.out_file->close();
  delete context.in_file;
  delete context.out_file;
  delete context.sliding_window;
  delete context.avg_window;
}

int main(int argc, char **argv) {
  struct Args args = parse_args_or_die(argc, argv);
  struct LoopContext context = initializeContext(args);
  runLoop(context);
  destroyContext(context);
  return 0;
}

