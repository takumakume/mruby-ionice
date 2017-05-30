/*
** mrb_ionice.c - Ionice class
**
** Copyright (c) takumakume 2017
**
** See Copyright Notice in LICENSE
*/

#include "mruby.h"
#include "mruby/data.h"
#include "mrb_ionice.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

typedef struct {
  char *str;
  int len;
} mrb_ionice_data;

static const struct mrb_data_type mrb_ionice_data_type = {
  "mrb_ionice_data", mrb_free,
};

static mrb_value mrb_ionice_hello(mrb_state *mrb, mrb_value self)
{
  mrb_ionice_data *data = DATA_PTR(self);

  return mrb_str_new(mrb, data->str, data->len);
}

static mrb_value mrb_ionice_hi(mrb_state *mrb, mrb_value self)
{
  return mrb_str_new_cstr(mrb, "hi!!");
}

void mrb_mruby_ionice_gem_init(mrb_state *mrb)
{
  struct RClass *ionice;
  ionice = mrb_define_class(mrb, "Ionice", mrb->object_class);
  mrb_define_method(mrb, ionice, "initialize", mrb_ionice_init, MRB_ARGS_REQ(1));
  mrb_define_method(mrb, ionice, "hello", mrb_ionice_hello, MRB_ARGS_NONE());
  mrb_define_class_method(mrb, ionice, "hi", mrb_ionice_hi, MRB_ARGS_NONE());
  DONE;
}

void mrb_mruby_ionice_gem_final(mrb_state *mrb)
{
}

