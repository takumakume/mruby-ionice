# mruby-ionice   [![Build Status](https://travis-ci.org/takumakume/mruby-ionice.svg?branch=master)](https://travis-ci.org/takumakume/mruby-ionice)

This is a binding for Linux system calls `ioprio_set(2)` , `ioprit_get(2)` .
And it is compatible with `ionice(1)` .

## install by mrbgems

- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'takumakume/mruby-ionice'
end
```

## example

#### ioprio classes

from linux/ionice.h

```ruby
IOnice::IOPRIO_CLASS_NONE  # => 0
IOnice::IOPRIO_CLASS_RT    # => 1
IOnice::IOPRIO_CLASS_BE    # => 2
IOnice::IOPRIO_CLASS_IDLE  # => 3
```

#### iopro who

from linux/ionice.h

```ruby
IOnice::IOPRIO_WHO_PROCESS  # => 1
IOnice::IOPRIO_WHO_PGRP     # => 2
IOnice::IOPRIO_WHO_USER     # => 3
```

#### ionice get

```ruby
# for process
IOnice::get(Process.pid)
# => { 'class' => IOnice::IOPRIO_CLASS_NONE }
IOnice::get(Process.pid)
# => { 'class' => IOnice::IOPRIO_CLASS_RT, 'priority' => 2 }
```

- for process

  ```ruby
  IOnice::get(pid)
  IOnice::get(pid, IOnice::IOPRIO_WHO_PROCESS)
  ```

- for process group

  ```ruby
  IOnice::get(pgid, IOnice::IOPRIO_WHO_PGRP)
  ```

- for uid

  ```ruby
  IOnice::get(uid, IOnice::IOPRIO_WHO_USER)
  ```

#### ionice set

```ruby
# for process
IOnice::set(Process.pid, IOnice::IOPRIO_CLASS_NONE)
IOnice::set(Process.pid, IOnice::IOPRIO_CLASS_IDLE)
IOnice::set(Process.pid, IOnice::IOPRIO_CLASS_BE, 7) # priority is 0..7
IOnice::set(Process.pid, IOnice::IOPRIO_CLASS_RT, 2) # priority is 0..7
# => true
```

- for process

  ```ruby
  IOnice::set(pid, IOnice::IOPRIO_CLASS_BE, 2)
  IOnice::set(pid, IOnice::IOPRIO_CLASS_BE, 2, IOnice::IOPRIO_WHO_PROCESS)
  ```

- for process group

  ```ruby
  IOnice::set(pgid, IOnice::IOPRIO_CLASS_BE, 2, IOnice::IOPRIO_WHO_PGRP)
  ```

- for uid

  ```ruby
  IOnice::set(uid, IOnice::IOPRIO_CLASS_BE, 2, IOnice::IOPRIO_WHO_USER)
  ```

## License

under the MIT License:
- see LICENSE file
