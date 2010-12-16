bin_PROGRAMS = tymp-id tymp-corr tymp-devices

tymp_id_LDFLAGS = -lsndfile
tymp_id_SOURCES = tymp-id.c

tymp_corr_LDFLAGS = -lsndfile -lSDL
tymp_corr_SOURCES = tymp-corr.cc \
	AveragingSlidingWindow.h \
	AveragingSlidingWindow.cc \
	CorrelationsFile.h \
	CorrelationsScreenGenerator.h \
	CorrelationsScreenGenerator.cc \
	SlidingWindow.h \
	SlidingWindow.cc

tymp_devices_LDFLAGS = -lportaudiocpp
tymp_devices_SOURCES = tymp-devices.cc
