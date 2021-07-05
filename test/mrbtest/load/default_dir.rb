current_directory = Controls::Require::Environment.current_directory

assert "Load, Default __dir__, Program Name (`$0') Not Set" do
  $0 = nil

  assert_equal(nil, __dir__)
end

assert "Load, Default __dir__, Program Name (`$0') Set, Relative Path" do
  $0 = 'path/to/some_file.rb'

  assert_equal("#{current_directory}/path/to", __dir__)
end

assert "Load, Default __dir__, Program Name (`$0') Set, .mrb Extension" do
  $0 = 'path/to/some_file.mrb'

  assert_equal("#{current_directory}/path/to", __dir__)
end

assert "Load, Default __dir__, Program Name (`$0') Set, Other Extension" do
  $0 = 'path/to/some_file.ext'

  assert_equal(nil, __dir__)
end

assert "Load, Default __dir__, Program Name (`$0') Set to `-'" do
  $0 = '-'

  assert_equal(nil, __dir__)
end

assert "Load, Default __dir__, Program Name (`$0') Set to `-e'" do
  $0 = '-e'

  assert_equal(nil, __dir__)
end

assert "Load, Default __dir__, Program Name (`$0') Starts With a Hyphen (`-')" do
  $0 = '-some-dir/some_file.rb'

  assert_equal("#{current_directory}/-some-dir", __dir__)
end
