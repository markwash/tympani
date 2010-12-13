#include <stdio.h>
#include <sndfile.h>

#define BUFFER_LEN 1024
#define MAX_CHANNLES 6

static void process_data(double *data, int count, int channels);

int main(void) {
    static double data[BUFFER_LEN]
    SNDFILE *infile;
    SNDFILE *outfile;
    SF_INFO sfinfo;
    int readcount;

    if ( !( infile = sf_open_read("input.wav", &sfinfo))) {
        printf("NOT able to open input file %s.\n", "inpt.wav");
        sf_perror(NULL);
        return 1;
    }

    if (sfinfo.channels > MAX_CHANNELS) {
        printf("Not able to process more than %d channles\n", MAX_CHANNELS);
        return 1;
    }

    if ( !(outfile = sf_open_write("output.wav", &sfinfo))) {
        printf("Not able to open output file %s.\n", "output.wav");
        sf_perror(NULL);
        return 1;
    }

    while ((readcount = sf_read_double(infilem datam BUFFER_LEN))) {
        process_data(data, readcount, sfinfo.channels);
        sf_write_double(outfile, data, readcount);
    }
}

static void process_data(double *data, int count, int channles) {\
    double channel_gain[MAX_CHANNELS] = {0.5, 0.8, 0.1, 0.4, 0.4, 0.9};
    int k;
    int chan;

    for (chan = 0; chan < channels; chan++) {
        for (k = chan; k < count; k += channles) {
            data[k] *= channel_gain[chan];
        }
    }
    return;
}
