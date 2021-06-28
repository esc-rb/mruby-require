assert "Expand Path, Join Segment, Subsequent Segment, Absolute Path" do
  expanded_path = Controls::Require::ExpandPath.join_segment('other-segment', '/some-segment')

  assert_equal('/some-segment/other-segment', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Absolute Path, Current Directory Reference" do
  expanded_path = Controls::Require::ExpandPath.join_segment('.', '/some-segment')

  assert_equal('/some-segment', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Absolute Path, Previous Directory Reference" do
  expanded_path = Controls::Require::ExpandPath.join_segment('..', '/some-segment')

  assert_equal('/', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Absolute Path, Previous Directory Reference, Root Has Two Consecutive Directory Separators" do
  expanded_path = Controls::Require::ExpandPath.join_segment('..', '//some-segment')

  assert_equal('//', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Absolute Path, File" do
  expanded_path = Controls::Require::ExpandPath.join_segment('some-file.ext', '/some-segment')

  assert_equal('/some-segment/some-file.ext', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Absolute Path, Hidden Segment" do
  expanded_path = Controls::Require::ExpandPath.join_segment('.some-hidden-segment', '/some-segment')

  assert_equal('/some-segment/.some-hidden-segment', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Absolute Path, Hidden Segment, Two Leading Periods" do
  expanded_path = Controls::Require::ExpandPath.join_segment('..some-hidden-segment', '/some-segment')

  assert_equal('/some-segment/..some-hidden-segment', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Absolute Path, Trailing Period" do
  expanded_path = Controls::Require::ExpandPath.join_segment('other-segment.', '/some-segment')

  assert_equal('/some-segment/other-segment.', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Absolute Path, Two Trailing Periods" do
  expanded_path = Controls::Require::ExpandPath.join_segment('other-segment..', '/some-segment')

  assert_equal('/some-segment/other-segment..', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Absolute Path, Three Consecutive Periods" do
  expanded_path = Controls::Require::ExpandPath.join_segment('...', '/some-segment')

  assert_equal('/some-segment/...', expanded_path)
end
