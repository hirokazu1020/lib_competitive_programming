pub mod treap {
    unsafe fn xor128() -> usize {
        static mut X: usize = 123456789;
        static mut Y: usize = 362436069;
        static mut Z: usize = 521288629;
        static mut W: usize = 88675123;
        let t = X ^ (X << 11);
        X = Y;
        Y = Z;
        Z = W;
        W = (W ^ (W >> 19)) ^ (t ^ (t >> 8));
        W
    }

    struct NodeType<K> {
        left: Option<Box<NodeType<K>>>,
        right: Option<Box<NodeType<K>>>,
        priority: usize,
        key: K,
    }

    impl<K: std::cmp::Ord> NodeType<K> {
        fn new(k: K) -> Self {
            NodeType {
                left: None,
                right: None,
                priority: unsafe { xor128() },
                key: k,
            }
        }
        fn insert(node: Option<Box<NodeType<K>>>, key: K) -> (bool, Box<NodeType<K>>) {
            if let Some(mut u) = node {
                if key < u.key {
                    let (success, left) = NodeType::insert(u.left, key);
                    u.left = Some(left);
                    if u.priority < u.left.as_ref().unwrap().priority {
                        u = NodeType::rotate_right(u);
                    }
                    (success, u)
                } else if u.key < key {
                    let (success, right) = NodeType::insert(u.right, key);
                    u.right = Some(right);
                    if u.priority < u.right.as_ref().unwrap().priority {
                        u = NodeType::rotate_left(u);
                    }
                    (success, u)
                } else {
                    (false, u)
                }
            } else {
                (true, Box::new(NodeType::<K>::new(key)))
            }
        }

        fn remove(node: Option<Box<NodeType<K>>>, key: &K) -> (bool, Option<Box<NodeType<K>>>) {
            if let Some(mut u) = node {
                if *key < u.key {
                    let (success, left) = NodeType::remove(u.left, key);
                    u.left = left;
                    (success, Some(u))
                } else if u.key < *key {
                    let (success, right) = NodeType::remove(u.right, key);
                    u.right = right;
                    (success, Some(u))
                } else {
                    if u.left.is_none() && u.right.is_none() {
                        (true, None)
                    } else if u.left.is_none() {
                        (true, u.right)
                    } else if u.right.is_none() {
                        (true, u.left)
                    } else {
                        u = NodeType::rotate_right(u);
                        u.right = NodeType::remove(u.right, key).1;
                        (true, Some(u))
                    }
                }
            } else {
                (false, None)
            }
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
        fn rotate_left(mut u: Box<NodeType<K>>) -> Box<NodeType<K>> {
            let mut v = u.right.unwrap();
            u.right = v.left;
            v.left = Some(u);
            v
        }
        fn rotate_right(mut u: Box<NodeType<K>>) -> Box<NodeType<K>> {
            let mut v = u.left.unwrap();
            u.left = v.right;
            v.right = Some(u);
            v
        }
    }

    pub struct Treap<K> {
        len: usize,
        root: Option<Box<NodeType<K>>>,
    }

    impl<K: std::cmp::Ord> Treap<K> {
        pub fn new() -> Self {
            Treap { len: 0, root: None }
        }
        pub fn insert(&mut self, key: K) -> bool {
            let mut tmp = None;
            std::mem::swap(&mut tmp, &mut self.root);
            let (success, node) = NodeType::insert(tmp, key);
            if success {
                self.len += 1
            }
            self.root = Some(node);
            success
        }
        pub fn remove(&mut self, key: &K) -> bool {
            let mut tmp = None;
            std::mem::swap(&mut tmp, &mut self.root);
            let (success, node) = NodeType::remove(tmp, key);
            if success {
                self.len -= 1
            }
            self.root = node;
            success
        }
        pub fn contains(&self, key: &K) -> bool {
            NodeType::contains(&self.root, key)
        }
        pub fn len(&self) -> usize {
            self.len
        }
        pub fn clear(&mut self) {
            self.len = 0;
            self.root = None;
        }
    }
}


#[test]
fn test() {
    let mut t = treap::Treap::<i32>::new();
    t.insert(2);
    assert_eq!(true, t.contains(&2));
    assert_eq!(false, t.contains(&3));
    assert_eq!(1, t.len());
    t.remove(&2);
    assert_eq!(0, t.len());
}

//https://atcoder.jp/contests/abc154/tasks/abc154_c
//https://atcoder.jp/contests/abc164/tasks/abc164_c
/*
fn main() {
    let mut scan = Scanner::new();
    let n: usize = scan.next();
    let mut set: Treap<String> = Treap::new();

    for _ in 0..n {
        let a: String = scan.next();
        set.insert(a);
    }
    println!("{}", set.len());
}
*/
