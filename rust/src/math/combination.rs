pub struct Combination {
    modv: u64,
    inv: Vec<u64>,
    fact: Vec<u64>,
    invfact: Vec<u64>,
}

impl Combination {
    pub fn new(n: usize, modv: u64) -> Self {
        let mut inv = vec![0; n + 1];
        let mut fact = vec![0; n + 1];
        let mut invfact = vec![0; n + 1];
        inv[0] = 0 as u64;
        inv[1] = 1 as u64;
        for i in 2..(n + 1) {
            inv[i] = inv[modv as usize % (i as usize)] * (modv - modv / i as u64) as u64 % modv;
        }
        fact[0] = 1 as u64;
        invfact[0] = 1 as u64;
        for i in 1..(n + 1) {
            fact[i] = i as u64 * fact[i - 1] % modv;
            invfact[i] = invfact[i - 1] * inv[i] % modv;
        }

        Combination {
            modv: modv,
            inv: inv,
            fact: fact,
            invfact: invfact,
        }
    }
    pub fn comb(&self, n: usize, r: usize) -> u64 {
        if r > n {
            return 0;
        }
        self.fact[n] * self.invfact[n - r] % self.modv * self.invfact[r] % self.modv
    }
    pub fn inv(&self, n: usize) -> u64 {
        self.inv[n]
    }
    pub fn fact(&self, n: usize) -> u64 {
        self.fact[n]
    }
    pub fn inv_fact(&self, n: usize) -> u64 {
        self.invfact[n]
    }
}
