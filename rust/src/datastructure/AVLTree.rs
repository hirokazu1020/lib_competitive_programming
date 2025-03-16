struct NodeType<K> {
    left: Option<Box<NodeType<K>>>,
    right: Option<Box<NodeType<K>>>,
    height: isize,
    len: usize,
    key: K,
}

impl<K: std::cmp::Ord> NodeType<K> {
    fn new(k: K) -> Self {
        NodeType {
            left: None,
            right: None,
            height: 1,
            len: 1,
            key: k,
        }
    }
    fn insert(node: Option<Box<NodeType<K>>>, key: K) -> (bool, Box<NodeType<K>>) {
        if let Some(mut u) = node {
            if key < u.key {
                let (success, left) = NodeType::insert(u.left, key);
                u.left = Some(left);
                (success, NodeType::balance(u))
            } else if u.key < key {
                let (success, right) = NodeType::insert(u.right, key);
                u.right = Some(right);
                (success, NodeType::balance(u))
            } else {
                (false, u)
            }
        } else {
            (true, Box::new(NodeType::<K>::new(key)))
        }
    }
    // TODO
    fn remove(_node: Option<Box<NodeType<K>>>, _key: &K) -> (bool, Option<Box<NodeType<K>>>) {
        unimplemented!("nya-----!");
    }
    fn contains(node: &Option<Box<NodeType<K>>>, key: &K) -> bool {
        if let Some(u) = node {
            if *key < u.key {
                NodeType::contains(&u.left, key)
            } else if u.key < *key {
                NodeType::contains(&u.right, key)
            } else {
                true
            }
        } else {
            false
        }
    }
    fn update(&mut self) {
        if let Some(left) = self.left.as_ref() {
            if let Some(right) = self.right.as_ref() {
                self.height = 1 + std::cmp::max(left.height, right.height);
                self.len = 1 + left.len + right.len;
            } else {
                self.height = 1 + left.height;
                self.len = 1 + left.len;
            }
        } else if let Some(right) = self.right.as_ref() {
            self.height = 1 + right.height;
            self.len = 1 + right.len;
        } else {
            self.height = 1;
            self.len = 1;
        }
    }
    fn height(u: &Option<Box<NodeType<K>>>) -> isize {
        if let Some(x) = u.as_ref() {
            x.height
        } else {
            0
        }
    }
    fn balance(mut u: Box<NodeType<K>>) -> Box<NodeType<K>> {
        let bf = NodeType::height(&u.left) - NodeType::height(&u.right);
        if bf == 2 {
            let mut v = u.left.unwrap();
            if NodeType::height(&v.left) < NodeType::height(&v.right) {
                v = NodeType::rotate_left(v);
            }
            u.left = Some(v);
            u = NodeType::rotate_right(u);
        } else if bf == -2 {
            let mut v = u.right.unwrap();
            if NodeType::height(&v.left) > NodeType::height(&v.right) {
                v = NodeType::rotate_right(v);
            }
            u.right = Some(v);
            u = NodeType::rotate_left(u);
        } else {
            u.update();
        }
        u
    }
    fn rotate_left(mut u: Box<NodeType<K>>) -> Box<NodeType<K>> {
        let mut v = u.right.unwrap();
        u.right = v.left;
        u.update();
        v.left = Some(u);
        v.update();
        v
    }
    fn rotate_right(mut u: Box<NodeType<K>>) -> Box<NodeType<K>> {
        let mut v = u.left.unwrap();
        u.left = v.right;
        u.update();
        v.right = Some(u);
        v.update();
        v
    }
}

pub struct AVLTree<K> {
    len: usize,
    root: Option<Box<NodeType<K>>>,
}

impl<K: std::cmp::Ord> AVLTree<K> {
    pub fn new() -> Self {
        AVLTree { len: 0, root: None }
    }
    pub fn insert(&mut self, key: K) -> bool {
        let mut tmp = None;
        std::mem::swap(&mut tmp, &mut self.root);
        let (success, node) = NodeType::insert(tmp, key);
        self.root = Some(node);
        success
    }
    /*pub fn remove(&mut self, key: &K) -> bool {
        let mut tmp = None;
        std::mem::swap(&mut tmp, &mut self.root);
        let (success, node) = NodeType::remove(tmp, key);
        self.root = node;
        success
    }*/
    pub fn contains(&self, key: &K) -> bool {
        NodeType::contains(&self.root, key)
    }
    pub fn len(&self) -> usize {
        if let Some(x) = self.root.as_ref() {
            x.len
        } else {
            0
        }
    }
    pub fn clear(&mut self) {
        self.len = 0;
        self.root = None;
    }
}
