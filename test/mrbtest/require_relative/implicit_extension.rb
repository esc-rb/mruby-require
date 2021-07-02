origin = './test/lib'

assert "Require Relative, Implicit Extension" do
  Controls::Require::DynamicLibrary.unload_all

  $LOADED_FEATURES.clear

  feature = 'some_feature'
  path = Controls::Require::ExpandPath.("#{feature}.so", origin)

  required = Controls::Require.require_relative(feature, origin)

  assert_true(required)
  assert_equal([path], $LOADED_FEATURES)
end

assert "Require Relative, Implicit Extension, Path Contains a Period (.)" do
  $LOADED_FEATURES.clear

  feature = '.some_hidden_dir/some_file'
  path = Controls::Require::ExpandPath.("#{feature}.rb", origin)

  required = Controls::Require.require_relative(feature, origin)

  assert_true(required)
  assert_equal([path], $LOADED_FEATURES)
end

assert "Require Relative, Implicit Extension, .so, .mrb and .rb Files Present" do
  Controls::Require::DynamicLibrary.unload_all

  $LOADED_FEATURES.clear

  feature = 'some_feature'
  so_path = Controls::Require::ExpandPath.("#{feature}.so", origin)

  required = Controls::Require.require_relative(feature, origin)

  assert_true(required)
  assert_equal([so_path], $LOADED_FEATURES)
end

assert "Require Relative, Implicit Extension, .so, .mrb and .rb Files Present, .rb File Already Loaded" do
  Controls::Require::DynamicLibrary.unload_all

  feature = 'some_feature'
  so_path = Controls::Require::ExpandPath.("#{feature}.so", origin)
  rb_path = Controls::Require::ExpandPath.("#{feature}.rb", origin)

  $LOADED_FEATURES.replace([rb_path])

  required = Controls::Require.require_relative(feature, origin)

  assert_true(required)
  assert_equal([rb_path, so_path], $LOADED_FEATURES)
end

assert "Require Relative, Implicit Extension, .so, .mrb and .rb Files Present, .mrb File Already Loaded" do
  Controls::Require::DynamicLibrary.unload_all

  feature = 'some_feature'
  so_path = Controls::Require::ExpandPath.("#{feature}.so", origin)
  mrb_path = Controls::Require::ExpandPath.("#{feature}.mrb", origin)

  $LOADED_FEATURES.replace([mrb_path])

  required = Controls::Require.require_relative(feature, origin)

  assert_true(required)
  assert_equal([mrb_path, so_path], $LOADED_FEATURES)
end

assert "Require Relative, Implicit Extension, .so, .mrb and .rb Files Present, .so File Already Loaded" do
  feature = 'some_feature'
  so_path = Controls::Require::ExpandPath.("#{feature}.so", origin)

  $LOADED_FEATURES.replace([so_path])

  required = Controls::Require.require_relative(feature, origin)

  assert_equal(false, required)
  assert_equal([so_path], $LOADED_FEATURES)
end
