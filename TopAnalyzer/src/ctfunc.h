#ifndef CTFUNC_H
#define CTFUNC_H

#include "topdata.h"
#include "topmesh.h"
#include "featureset.h"

extern "C"
{
#include <tourtre.h>
}

#ifdef  __cplusplus
extern "C" {
#endif

/**
 * @brief The TFShape enum
 * This enumeration lists the possible shapes that a transfer function may
 * assume.
 */
enum TFShape
{
  ///y = ax + b. Standard linear growth.
  LINEAR,
  ///0.45 to 0.55 is different than 0. Max at 0.5.
  HAT,
  ///Linear grow up to 0.5 linear decrease afterwards.
  TRIANGLE,
  ///Linear growth until 0.45. Max value to the end.
  TRAPEZOID,
};

/**
 * @brief count_branches Counts the number of branches in the tree.
 * @param b The root branch.
 * @return The number of branches.
 */
size_t count_branches(ctBranch* root);

/**
 * @brief calc_branch_features Calculates the branch's features (volume,
 * persistence and hypervolume).
 * @param bmap The vertex to branch map provided by libtourtre.
 * @param data The analyzed dataset.
 */
void calc_branch_features(ctBranch** bmap, top::Dataset* data);

/**
 * @brief calc_avg_importance Calculates the average importance of the tree
 * given a callback to the importance function.
 * @param b The root branch.
 * @param importance_measure The callback to calculate a branch's importance.
 * @return The tree's average importance.
 */
double calc_avg_importance(ctBranch* b, double (*importance_measure)(ctBranch*));

/**
 * @brief calc_branch_depth Calculates a branch's depth.
 * @param b The current branch.
 * @param max_depth The tree's maximum depth.
 * @param curr_depth The current depth.
 */
void calc_branch_depth(ctBranch* b, size_t& max_depth, size_t curr_depth);

/**
 * @brief calc_branch_num_children Calculates the number of child branches of a
 * given branch.
 * @param b The current branch.
 */
void calc_branch_num_children(ctBranch* b);

/**
 * @brief calc_centroid Calculates the centroid of the voxels belonging to a
 * branch.
 * @param b The current branch.
 * @param data The data being analyzed.
 */
void calc_centroid(ctBranch* b, top::Dataset* data);

/**
 * @brief normalize_features Normalizes the branch's features. Needed by the
 * opacity flow step.
 * @param b The current branch.
 */
void normalize_features(ctBranch* b);

/**
 * @brief label_branches Labels the branches to save them in a 2D textures that
 * will be the transfer functions (not used).
 * @param b The root branch.
 * @return The last label attributed.
 */
int label_branches(ctBranch* b);

/**
 * @brief zero_branches Initializes the data field of the contour tree to 0.
 * @param b The root of the branch decomposition.
 */
void zero_branches(ctBranch* b);

/**
 * @brief calc_vertices_branch Iterates through the branch map and stores the
 * corresponding vertex indices in the branch. This speeds up the merging of two
 * branches during the simplification.
 * @param bmap The branch map given by libtourtre.
 * @param bmap_size The size of the branch map.
 */
void calc_vertices_branch(ctBranch** bmap, size_t bmap_size);

/**
 * @brief rebuild_branch_map Rebuilds the branch map to keep the data structures
 * consistent. This is done by iterating on the contour tree and reassigning the
 * corresponding vertices to each branch.
 * @param root_branch The contour tree's root branch.
 * @param bmap The branch map to be updated.
 */
void rebuild_branch_map(ctBranch* root_branch, ctBranch** bmap);

//Residue flow functions

/**
 * @brief calc_residue_flow Applies the Darcy's Law to flow the opacity through
 * the tree.
 * @param root_branch The tree's root branch.
 * @param alpha_d The alpha bandwidth for each level. If the max alpha is 1 and
 * there are 4 depth levels, then this value will be 0.25.
 * @param rate_Q The rate to flow the opacity.
 * @param data The dataset.
 * @return true if the opacity was properly flown, false otherwise.
 */
bool calc_residue_flow(ctBranch* root_branch, double alpha_d, double rate_Q, top::Dataset* data);

/**
 * @brief calc_saddle_min_max Calculates the maximum and minimum saddle values
 * of the branch's children.
 * @param root_branch The tree's root branch.
 * @param data The pointer to the dataset.
 */
void calc_saddle_min_max(ctBranch* root_branch, top::Dataset* data);

/**
 * @brief calc_gsd Calculates the gsd value proposed by Zhou, 2009.
 * @param b The current branch
 * @param data Pointer to the data
 * @return The gsd value
 */
double calc_gsd(ctBranch* b, top::Dataset* data);

/**
 * @brief half_std_avg_importance_normalized Calculates the half of the
 * importance of the normalized values.
 * @param b The current branch.
 * @return Half the importance measure of the normalized values.
 */
double half_std_avg_importance_normalized(ctBranch* b);

/**
 * @brief calc_alpha_sum Sums the alpha values of the branch's parents up to the
 * root.
 * @param b The current branch.
 * @return The accumulated alpha.
 */
double calc_alpha_sum(ctBranch* b);

/**
 * @brief calc_final_alpha Applies the transfer function shape to the alpha
 * values and calculates the final transfer function for each branch.
 * @param b The current branch.
 * @param shape The desired shape.
 * @return The array containing the transfer function ready for use.
 */
double* calc_final_alpha(ctBranch* b, TFShape shape);

//Importance measure callbacks.
/**
 * @brief std_avg_importance Callback to calculate the average importance of the
 * contour tree in linear scale.
 * @param root_branch The contour tree's root branch.
 * @return The tree's average importance in linear scale.
 */
double std_avg_importance(ctBranch* root_branch);

/**
 * @brief std_log_importance Callback to calculate the average importance of the
 * contour tree in logarithmic scale.
 * @param root_branch The contour tree's root branch.
 * @return The tree's average importance in log scale.
 */
double std_log_importance(ctBranch* root_branch);

//Callback functions provided to libtourtre.
/**
 * @brief std_value Returns the intensity value of a voxel.
 * @param v The voxel's linear address.
 * @param d Data field (ignored).
 * @return The voxel's intensity.
 */
double std_value(size_t v, void* d);

/**
 * @brief std_neighbors Returns the neighbors of a voxel.
 * @param v The voxel's linear address.
 * @param nbrs The array to be filled with the neighbors.
 * @param d Data field (ignored).
 * @return The size of the neighbors array (nbrs).
 */
size_t std_neighbors(size_t v, size_t* nbrs, void* d);

/**
 * @brief vertex_proc Callback called when a vertex is being processed by
 * libtourtre.
 * @param v The vertex's linear address.
 * @param a Pointer to the inciding arc.
 * @param d Data field (ignored).
 */
void vertex_proc(size_t v, ctArc* a, void* d);

/**
 * @brief arc_merge_proc Procedure called when two arcs will be merged by
 * libtourtre.
 * @param a The arc that will remain.
 * @param b The arc to be deleted.
 * @param d Data field (ignored).
 */
void arc_merge_proc(ctArc* a, ctArc* b, void* d);

/**
 * @brief arc_priority_proc Callback to calculate the priority of an Arc during
 * the contour tree's construction. According to the library's documentation, it
 * will influence on the Arc's position.
 * @param leaf The leaf node indiced by the arc.
 * @param d Data field (ignored).
 * @return The Arc's importance.
 */
double arc_priority_proc(ctNode* leaf, void* d);

#ifdef  __cplusplus
}
#endif

#endif // CTFUNC_H
