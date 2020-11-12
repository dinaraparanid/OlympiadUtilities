use std::collections::VecDeque;

const INF: u32 = std::u32::MAX;

/// Graph trait with functions

trait Graph {
    fn new(vertices: usize, edges: usize) -> Self;
    fn bfs(&mut self, v: usize) -> Vec<u32>;
    fn dfs(&mut self, v: usize);
}

/// Matrix variation of Graph

#[derive(Debug, Clone)]
pub struct MatrixGraph {
    gr: Vec<Vec<bool>>,
    visited: Vec<bool>,
}

/// Implementing functions from Graph trait

impl Graph for MatrixGraph {
    /// Method **new()** creates new graph by reading all input data.
    /// Input data should be as in the example:
    ///
    /// 5 3 (amount of vertices and connections)
    /// 1 3 ( zero is connected with 2-nd)
    /// 2 3
    /// 1 2
    ///
    /// Graph will looks like this:
    ///
    ///     0
    ///   /  \
    ///  1 —  2

    fn new(vertices: usize, edges: usize) -> Self {
        let mut graph = vec![vec![false; vertices]; vertices];

        for _ in 0..edges {
            let mut input = String::new();
            std::io::stdin().read_line(&mut input).unwrap();
            let mut input = input.split_ascii_whitespace();

            let x = input.next().unwrap().parse::<usize>().unwrap() - 1;
            let y = input.next().unwrap().parse::<usize>().unwrap() - 1;

            unsafe {
                *graph.get_unchecked_mut(x).get_unchecked_mut(y) = true;
                *graph.get_unchecked_mut(y).get_unchecked_mut(x) = true;
            }
        }

        MatrixGraph {
            gr: graph,
            visited: vec![false; vertices],
        }
    }

    /// Breadth First Search
    /// Return vector with distances from some vertex.
    /// If value is equal to u32::MAX than there is no way
    /// to get from out vertex to searchable

    fn bfs(&mut self, v: usize) -> Vec<u32> {
        let mut queue = VecDeque::<usize>::with_capacity(self.visited.len());
        queue.push_back(v);

        let mut length = vec![INF; self.visited.len()];

        unsafe {
            *length.get_unchecked_mut(v) = 0;
        }

        while !queue.is_empty() {
            let u = *queue.front().unwrap();
            queue.pop_front();

            for j in 0..self.visited.len() {
                unsafe {
                    if *self.gr.get_unchecked(u).get_unchecked(j) && *length.get_unchecked(j) == INF
                    {
                        *length.get_unchecked_mut(j) = *length.get_unchecked(j) + 1;
                        queue.push_back(j);
                    }
                }
            }
        }

        length
    }

    /// Deep First Search

    fn dfs(&mut self, v: usize) {
        unsafe {
            *self.visited.get_unchecked_mut(v) = true;

            for elem in 0..self.gr.len() {
                if !*self.visited.get_unchecked(elem)
                    && *self.gr.get_unchecked(v).get_unchecked(elem)
                {
                    self.dfs(elem);
                }
            }
        }
    }
}

/// Constructor from another matrix
/// It moves matrix

impl From<Vec<Vec<bool>>> for MatrixGraph {
    fn from(graph: Vec<Vec<bool>>) -> Self {
        MatrixGraph {
            visited: vec![false; graph.len()],
            gr: graph,
        }
    }
}
