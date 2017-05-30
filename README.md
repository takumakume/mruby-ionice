# mruby-ionice   [![Build Status](https://travis-ci.org/takumakume/mruby-ionice.svg?branch=master)](https://travis-ci.org/takumakume/mruby-ionice)
Ionice class
## install by mrbgems
- add conf.gem line to `build_config.rb`

```ruby
MRuby::Build.new do |conf|

    # ... (snip) ...

    conf.gem :github => 'takumakume/mruby-ionice'
end
```
## example
```ruby
# for single process or thread
Ionice.set(
  Ionice::IOPRIO_WHO_PROCESS,
  pid,
  Ionice::IOPRIO_CLASS_RT,
  1
)
Ionice.get(
  Ionice::IOPRIO_WHO_PROCESS,
  pid
)
#=> [Ionice::IOPRIO_CLASS_RT, 1]
```

## License
under the MIT License:
- see LICENSE file
