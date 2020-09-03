
GXX=g++
FLAGS= -g -std=c++11
LIBS= -lboost_thread -lpthread -lboost_system -lboost_filesystem

ifeq ($(PREFIX),)
	PREFIX := /usr/local/
endif
all: PBotManager GetPBotBasePath PBotClient


PBotManager : src/PBotManager.cpp src/GetConfigPath.cpp
	$(GXX) $(FLAGS) -I . $^ -o bin/$@ $(LIBS)

PBotClient : src/PBotClientAsync.cpp
	$(GXX) $(FLAGS) -I . $^ -o bin/$@ $(LIBS)

GetPBotBasePath : src/GetBasePath.cpp src/GetConfigPath.cpp
	$(GXX) $(FLAGS) -I . $^ -o bin/$@  $(LIBS)


install: bin/PBotManager bin/GetPBotBasePath bin/PBotClient
	install -d $(DESTDIR)$(PREFIX)/
	install bin/* $(DESTDIR)$(PREFIX)/bin/
	install -d ~/.mozilla/native-messaging-hosts
	install interactor/app_manifest.json ~/.mozilla/native-messaging-hosts/pbot.json
	install src/AddScriptPBot $(DESDIR)$(PREFIX)/bin/

