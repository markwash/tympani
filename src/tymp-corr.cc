// Copyright 2010 <Mark Washenberger>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <sndfile.hh>

#include "AveragingSlidingWindow.h"
#include "CorrelationsScreenGenerator.h"
#include "SlidingWindow.h"

const int kBufSize = 4096;

struct Args {
  const char *input_filepath;
  int width;
};

struct Args parse_args_or_die(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "Usage: %s <input file> <width>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  struct Args args;
  args.input_filepath = argv[1];
  args.width = atoi(argv[2]);
  return args;
}

struct LoopContext {
  SndfileHandle *in_file;
  tympani::SlidingWindow *sliding_window;
  tympani::AveragingSlidingWindow *avg_window;
  tympani::CorrelationsScreenGenerator *screen_generator;
  SDL_Surface *screen;
};

struct LoopContext initializeContext(struct Args args) {
  struct LoopContext context;
  context.in_file = new SndfileHandle(args.input_filepath);
  context.sliding_window = new tympani::SlidingWindow(args.width);
  context.avg_window = new tympani::AveragingSlidingWindow(
        context.sliding_window, 44100/4);
  context.screen_generator =
        new tympani::CorrelationsScreenGenerator(args.width);
  context.screen = SDL_SetVideoMode(1024, 1024, 32, SDL_HWSURFACE);
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
        context.screen_generator->draw(context.screen, corr);
        usleep(5000);
      }
    }
  }
  delete[] corr;
}

void destroyContext(struct LoopContext context) {
  delete context.in_file;
  delete context.sliding_window;
  delete context.avg_window;
  delete context.screen_generator;
}

int main(int argc, char **argv) {
  SDL_Init(SDL_INIT_EVERYTHING);
  struct Args args = parse_args_or_die(argc, argv);
  struct LoopContext context = initializeContext(args);
  runLoop(context);
  destroyContext(context);
  return 0;
}

