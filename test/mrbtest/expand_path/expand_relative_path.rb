assert "Expand Path, Expand Relative Path" do
  path = 'some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.relative(path)

  assert_equal(path, output_path)
  assert_not_same(path, output_path)
end

assert "Expand Path, Expand Relative Path, Consecutive Directory Separators" do
  output_path = Controls::Require::ExpandPath.relative('some-segment//other-segment')

  assert_equal('some-segment/other-segment', output_path)
end

assert "Expand Path, Expand Relative Path, Current Directory Reference" do
  output_path = Controls::Require::ExpandPath.relative('some-segment/./other-segment')

  assert_equal('some-segment/other-segment', output_path)
end

assert "Expand Path, Expand Relative Path, Current Directory Reference And Consecutive Directory Separators" do
  path = 'some-segment//.//other-segment'

  output_path = Controls::Require::ExpandPath.relative(path)

  assert_equal('some-segment/other-segment', output_path)
end

assert "Expand Path, Expand Relative Path, Previous Directory Reference, Relative Path" do
  output_path = Controls::Require::ExpandPath.relative('some-segment/../other-segment')

  assert_equal('other-segment', output_path)
end

assert "Expand Path, Expand Relative Path, Previous Directory Reference, Relative Path, Leading Previous Directory Reference" do
  output_path = Controls::Require::ExpandPath.relative('../some-segment/other-segment')

  assert_equal('../some-segment/other-segment', output_path)
end

assert "Expand Path, Expand Relative Path, Previous Directory Reference, Relative Path, Segments Exhausted" do
  output_path = Controls::Require::ExpandPath.relative('some-segment/../../other-segment')

  assert_equal('../other-segment', output_path)
end
