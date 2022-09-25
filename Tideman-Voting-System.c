#include <cs50.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];     // size = 36

int pair_count;
int candidate_count;
int array_size = MAX * (MAX - 1) / 2;


int vote_count[MAX]; // where i is the index of the candidate and the return value is the number of votes


// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX + 1);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // iterate through candidates
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)         // if vote is for a valid candidate
        {
            // update rank array
            ranks[rank] = i;
            vote_count[i]++;
            return true;
        }

    }
    // if no candidate is found
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Iterate over voter ranks
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Update number of voters who prefer [i] to [j]
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // iterate over preferences
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // if pair1 has more votes than pair2
            if (j != i && preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;   // candidate[i]
                pairs[pair_count++].loser = j;     // canidate[j]
            }
        }
    }
    return;
}


int compar(const void *a, const void *b)
{
    const pair *pa = a, *pb = b;
    return (preferences[pa->winner][pa->loser] < preferences[pb->winner][pb->loser]) -
           (preferences[pa->winner][pa->loser] > preferences[pb->winner][pb->loser]);
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    qsort(pairs, pair_count, sizeof(*pairs), compar);
}


// a is origional winner, b is origional loser
bool iscycle(int a, int b)
{
    // if loser has an edge to the winner
    if (locked[b][a] == true)
    {
        return true;
    }

    // loop through all candidates, see if any have an edge on the winner
    for (int i = 0; i < candidate_count; i++)
    {
        // if winner becomes a loser at some stage
        if (locked[i][a] == true)
        {
            // recursively call function to find the new graph head
            if (iscycle(i, b) == true)
            {
                return true;
            }
        }
    }
    return false;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // iterate through pairs
    for (int i = 0; i < pair_count; i++)
    {
        if (!iscycle(pairs[i].winner, pairs[i].loser))              // determine whether there will be a cycle
        {
            locked[pairs[i].winner][pairs[i].loser] = true;         // lock pair
        }
    }
    return;
}

// checks whether a candidate is eliminated
bool eliminated(int a)
{
    // loop through pairs
    for (int i = 0; i < pair_count; i++)
    {
        if (locked[i][a])       // checks whether candidate has an edge pointing to them
        {
            return true;
        }
    }
    // returns false if there is no edge pointing to them
    return false;
}

// Print the winner of the election (assuming only one source)
void print_winner(void)
{
    // print candidate with no edge pointing towards them in the locked array
    for (int i = 0; i < pair_count; i++)
    {
        if (!eliminated(i))    // if no edge pointing to candidate
        {
            printf("%s\n", candidates[i]);   // print name of winner
        }
    }
    return;
}