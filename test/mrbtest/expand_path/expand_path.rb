home_directory = Controls::Require::Environment.home_directory
current_directory = Controls::Require::Environment.current_directory

assert "Expand Path, Absolute Path" do
  path = '/some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.(path)

  assert_equal(path, output_path)
end

assert "Expand Path, Absolute Path, Origin Given" do
  path = '/some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.(path, '/some-origin')

  assert_equal(path, output_path)
end

assert "Expand Path, Home Reference" do
  path = '~/some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.(path)

  assert_equal("#{home_directory}/some-segment/other-segment", output_path)
end

assert "Expand Path, Home Reference, Origin Given" do
  path = '~/some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.(path, '/some-origin')

  assert_equal("#{home_directory}/some-segment/other-segment", output_path)
end

assert "Expand Path, Relative Path, Origin Omitted" do
  path = 'some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.(path)

  assert_equal("#{current_directory}/#{path}", output_path)
end

assert "Expand Path, Relative Path, Origin Given, Origin Is An Empty String" do
  path = 'some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.(path, '')

  assert_equal("#{current_directory}/#{path}", output_path)
end

assert "Expand Path, Relative Path, Origin Given" do
  path = 'some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.(path, '/some-origin')

  assert_equal('/some-origin/some-segment/other-segment', output_path)
end

assert "Expand Path, Relative Path, Origin Given, Trailing Directory Separator" do
  path = 'some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.(path, '/some-origin/')

  assert_equal('/some-origin/some-segment/other-segment', output_path)
end

assert "Expand Path, Relative Path, Relative Origin Given" do
  path = 'some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.(path, 'some-origin')

  assert_equal("#{current_directory}/some-origin/some-segment/other-segment", output_path)
end

assert "Expand Path, Relative Path, Origin Given, Home Reference" do
  path = 'some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.(path, '~/some-origin')

  assert_equal("#{home_directory}/some-origin/some-segment/other-segment", output_path)
end
