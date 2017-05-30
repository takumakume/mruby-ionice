/*
** mrb_ionice.c - Ionice class
**
** Copyright (c) takumakume 2017
**
** See Copyright Notice in LICENSE
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <strings.h>
#include <sys/syscall.h>

#include "mruby.h"
#include "mruby/hash.h"
#include "mruby/data.h"
#include "mruby/value.h"
#include "ionice.h"

#define DONE mrb_gc_arena_restore(mrb, 0);

static inline int ioprio_set(int which, int who, int ioprio)
{
  return syscall(SYS_ioprio_set, which, who, ioprio);
}

static inline int ioprio_get(int which, int who)
{
  return syscall(SYS_ioprio_get, which, who);
}

void ioprio_error_handler(mrb_state *mrb, int error)
{
  switch (error) {
    case EINVAL:
      mrb_raise(mrb, E_RUNTIME_ERROR, "ioprio: invalid target IOPRIO_WHO_* or pid, pgid, uid");
    case EPERM:
      mrb_raise(mrb, E_RUNTIME_ERROR, "ioprio: peration not permitted");
    case ESRCH:
      mrb_raise(mrb, E_RUNTIME_ERROR, "ioprio: no such process");
    default:
      mrb_raise(mrb, E_RUNTIME_ERROR, "ioprio: unknown error");
  }
}

static mrb_value mrb_ioprio_set(mrb_state *mrb, mrb_value self)
{
  mrb_int id;
  mrb_int ioclass;
  mrb_int data = -1;
  mrb_int who = IOPRIO_WHO_PROCESS;

  mrb_get_args(mrb, "ii|ii", &id, &ioclass, &data, &who);
  switch (ioclass) {
    case IOPRIO_CLASS_NONE:
      data = 0;
      break;
    case IOPRIO_CLASS_IDLE:
      data = 7;
      break;
    case IOPRIO_CLASS_RT:
    case IOPRIO_CLASS_BE:
      if (data < 0 || data > 7) {
        mrb_raise(mrb, E_RUNTIME_ERROR, "priority can only be specified from 0..7");
      }
      break;
    default:
      mrb_raise(mrb, E_RUNTIME_ERROR, "unknown ioclass");
  }

  if (ioprio_set(who, id, IOPRIO_PRIO_VALUE(ioclass, data)) == -1) {
    ioprio_error_handler(mrb, errno);
  }

  return mrb_true_value();
}

static mrb_value mrb_ioprio_get(mrb_state *mrb, mrb_value self)
{
  int ioprio;
  int ioclass;
  mrb_int id;
  mrb_int who = IOPRIO_WHO_PROCESS;
  mrb_value result;

  mrb_get_args(mrb, "i|i", &id, &who);
  ioprio = ioprio_get(who, id);

  if (ioprio == -1) {
    ioprio_error_handler(mrb, errno);
  }

  ioclass = IOPRIO_PRIO_CLASS(ioprio);

  result = mrb_hash_new(mrb);
  mrb_hash_set(mrb, result, mrb_str_new_cstr(mrb, "class"), mrb_fixnum_value(ioclass));
  if (ioclass == IOPRIO_CLASS_RT || ioclass == IOPRIO_CLASS_BE) {
    mrb_hash_set(mrb, result, mrb_str_new_cstr(mrb, "priority"), mrb_fixnum_value(IOPRIO_PRIO_DATA(ioprio)));
  }
  return result;
}

void mrb_mruby_ionice_gem_init(mrb_state *mrb)
{
  struct RClass *ionice;

  ionice = mrb_define_module(mrb, "IOnice");
  mrb_define_module_function(mrb, ionice, "set", mrb_ioprio_set, MRB_ARGS_ANY());
  mrb_define_module_function(mrb, ionice, "get", mrb_ioprio_get, MRB_ARGS_ANY());
  // ioprio class const
  mrb_define_const(mrb, ionice, "IOPRIO_CLASS_NONE",  mrb_fixnum_value(IOPRIO_CLASS_NONE));
  mrb_define_const(mrb, ionice, "IOPRIO_CLASS_RT",    mrb_fixnum_value(IOPRIO_CLASS_RT));
  mrb_define_const(mrb, ionice, "IOPRIO_CLASS_BE",    mrb_fixnum_value(IOPRIO_CLASS_BE));
  mrb_define_const(mrb, ionice, "IOPRIO_CLASS_IDLE",  mrb_fixnum_value(IOPRIO_CLASS_IDLE));
  // ioprio who const
  mrb_define_const(mrb, ionice, "IOPRIO_WHO_PROCESS", mrb_fixnum_value(IOPRIO_WHO_PROCESS));
  mrb_define_const(mrb, ionice, "IOPRIO_WHO_PGRP",    mrb_fixnum_value(IOPRIO_WHO_PGRP));
  mrb_define_const(mrb, ionice, "IOPRIO_WHO_USER",    mrb_fixnum_value(IOPRIO_WHO_USER));

  DONE;
}

void mrb_mruby_ionice_gem_final(mrb_state *mrb)
{
}
