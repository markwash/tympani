#include "sndfile.hh"

#include <stdlib.h>

int main(int argc, char **argv) {

  const char *path = argv[1];
  int time = atoi(argv[2]);
  int offset = atoi(argv[3]);

  SndfileHandle file = SndfileHandle(path, SFM_WRITE, 0x10002, 2, 44100);

  int increase = 0x10000 / 260;
  short left_level = 0;
  short right_level = (left_level + offset * increase) % 0x10000;
  int frame[2];
  for (int i = 0; i < 44100 * time; i++) {
    frame[0] = left_level;
    frame[1] = right_level;
    left_level = (left_level + increase) % 0x10000;
    right_level = (right_level + increase) % 0x10000;
    file.writef(frame, 1);
  }
}
