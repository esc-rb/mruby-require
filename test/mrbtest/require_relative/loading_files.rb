relative_path = 'test/lib/some_feature.rb'
path = Controls::Require::ExpandPath.(relative_path)

assert "Require Relative, Loading Files, File Isn't Being Loaded" do
  Controls::Require.loading_files.clear

  required = Controls::Require.require_relative(relative_path, '.')
  assert_true(required)

  assert_equal({}, Controls::Require.loading_files)
end

assert "Require Relative, Loading Files, File Is Being Loaded" do
  Controls::Require.loading_files.replace({ path => true })

  required = Controls::Require.require_relative(relative_path, '.')
  assert_equal(false, required)

  assert_equal({ path => true }, Controls::Require.loading_files)
end
