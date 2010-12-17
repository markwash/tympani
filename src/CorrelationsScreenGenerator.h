#ifndef SRC_CORRELATIONSSCREENGENERATOR_H_
#define SRC_CORRELATIONSSCREENGENERATOR_H_

#include <stdint.h>

#include <SDL/SDL.h>

namespace tympani {

class CorrelationsScreenGenerator {
 public:
  CorrelationsScreenGenerator(int samples);
  void draw(SDL_Surface *screen, const int *corr);

 private:
  void lock_if_necessary(SDL_Surface *screen);
  void unlock_if_necessary(SDL_Surface *screen);
  void update_max(const int *corr);
  void draw_background(SDL_Surface *screen);
  void draw_sample(SDL_Surface *screen, int sample, int corr);
  int samples_;
  int max_;

};

}  // namespace tympani

#endif  // SRC_CORRELATIONSSCREENGENERATOR_H_
