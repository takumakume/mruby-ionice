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

static inline int _sys_ioprio_set(int which, int who, int ioprio)
{
  return syscall(SYS_ioprio_set, which, who, ioprio);
}

static inline int _sys_ioprio_get(int which, int who)
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

static mrb_value sys_ioprio_set(mrb_state *mrb, int which, int who, int ioclass, int data)
{
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

  if (_sys_ioprio_set(which, who, IOPRIO_PRIO_VALUE(ioclass, data)) == -1) {
    ioprio_error_handler(mrb, errno);
    return mrb_false_value();
  }

  return mrb_true_value();
}

static mrb_value sys_ioprio_get(mrb_state *mrb, int which, int who)
{
  int ioprio;
  int ioclass;
  mrb_value result;

  ioprio = _sys_ioprio_get(which, who);

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

static mrb_value mrb_ioprio_get_process(mrb_state *mrb, mrb_value self)
{
  mrb_value pid;
  mrb_get_args(mrb, "i", &pid);
  return sys_ioprio_get(mrb, IOPRIO_WHO_PROCESS, mrb_fixnum_p(pid));
}

static mrb_value mrb_ioprio_set_process(mrb_state *mrb, mrb_value self)
{
  mrb_value pid;
  mrb_value ioclass;
  mrb_value data;

  mrb_get_args(mrb, "ii|i", &pid, &ioclass, &data);
  if (mrb_nil_p(data)) {
    data = mrb_fixnum_value(0);
  }
  return sys_ioprio_set(mrb, IOPRIO_WHO_PROCESS, mrb_fixnum_p(pid), mrb_fixnum_p(ioclass), mrb_fixnum_p(data));
}

/*
static mrb_value mrb_ioprio_get_process_group(mrb_state *mrb, mrb_value self){
  mrb_int pgid;
  mrb_get_args(mrb, "i", &pgid);
  return sys_ioprio_get(mrb, IOPRIO_WHO_PGRP, pgid);
}

static mrb_value mrb_ioprio_set_process_group(mrb_state *mrb, mrb_value self){
  mrb_int pgid;
  mrb_int priority;
  mrb_get_args(mrb, "ii", &pgid, &priority);
  return sys_ioprio_set(mrb, IOPRIO_WHO_PGRP, pgid, priority);
}

static mrb_value mrb_ioprio_get_user(mrb_state *mrb, mrb_value self){
  mrb_int uid;
  mrb_get_args(mrb, "i", &uid);
  return sys_ioprio_get(mrb, IOPRIO_WHO_USER, uid);
}

static mrb_value mrb_ioprio_set_user(mrb_state *mrb, mrb_value self){
  mrb_int uid;
  mrb_int priority;
  mrb_get_args(mrb, "ii", &uid, &priority);
  return sys_ioprio_set(mrb, IOPRIO_WHO_USER, uid, priority);
}
*/

void mrb_mruby_ionice_gem_init(mrb_state *mrb)
{
  struct RClass *ionice, *ionice_process, *ionice_process_group, *ionice_user;

  ionice               = mrb_define_class(mrb, "IOnice", mrb->object_class);
  ionice_process       = mrb_define_class_under(mrb, ionice, "Process",      mrb->object_class);
  ionice_process_group = mrb_define_class_under(mrb, ionice, "ProcessGroup", mrb->object_class);
  ionice_user          = mrb_define_class_under(mrb, ionice, "User",         mrb->object_class);

  // IOnice::Process
  mrb_define_class_method(mrb, ionice_process, "get", mrb_ioprio_get_process, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, ionice_process, "set", mrb_ioprio_set_process, MRB_ARGS_REQ(2));
  /*
  // IOnice::ProcessGroup
  mrb_define_class_method(mrb, ionice_process_group, "get", mrb_ioprio_get_process_group, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, ionice_process_group, "set", mrb_ioprio_set_process_group, MRB_ARGS_REQ(2));
  // IOnice::User
  mrb_define_class_method(mrb, ionice_user, "get", mrb_ioprio_get_user, MRB_ARGS_REQ(1));
  mrb_define_class_method(mrb, ionice_user, "set", mrb_ioprio_set_user, MRB_ARGS_REQ(2));
  */
  // ioprio class const
  mrb_define_const(mrb, ionice, "IOPRIO_CLASS_NONE",  mrb_fixnum_value(IOPRIO_CLASS_NONE));
  mrb_define_const(mrb, ionice, "CLASS_NONE",         mrb_fixnum_value(IOPRIO_CLASS_NONE));
  mrb_define_const(mrb, ionice, "IOPRIO_CLASS_RT",    mrb_fixnum_value(IOPRIO_CLASS_RT));
  mrb_define_const(mrb, ionice, "CLASS_RT",           mrb_fixnum_value(IOPRIO_CLASS_RT));
  mrb_define_const(mrb, ionice, "IOPRIO_CLASS_BE",    mrb_fixnum_value(IOPRIO_CLASS_BE));
  mrb_define_const(mrb, ionice, "CLASS_BE",           mrb_fixnum_value(IOPRIO_CLASS_BE));
  mrb_define_const(mrb, ionice, "IOPRIO_CLASS_IDLE",  mrb_fixnum_value(IOPRIO_CLASS_IDLE));
  mrb_define_const(mrb, ionice, "CLASS_IDLE",         mrb_fixnum_value(IOPRIO_CLASS_IDLE));

  DONE;
}

void mrb_mruby_ionice_gem_final(mrb_state *mrb)
{
}
