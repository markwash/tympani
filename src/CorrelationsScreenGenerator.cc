#include "CorrelationsScreenGenerator.h"

#include <stdint.h>
#include <string.h>

#include <stdio.h>

namespace tympani {

CorrelationsScreenGenerator::CorrelationsScreenGenerator(int samples)
    : samples_(samples), max_(1) {} 

void CorrelationsScreenGenerator::draw(SDL_Surface *screen, 
                                       const int *corr) {
  lock_if_necessary(screen);
  draw_background(screen);
  update_max(corr);
  for (int i = 0; i < samples_; i++) {
    draw_sample(screen, i, corr[i]);
  }
  unlock_if_necessary(screen);
  SDL_UpdateRect(screen, 0, 0, 0, 0);
}

void CorrelationsScreenGenerator::update_max(const int *corr) {
  for (int i = 0; i < samples_; i++) {
    if (corr[i] > max_)
      max_ = corr[i];
    else if (-corr[i] > max_)
      max_ = -corr[i];
  }
}

void CorrelationsScreenGenerator::draw_background(SDL_Surface *screen) {
  memset(screen->pixels, 0, screen->h * screen->pitch);
}

void CorrelationsScreenGenerator::lock_if_necessary(SDL_Surface *screen) {
  if (SDL_MUSTLOCK(screen))
    SDL_LockSurface(screen);
}

void CorrelationsScreenGenerator::unlock_if_necessary(SDL_Surface *screen) {
  if (SDL_MUSTLOCK(screen))
    SDL_UnlockSurface(screen);
}

void CorrelationsScreenGenerator::draw_sample(
      SDL_Surface *screen, int sample, int corr) {
  int x, y, line_width;
  int red, blue;
  uint32_t color;
  uint32_t *pixel;
  line_width = screen->w / samples_;
  x = sample * line_width;
  y = screen->h / 2 * (1.0 - 1.0 * corr / (max_ + 1));
  red = (1.0 * (max_ + corr)) / (2.0 * max_) * 255;
  blue = (1.0 * (max_ - corr)) / (2.0 * max_) * 255;
  color = SDL_MapRGB(screen->format, red, 0, blue);
  for (int i = 0; i < line_width; i++) {
    pixel = (uint32_t *)screen->pixels + y * screen->pitch / 4 + x + i;
    //fprintf(stderr, "(%d) %d, %d -> %d, %d\n", max_, sample, corr, x + i, y);
    *pixel = color;
  }
}


}  // namespace tympani
