assert('IOnice::IOPRIO_WHO_PROCESS') do
  assert_equal(1, IOnice::IOPRIO_WHO_PROCESS)
end

assert('IOnice::IOPRIO_WHO_PGRP') do
  assert_equal(2, IOnice::IOPRIO_WHO_PGRP)
end

assert('IOnice::IOPRIO_WHO_USER') do
  assert_equal(3, IOnice::IOPRIO_WHO_USER)
end

assert('IOnice::IOPRIO_CLASS_NONE') do
  assert_equal(0, IOnice::IOPRIO_CLASS_NONE)
end

assert('IOnice::IOPRIO_CLASS_RT') do
  assert_equal(1, IOnice::IOPRIO_CLASS_RT)
end

assert('IOnice::IOPRIO_CLASS_BE') do
  assert_equal(2, IOnice::IOPRIO_CLASS_BE)
end

assert('IOnice::IOPRIO_CLASS_IDLE') do
  assert_equal(3, IOnice::IOPRIO_CLASS_IDLE)
end

#assert('IOnice#set') do
#end

#assert('IOnice#get') do
#end
