#ifndef TOPSEARCH_HPP
#define TOPSEARCH_HPP

#include "ctBranch.h"

#include <functional>
#include <utility>
#include <queue>
#include <stack>

/**
 * @brief Depth first search function.
 *
 * This function executes a depth first search on a contour tree. For each node
 * visited, a callback function is called and passed the node's pointer and the
 * parameters passed to the function.
 *
 * @param root_branch The root of the tree.
 * @param fn The callback function to be called for each node.
 * @param args The argument to pass to the callback.
 */
template <typename Fn, typename... Args>
void DFS(ctBranch* root_branch, Fn&& fn, Args&&... args)
{
  if(root_branch == NULL) return;

  std::stack<ctBranch*> stack;
  stack.push(root_branch);

  do {
    ctBranch* branch = stack.top();
    stack.pop();

    auto bound_fn = std::bind(std::forward<Fn>(fn), branch, std::forward<Args>(args)...);
    bound_fn();

    for(ctBranch* c = branch->children.head; c != NULL; c = c->nextChild) {
      stack.push(c);
    }

  } while(!stack.empty());

}

/**
 * @brief Breadth first search function.
 *
 * This function executes a breadth first search on a contour tree. For each
 * node visited, a callback function is called and passed the node's pointer and
 * the parameters passed to the function.
 *
 * @param root_branch The root of the tree.
 * @param fn The callback function to be called for each node.
 * @param args The argument to pass to the callback.
 */
template <typename Fn, typename... Args>
void BFS(ctBranch* root_branch, Fn&& fn, Args&&... args)
{
  if(root_branch == NULL) return;

  std::queue<ctBranch*> queue;
  queue.push(root_branch);

  do {
    ctBranch* branch = queue.front();
    queue.pop();

    auto bound_fn = std::bind(std::forward<Fn>(fn), branch, std::forward<Args>(args)...);
    bound_fn();

    for(ctBranch* c = branch->children.head; c != NULL; c = c->nextChild) {
      queue.push(c);
    }

  } while(!queue.empty());

}

#endif // TOPSEARCH_HPP
