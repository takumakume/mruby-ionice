assert('IOnice::IOPRIO_CLASS_NONE') do
  assert_equal(0, IOnice::IOPRIO_CLASS_NONE)
  assert_equal(0, IOnice::CLASS_NONE)
end

assert('IOnice::IOPRIO_CLASS_RT') do
  assert_equal(1, IOnice::IOPRIO_CLASS_RT)
  assert_equal(1, IOnice::CLASS_RT)
end

assert('IOnice::IOPRIO_CLASS_BE') do
  assert_equal(2, IOnice::IOPRIO_CLASS_BE)
  assert_equal(2, IOnice::CLASS_BE)
end

assert('IOnice::IOPRIO_CLASS_IDLE') do
  assert_equal(3, IOnice::IOPRIO_CLASS_IDLE)
  assert_equal(3, IOnice::CLASS_IDLE)
end

assert('IOnice::Process.get') do
  expect = {
    "class" => IOnice::CLASS_NONE
  }
  assert_equal(expect, IOnice::Process.get(Process.pid))
end

assert('IOnice::Process.set') do
  assert_true(IOnice::Process.set(Process.pid, IOnice::CLASS_NONE))
  expect = {
    "class" => IOnice::CLASS_NONE
  }
  assert_true(IOnice::Process.set(Process.pid, IOnice::CLASS_IDLE))
  expect = {
    "class" => IOnice::CLASS_IDLE
  }
  assert_equal(expect, IOnice::Process.get(Process.pid))
end

assert('IOnice::ProcessGroup.get') do
  true
end

assert('IOnice::ProcessGroup.set') do
  true
end

assert('IOnice::User.get') do
  true
end

assert('IOnice::User.set') do
  true
end

#assert('IOnice::get') do
#  r = {
#    'class' => IOnice::IOPRIO_CLASS_NONE,
#  }
#  assert_equal(r, IOnice::get(Process.pid))
#  assert_equal(r, IOnice::get(Process.pid, IOnice::IOPRIO_WHO_PROCESS))
#
#  assert_raise(RuntimeError, 'ioprio: invalid target IOPRIO_WHO_* or pid, pgid, uid') do
#    IOnice::get(Process.pid, IOnice::IOPRIO_WHO_PROCESS + 1000)
#  end
#
#  assert_raise(RuntimeError, 'ioprio: no such process') do
#    IOnice::get(12000)
#  end
#end
#
#assert('IOnice::set') do
#  r = {
#    'class' => IOnice::IOPRIO_CLASS_NONE,
#  }
#  assert_true(IOnice::set(Process.pid, IOnice::IOPRIO_CLASS_NONE))
#  assert_equal(r, IOnice::get(Process.pid))
#  assert_true(IOnice::set(Process.pid, IOnice::IOPRIO_CLASS_NONE, 0, IOnice::IOPRIO_WHO_PROCESS))
#  assert_equal(r, IOnice::get(Process.pid))
#
#  r = {
#    'class' => IOnice::IOPRIO_CLASS_IDLE,
#  }
#  assert_true(IOnice::set(Process.pid, IOnice::IOPRIO_CLASS_IDLE))
#  assert_equal(r, IOnice::get(Process.pid))
#
#  r = {
#    'class'    => IOnice::IOPRIO_CLASS_BE,
#    'priority' => 7
#  }
#  assert_true(IOnice::set(Process.pid, IOnice::IOPRIO_CLASS_BE, 7))
#  assert_equal(r, IOnice::get(Process.pid))
#end
