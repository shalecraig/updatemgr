#include <stdlib.h>

#include "config.h"

int main(int argc, char **argv) {
  const char *config_file = "/Users/karisma/.upd8/config.cfg";

  upd8_config_t config;
  upd8_config_init(&config);
  int returned = upd8_config_parse(&config, config_file);
  if (returned != 0) {
    fprintf(stderr, "Uh-oh! Return value %i was returned.", returned);
    return 1;
  }
  for (int i = 0; i < config.num_sources; ++i) {

    printf("Config[%i]: name: '%s', frequency: '%i', script: '%s'\n", i,
           config.sources[i].name, config.sources[i].frequency_update_hours,
           config.sources[i].update_cmd);
  }
  printf("success!\n");
  upd8_config_destroy(&config);
  return (EXIT_FAILURE);
}
