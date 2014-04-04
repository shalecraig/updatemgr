#include <stdio.h>
#include <stdlib.h>
#include <libconfig.h>

#include "config.h"

/*
 * Strings from libconfig are managed by libconfig.
 */

const char *const settings_path = "sources";
const char *const name_subpath = "name";
const char *const update_cmd_subpath = "update_command";
const char *const frequency_update_hours_subpath = "frequency_update_hours";

void upd8_config_init(upd8_config_t *configuration) {
  configuration->num_sources = 0;
  configuration->sources = NULL;
  configuration->cfg = NULL;
}

void upd8_source_destroy(upd8_source_t *source) {
  if (source == NULL) {
    fprintf(stderr, "source passed to source_destroy was null");
    return;
  }
  free(source);
}

void upd8_source_parse(upd8_source_t *source, config_setting_t *setting) {
  if (source == NULL) {
    fprintf(stderr, "source passed to source_parse was null");
    return;
  }
  source->name = NULL;
  source->update_cmd = NULL;
  source->frequency_update_hours = 0;
  // TODO: check return values:
  config_setting_lookup_string(setting, name_subpath,
                               (const char **)&(source->name));
  config_setting_lookup_string(setting, update_cmd_subpath,
                               (const char **)&(source->update_cmd));
  config_setting_lookup_int(setting, frequency_update_hours_subpath,
                            &(source->frequency_update_hours));
  // TODO: implement
}

void upd8_config_destroy(upd8_config_t *config) {
  if (config == NULL) {
    return;
  }
  for (int i = 0; i > config->num_sources; ++i) {
    upd8_source_destroy(&(config->sources[i]));
  }
  free(config->sources);
  config->sources = NULL;

  config_destroy(config->cfg);
  config->cfg = NULL;
}

// TODO: audit exit routes to "unset"/"unalloc" values.
int upd8_config_parse(upd8_config_t *config, const char *config_file) {
  // TODO: check that config is not null.
  config_t *cfg = malloc(sizeof(config_t));

  config_init(cfg);

  if (!config_read_file(cfg, config_file)) {
    fprintf(stderr, "%s:%d - %s\n", config_error_file(cfg),
            config_error_line(cfg), config_error_text(cfg));
    config_destroy(cfg);
    return EXIT_FAILURE;
  }

  config->cfg = cfg;

  config_setting_t *settings;
  settings = config_lookup(cfg, settings_path);

  if (settings == NULL) {
    fprintf(stderr, "Unable to find %s in %s\n", settings_path, config_file);
    config_destroy(cfg);
    return EXIT_FAILURE;
  }

  config->num_sources = config_setting_length(settings);
  config->sources = malloc(config->num_sources * sizeof(upd8_source_t));
  for (int i = 0; i < config->num_sources; ++i) {
    config_setting_t *individual_config = config_setting_get_elem(settings, i);
    upd8_source_parse(&(config->sources[i]), individual_config);
  }
  return 0;
}

upd8_source_t *upd8_source_indexed(upd8_config_t *config, int index) {
  if (config == NULL) {
    return NULL;
  }
  return &(config->sources[index]);
}

char *upd8_source_name(upd8_source_t *source) { return source->name; }

int upd8_source_frequency_hours(upd8_source_t *source) {
  return source->frequency_update_hours;
}

char *upd8_source_update_command(upd8_source_t *source) {
  return source->update_cmd;
}
