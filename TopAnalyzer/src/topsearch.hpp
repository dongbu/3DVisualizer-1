#ifndef TOPSEARCH_HPP
#define TOPSEARCH_HPP

#include "ctBranch.h"

#include <functional>
#include <utility>
#include <queue>
#include <stack>

template <typename Fn, typename... Args>
void DFS(ctBranch* root_branch, Fn&& fn, Args&&... args)
{
  if(root_branch == NULL) return;

  std::stack<ctBranch*> stack;
  stack.push(root_branch);

  do {
    ctBranch* branch = stack.top();
    stack.pop();

    std::bind(std::forward<Fn>(fn), branch, std::forward<Args>(args)...)();

    for(ctBranch* c = branch->children.head; c != NULL; c = c->nextChild) {
      stack.push(c);
    }

  } while(!stack.empty());

}

template <typename Fn, typename... Args>
void BFS(ctBranch* root_branch, Fn&& fn, Args&&... args)
{
  if(root_branch == NULL) return;

  std::queue<ctBranch*> queue;
  queue.push(root_branch);

  do {
    ctBranch* branch = queue.front();
    queue.pop();

    std::bind(std::forward<Fn>(fn), branch, std::forward<Args>(args)...)();

    for(ctBranch* c = branch->children.head; c != NULL; c = c->nextChild) {
      queue.push(c);
    }

  } while(!queue.empty());

}

#endif // TOPSEARCH_HPP
