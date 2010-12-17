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
  int x, y, line_width, margin;
  int red, blue;
  uint32_t color;
  uint32_t *pixel;
  line_width = screen->w / samples_;
  margin = (screen->w - samples_ * line_width) / 2;
  x = sample * line_width + margin;
  y = screen->h / 2 * (1.0 - 1.0 * corr / (max_ + 1));
  red = (1.0 * (max_ + corr)) / (2.0 * max_) * 255;
  blue = (1.0 * (max_ - corr)) / (2.0 * max_) * 255;
  color = SDL_MapRGB(screen->format, red, 0, blue);

  if (line_width > 1)
    line_width--;

  if (y < screen->h / 2)
    draw_rect(screen, color, x, y, line_width, screen->h/2 - y);
  else if (y > screen->h / 2)
    draw_rect(screen, color, x, screen->h/2, line_width, y - screen->h/2);
  else
    draw_rect(screen, color, x, y, line_width, 1);
}

void CorrelationsScreenGenerator::draw_rect(
    SDL_Surface *screen, uint32_t color, int x, int y, int w, int h) {
  uint32_t *pixel;
  for (int i = x; i < x + w; i++) {
    for (int j = y; j < y + h; j++) {
      pixel = (uint32_t *)screen->pixels + j * screen->pitch / 4 + i;
      *pixel = color;
    }
  }
}


}  // namespace tympani
