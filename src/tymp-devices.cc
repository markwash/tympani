#include <stdio.h>

#include <portaudiocpp/AutoSystem.hxx>
#include <portaudiocpp/System.hxx>
#include <portaudiocpp/Device.hxx>

using portaudio::System;
using portaudio::Device;

int main(int argc, char **argv) {

  portaudio::AutoSystem autoSys;

  System &audio_system = System::instance();

  for (int i = 0; i < audio_system.deviceCount(); i++) {
    Device &device = audio_system.deviceByIndex(i);
    printf("Device %d - %s\n", device.index(), device.name());
    printf("   max input channels: %d\n", device.maxInputChannels());
    printf("   max output channels: %d\n", device.maxOutputChannels());
    printf("\n");
  }

}
