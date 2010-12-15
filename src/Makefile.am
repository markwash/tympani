bin_PROGRAMS = tymp-id tymp-corr

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
