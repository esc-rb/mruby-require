assert "Dynamic Library, Inspect Path" do
  path = 'test/lib/some_feature.so'

  dynamic_library = Controls::Require::DynamicLibrary.new(path)

  assert_equal(path, dynamic_library.path)
end

assert "Dynamic Library, Inspect Init Function Name" do
  path = 'test/lib/some_feature.so'

  dynamic_library = Controls::Require::DynamicLibrary.new(path)

  assert_equal('GENERATED_TMP_mrb_some_feature_gem_init', dynamic_library.init_function_name)
end

assert "Dynamic Library, Inspect Init Function Name, Namespacing" do
  path = 'test/lib/some_namespace-other_namespace-some_feature.so'

  dynamic_library = Controls::Require::DynamicLibrary.new(path)

  assert_equal('GENERATED_TMP_mrb_some_namespace_other_namespace_some_feature_gem_init', dynamic_library.init_function_name)
end

assert "Dynamic Library, Inspect Final Function Name" do
  path = 'test/lib/some_feature.so'

  dynamic_library = Controls::Require::DynamicLibrary.new(path)

  assert_equal('GENERATED_TMP_mrb_some_feature_gem_final', dynamic_library.final_function_name)
end

assert "Dynamic Library, Inspect Final Function Name, Namespacing" do
  path = 'test/lib/some_namespace-other_namespace-some_feature.so'

  dynamic_library = Controls::Require::DynamicLibrary.new(path)

  assert_equal('GENERATED_TMP_mrb_some_namespace_other_namespace_some_feature_gem_final', dynamic_library.final_function_name)
end

assert "Dynamic Library, Inspect" do
  control_path = 'test/lib/some_feature.so'

  dynamic_library = Controls::Require::DynamicLibrary.new(control_path)

  path = dynamic_library.path.inspect
  init_function_name = dynamic_library.init_function_name.inspect
  final_function_name = dynamic_library.final_function_name.inspect
  object_id = dynamic_library.object_id.to_s(16)

  assert_equal(<<~INSPECT.chomp, dynamic_library.inspect)
  #<#{dynamic_library.class}:0x#{object_id} path=#{path}, init_function_name=#{init_function_name}, final_function_name=#{final_function_name}>
  INSPECT
end
