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
