assert "Expand Path, Join Path" do
  path = 'some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.join('/some-origin', path)

  assert_equal('/some-origin/some-segment/other-segment', output_path)
end

assert "Expand Path, Join Path, Relative Path" do
  path = 'some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.join('some-origin', path)

  assert_equal('some-origin/some-segment/other-segment', output_path)
end

assert "Expand Path, Join Path, Consecutive Directory Separators" do
  path = 'some-segment//other-segment'

  output_path = Controls::Require::ExpandPath.join('/some-origin', path)

  assert_equal('/some-origin/some-segment/other-segment', output_path)
end

assert "Expand Path, Join Path, Current Directory Reference" do
  path = 'some-segment/./other-segment'

  output_path = Controls::Require::ExpandPath.join('/some-origin', path)

  assert_equal('/some-origin/some-segment/other-segment', output_path)
end

assert "Expand Path, Join Path, Current Directory Reference And Consecutive Directory Separators" do
  path = 'some-segment//.//other-segment'

  output_path = Controls::Require::ExpandPath.join('/some-origin', path)

  assert_equal('/some-origin/some-segment/other-segment', output_path)
end

assert "Expand Path, Join Path, Previous Directory Reference" do
  path = 'some-segment/../other-segment'

  output_path = Controls::Require::ExpandPath.join('/some-origin', path)

  assert_equal('/some-origin/other-segment', output_path)
end

assert "Expand Path, Join Path, Previous Directory Reference, Reaches Root Directory" do
  path = '../some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.join('/some-origin', path)

  assert_equal('/some-segment/other-segment', output_path)
end

assert "Expand Path, Join Path, Previous Directory Reference, Reaches Root Directory, Root Directory Contains Two Consecutive Separators" do
  path = '../some-segment/other-segment'

  output_path = Controls::Require::ExpandPath.join('//some-origin', path)

  assert_equal('//some-segment/other-segment', output_path)
end
