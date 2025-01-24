# Advent of Code 2024


Here's my go at [Advent of Code 2024](https://adventofcode.com/2024).


## Log


### December 3rd

#### Day 0

Setting up a repository and project, and giving auth to Advent of Code.

Using C++20, and setting up a project using Visual Studio's native unit tests, since that worked pretty well when I did that last time, for AOC 2021.

Starting with some existing template code from AOC 2021.

#### Day 1

Ah yes, I remember now.  The first real challenge of doing AOC in C++ is parsing.

Going with a `std::priority_queue` because that seems to be the quickest and easiest way to get both lists in sorted order.

Part 1 success on the first try!  Yes, it's the first day, but I'm still excited.

Part 2 done pretty simply with a map.


### December 4th

#### Day 2

I have 15 minutes, can I get day 2 done in that amount of time?

Had a couple minor mistakes, one with parsing, and the other with checking for whether the levels were increasing or decreasing, but got part 1 to pass!  It took longer than 15 minutes, so I'll have to come back to part 2 later.


### December 5th

#### Day 2 - continued

I did take a quick look at part 2 yesterday, so I had a little bit of time to
think about it.  I thing all I need to do is to keep a count of how many times
an unsafe level is encountered in the report, and so long as it's not more
than 1, that means it's safe.

This approach works with the given sample, but the result was too low for my
given input.

I managed to spot an example of where this approach fails: 55 52 53 54 56 57.
This one fails because my function does not attempt to ignore the first level.

I could brute force this by removing one level at a time, but that seems silly.
I could reverse the list, and my current function would work without any
further modifications.

The reversed list approach worked!  I don't feel like it's the best solution,
but it's good enough to move on for now.  If I feel like revisiting any of
these days, this is one to revisit.

#### Day 3

Ah, now we're *intentionally* getting into parsing!

Not sure if there will be a case where a number has more than three digits,
but I suppose I might as well handle that case.

I'm kinda surprised there still isn't a built-in `string_view` to integer
function.  I could try using `std::from_chars` to avoid making a copy of the
string, but `std::stoi` is simpler, and I'm not that concerned about
performance right now.

Part 1 input worked on the first try.

Now we're parsing with state!  I noticed part 2 has a different sample than
part 1, which is only a minor inconvenience.

For a brief moment, I thought it might be time to introduce a class, but then
I realized all I needed was to add a couple more ParseMatch calls.


### December 6th

#### Day 4

Word search seems like a fun challenge that I haven't done before!  I imagine the general approach will be to load the input into memory, and then search for the given word in each of the 8 directions.

In this case, I think it'll be nice to create a class to contain the input, so that I can do easier access with built-in bounds checking.

I also can't help but think how much more difficult this would be if it was necessary to support Unicode in a puzzle like this.  For now I'm assuming everything is in ASCII.

Part 1 success!  Code worked on the first try.

Hah, I was wondering what they would do for part 2.  This is quite clever!  And also not a quick solution with my current implementation.  The challenge here is how `MAS` needs to cross in the middle.

I was thinking that I could still make use of my `IsWordAt` function, but then I also thought it would be most efficient to first check for the `A` in the middle.  These two ideas don't really go together, so I need to pick one.  Since I know I'm specializing for `MAS` and not any generic word, the second approach might be smarter.

Hmm, on my first run at the sample, I got 10 instead of the expected 9 matches.  So now I'm going to need to do some debugging.  Oh, I had `>` instead of `>=` when checking for out of bounds columns with the line length.


### December 15th

#### Day 5: Print Queue

This one feels pretty involved, but that might just be because there are
multiple steps to get to a single number result.

I feel like I want a split string utility function, to help with parsing comma
separated lists.  So I made a quick `n_StringUtil::SplitString` for this
purpose.

I wonder if 0 is a valid page number.  For the moment I'm going to
optimistically assume that it is not, since that helps make the code simpler
for me.

Now that I'm thinking about the meat of this problem, I'm seeing two different
approaches.  The first approach is to iterate through all the rules for each
page in the update, to make sure it's valid.  The second approach is to build
an ordering for all pages using the ordering rules, and then compare the
updates to that.  I'm interested in trying this second approach.

My first attempt at my given input indicated that there were no
correctly-ordered updates, which is not correct, so time to debug.  It looks
like my code for figuring out the complete order of pages is incorrect.

In debugging my code, I discovered that the sequence of pages defined by the
input appears to be circular.  So my approach of trying to create a linear
sequence just won't work.  Time for a different approach.

Taking the first approach I described earlier, that one also passes with the
sample, but not with the input.  I'm struggling to debug this because I don't
know what case is going wrong.  I'll have to revisit this another day.


### December 22nd

I'm going to set day 5 aside and see if I can move on with day 6.

#### Day 6: Guard Gallivant

The input is a grid similar to day 4, so that's a good starting point.

Actually, I don't need to store the entire map, I just need to load the
positions of things!

Deciding to use `std::set` instead of `std::unordered_set` because I can easily
get a built-in less-than operator using the default `<=>` operator, but I can't
get a built-in hash function.

My initial implementation worked fine, and now it's on to part 2!

Part 2 is an interesting puzzle!  My initial idea is that any time the guard
encounters an obstruction, mark all the positions back from that position as
potential places to obstruct the guard.

I got the example to pass, but the answer I tried for my input (340) was too
low.  I assume that means there are special cases that I'm not considering.

Taking a peek at the [subreddit](https://www.reddit.com/r/adventofcode/), I see
a lot of people mentioning a brute force method.  I assume the brute force
method is to test adding an obstruction at every possible position, and seeing
if that causes the guard to loop.  Thinking about this approach, I think I see
now how my initial approach failed.

I'm guessing that since I'm not attempting to divert from the default path, I'm
missing out on other potential loops on the map.  So I need to see about adding
obstructions that will push the guard into new paths, in case those new paths
result in a loop.

Once again the sample passed, but this time my input result (1444) is too high.
And it does take a lot longer to test every option along the path.

I made it faster by not having to recheck the entire path, but I still get the
same too-high result.  I'm going to have to leave it here for today.


### December 24th

It's Christmas Eve—Christmas Day by Advent of Code standards—so I won't be
saving Christmas on time as the story is presented, but I'm still enjoying
these puzzles and intend to continue solving them.

That said, I don't want to put all my energy into the parts that I'm stuck on
and have no clear way forward.  So, onward to day 7!

#### Day 7: Bridge Repair

Seems pretty simple.  For each line, try every combination of operators until a
result matches the test value.  Since it looks like we're only dealing with
positive numbers, we could potentially exit early if the calculated result
becomes larger than the test value.

Once again, the first step is writing more string parsing functions.

I'm probably over-engineering this by making it possible to add more than 2
operators, but it feels better this way.

I just glanced at the input, and noticed that I will very likely need to
support larger than 32-bit integers.

Part 1 passed on the first try.

Ah hah, there are more than 2 operators!

Duplicating my code from part 1 with modifications for part 2 feels icky, but
this provides a quicker start and it'll be easier to clean it up later.  Using
strings for concatenation also feels icky, but it should get me to the correct
solution quicker than figuring out the integer math.

Fortunately the early exit still applies, since concatenate will also only
result in larger numbers.

Had to run this one on an optimized Release build, because Debug was taking too
long.  But it works, and I passed part 2!


### December 26th

#### Day 8: Resonant Collinearity

First step is deciding how I want to load the data.  Tiles like day 4, or
positions like day 6?  Positions makes more sense for this one.

Next step is to get all the pairs of antennas for each frequency.  I'm tempted
to create a range factory that generates a range of all unique pairs from a
given range, but that feels like more work than it's worth right now.  Might be
an interesting exercise for later though.

Part 1 done and passed on the first try!

And part 2 took less than 15 minutes.

Things I've learned or refreshed in this exercise:

* `std::multimap`
* `std::ranges::subrange`
* `std::ranges::range_value_t`
* `std::next`

#### Day 9: Disk Fragmenter

I feel like I might need larger than 32-bit integers for these checksums.

The most obvious initial way of attempting this is to create a vector of file
IDs for each block on the disk, but I suspect they may have designed the puzzle
such that this approach would take an unnecessarily large amount of memory.  So
perhaps a smarter approach will be to store and work with the spans themselves.
The challenge of doing it this way is that I'll need to split spans.

To simplify inserting spans, and not having to worry about invalidated
iterators, I'm going to use a `std::list` instead of a `std::vector`.

Worked on the first try.

For part 2, it doesn't look like I need to handle the case where moving files
would open a gap large enough for a subsequent file, because of the order in
which files get moved.  In fact, I want to make sure to avoid moving a file
farther back on the disk.

Part 2 might have been a simpler algorithm than part 1!


### December 29th

#### Day 10: Hoof It

Here we have another map.  Maybe I should make common code for loading maps
like this, but for right now I'm going to just copy the code from day 4.

This is a minor thing, but if I describe the map as having a width and height,
that height could be confused with how it is also a map of heights.

So I believe the general approach is:

1. Find each trailhead (height of 0).
2. For each trailhead, count every unique path (to a height of 9).
3. Add those counts together.

Easy, right?

I was going to assuming that if two trails diverge and then merge back to the
same destination, that counts as a score of 2, not 1; but I see that the task
specifies **the number of 9-height positions reachable from that trailhead**.
So if a trailhead has multiple paths to the same 9-height position, that still
only counts as one.

The simplest implementation seems to use recursion.  Guess I'll find out if I
run out of stack space.

Looks like that worked!

Oh, I nearly completed part 2 when I was implementing part 1.  A few quick
edits, and that's part 2 done!


### January 23

Wow, January really has not given me much time to do programming puzzles, but
let's see if I can get one done today.

#### Day 11: Plutonian Pebbles

Given the multiple examples, I may want to create multiple unit tests.  But for
now, I'm using the initial arrangement in the longer example, since the prompt
provides an answer for blinking 25 times from this initial arrangement.

Since this problem primarily deals with inserting items in the middle of a
series, my first thought is to use a linked list, or `std::list` here.  I do
wonder if the actual input is designed such that the result will be more than
can easily fit in memory, in which case I'll need to come up with a more clever
approach, but best not to pre-emptively optimize.

Starting with the parsing from Day 7, along with loading the input into a
string from Day 3.

On second thought, instead of modifying the same container, I could just
construct a new container on each blink.  Not sure which will be more efficient.
Might start with `std::vector`, since that's a good simple default, until I
find that something else might be more appropriate.

It looks like I'll need to expect large numbers.  Definitely larger than 32 bit
integers.  Hopefully not more than 64 bit integers!  And it looks like they'll
all be positive (or zero).

I need to remember how to get the number of digits from an integer.  This is
something I know I've done before, but don't do it often enough to remember it
off the top of my head.  I think it has to do with using log base 10.

I got a result for my input, and it was correct!  No special optimizations
needed.  So far.

Hah, I'm amused that part 2 is just "what if you did it more?"

I'm assuming this is designed so that my original approach will not work here.
Indeed, it fails after a minute due to running out of memory, so I have to
think about a different approach now.

The actual numbers on the pebbles do not matter as much as the total number of
pebbles at the end.  So the answer here is probably not to keep track of every
individual pebble, but to figure out the total number of pebbles some other way.

I suspect there are going to be repeating patterns.  For example:

1. 0
2. 1
3. 2024
4. 20 24
5. 2 0 2 4
6. 4048 1 4048 8096

Already within 5 steps, there is a repeat of `0`, and `2` shows up twice.  So
maybe I want to keep a map of these patterns, and use that to figure out the
resulting number of pebbles.

Ultimately I realized that the order of these numbers don't matter, and all I
needed to keep track of was the count for each number.  This meant that all I
needed was a simple map from the number to the count of that number.  All the
stones of the same number will end up as the same result after a single blink.
This approach worked, and part 2 passed without any issues.  And only took
28 milliseconds.
