assert "Load, Kernel#load" do
  $loaded_file = nil

  path = 'test/lib/some_feature.rb'

  loaded = load(path)

  assert_true(loaded)
  assert_equal(path, $loaded_file)
end

assert "Load, Kernel#load, File Not Found" do
  path = 'no_such_file.rb'

  assert_raise_with_message(LoadError, "cannot load such file -- #{path}") do
    load(path)
  end
end

assert "Load, Kernel#load, Error" do
  $error = RuntimeError.new('Some Error')

  path = 'test/lib/raise_error.rb'

  begin
    load(path)
  rescue RuntimeError => exception
  end

  assert_not_nil(exception)
  assert_same($error, exception)
end
