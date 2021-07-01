origin = './test/lib'

assert "Require Relative, Explicit Extension, .rb File" do
  $LOADED_FEATURES.clear

  relative_path = 'some_feature.rb'
  path = Controls::Require::ExpandPath.(relative_path, origin)

  required = Controls::Require.require_relative(relative_path, origin)

  assert_true(required)
  assert_equal(path, $loaded_file)
  assert_equal([path], $LOADED_FEATURES)
end

assert "Require Relative, Explicit Extension, .mrb File" do
  $LOADED_FEATURES.clear

  relative_path = 'some_feature.mrb'
  path = Controls::Require::ExpandPath.(relative_path, origin)

  required = Controls::Require.require_relative(relative_path, origin)

  assert_true(required)

  assert_equal('test/lib/some_feature.rb', $loaded_file)
  assert_equal([path], $LOADED_FEATURES)
end

assert "Require Relative, Explicit Extension, Dynamic Library" do
  $LOADED_FEATURES.clear

  relative_path = 'some_feature.so'
  path = Controls::Require::ExpandPath.(relative_path, origin)

  required = Controls::Require.require_relative(relative_path, origin)

  assert_true(required)

  assert_match('*/test/example-mrbgems/some-feature/mrblib/mrbgem.rb', $loaded_file)
  assert_equal([path], $LOADED_FEATURES)
end

assert "Require Relative, Explicit Extension, __dir__ Value" do
  $dirs = []
  $LOADED_FEATURES.clear

  relative_path = 'some_dir/some_file.rb'
  path = Controls::Require::ExpandPath.(relative_path, origin)

  required = Controls::Require.require_relative(relative_path, origin)

  assert_equal(1, $dirs.size)
  assert_match('*/test/lib/some_dir', $dirs[0])
  assert_nil(__dir__)
end

assert "Require Relative, Explicit Extension, File Not Found" do
  $LOADED_FEATURES.clear

  relative_path = 'some_non_existent_file.rb'
  path = Controls::Require::ExpandPath.(relative_path, origin)

  required = Controls::Require.require_relative(relative_path, origin)

  assert_nil(required)
  assert_equal([], $LOADED_FEATURES)
end

assert "Require Relative, Explicit Extension, File Requires Itself" do
  $LOADED_FEATURES.clear

  relative_path = 'require_relative.rb'
  path = Controls::Require::ExpandPath.(relative_path, origin)

  $other_file = relative_path
  $required = nil

  required_outer = Controls::Require.require_relative(relative_path, origin)

  assert_equal(false, $required)
  assert_true(required_outer)
  assert_equal([path], $LOADED_FEATURES)
end

assert "Require Relative, Explicit Extension, Error Recovery" do
  $LOADED_FEATURES.clear
  Controls::Require.loading_files.clear

  $error = RuntimeError.new('Some Error')

  relative_path = 'raise_error.rb'
  path = Controls::Require::ExpandPath.(relative_path, origin)

  begin
    required_1 = Controls::Require.require_relative(relative_path, origin)
  rescue RuntimeError => error
    $error = false
    required_2 = Controls::Require.require_relative(relative_path, origin)
  end

  assert_nil(required_1)
  assert_true(required_2)

  assert_equal([path], $LOADED_FEATURES)
  assert_equal({}, Controls::Require.loading_files)
end
