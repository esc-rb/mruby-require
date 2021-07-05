relative_path = 'test/lib/some_feature.rb'
path = Controls::Require::ExpandPath.(relative_path)

assert "Require Relative, Loaded Features, Not Loaded" do
  $LOADED_FEATURES.clear

  required = Controls::Require.require_relative(relative_path, '.')
  assert_true(required)

  assert_equal([path], $LOADED_FEATURES)
end

assert "Require Relative, Loaded Features, Loaded" do
  $LOADED_FEATURES.replace([path])

  required = Controls::Require.require_relative(relative_path, '.')
  assert_equal(false, required)

  assert_equal([path], $LOADED_FEATURES)
end
