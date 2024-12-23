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

I got the example to pass, but the answer I tried for my input was too low.  I
assume that means there are special cases that I'm not considering.

Taking a peek at the [subreddit](https://www.reddit.com/r/adventofcode/), I see
a lot of people mentioning a brute force method.  I assume the brute force
method is to test adding an obstruction at every possible position, and seeing
if that causes the guard to loop.  Thinking about this approach, I think I see
now how my initial approach failed.

I'm guessing that since I'm not attempting to divert from the default path, I'm
missing out on other potential loops on the map.  So I need to see about adding
obstructions that will push the guard into new paths, in case those new paths
result in a loop.

Once again the sample passed, but this time my input result is too high.  And
it does take a lot longer to test every option along the path.

340 - too low
1444 - too high