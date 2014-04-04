#pragma once

#include <libconfig.h>

typedef struct {
  char *name;
  int frequency_update_hours;
  char *update_cmd;
} upd8_source_t;

typedef struct {
  int num_sources;
  upd8_source_t *sources;
  config_t *cfg;
} upd8_config_t;

void upd8_config_init(upd8_config_t *);

void upd8_config_destroy(upd8_config_t *);

int upd8_config_parse(upd8_config_t *, const char *);

upd8_source_t *upd8_source_indexed(upd8_config_t *, int index);

char *upd8_source_name(upd8_source_t *);

int upd8_source_frequency_hours(upd8_source_t *);

char *upd8_source_update_command(upd8_source_t *);
