#ifndef FEATURESET_H
#define FEATURESET_H

#include <cstdlib>
#include <vector>

/**
 * @brief The feature_set Struct
 * This struct stores the ctBranch's features calculated during the topological
 * analysis process.
 */
typedef struct feature_set
{
  size_t v; // Branch volume (number of voxels).
  size_t p; // Branch persistence (intensity difference between extremum and saddle).
  size_t hv; // Branch hyper-volume (sum of all voxel's intensities).
  size_t depth;
  size_t num_children;

  int label;   // Branch label. Used to save the transfer functions in the right order.
  double porosity;
  std::vector<size_t> vertices;
  size_t centroid; // Linear index of the branch's centroid.
  bool visited; // Used during the simplification.

  bool remove; // If set to true, the branch is removed from the tree.
  double delta_h; // (non leaf nodes).
  double* alpha; // Opacity transfer function.
  double alpha_i; // Initial alpha value (non leaf nodes).
  double alpha_i_j; //
  double alpha_lo; // Final lower opacity value band.
  double alpha_hi; // Final upper opacity value band.
  double delta_alpha_i; // Residual opacity.
  double c_s_min; // Children minimum saddle value.
  double c_s_max; // Children maximum saddle value.

  size_t min_intensity; // Maximum scalar value of the vertices.
  size_t max_intensity; // Minimum scalar value of the vertices.

  double norm_v;
  double norm_p;
  double norm_hv;
} FeatureSet;

#endif // FEATURESET_H
