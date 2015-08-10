/******************************************************************************
 * vertex_separator_algorithm.h 
 *
 * Source of KaHIP -- Karlsruhe High Quality Partitioning.
 *
 ******************************************************************************
 * Copyright (C) 2013-2015 Christian Schulz <christian.schulz@kit.edu>
 *
 * This program is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *****************************************************************************/

#ifndef VERTEX_SEPARTATOR_ALGORITHM_XUDNZZM8
#define VERTEX_SEPARTATOR_ALGORITHM_XUDNZZM8

#include <unordered_map>

#include "data_structure/graph_access.h"
#include "data_structure/flow_graph.h"
#include "partition_config.h"
#include "uncoarsening/refinement/quotient_graph_refinement/complete_boundary.h"

class vertex_separator_algorithm {
        public:
                vertex_separator_algorithm();
                virtual ~vertex_separator_algorithm();

                void build_flow_problem(const PartitionConfig & config, 
                                        graph_access & G, 
                                        std::vector< NodeID > & lhs_nodes,
                                        std::vector< NodeID > & rhs_nodes,
                                        std::vector< NodeID > & separator_nodes,
                                        flow_graph & rG, 
                                        std::vector< NodeID > & forward_mapping, 
                                        NodeID & source, NodeID & sink);
 
                void compute_vertex_separator(const PartitionConfig & config, 
                                              graph_access & G, 
                                              complete_boundary & boundary, 
                                              std::vector<NodeID> & overall_separator);

                NodeWeight improve_vertex_separator_internal(const PartitionConfig & config, 
                                              graph_access & G, 
                                              std::vector<NodeID> & input_separator,
                                              std::vector<NodeID> & output_separator);

                NodeWeight improve_vertex_separator(const PartitionConfig & config, 
                                              graph_access & G, 
                                              std::vector<NodeID> & input_separator,
                                              std::vector<NodeID> & output_separator);

                void compute_vertex_separator_simple(const PartitionConfig & config, 
                                                     graph_access & G, 
                                                     complete_boundary & boundary, 
                                                     std::vector<NodeID> & overall_separator);

                void compute_vertex_separator_simpler(const PartitionConfig & config, 
                                                     graph_access & G, 
                                                     complete_boundary & boundary, 
                                                     std::vector<NodeID> & overall_separator);

                void compute_vertex_separator(const PartitionConfig & config, 
                                              graph_access & G, 
                                              complete_boundary & boundary);

        private:
                void convert_residualGraph( graph_access & G, std::vector< NodeID > & forward_mapping, 
                                            NodeID & source, NodeID & sink, 
                                            flow_graph & rG, graph_access & residualGraph);

                //ASSERTIONS
                bool is_vertex_separator(graph_access & G, std::unordered_map<NodeID, bool> & separator);

};

inline
void vertex_separator_algorithm::convert_residualGraph( graph_access & G, std::vector< NodeID > & forward_mapping, 
                                                        NodeID & source, NodeID & sink, 
                                                        flow_graph & rG, graph_access & residualGraph) {

        residualGraph.start_construction(rG.number_of_nodes(), rG.number_of_edges());
        forall_nodes(rG, node) {
                NodeID node = residualGraph.new_node(); // for each node here create a new node 
                if( node != sink && node != source) {
                        residualGraph.setNodeWeight(node, G.getNodeWeight(forward_mapping[node]));
                }

                forall_out_edges(rG, e, node) {
                        NodeID target = rG.getEdgeTarget(node, e);
                        FlowType resCap = rG.getEdgeCapacity(node, e) - rG.getEdgeFlow(node, e);
                        if(resCap > 0) {
                                residualGraph.new_edge(node, target);
                        } else {
                                EdgeID e_bar = rG.getReverseEdge(node,e);
                                if(rG.getEdgeFlow(target, e_bar) > 0) {
                                        residualGraph.new_edge(node, target);
                                }
                        }
                } endfor
        } endfor

        residualGraph.setNodeWeight(source, 0);
        residualGraph.setNodeWeight(sink, 0);
        residualGraph.finish_construction();
}
#endif /* end of include guard: VERTEX_SEPARTATOR_ALGORITHM_XUDNZZM8 */
