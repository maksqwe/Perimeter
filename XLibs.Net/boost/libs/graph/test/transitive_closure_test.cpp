// Copyright (C) 2001 Vladimir Prus <ghost@cs.msu.su>
// Copyright (C) 2001 Jeremy Siek <jsiek@cs.indiana.edu>
// Permission to copy, use, modify, sell and distribute this software is
// granted, provided this copyright notice appears in all copies and 
// modified version are clearly marked as such. This software is provided
// "as is" without express or implied warranty, and with no claim as to its
// suitability for any purpose.


#include <boost/graph/vector_as_graph.hpp>
#include <boost/graph/transitive_closure.hpp>

#include <iostream>

#include <boost/graph/vector_as_graph.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_list_io.hpp>
#include <boost/graph/graph_utility.hpp>

#include <cstdlib>
#include <ctime>
#include <boost/progress.hpp>
using namespace std;
using namespace boost;

void generate_graph(int n, double p, vector< vector<int> >& r1)
{
  static class {
  public:
    double operator()() {
      return double(rand())/RAND_MAX;
    }
  } gen;  
  r1.clear();
  r1.resize(n);
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j) 
      if (gen() < p)
        r1[i].push_back(j);
}

template <class Graph>
typename graph_traits<Graph>::degree_size_type
num_incident(typename graph_traits<Graph>::vertex_descriptor u,
             typename graph_traits<Graph>::vertex_descriptor v,
             const Graph& g)
{
  typename graph_traits<Graph>::degree_size_type d = 0;
  typename graph_traits<Graph>::out_edge_iterator i, i_end;
  for (tie(i, i_end) = out_edges(u, g); i != i_end; ++i)
    if (target(*i, g) == v)
      ++d;
  return d;
}


// (i,j) is in E' iff j is reachable from i
// Hmm, is_reachable does not detect when there is a non-trivial path
// from i to i. It always returns true for is_reachable(i,i).
// This needs to be fixed/worked around.
template <typename Graph, typename GraphTC>
bool check_transitive_closure(Graph& g, GraphTC& tc)
{
  typename graph_traits<Graph>::vertex_iterator i, i_end;
  for (tie(i, i_end) = vertices(g); i != i_end; ++i) {
    typename graph_traits<Graph>::vertex_iterator j, j_end;
    for (tie(j, j_end) = vertices(g); j != j_end; ++j) {
      bool g_has_edge;
      typename graph_traits<Graph>::edge_descriptor e_g;
      typename graph_traits<Graph>::degree_size_type num_tc;
      tie (e_g, g_has_edge) = edge(*i, *j, g);
      num_tc = num_incident(*i, *j, tc);
      if (*i == *j) {
        if (g_has_edge) {
          if (num_tc != 1)
            return false;
        } else {
          bool can_reach = false;
          typename graph_traits<Graph>::adjacency_iterator k, k_end;
          for (tie(k, k_end) = adjacent_vertices(*i, g); k != k_end; ++k) {
            std::vector<default_color_type> color_map_vec(num_vertices(g));
            if (is_reachable(*k, *i, g, &color_map_vec[0])) {
              can_reach = true;
              break;
            }
          }
          if (can_reach) {
            if (num_tc != 1) {
              std::cout << "1. " << *i << std::endl;
              return false;
            }
          } else {
            if (num_tc != 0) {
              std::cout << "2. " << *i << std::endl;
              return false;
            }
          }       
        }
      } else {
        std::vector<default_color_type> color_map_vec(num_vertices(g));
        if (is_reachable(*i, *j, g, &color_map_vec[0])) {
          if (num_tc != 1)
            return false;
        } else {
          if (num_tc != 0)
            return false;
        }
      }
    }
  }
  return true;
}

bool test(int n, double p)
{
  vector< vector<int> > g1, g1_tc;
  generate_graph(n, p, g1);
  cout << "Created graph with " << n << " vertices.\n";

  vector< vector<int> > g1_c(g1);

  {
    progress_timer t;
    cout << "transitive_closure" << endl;
    transitive_closure(g1, g1_tc, vertex_index_map(identity_property_map()));
  }

  if(check_transitive_closure(g1, g1_tc))
    return true;
  else {
    cout << "Original graph was ";
    print_graph(g1, identity_property_map());
    cout << "Result is ";
    print_graph(g1_tc, identity_property_map());
    return false;
  }
}


int main()
{
  srand(time(0));
  static class {
  public:
    double operator()() {
      return double(rand())/RAND_MAX;
    }
  } gen;  


  for (size_t i = 0; i < 100; ++i) {
    int n = 0 + int(20*gen());
    double p = gen();
    if (!test(n, p)) {
      cout << "Failed." << endl;
      return 1; 
    }
  }
  cout << "Passed." << endl;
}

