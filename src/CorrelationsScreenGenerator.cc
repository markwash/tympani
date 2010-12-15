#include "CorrelationsScreenGenerator.h"

#include <stdint.h>
#include <string.h>

namespace tympani {

CorrelationsScreenGenerator::CorrelationsScreenGenerator(int samples)
    : samples_(samples), max_(0x40000000) {} 

void CorrelationsScreenGenerator::draw(SDL_Surface *screen, 
                                       const int *corr) {
  draw_background(screen);
  for (int i = 0; i < samples_; i++) {
    draw_sample(screen, i, corr[i]);
  }
  SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void CorrelationsScreenGenerator::draw_background(SDL_Surface *screen) {
  memset(screen->pixels, 0, screen->h * screen->pitch);
}

void CorrelationsScreenGenerator::draw_sample(
      SDL_Surface *screen, int sample, int corr) {
  int x, y, line_width;
  int red, blue;
  uint32_t color;
  uint32_t *pixel;
  line_width = screen->w / samples_;
  x = sample * line_width;
  y = screen->w / 2 * (1.0 - 1.0 * corr / max_);
  red = (1.0 * (max_ + corr)) / (2.0 * max_) * 255;
  blue = (1.0 * (max_ - corr)) / (2.0 * max_) * 255;
  color = SDL_MapRGB(screen->format, red, blue, 0);
  for (int i = 0; i < line_width; i++) {
    pixel = (uint32_t *)screen->pixels + y * screen->pitch / 4 + x + i;
    *pixel = color;
  }
}


}  // namespace tympani
