pub struct FenwickTree {
    bit: Vec<i64>,
}

impl FenwickTree {
    pub fn new(n: usize) -> Self {
        FenwickTree {
            bit: vec![0; n + 1],
        }
    }
    pub fn add(&mut self, i: usize, x: i64) {
        let mut i = i + 1;
        while i <= self.bit.len() {
            self.bit[i] += x;
            i += i & (!i + 1);
        }
    }
    pub fn sum(&self, i: usize) -> i64 {
        let mut i = i + 1;
        let mut ret = 0;
        while 0 < i {
            ret += self.bit[i];
            i -= i & (!i + 1);
        }
        ret
    }
}

pub struct RangeFenwickTree {
    bit0: FenwickTree,
    bit1: FenwickTree,
}

impl RangeFenwickTree {
    pub fn new(n: usize) -> Self {
        RangeFenwickTree {
            bit0: FenwickTree::new(n),
            bit1: FenwickTree::new(n),
        }
    }
    pub fn add(&mut self, l: usize, r: usize, x: i64) {
        self.bit0.add(l, -x * l as i64);
        self.bit0.add(r, x * r as i64);
        self.bit1.add(l, x);
        self.bit1.add(r, -x);
    }
    pub fn sum(&self, i: usize) -> i64 {
        self.bit0.sum(i) + (i as i64 + 1) * self.bit1.sum(i)
    }
}

#[test]
fn test() {
    let mut b = FenwickTree::new(10);
    b.add(9, 1);
    b.add(5, 2);
    let expected = vec![0, 0, 0, 0, 0, 2, 2, 2, 2, 3];
    assert_eq!(
        expected,
        (0..expected.len()).map(|x| b.sum(x)).collect::<Vec<_>>()
    );
}
