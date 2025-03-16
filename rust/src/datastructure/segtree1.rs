/// 一点更新　範囲クエリ
/// ref: http://codeforces.com/blog/entry/18051

pub struct Segtree<T, E, OP> {
    n: usize,
    v: Vec<T>,
    e: E,
    op: OP,
}

impl<T, E, OP> Segtree<T, E, OP>
where
    E: Fn() -> T,
    OP: Fn(&T, &T) -> T,
{
    pub fn new(n: usize, e: E, op: OP) -> Self {
        let v = std::iter::repeat_with(|| e())
            .take(2 * n)
            .collect::<Vec<_>>();
        Segtree {
            n: n,
            v: v,
            e: e,
            op: op,
        }
    }
    pub fn set(&mut self, p: usize, x: T) {
        let mut p = p + self.n;
        self.v[p] = x;
        while p > 1 {
            self.v[p >> 1] = (self.op)(&self.v[p], &self.v[p ^ 1]);
            p >>= 1;
        }
    }
    pub fn prod(&self, l: usize, r: usize) -> T {
        let mut ret = (self.e)();
        let (mut l, mut r) = (l + self.n, r + self.n);
        while l < r {
            if l & 1 != 0 {
                ret = (self.op)(&ret, &self.v[l]);
                l += 1;
            }
            if r & 1 != 0 {
                r -= 1;
                ret = (self.op)(&ret, &self.v[r]);
            }
            l >>= 1;
            r >>= 1;
        }
        ret
    }
}

#[test]
fn test_rmq() {
    let e = || i32::max_value();
    let mut seg = Segtree::new(5, e, |a, b| *std::cmp::min(a, b));
    seg.set(0, 1);
    seg.set(2, 3);
    assert_eq!(1, seg.prod(0, 5));
    assert_eq!(3, seg.prod(2, 5));
}
