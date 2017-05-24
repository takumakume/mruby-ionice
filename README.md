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
p Ionice.hi
#=> "hi!!"
t = Ionice.new "hello"
p t.hello
#=> "hello"
p t.bye
#=> "hello bye"
```

## License
under the MIT License:
- see LICENSE file