#include <stdio.h>
#include <sndfile.h>

int main(int argc, char **argv) {

	SNDFILE *file;
	SF_INFO sfinfo;

	sfinfo.format = 0;
	file = sf_open(argv[1], SFM_READ, &sfinfo);

	printf("frames: %d\n", sfinfo.frames);
	printf("samplerate: %d\n", sfinfo.samplerate);
	printf("channels: %d\n", sfinfo.channels);
	printf("format: %x\n", sfinfo.format);
	printf("sections: %d\n", sfinfo.sections);
	printf("seekable: %d\n", sfinfo.seekable);

}
