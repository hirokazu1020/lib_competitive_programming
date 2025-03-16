/// 範囲更新　一点取得
/// ref: http://codeforces.com/blog/entry/18051

pub struct Segtree<T, E, MODI, OP> {
    n: usize,
    v: Vec<T>,
    e: E,
    modify: MODI,
    summary: OP,
}

impl<T, E, MODI, OP> Segtree<T, E, MODI, OP>
where
    T: Clone + Copy,
    E: Fn() -> T,
    MODI: Fn(&T, &T) -> T,
    OP: Fn(&T, &T) -> T,
{
    pub fn new(n: usize, e: E, modify: MODI, summary: OP) -> Self {
        let v = std::iter::repeat_with(|| e())
            .take(2 * n)
            .collect::<Vec<_>>();
        Segtree {
            n: n,
            v: v,
            e: e,
            modify: modify,
            summary: summary,
        }
    }
    pub fn modify(&mut self, l: usize, r: usize, x: T) {
        let (mut l, mut r) = (l + self.n, r + self.n);
        while l < r {
            if l & 1 != 0 {
                self.v[l] = (self.modify)(&self.v[l], &x);
                l += 1;
            }
            if r & 1 != 0 {
                r -= 1;
                self.v[r] = (self.modify)(&self.v[r], &x);
            }
            l >>= 1;
            r >>= 1;
        }
    }
    pub fn get(&self, p: usize) -> T {
        let mut ret = (self.e)();
        let mut p = p + self.n;
        while p > 0 {
            ret = (self.summary)(&ret, &self.v[p]);
            p >>= 1;
        }
        ret
    }
}

#[test]
fn test_assign_range() {
    #[derive(Clone, Copy)]
    struct AssignStruct<T: Clone + Copy> {
        epoch: usize,
        val: T,
    }

    let mut epoch: usize = 0;
    let mut seg = Segtree::new(
        8,
        || AssignStruct { epoch: 0, val: 1 },
        |a, b| if a.epoch < b.epoch { *b } else { *a },
        |a, b| if a.epoch < b.epoch { *b } else { *a },
    );
    assert_eq!(
        vec![1, 1, 1, 1, 1, 1, 1, 1],
        (0..8).map(|i| seg.get(i).val).collect::<Vec<_>>()
    );

    epoch += 1;
    seg.modify(
        1,
        4,
        AssignStruct {
            epoch: epoch,
            val: 2,
        },
    );
    assert_eq!(
        vec![1, 2, 2, 2, 1, 1, 1, 1],
        (0..8).map(|i| seg.get(i).val).collect::<Vec<_>>()
    );

    epoch += 1;
    seg.modify(
        3,
        6,
        AssignStruct {
            epoch: epoch,
            val: 3,
        },
    );
    assert_eq!(
        vec![1, 2, 2, 3, 3, 3, 1, 1],
        (0..8).map(|i| seg.get(i).val).collect::<Vec<_>>()
    );
}
