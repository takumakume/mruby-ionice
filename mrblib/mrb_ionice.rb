class Ionice
  IOPRIO_WHO_PROCESS = 1
  IOPRIO_WHO_PGRP    = 2
  IOPRIO_WHO_USER    = 3

  IOPRIO_CLASS_NONE = 0
  IOPRIO_CLASS_RT   = 1
  IOPRIO_CLASS_BE   = 2
  IOPRIO_CLASS_IDLE = 3

  def self.get(which, who)
    self.ioprio_get(which, who)
  end

  def self.set(which, who, klass, prio=0)
    self.ioprio_set(which, who, klass, prio=0)
  end
end
