loaded_libraries = Controls::Require::DynamicLibrary.loaded_libraries

assert "Dynamic Library, Load" do
  loaded_libraries.clear

  path = 'test/lib/some_feature.so'

  dynamic_library = Controls::Require::DynamicLibrary.load(path)

  assert_not_nil(dynamic_library)
  assert_equal({ path => dynamic_library }, loaded_libraries)
end

assert "Dynamic Library, Load, File Not Found" do
  loaded_libraries.clear

  dynamic_library = Controls::Require::DynamicLibrary.load('test/lib/no_such_file.so')
  assert_nil(dynamic_library)

  assert_true(loaded_libraries.empty?)
end

assert "Dynamic Library, Load, Already Loaded" do
  loaded_libraries.clear

  path = 'test/lib/some_feature.so'

  Controls::Require::DynamicLibrary.load(path)

  assert_raise_with_message(LoadError, "cannot load such file -- #{path} (already loaded)") do
    Controls::Require::DynamicLibrary.load(path)
  end
end
