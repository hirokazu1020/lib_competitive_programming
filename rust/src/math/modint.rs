const MOD: u32 = 1_000_000_007;

#[derive(Debug, Copy, Clone)]
pub struct Modint {
    val: u32,
}

impl Modint {
    pub fn new(v: u64) -> Self {
        Modint {
            val: (v % MOD as u64) as u32,
        }
    }
    pub fn raw(v: u32) -> Self {
        Modint { val: v }
    }
    pub fn val(&self) -> u32 {
        self.val
    }
    pub fn pow(&self, mut y: u64) -> Self {
        let mut ret = 1u64;
        let mut x = self.val as u64;
        while y > 0 {
            if (y & 1) != 0 {
                ret = ret * x % MOD as u64;
            }
            y >>= 1;
            x = x * x % MOD as u64;
        }
        Modint { val: ret as u32 }
    }
    pub fn inv(&self) -> Self {
        self.pow(MOD as u64 - 2)
    }
}

impl std::ops::Add<Modint> for Modint {
    type Output = Modint;
    fn add(self, _rhs: Modint) -> Modint {
        Modint {
            val: (self.val + _rhs.val) % MOD,
        }
    }
}
impl std::ops::Sub<Modint> for Modint {
    type Output = Modint;
    fn sub(self, _rhs: Modint) -> Modint {
        Modint {
            val: if self.val >= _rhs.val {
                self.val - _rhs.val
            } else {
                MOD + self.val - _rhs.val
            },
        }
    }
}
impl std::ops::Mul<Modint> for Modint {
    type Output = Modint;
    fn mul(self, _rhs: Modint) -> Modint {
        Modint::new(self.val as u64 * _rhs.val as u64)
    }
}
impl std::ops::Div<Modint> for Modint {
    type Output = Modint;
    fn div(self, _rhs: Modint) -> Modint {
        self * _rhs.inv()
    }
}

pub struct Combination {
    inv: Vec<Modint>,
    fact: Vec<Modint>,
    invfact: Vec<Modint>,
}

impl Combination {
    pub fn new(n: usize) -> Self {
        let mut fact = vec![Modint::raw(0); n + 1];
        fact[0] = Modint::raw(1);
        for i in 1..(n + 1) {
            fact[i] = Modint::raw(i as u32) * fact[i - 1];
        }
        let mut inv = vec![Modint::raw(0); n + 1];
        inv[1] = Modint::raw(1);
        for i in 2..(n + 1) {
            inv[i] = inv[MOD as usize % (i as usize)] * Modint::raw(MOD - MOD / i as u32);
        }
        let mut invfact = vec![Modint::raw(0); n + 1];
        invfact[0] = Modint::raw(1);
        for i in 1..(n + 1) {
            invfact[i] = invfact[i - 1] * inv[i];
        }
        Combination {
            inv: inv,
            fact: fact,
            invfact: invfact,
        }
    }
    pub fn inv(&self, n: usize) -> Modint {
        self.inv[n]
    }
    pub fn fact(&self, n: usize) -> Modint {
        self.fact[n]
    }
    pub fn inv_fact(&self, n: usize) -> Modint {
        self.invfact[n]
    }
    pub fn choose(&self, n: usize, r: usize) -> Modint {
        if r <= n {
            self.fact[n] * self.invfact[n - r] * self.invfact[r]
        } else {
            Modint::raw(0)
        }
    }
}

#[test]
fn test_mod() {
    let a = Modint::new(MOD as u64 + 1u64);
    let b = Modint::raw(2);
    let c = Modint::raw(MOD - 2);
    assert_eq!(1, a.val);
    assert_eq!(2, b.val);
    assert_eq!(3, (a + b).val);
    assert_eq!(2, (a * b).val);
    assert_eq!(4, (c * c).val);
    assert_eq!((a * Modint::raw(3).inv()).val, (a / Modint::raw(3)).val);
}
