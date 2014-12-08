#ifndef SIMPLIFICATION_H
#define SIMPLIFICATION_H

#include "topdata.h"
#include "topmesh.h"
#include "featureset.h"

extern "C"
{
#include <tourtre.h>
}

/**
 * @brief topSimplifyTree
 * This function applies the simplification operation on the contour tree.
 * 
 * The simplification is follows a simple algorithm:
 * do {
 *   queue all unvisited leaves of the contour tree.
 *   sort the leaves by their importance.
 *   find the first leaf with an importance lower than the threshold.
 *   for every leaf after this point
 *     merge their features with their parents' features
 *     delete the branch from the tree.
 *   mark the other leaves as visited.
 * } while there are unvisited leaves.
 *
 * @param ctx The libtourtre context. Allows access to the branches' delete function.
 * @param root_branch The branch to start the simplification. Usually the root of the tree.
 * @param dataset A reference to the dataset. Allows access to the voxels.
 * @param importance_cb The importance calculation callback.
 * @param threshold The simplifcation threshold.
 */
void topSimplifyTree(ctContext*, ctBranch*, top::Dataset&, double(*importance_cb)(ctBranch*), double);

void topSimplifyTreeZhou(ctContext*, ctBranch*, ctBranch**, top::Dataset&, double(*importance_cb)(ctBranch*), double);

#endif // SIMPLIFICATION_H
