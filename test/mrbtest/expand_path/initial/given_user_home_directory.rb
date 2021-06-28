control_home_directory = Controls::Require::Environment.home_directory
control_user = Controls::Require::Environment.current_user

assert "Expand Path, Initial, Given User's Home Directory" do
  output_path = Controls::Require::ExpandPath.initial("~#{control_user}/some-segment/other-segment")

  assert_equal("#{control_home_directory}/some-segment/other-segment", output_path)
end

assert "Expand Path, Initial, Given User's Home Directory, No Segments" do
  output_path = Controls::Require::ExpandPath.initial("~#{control_user}")

  assert_equal(control_home_directory, output_path)
end

assert "Expand Path, Initial, Given User's Home Directory, Unknown User" do
  user = 'unknown-user--mruby-require-expand-path-test'

  assert_raise_with_message(LoadError, "cannot get home directory -- #{user}") do
    Controls::Require::ExpandPath.initial("~#{user}/some-segment/other-segment")
  end
end

assert "Expand Path, Initial, Given User's Home Directory, Memory Allocation" do
  output_path = String.new

  allocations = Controls::Require::ExpandPath.home_directory(output_path, control_user)
  assert_equal(control_home_directory, output_path)

  assert_equal(1, allocations);
end

assert "Expand Path, Initial, Given User's Home Directory, Multiple Memory Allocations" do
  allocation_size_bytes = 4

  output_path = String.new

  minimum_allocations = control_home_directory.bytesize / allocation_size_bytes
  assert_true(minimum_allocations > 1)

  allocations = Controls::Require::ExpandPath.home_directory(output_path, control_user, allocation_size_bytes)
  assert_equal(control_home_directory, output_path)

  assert_true(allocations >= minimum_allocations);
end

assert "Expand Path, Initial, Given User's Home Directory, getpwnam_r(3) Error" do
  # Causes getpwnam_r(3) to fail and set errno to ERANGE ("Numerical result out of range")
  allocation_size_bytes = 0

  assert_raise_with_message(LoadError, "cannot get home directory -- #{control_user} (Numerical result out of range)") do
    Controls::Require::ExpandPath.home_directory(String.new, control_user, allocation_size_bytes)
  end
end
