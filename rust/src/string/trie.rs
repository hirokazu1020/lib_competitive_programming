pub struct Trie {
    begin: Vec<usize>,
    edge: Vec<u8>,
    contain: Vec<bool>,
}

impl Trie {
    const NOT_FOUND: usize = usize::max_value();
    pub fn new(s: &mut Vec<Vec<u8>>) -> Self {
        s.sort();
        let mut lcp: Vec<usize> = vec![0; s.len()];
        for i in 1..s.len() {
            let size: usize = std::cmp::min(s[i - 1].len(), s[i].len());
            for j in 0..size {
                if s[i - 1][j] != s[i][j] {
                    break;
                }
                lcp[i] += 1;
            }
        }
        let nodes: usize = (0..s.len()).fold(0, |sum, i| sum + s[i].len() - lcp[i]);
        let mut begin: Vec<usize> = Vec::with_capacity(nodes + 2);
        let mut edge: Vec<u8> = Vec::with_capacity(nodes);
        let mut contain: Vec<bool> = Vec::with_capacity(nodes + 1);
        begin.push(0);
        contain.push(false);

        let mut rows: std::collections::VecDeque<usize> = (0..s.len()).collect();
        let mut cur_node_id: Vec<usize> = vec![0; s.len()];
        let mut nxt_node_id: Vec<usize> = vec![0; s.len()];
        for col in 0.. {
            let size = rows.len();
            if size == 0 {
                break;
            }
            for _ in 0..size {
                let row = rows.pop_front().unwrap();
                let parent = cur_node_id[row];
                if col == s[row].len() {
                    contain[parent] = true;
                    continue;
                }
                rows.push_back(row);
                if lcp[row] <= col {
                    if begin[parent] == Self::NOT_FOUND {
                        begin[parent] = edge.len();
                    }
                    edge.push(s[row][col]);
                    begin.push(Self::NOT_FOUND);
                    contain.push(false);
                }
                nxt_node_id[row] = begin.len() - 1;
            }
            let tmp = cur_node_id;
            cur_node_id = nxt_node_id;
            nxt_node_id = tmp;
        }
        begin.push(edge.len());
        for i in (0..begin.len() - 1).rev() {
            if begin[i] == Self::NOT_FOUND {
                begin[i] = begin[i + 1];
            }
        }
        Trie {
            begin: begin,
            edge: edge,
            contain: contain,
        }
    }
    pub fn contains(&self, s: &Vec<u8>) -> bool {
        let mut node_id = 0;
        for &ch in s {
            node_id = self.find_child(node_id, &ch);
            if node_id == Self::NOT_FOUND {
                return false;
            }
        }
        self.contain[node_id]
    }
    pub fn find_child(&self, node_id: usize, ch: &u8) -> usize {
        let mut range = self.begin[node_id]..self.begin[node_id + 1];
        if range.len() <= 8 {
            match range.find(|i| self.edge[*i] == *ch) {
                Some(index) => index + 1,
                None => Self::NOT_FOUND,
            }
        } else {
            match self.edge[range].binary_search(ch) {
                Ok(index) => self.begin[node_id] + index + 1,
                Err(_msg) => Self::NOT_FOUND,
            }
        }
    }
    pub fn node_size(&self) -> usize {
        self.contain.len()
    }
}


#[test]
fn test() {
    let mut s = vec![vec![1, 2, 3], vec![4, 5, 6, 7], vec![1, 2, 8]];
    let trie = Trie::new(&mut s);
    assert!(trie.contains(&mut vec![1, 2, 3]));
    assert!(trie.contains(&mut vec![4, 5, 6, 7]));
    assert!(trie.contains(&mut vec![1, 2, 8]));
    assert!(!trie.contains(&mut vec![1, 2, 3, 4]));
}

#[test]
fn test_empty() {
    let mut s = vec![];
    let trie = Trie::new(&mut s);
    assert!(!trie.contains(&vec![1, 2, 3]));
}

#[test]
fn test_large() {
    let mut s: Vec<Vec<u8>> = vec![];
    for i in 0u32..(1 << 16) {
        s.push(vec![(i & 0xFF) as u8, (i >> 8 & 0xFF) as u8]);
    }
    s.push(vec![0; 1_00_000]);
    let trie = Trie::new(&mut s);
    assert!(trie.contains(&vec![0; 1_00_000]));
}
