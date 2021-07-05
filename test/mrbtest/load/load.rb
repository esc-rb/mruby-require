assert "Load, .rb File" do
  $loaded_file = nil

  path = 'test/lib/some_feature.rb'

  return_value = Controls::Require.load(path)

  assert_true(return_value)
  assert_equal(path, $loaded_file)
end

assert "Load, .mrb File" do
  $loaded_file = nil

  return_value = Controls::Require.load('test/lib/some_feature.mrb')

  assert_true(return_value)
  assert_equal('test/lib/some_feature.rb', $loaded_file)
end

assert "Load, Dynamic Library File" do
  $loaded_file = nil

  return_value = Controls::Require.load('test/lib/some_feature.so')

  assert_true(return_value)
  assert_match('*/test/example-mrbgems/some-feature/mrblib/mrbgem.rb', $loaded_file)
end

assert "Load, File Not Found, .rb File" do
  return_value = Controls::Require.load('no_such_file.rb')

  assert_equal(false, return_value)
end

assert "Load, File Not Found, .mrb File" do
  return_value = Controls::Require.load('no_such_file.mrb')

  assert_equal(false, return_value)
end

assert "Load, File Not Found, Dynamic Library File" do
  return_value = Controls::Require.load('no_such_file.so')

  assert_equal(false, return_value)
end

assert "Load, Error" do
  $error = RuntimeError.new('Some Error')

  path = 'test/lib/raise_error.rb'

  return_value = Controls::Require.load(path)

  assert_kind_of(Exception, return_value)
  assert_same($error, return_value)
end

assert "Load, __dir__ Value" do
  $dirs = []

  path = 'test/lib/some_dir/some_file.rb'

  Controls::Require.load(path)

  assert_equal(1, $dirs.size)
  assert_match('*/test/lib/some_dir', $dirs[0])
  assert_nil(__dir__)
end

assert "Load, __dir__ Value, Nesting" do
  $dirs = []

  path = 'test/lib/load_other_file.rb'

  Controls::Require.load(path)

  assert_equal(3, $dirs.size)
  assert_match('*/test/lib', $dirs[0])
  assert_match('*/test/lib/some_dir', $dirs[1])
  assert_match('*/test/lib', $dirs[2])
  assert_nil(__dir__)
end

assert "Load, __dir__ Value, Error Recovery" do
  $dirs = []
  $error = RuntimeError.new('Some Error')

  path = 'test/lib/raise_error.rb'

  exception = Controls::Require.load(path)

  assert_same($error, exception)

  assert_equal(1, $dirs.size)
  assert_match('*/test/lib', $dirs[0])
  assert_nil(__dir__)
end

assert "Load, __dir__ Value, Error Recovery, Nesting" do
  $dirs = []

  $error = RuntimeError.new('Some Error')
  $other_file = 'test/lib/raise_error.rb'
  $raised_error = nil

  path = 'test/lib/load_other_file.rb'

  Controls::Require.load(path)

  assert_not_nil($raised_error)
  assert_same($error, $raised_error)

  assert_equal(3, $dirs.size)
  assert_match('*/test/lib', $dirs[0])
  assert_match('*/test/lib', $dirs[1])
  assert_match('*/test/lib', $dirs[2])
  assert_nil(__dir__)
end
