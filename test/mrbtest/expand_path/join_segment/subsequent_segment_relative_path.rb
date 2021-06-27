assert "Expand Path, Join Segment, Subsequent Segment, Relative Path" do
  expanded_path = Controls::Require::ExpandPath.join_segment('other-segment', 'some-segment')

  assert_equal('some-segment/other-segment', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Relative Path, Current Directory Reference" do
  expanded_path = Controls::Require::ExpandPath.join_segment('.', 'some-segment')

  assert_equal('some-segment', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Relative Path, Previous Directory Reference" do
  expanded_path = Controls::Require::ExpandPath.join_segment('..', 'some-segment')

  assert_equal('', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Relative Path, File" do
  expanded_path = Controls::Require::ExpandPath.join_segment('some-file.ext', 'some-segment')

  assert_equal('some-segment/some-file.ext', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Relative Path, Hidden Segment" do
  expanded_path = Controls::Require::ExpandPath.join_segment('.some-hidden-segment', 'some-segment')

  assert_equal('some-segment/.some-hidden-segment', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Relative Path, Hidden Segment, Two Leading Periods" do
  expanded_path = Controls::Require::ExpandPath.join_segment('..some-hidden-segment', 'some-segment')

  assert_equal('some-segment/..some-hidden-segment', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Relative Path, Trailing Period" do
  expanded_path = Controls::Require::ExpandPath.join_segment('other-segment.', 'some-segment')

  assert_equal('some-segment/other-segment.', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Relative Path, Two Trailing Periods" do
  expanded_path = Controls::Require::ExpandPath.join_segment('other-segment..', 'some-segment')

  assert_equal('some-segment/other-segment..', expanded_path)
end

assert "Expand Path, Join Segment, Subsequent Segment, Relative Path, Three Consecutive Periods" do
  expanded_path = Controls::Require::ExpandPath.join_segment('...', 'some-segment')

  assert_equal('some-segment/...', expanded_path)
end
