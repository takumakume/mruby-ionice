##
## Ionice Test
##

assert("Ionice#hello") do
  t = Ionice.new "hello"
  assert_equal("hello", t.hello)
end

assert("Ionice#bye") do
  t = Ionice.new "hello"
  assert_equal("hello bye", t.bye)
end

assert("Ionice.hi") do
  assert_equal("hi!!", Ionice.hi)
end
