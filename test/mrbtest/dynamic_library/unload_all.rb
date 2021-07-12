loaded_libraries = Controls::Require::DynamicLibrary.loaded_libraries

assert "Dynamic Library, Unload All" do
  loaded_libraries.clear

  path_1 = 'test/lib/some_feature.so'
  dynamic_library_1 = Controls::Require::DynamicLibrary.load(path_1)

  path_2 = 'test/lib/some_namespace-other_namespace-some_feature.so'
  dynamic_library_2 = Controls::Require::DynamicLibrary.load(path_2)

  assert_equal(2, loaded_libraries.length)
  assert_equal(dynamic_library_1, loaded_libraries[path_1])
  assert_true(dynamic_library_1.loaded?)
  assert_equal(dynamic_library_2, loaded_libraries[path_2])
  assert_true(dynamic_library_2.loaded?)

  Controls::Require::DynamicLibrary.unload_all

  assert_true(loaded_libraries.empty?)
  assert_equal(false, dynamic_library_1.loaded?)
  assert_equal(false, dynamic_library_2.loaded?)
end
