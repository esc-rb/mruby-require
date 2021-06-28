assert "Expand Path, Initial Relative Segment, Initial Segment" do
  output_path = ''

  result = Controls::Require::ExpandPath.join_segment('some-segment', output_path)

  assert_equal('some-segment', output_path)
  assert_same(output_path, result)
end

assert "Expand Path, Initial Relative Segment, Initial Segment, Optional Expanded Path Argument Omitted" do
  output_path = Controls::Require::ExpandPath.join_segment('some-segment')

  assert_equal('some-segment', output_path)
end

assert "Expand Path, Initial Relative Segment, Initial Segment, Current Directory Reference" do
  output_path = Controls::Require::ExpandPath.join_segment('.')

  assert_equal('', output_path)
end

assert "Expand Path, Initial Relative Segment, Initial Segment, Previous Directory Reference" do
  output_path = Controls::Require::ExpandPath.join_segment('..')

  assert_equal('..', output_path)
end

assert "Expand Path, Initial Relative Segment, Initial Segment, File" do
  output_path = Controls::Require::ExpandPath.join_segment('some-file.ext')

  assert_equal('some-file.ext', output_path)
end

assert "Expand Path, Initial Relative Segment, Initial Segment, Hidden Segment" do
  output_path = Controls::Require::ExpandPath.join_segment('.some-hidden-segment')

  assert_equal('.some-hidden-segment', output_path)
end

assert "Expand Path, Initial Relative Segment, Initial Segment, Hidden Segment, Two Leading Periods" do
  output_path = Controls::Require::ExpandPath.join_segment('..some-hidden-segment')

  assert_equal('..some-hidden-segment', output_path)
end

assert "Expand Path, Initial Relative Segment, Initial Segment, Trailing Period" do
  output_path = Controls::Require::ExpandPath.join_segment('some-segment.')

  assert_equal('some-segment.', output_path)
end

assert "Expand Path, Initial Relative Segment, Initial Segment, Two Trailing Periods" do
  output_path = Controls::Require::ExpandPath.join_segment('some-segment..')

  assert_equal('some-segment..', output_path)
end

assert "Expand Path, Initial Relative Segment, Initial Segment, Three Consecutive Periods" do
  output_path = Controls::Require::ExpandPath.join_segment('...')

  assert_equal('...', output_path)
end
