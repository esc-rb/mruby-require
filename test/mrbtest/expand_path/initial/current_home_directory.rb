control_home_directory = Controls::Require::Environment.home_directory
current_user = Controls::Require::Environment.current_user

assert "Expand Path, Initial, Current User's Home Directory" do
  output_path = Controls::Require::ExpandPath.initial('~/some-segment/other-segment')

  assert_equal("#{control_home_directory}/some-segment/other-segment", output_path)
end

assert "Expand Path, Initial, Current User's Home Directory, No Segments" do
  output_path = Controls::Require::ExpandPath.initial('~')

  assert_equal(control_home_directory, output_path)
end

assert "Expand Path, Initial, Current User's Home Directory, Memory Allocation" do
  output_path = String.new

  allocations = Controls::Require::ExpandPath.current_home_directory(output_path)
  assert_equal(control_home_directory, output_path)

  assert_equal(0, allocations);
end

assert "Expand Path, Initial, Current User's Home Directory, HOME Not Set" do
  output_path = String.new

  allocations = Controls::Require::ExpandPath.current_home_directory(output_path, 'SOME_OTHER_ENV_VAR')

  assert_equal(control_home_directory, output_path)

  # getlogin_r(3) and getpwnam_r(3) both require a memory allocation
  assert_equal(2, allocations);
end

assert "Expand Path, Initial, Current User's Home Directory, Multiple Memory Allocations" do
  output_path = String.new

  allocation_size_bytes = 2

  minimum_allocations = (current_user.bytesize + control_home_directory.bytesize) / allocation_size_bytes
  assert_true(minimum_allocations > 1)

  allocations = Controls::Require::ExpandPath.current_home_directory(output_path, 'SOME_OTHER_ENV_VAR', allocation_size_bytes)
  assert_equal(control_home_directory, output_path)

  assert_true(allocations >= minimum_allocations);
end

assert "Expand Path, Initial, Current User's Home Directory, getlogin_r(3) Error" do
  # Causes getlogin_r(3) to fail and set errno to ERANGE ("Numerical result out of range")
  allocation_size_bytes = 0

  assert_raise_with_message(LoadError, "cannot get home directory (Numerical result out of range)") do
    Controls::Require::ExpandPath.current_home_directory(String.new, 'SOME_OTHER_ENV_VAR', allocation_size_bytes)
  end
end
