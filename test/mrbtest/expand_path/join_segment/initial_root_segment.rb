assert "Expand Path, Initial Root Segment, Root Segment" do
  output_path = Controls::Require::ExpandPath.join_segment('some-segment', '/')

  assert_equal('/some-segment', output_path)
end

assert "Expand Path, Initial Root Segment, Root Segment, Two Consecutive Directory Separators" do
  output_path = Controls::Require::ExpandPath.join_segment('some-segment', '//')

  assert_equal('//some-segment', output_path)
end

assert "Expand Path, Initial Root Segment, Root Segment, Current Directory Reference" do
  output_path = Controls::Require::ExpandPath.join_segment('.', '/')

  assert_equal('/', output_path)
end

assert "Expand Path, Initial Root Segment, Root Segment, Previous Directory Reference" do
  output_path = Controls::Require::ExpandPath.join_segment('..', '/')

  assert_equal('/', output_path)
end

assert "Expand Path, Initial Root Segment, Root Segment, Previous Directory Reference, Root Has Two Consecutive Directory Separators" do
  output_path = Controls::Require::ExpandPath.join_segment('..', '//')

  assert_equal('//', output_path)
end

assert "Expand Path, Initial Root Segment, Root Segment, File" do
  output_path = Controls::Require::ExpandPath.join_segment('some-file.ext', '/')

  assert_equal('/some-file.ext', output_path)
end

assert "Expand Path, Initial Root Segment, Root Segment, Hidden Segment" do
  output_path = Controls::Require::ExpandPath.join_segment('.some-hidden-segment', '/')

  assert_equal('/.some-hidden-segment', output_path)
end

assert "Expand Path, Initial Root Segment, Root Segment, Hidden Segment, Two Leading Periods" do
  output_path = Controls::Require::ExpandPath.join_segment('..some-hidden-segment', '/')

  assert_equal('/..some-hidden-segment', output_path)
end

assert "Expand Path, Initial Root Segment, Root Segment, Trailing Period" do
  output_path = Controls::Require::ExpandPath.join_segment('some-segment.', '/')

  assert_equal('/some-segment.', output_path)
end

assert "Expand Path, Initial Root Segment, Root Segment, Two Trailing Periods" do
  output_path = Controls::Require::ExpandPath.join_segment('some-segment..', '/')

  assert_equal('/some-segment..', output_path)
end

assert "Expand Path, Initial Root Segment, Root Segment, Three Consecutive Periods" do
  output_path = Controls::Require::ExpandPath.join_segment('...', '/')

  assert_equal('/...', output_path)
end
