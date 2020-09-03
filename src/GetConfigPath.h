
#ifndef GET_CONFIG_PATH_H

#define GET_CONFIG_PATH_H

#include <string>

using namespace std;

#define CONFIG_DIR ".PBot"
#define CONFIG_FILE "PBotConfig.json"
#define BASE_PATH_KEY "base_path"


extern string getConfigPath();
extern void generateConfigFile(string path, string basePath);
extern string getData(string file);
extern string getBasePath();

#endif

