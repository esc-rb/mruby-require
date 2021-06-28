assert "Expand Path, Initial, Absolute Path" do
  path = '/some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.initial(path)

  assert_equal(path, output_path)
end

assert "Expand Path, Initial, Absolute Path, Root Path Has Two Consecutive Directory Separators" do
  path = '//some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.initial(path)

  assert_equal(path, output_path)
end

assert "Expand Path, Initial, Absolute Path, Root Path Has Three Consecutive Directory Separators" do
  output_path = Controls::Require::ExpandPath.initial('///some-segment/other-segment')

  assert_equal('/some-segment/other-segment', output_path)
end

assert "Expand Path, Initial, Absolute Path, No Segments" do
  output_path = Controls::Require::ExpandPath.initial('/')

  assert_equal('/', output_path)
end
