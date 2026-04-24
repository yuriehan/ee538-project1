#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

/********************DO NOT EDIT**********************/
// Function prototype. Defined later.
void read_opinions(string filename); // reads file into opinions vector and updates total_nodes as needed
void read_edges(string filename); // reads file into edge_list, defined later
void build_adj_matrix(); // convert edge_list to adjacency matrix

int total_nodes = 0; // We keep track of the total number of nodes based on largest node id.


/****************************************************************/

/******** Create adjacency matrix and vector of opinions */
// simple vector to hold each node's opinion (0 or 1)
std::vector<int> opinions;

// global adjacency matrix initialized later
std::vector<std::vector<int>> adj;

// edge list: each row contains {source, target}
std::vector<std::vector<int>> edge_list;

void build_adj_matrix() // actually a list
{
    // (1) allocate matrix adj of appropriate size
    adj.assign(total_nodes, vector<int>());

    // (2) run through edge list and populate adj
    for (int i = 0; i < (int)edge_list.size(); i++)
    {
        int source = edge_list[i][0];
        int target = edge_list[i][1];
        adj[target].push_back(source); // source -> target, source is neighbor of target
    }
}

double calculate_fraction_of_ones()
{
    // (3) Calculate the fraction of nodes with opinion 1 and return it.
   int count = 0;
   for(int opinion : opinions)
   {
       count += opinion;
   }
   return (double)count / (double)opinions.size();
}

// For a given node, count majority opinion among its neighbours. Tie -> 0.
int get_majority_friend_opinions(int node)
{
    // (4) Count the number of neighbours with opinion 0 and opinion 1. Return the majority (0 or 1). 
    //If tie, return 0.
    int count_ones = 0;
    int count_zeros = 0;
    for (int neighbor : adj[node]) // only iterates thru actual neighbors
    {
        if (opinions[neighbor] == 1){
            count_ones++;
        }
        else{
            count_zeros++;
        }
    }
    // Return majority, tie -> 0
    if(count_ones > count_zeros){
        return 1;
    }
    else{
        return 0;
    }
}

// Calculate new opinions for all voters and return if anyone's opinion changed
bool update_opinions()
{
    // (5) For each node, calculate the majority opinion among its neighbours and update the node's opinion.
    // Return true if any node's opinion changed, false otherwise.
    vector<int> new_opinions(total_nodes);
    bool changed = false;
    for (int i = 0; i < total_nodes; i++)
    {
        new_opinions[i] = get_majority_friend_opinions(i);
        if (!changed && new_opinions[i] != opinions[i]){ // if not already flagged
            changed = true;
        }
    }
    opinions.swap(new_opinions); // https://www.geeksforgeeks.org/cpp/swap-in-cpp/
    return changed;
}

int main() {
    // no preallocation; vectors grow on demand

    // Read input files
    read_opinions("opinions.txt"); 
    read_edges("edge_list.txt");

    // convert edge list into adjacency matrix once we know total_nodes
    build_adj_matrix();
    
    cout << "Total nodes: " << total_nodes << endl;
    
    // Run simulation
    int max_iterations = 30;
    int iteration = 0;
    bool opinions_changed = true;
    
    // Print initial state
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << calculate_fraction_of_ones() << endl;
    
    /// (6)  //////////////////////////////////////////////
    // Run until consensus or max iterations
    //while( ... )
    while(iteration < max_iterations) // until no opinions change 
    {
        opinions_changed = update_opinions();

        if(!opinions_changed){
            break; // exit immediately if no change
        }
        iteration++;
        cout << "Iteration " << iteration << ": fraction of 1's = "
            << calculate_fraction_of_ones() << endl;
    }

    ////////////////////////////////////////////////////////
    // Print final result
    double final_fraction = calculate_fraction_of_ones();
    cout << "Iteration " << iteration << ": fraction of 1's = " 
         << final_fraction << endl;
    
    if(final_fraction == 1.0)
        cout << "Consensus reached: all 1's" << endl;
    else if(final_fraction == 0.0)
        cout << "Consensus reached: all 0's" << endl;
    else
        cout << "No consensus reached after " << iteration << " iterations" << endl;
    
    return 0;
}


/*********** Functions to read files **************************/ 

// Read opinion vector from file.
void read_opinions(string filename)
{
    ifstream file(filename);
    int id, opinion;
    while(file >> id >> opinion)
    {
        opinions.push_back(opinion);
        if(id >= total_nodes) total_nodes = id+1;
    }
    file.close();
}

// Read edge list from file and update total nodes as needed.
void read_edges(string filename)
{
    ifstream file(filename);
    int source, target;
    
    while(file >> source >> target)
    {
        edge_list.push_back({source, target});
        if(source >= total_nodes) total_nodes = source+1;
        if(target >= total_nodes) total_nodes = target+1;
    }
    file.close();
}

/********************************************************************** */