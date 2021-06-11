path = '/some/path.rb'

assert "Load Error" do
  assert_raise_with_message(LoadError, "cannot load such file -- #{path}") do
    Controls::Require::LoadError.raise(path)
  end
end

assert "Load Error, Reason" do
  reason = "Some reason"

  assert_raise_with_message(LoadError, "cannot load such file -- #{path} (#{reason})") do
    Controls::Require::LoadError.raise_reason(path, reason)
  end
end

assert "Load Error, Alternate Message" do
  message = "Some error message"

  assert_raise_with_message(LoadError, message) do
    Controls::Require::LoadError.raise_message(message)
  end
end
