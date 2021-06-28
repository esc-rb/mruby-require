current_directory = Controls::Require::Environment.current_directory

assert "Expand Path, Initial, Relative Path" do
  path = 'some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.initial(path)

  assert_equal("#{current_directory}/#{path}", output_path)
end

assert "Expand Path, Initial, Relative Path, No Segments" do
  output_path = Controls::Require::ExpandPath.initial('')

  assert_equal(current_directory, output_path)
end

assert "Expand Path, Initial, Current Directory, Memory Allocation" do
  output_path = String.new

  allocations = Controls::Require::ExpandPath.current_directory(output_path)
  assert_equal(current_directory, output_path)

  assert_equal(1, allocations);
end

assert "Expand Path, Initial, Current Directory, Multiple Memory Allocations" do
  output_path = String.new

  allocation_size_bytes = 4

  minimum_allocations = current_directory.bytesize / allocation_size_bytes
  assert_true(minimum_allocations > 1)

  allocations = Controls::Require::ExpandPath.current_directory(output_path, allocation_size_bytes)
  assert_equal(current_directory, output_path)

  assert_true(allocations >= minimum_allocations);
end

assert "Expand Path, Initial, Current Directory, getcwd(3) Error" do
  # Causes getcwd(3) to fail and set errno to EINVAL ("Invalid argument")
  allocation_size_bytes = 0

  assert_raise_with_message(LoadError, "cannot get current directory (Invalid argument)") do
    Controls::Require::ExpandPath.current_directory(String.new, allocation_size_bytes)
  end
end
