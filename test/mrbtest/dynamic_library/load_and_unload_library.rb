assert "Dynamic Library, Load And Unload Library" do
  dynamic_library = Controls::Require::DynamicLibrary.new('test/lib/some_feature.so')

  assert_equal(false, dynamic_library.loaded?)

  $some_feature_native_initialized = false
  $some_feature_mrblib_loaded = false
  loaded = dynamic_library.load
  assert_true(loaded)

  assert_true($some_feature_native_initialized)
  assert_true($some_feature_mrblib_loaded)
  assert_true(dynamic_library.loaded?)

  $some_feature_native_finalized = false
  dynamic_library.unload

  assert_true($some_feature_native_finalized)
  assert_equal(false, dynamic_library.loaded?)
end

assert "Dynamic Library, Load And Unload Library, Namespacing" do
  dynamic_library = Controls::Require::DynamicLibrary.new('test/lib/some_namespace-other_namespace-some_feature.so')

  loaded = nil

  $some_namespace_other_namespace_some_feature_loaded = false

  loaded = dynamic_library.load
  assert_true(loaded)

  dynamic_library.unload

  assert_true($some_namespace_other_namespace_some_feature_loaded)
  assert_equal(false, dynamic_library.loaded?)
end

assert "Dynamic Library, Load And Unload Library, MRuby Error" do
  dynamic_library = Controls::Require::DynamicLibrary.new('test/lib/some_feature.so')

  $error = RuntimeError.new('Some Error')

  begin
    dynamic_library.load
  rescue RuntimeError => raised_error
  end

  assert_not_nil(raised_error)
  assert_same($error, raised_error)

  assert_equal(false, dynamic_library.loaded?)
end

assert "Dynamic Library, Load And Unload Library, File Not Found" do
  path = 'test/lib/no_such_file.so'

  dynamic_library = Controls::Require::DynamicLibrary.new(path)

  loaded = dynamic_library.load

  assert_equal(false, loaded)
  assert_equal(false, dynamic_library.loaded?)
end

assert "Dynamic Library, Load And Unload Library, Not An MRuby Gem" do
  path = 'test/lib/not_an_mrbgem.so'

  dynamic_library = Controls::Require::DynamicLibrary.new(path)

  assert_raise_with_message(LoadError, "cannot load such file -- #{path} (missing gem init function: GENERATED_TMP_mrb_not_an_mrbgem_gem_init)") do
    dynamic_library.load
  end

  assert_equal(false, dynamic_library.loaded?)
end

assert "Dynamic Library, Load And Unload Library, Not A Dynamic Library" do
  path = 'test/lib/not_a_dynamic_library.so'

  dynamic_library = Controls::Require::DynamicLibrary.new(path)

  assert_raise_with_message(LoadError, "cannot load such file -- #{path} (#{path}: file too short)") do
    dynamic_library.load
  end

  assert_equal(false, dynamic_library.loaded?)
end
