assert "Require" do
  control_path = Controls::Require::ExpandPath.('test/lib/some_feature.so')

  $LOADED_FEATURES.clear
  $LOAD_PATH.replace(['test/lib'])

  required = require 'some_feature'

  assert_true(required)
  assert_equal([control_path], $LOADED_FEATURES)

  Controls::Require::DynamicLibrary.unload_all
end

assert "Require, Explicit Extension" do
  control_path = Controls::Require::ExpandPath.('test/lib/some_feature.rb')

  $LOADED_FEATURES.clear
  $LOAD_PATH.replace(['test/lib'])

  required = require 'some_feature.rb'

  assert_true(required)
  assert_equal([control_path], $LOADED_FEATURES)
end

assert "Require, Dynamic Library With Namespacing" do
  control_path = Controls::Require::ExpandPath.('test/lib/some_namespace-other_namespace-some_feature.so')

  $LOADED_FEATURES.clear
  $LOAD_PATH.replace(['test/lib'])

  required = require 'some_namespace/other_namespace/some_feature'

  assert_true(required)
  assert_equal([control_path], $LOADED_FEATURES)

  Controls::Require::DynamicLibrary.unload_all
end

assert "Require, Dynamic Library With Namespacing, Alternate Directory Separator" do
  control_path = Controls::Require::ExpandPath.('test/lib/some_namespace-other_namespace-some_feature.so')

  $LOADED_FEATURES.clear
  $LOAD_PATH.replace(['test/lib'])

  required = require 'some_namespace\other_namespace\some_feature'

  assert_true(required)
  assert_equal([control_path], $LOADED_FEATURES)

  Controls::Require::DynamicLibrary.unload_all
end

assert "Require, Dynamic Library With Namespacing, Explicit Extension" do
  $LOADED_FEATURES.clear
  $LOAD_PATH.replace(['test/lib'])

  feature = 'some_namespace/other_namespace/some_feature.so'

  assert_raise_with_message(LoadError, "cannot load such file -- #{feature}") do
    require feature
  end
end

assert "Require, Already Required" do
  control_path = Controls::Require::ExpandPath.('test/lib/some_feature.rb')

  $LOADED_FEATURES.replace([control_path])
  $LOAD_PATH.replace(['test/lib'])

  required = require 'some_feature.rb'

  assert_equal(false, required)
  assert_equal([control_path], $LOADED_FEATURES)
end

assert "Require, File Not Found" do
  $LOADED_FEATURES.clear
  $LOAD_PATH.replace(['test/lib'])

  assert_raise_with_message(LoadError, "cannot load such file -- no_such_file") do
    require 'no_such_file'
  end
end

assert "Require, Not Found In First Load Path" do
  control_path = Controls::Require::ExpandPath.('test/lib/some_dir/some_file.rb')

  $LOADED_FEATURES.clear
  $LOAD_PATH.replace(['test/lib', 'test/lib/some_dir'])

  required = require 'some_file'

  assert_true(required)
  assert_equal([control_path], $LOADED_FEATURES)
end

assert "Require, File Outside Of Load Path" do
  $LOADED_FEATURES.clear
  $LOAD_PATH.replace(['test/lib/some_dir'])

  assert_raise_with_message(LoadError, "cannot load such file -- ../some_feature") do
    require '../some_feature'
  end
end
