use std::collections::VecDeque;
use std::fmt::{Display, Formatter};

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
                        *length.get_unchecked_mut(j) = *length.get_unchecked(u) + 1;
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

/// Display trait helps us to print out matrix

impl Display for MatrixGraph {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        let mut out = String::new();

        for vec in &self.gr {
            for elem in vec {
                out.push_str(if let true = *elem { "1 " } else { "0 " });
            }

            out.push('\n');
        }

        write!(f, "{}", out)
    }
}

#[test]
fn matrix_from_bfs_display_test() {
    let mut graph = MatrixGraph::from(vec![
        vec![false, true, false, false, false, false, false, false, false],
        vec![true, false, true, true, false, false, false, false, false],
        vec![false, true, false, false, false, false, false, false, false],
        vec![false, true, false, false, true, true, false, false, false],
        vec![false, false, false, true, false, true, false, false, false],
        vec![false, false, false, true, true, false, false, false, false],
        vec![false, false, false, false, false, false, false, true, false],
        vec![false, false, false, false, false, false, true, false, true],
        vec![false, false, false, false, false, false, false, true, false],
    ]);

    assert_eq!(
        format!("{}", graph),
        "0 1 0 0 0 0 0 0 0 \n\
        1 0 1 1 0 0 0 0 0 \n\
        0 1 0 0 0 0 0 0 0 \n\
        0 1 0 0 1 1 0 0 0 \n\
        0 0 0 1 0 1 0 0 0 \n\
        0 0 0 1 1 0 0 0 0 \n\
        0 0 0 0 0 0 0 1 0 \n\
        0 0 0 0 0 0 1 0 1 \n\
        0 0 0 0 0 0 0 1 0 \n"
    );

    assert_eq!(vec![0, 1, 2, 2, 3, 3, INF, INF, INF], graph.bfs(0));
}
