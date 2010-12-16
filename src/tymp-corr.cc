// Copyright 2010 <Mark Washenberger>

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <portaudiocpp/AutoSystem.hxx>
#include <portaudiocpp/BlockingStream.hxx>
#include <portaudiocpp/Device.hxx>
#include <portaudiocpp/DirectionSpecificStreamParameters.hxx>
#include <portaudiocpp/StreamParameters.hxx>
#include <portaudiocpp/System.hxx>

#include "AveragingSlidingWindow.h"
#include "CorrelationsScreenGenerator.h"
#include "SlidingWindow.h"


const int kMaxBufSize = 65535;

struct Args {
  int width;
};

struct Args parse_args_or_die(int argc, char **argv) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <width>\n", argv[0]);
    exit(EXIT_FAILURE);
  }
  struct Args args;
  args.width = atoi(argv[1]);
  return args;
}

struct LoopContext {
  portaudio::BlockingStream *input_stream;
  tympani::SlidingWindow *sliding_window;
  tympani::AveragingSlidingWindow *avg_window;
  tympani::CorrelationsScreenGenerator *screen_generator;
  SDL_Surface *screen;
};

void initializeInputStream(portaudio::BlockingStream **input_stream) {
  portaudio::System &system = portaudio::System::instance();
  const portaudio::Device &input_device = system.defaultInputDevice();
  portaudio::DirectionSpecificStreamParameters input_params;
  input_params.setDevice(input_device);
  input_params.setNumChannels(2);
  input_params.setSampleFormat(portaudio::INT16);
  input_params.setSuggestedLatency(input_device.defaultLowInputLatency());
  portaudio::StreamParameters params;
  params.setInputParameters(input_params);
  params.setOutputParameters(
      portaudio::DirectionSpecificStreamParameters::null());
  params.setSampleRate(input_device.defaultSampleRate());
  params.setFramesPerBuffer(paFramesPerBufferUnspecified);
  params.clearFlags();
  assert(params.isSupported());
  *input_stream = new portaudio::BlockingStream(params);
  (*input_stream)->start();
}

struct LoopContext initializeContext(struct Args args) {
  struct LoopContext context;
  initializeInputStream(&context.input_stream);
  context.sliding_window = new tympani::SlidingWindow(args.width);
  context.avg_window = new tympani::AveragingSlidingWindow(
        context.sliding_window, context.input_stream->sampleRate() / 2);
  context.screen_generator =
        new tympani::CorrelationsScreenGenerator(args.width);
  context.screen = SDL_SetVideoMode(1024, 1024, 32, SDL_HWSURFACE);
  return context;
}

int readFrames(portaudio::BlockingStream *in, int *buf, 
               int max_size, int frame_size) {
  int max_frames_to_read = max_size / frame_size;
  int min_frames_to_read = in->sampleRate() / 24;
  int frames, frames_available;

  frames_available = in->availableReadSize();
  if (frames_available < min_frames_to_read)
    frames = min_frames_to_read;
  else if (frames_available > max_frames_to_read)
    frames = max_frames_to_read;
  else
    frames = frames_available;
  in->read(buf, frames);
  return frames;
}

void runLoop(struct LoopContext context) {
  int frames, left, right;
  int buf[kMaxBufSize];
  int *corr = new int[context.sliding_window->width()];

  int count = 0;
  while (1) {

    frames = readFrames(context.input_stream, buf, kMaxBufSize, 2);

    for (int i = 0; i < frames; i++) {
      count++;
      left = buf[2 * i];
      right = buf[2 * i + 1];
      context.avg_window->add(left, right);
    }

    context.avg_window->correlations(corr);
    context.screen_generator->draw(context.screen, corr);
  }
  delete[] corr;
}

void destroyContext(struct LoopContext context) {
  delete context.input_stream;
  delete context.sliding_window;
  delete context.avg_window;
  delete context.screen_generator;
}

int main(int argc, char **argv) {
  portaudio::AutoSystem auto_sys;
  SDL_Init(SDL_INIT_EVERYTHING);
  struct Args args = parse_args_or_die(argc, argv);
  struct LoopContext context = initializeContext(args);
  runLoop(context);
  destroyContext(context);
  return 0;
}

