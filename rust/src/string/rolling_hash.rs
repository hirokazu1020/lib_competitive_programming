pub struct RollingHash {
    hash: Vec<u64>,
    inv: Vec<u64>,
}

impl RollingHash {
    //const MOD: u64 = 1_000_000_007;

    fn inverse(mut x: u64) -> u64 {
        let mut ret: u64 = 1;
        for _i in 0..64 {
            ret = ret.wrapping_mul(x);
            x = x.wrapping_mul(x);
        }
        ret
    }
    pub fn new<T>(text: &[T], base: u64) -> Self
    where
        T: Copy,
        u64: std::convert::From<T>,
    {
        assert!(base % 2 == 1);
        let inv = RollingHash::inverse(base);
        let mut b = base;
        let mut h_i = 0u64;
        let mut inv_i = 1u64;
        let mut hash: Vec<u64> = Vec::with_capacity(text.len() + 1);
        let mut invs: Vec<u64> = Vec::with_capacity(text.len() + 1);
        hash.push(h_i);
        invs.push(inv_i);
        for ch in text {
            b = b.wrapping_mul(base);
            h_i = b.wrapping_mul(u64::from(*ch)).wrapping_add(h_i);
            hash.push(h_i);
            inv_i = inv_i.wrapping_mul(inv);
            invs.push(inv_i);
        }
        RollingHash {
            hash: hash,
            inv: invs,
        }
    }
    // [s, e)
    pub fn hash(&self, s: usize, e: usize) -> u64 {
        (self.hash[e].wrapping_sub(self.hash[s])).wrapping_mul(self.inv[s])
    }
}

#[test]
fn test_inv() {
    let base = 17;
    let inv = RollingHash::inverse(base);
    assert_eq!(1, base.wrapping_mul(inv));
}

#[test]
fn test_hash() {
    let s: Vec<u8> = b"abracadabra".to_vec();
    let rh = RollingHash::new(&s, 1_000_000_007);
    assert_eq!(rh.hash(0, 4), rh.hash(7, 11));
    assert_ne!(rh.hash(0, 3), rh.hash(7, 11));
}

//http://codeforces.com/problemset/problem/271/D
//https://atcoder.jp/contests/abc141/tasks/abc141_e Rust1.15
