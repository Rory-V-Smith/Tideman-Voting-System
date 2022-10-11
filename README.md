# Tideman-Voting-System
Calculates the condorcet winner of an election utilising a Tideman, or 'ranked-pairs', voting system algorithm, per the below.

>./tideman Alice Bob Charlie
vNumber of voters: 5
>Rank 1: Alice
>Rank 2: Charlie
>Rank 3: Bob

>Rank 1: Alice
>Rank 2: Charlie
>Rank 3: Bob

>Rank 1: Bob
>Rank 2: Charlie
>Rank 3: Alice

>Rank 1: Bob
>Rank 2: Charlie
>Rank 3: Alice

>Rank 1: Charlie
>Rank 2: Alice
>Rank 3: Bob

>Charlie

### Background
The Tideman voting method (also known as “ranked pairs”) is a ranked-choice voting method that’s guaranteed to produce the Condorcet winner (the person who would have won any head-to-head matchup against another candidate) of the election if one exists.

Generally speaking, the Tideman method works by constructing a “graph” of candidates, where an arrow (i.e. edge) from candidate A to candidate B indicates that candidate A wins against candidate B in a head-to-head matchup.

The Tideman algorithm must be careful to avoid creating cycles in the candidate graph. To achieve this, the algorithm locks in the strongest edges first, since those are arguably the most significant. In particular, the Tideman algorithm specifies that matchup edges should be “locked in” to the graph one at a time, based on the “strength” of the victory (the more people who prefer a candidate over their opponent, the stronger the victory). So long as the edge can be locked into the graph without creating a cycle, the edge is added; otherwise, the edge is ignored.

Put more formally, the Tideman voting method consists of three parts:

- Tally: Once all of the voters have indicated all of their preferences, determine, for each pair of candidates, who the preferred candidate is and by what margin they are preferred.
- Sort: Sort the pairs of candidates in decreasing order of strength of victory, where strength of victory is defined to be the number of voters who prefer the preferred candidate.
- Lock: Starting with the strongest pair, go through the pairs of candidates in order and “lock in” each pair to the candidate graph, so long as locking in that pair does not create a cycle in the graph.
Once the graph is complete, the source of the graph (the one with no edges pointing towards it) is the winner!

