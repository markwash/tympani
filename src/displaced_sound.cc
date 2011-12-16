#include "sndfile.hh"

#include <stdlib.h>

int sound(int time, int period) {
  int rel = time % period;
  int level;
  if (rel < period/2)
    level = 4 * 0x6000 * rel / period - 0x6000;
  else
    level = 3 * 0x6000 - 4 * 0x6000 * rel / period;
  if (level > 0x4000)
    return 0x4000;
  else if (level < -0x4000)
    return -0x4000;
  else
    return level;
}

int main(int argc, char **argv) {

  const char *path = argv[1];
  int time = atoi(argv[2]);
  int offset = atoi(argv[3]);

  SndfileHandle file = SndfileHandle(path, SFM_WRITE, 0x10002, 2, 44100);
  int period = 44100 / 260;

  short buf[2 * 4096];
  int count = 0;
  while (count < 44100 * time) {
    for (int i = 0; i < 4096; i++) {
      buf[2 * i] = sound(count, period);
      buf[2 * i + 1] = sound(count + offset, period);
      //buf[2 * i] = (count / period) % 2 ? 0x4000 : -0x4000;
      //buf[2 * i + 1] = ((count + offset) / period) % 2 ? 0x4000 : -0x4000;
      count++;
    }
    file.writef(buf, 4096);
  }
}
