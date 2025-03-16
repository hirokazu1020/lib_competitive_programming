struct BitVector {
    len: usize,
    zeros: usize,
    bit: Vec<u64>,
    rank: Vec<usize>,
}

impl BitVector {
    fn new(n: usize) -> Self {
        BitVector {
            len: n,
            zeros: n,
            bit: vec![0; n / 64 + 1],
            rank: vec![0; n / 64 + 1],
        }
    }
    fn get(&self, p: usize) -> bool {
        (self.bit[p / 64] & 1 << p as u64 % 64) != 0
    }
    fn set(&mut self, p: usize) {
        self.bit[p / 64] |= 1 << p as u64 % 64;
    }
    fn build(&mut self) {
        for i in 1..self.bit.len() {
            self.rank[i] = self.rank[i - 1] + self.bit[i - 1].count_ones() as usize;
        }
        let last = self.rank.len() - 1;
        self.zeros = self.len - (self.rank[last] + self.bit[last].count_ones() as usize);
    }
    fn rank(&self, p: usize) -> usize {
        let rem = p % 64;
        let mask = (1 << rem) - 1;
        self.rank[p / 64] + (self.bit[p / 64] & mask).count_ones() as usize
    }
    fn zeros(&self) -> usize {
        self.zeros
    }
}

pub struct WaveletMatrix {
    len: usize,
    bit_len: usize,
    bv: Vec<BitVector>,
}

impl WaveletMatrix {
    pub fn new(vec: &Vec<u64>, bit_len: usize) -> Self {
        assert!(bit_len <= 64);
        let len = vec.len();
        let mut bv = std::iter::repeat_with(|| BitVector::new(len))
            .take(bit_len)
            .collect::<Vec<_>>();
        let mut v = vec.clone();
        for i in (0..bit_len).rev() {
            let mut zeros = Vec::with_capacity(len);
            let mut ones = Vec::with_capacity(len);
            for j in 0..v.len() {
                if Self::is_one(v[j], i) {
                    bv[i].set(j);
                }
            }
            bv[i].build();
            for x in &v {
                if Self::is_one(*x, i) {
                    ones.push(*x);
                } else {
                    zeros.push(*x);
                }
            }
            v.clear();
            for x in zeros {
                v.push(x);
            }
            for x in ones {
                v.push(x);
            }
        }
        WaveletMatrix {
            len: len,
            bit_len: bit_len,
            bv: bv,
        }
    }
    fn is_one(v: u64, p: usize) -> bool {
        (v & 1 << p) != 0
    }
    pub fn len(&self) -> usize {
        self.len
    }
    pub fn access(&self, p: usize) -> u64 {
        let (mut p, mut ret) = (p, 0);
        for i in (0..self.bit_len).rev() {
            if self.bv[i].get(p) {
                ret |= 1 << i;
                p = self.bv[i].zeros() + self.bv[i].rank(p);
            } else {
                p -= self.bv[i].rank(p);
            }
        }
        ret
    }
    pub fn rank(&self, s: usize, e: usize, v: u64) -> usize {
        let (mut s, mut e) = (s, e);
        for i in (0..self.bit_len).rev() {
            let (sr, er) = (self.bv[i].rank(s), self.bv[i].rank(e));
            if Self::is_one(v, i) {
                s = self.bv[i].zeros() + sr;
                e = self.bv[i].zeros() + er;
            } else {
                s -= sr;
                e -= er;
            }
        }
        e - s
    }
    pub fn rank_lt(&self, s: usize, e: usize, v: u64) -> usize {
        let (mut s, mut e) = (s, e);
        let mut ret = 0;
        for i in (0..self.bit_len).rev() {
            let (sr, er) = (self.bv[i].rank(s), self.bv[i].rank(e));
            if Self::is_one(v, i) {
                let num0 = e - s - (er - sr);
                ret += num0;
                s = self.bv[i].zeros() + sr;
                e = self.bv[i].zeros() + er;
            } else {
                s -= sr;
                e -= er;
            }
        }
        ret
    }
    pub fn rank_range(&self, s: usize, e: usize, l: u64, r: u64) -> usize {
        self.rank_lt(s, e, r) - self.rank_lt(s, e, l)
    }
    pub fn select(&self, _k: usize, _r: u64) -> usize {
        unimplemented!("nya-----!");
    }
    pub fn quantile(&self, s: usize, e: usize, k: usize) -> u64 {
        let (mut s, mut e, mut k) = (s, e, k);
        let mut ret = 0;
        for i in (0..self.bit_len).rev() {
            let (sr, er) = (self.bv[i].rank(s), self.bv[i].rank(e));
            let num0 = e - s - (er - sr);
            if num0 <= k {
                k -= num0;
                ret |= 1 << i;
                s = self.bv[i].zeros() + sr;
                e = self.bv[i].zeros() + er;
            } else {
                s -= sr;
                e -= er;
            }
        }
        ret
    }
}

#[test]
fn test() {
    let vec = vec![3, 7, 2, 7, 3, 8, 4, 5, 5, 7, 8];
    let wm = WaveletMatrix::new(&vec, 4);
    assert_eq!(
        vec,
        (0..vec.len()).map(|i| wm.access(i)).collect::<Vec<_>>()
    );
    assert_eq!(1, wm.rank(2, 6, 7));
}
