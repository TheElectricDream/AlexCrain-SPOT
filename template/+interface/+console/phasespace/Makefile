# Builds StreamData and the PhaseSpace client library (libowlsock.so, compiled from the
# PhaseSpace source in libowl-src-410) for whatever Linux machine this runs on: x86 laptop,
# WSL on Windows, or ARM boards such as the Jetson TX2. Needs only g++ and make.
#
#   make                      -> ../output/linux-<arch>/StreamData + libowlsock.so (the console's folder)
#   make OUT=/some/folder     build into another folder (the webapp does this)
#   make clean                remove the build folder and the output folder's files
#
# Everything is built in ~/.cache (outside Dropbox, and free of spaces, which make cannot handle
# in file names) so rebuilds only recompile what changed; the result is then copied to OUT.

ARCH  := $(shell uname -m)
OUT   ?= ../output/linux-$(ARCH)
BUILD ?= $(HOME)/.cache/streamdata-build/$(ARCH)
OWL   := libowl-src-410

CXX      ?= g++
LIBFLAGS := -fPIC -O3 -funroll-loops -Wall -Wno-terminate -MMD -MP
APPFLAGS := -std=c++11 -O2 -Wall -MMD -MP -pthread

LIB_SRC := owl.cc libowl.cc owl_rpd.cc libowl_socket.cc socket.cc
LIB_OBJ := $(addprefix $(BUILD)/owl/,$(LIB_SRC:.cc=.o))
APP_OBJ := $(BUILD)/StreamData.o
LIB     := $(BUILD)/libowlsock.so
APP     := $(BUILD)/StreamData

.PHONY: all clean
all: $(APP) $(LIB)
	@mkdir -p "$(OUT)"
	cp -f $(APP) $(LIB) "$(OUT)/"

$(LIB): $(LIB_OBJ)
	$(CXX) -shared $^ -lrt -lpthread -o $@

# StreamData finds libowlsock.so in its own folder ($ORIGIN), wherever that folder is moved
$(APP): $(APP_OBJ) $(LIB)
	$(CXX) $(APP_OBJ) -pthread -L$(BUILD) -lowlsock -lpthread -Wl,-rpath,'$$ORIGIN' -o $@

$(BUILD)/owl/libowl.o $(BUILD)/owl/libowl_socket.o: LIBFLAGS += -DLIBOWL_REV=410

$(BUILD)/owl/%.o: $(OWL)/%.cc
	@mkdir -p $(@D)
	$(CXX) $(LIBFLAGS) -c $< -o $@

$(BUILD)/StreamData.o: StreamData.cc
	@mkdir -p $(@D)
	$(CXX) $(APPFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD) "$(OUT)/StreamData" "$(OUT)/libowlsock.so"

-include $(LIB_OBJ:.o=.d) $(APP_OBJ:.o=.d)
