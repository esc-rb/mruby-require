$0 = __FILE__

$loaded_file = nil
required = require_relative('../../lib/some_feature.rb')
loaded_file = $loaded_file

assert "Require Relative, kernel#require_relative" do
  assert_true(required)

  path = Controls::Require::ExpandPath.('test/lib/some_feature.rb')
  assert_equal(path, loaded_file)
end

begin
  require_relative('../../lib/no_such_file.rb')
rescue LoadError => load_error
end

assert "Require Relative, kernel#require_relative, File Not Found" do
  path = Controls::Require::ExpandPath.('test/lib/no_such_file.rb')

  assert_kind_of(LoadError, load_error)
  assert_equal("cannot load such file -- #{path}", load_error.message)
end

error = RuntimeError.new('Some Error')
begin
  $error = error
  require_relative('../../lib/raise_error.rb')
rescue RuntimeError => raised_error
end

assert "Require Relative, kernel#require_relative, Error" do
  assert_kind_of(RuntimeError, raised_error)
  assert_same(error, raised_error)
end

module SomeModule
  def self.require_relative_test_call_from_outside_toplevel
    require_relative '../../lib/some_feature.rb'
  end
end

assert "Require Relative, kernel#require_relative, Called From Outside Toplevel" do
  assert_raise_with_message(LoadError, "cannot load such file -- ../../lib/some_feature.rb (require_relative cannot be called from a method)") do
    SomeModule.require_relative_test_call_from_outside_toplevel
  end
end

begin
  $0 = nil
  require_relative('../../lib/some_feature.rb')
rescue LoadError => load_error
end

assert "Require Relative, kernel#require_relative, __dir__ Not Set" do
  assert_kind_of(LoadError, load_error)
  assert_equal("cannot load such file -- ../../lib/some_feature.rb (__dir__ could not be determined)", load_error.message)
end
