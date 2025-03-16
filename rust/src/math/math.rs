pub fn gcd(a: u64, b: u64) -> u64 {
    if b == 0 {
        a
    } else {
        gcd(b, a % b)
    }
}

pub fn is_prime(n: u64) -> bool {
    for i in 2.. {
        if n < i * i {
            break;
        }
        if n % i == 0 {
            return false;
        }
    }
    true
}

pub fn prime_factor(mut n: u64) -> Vec<(u64, usize)> {
    let mut res: Vec<(u64, usize)> = vec![];
    let mut i: u64 = 2;
    while i * i <= n {
        let mut c: usize = 0;
        while n % i == 0 {
            n /= i;
            c += 1;
        }
        if 0 < c {
            res.push((i, c));
        }
        i += 1;
    }
    if 1 < n {
        res.push((n, 1));
    }
    res
}

pub struct Sieve {
    prime: Vec<usize>,
    is_prime: Vec<bool>,
    min_facor: Vec<usize>,
}

impl Sieve {
    pub fn new(n: usize) -> Self {
        let mut prime = vec![];
        let mut is_prime = vec![true; n + 1];
        let mut min_facor = vec![0; n + 1];
        is_prime[0] = false;
        is_prime[1] = false;
        min_facor[0] = 0;
        min_facor[1] = 1;
        for i in 2..n + 1 {
            if is_prime[i] {
                for j in 1.. {
                    if n < i * j {
                        break;
                    }
                    if is_prime[i * j] {
                        is_prime[i * j] = false;
                        min_facor[i * j] = i;
                    }
                }
                is_prime[i] = true;
                prime.push(i);
            }
        }
        Sieve {
            prime: prime,
            is_prime: is_prime,
            min_facor: min_facor,
        }
    }
    pub fn is_prime(&self, n: usize) -> bool {
        self.is_prime[n]
    }
    pub fn prime(&self) -> &Vec<usize> {
        &self.prime
    }
    pub fn prime_factor(&self, mut n: usize) -> Vec<(usize, usize)> {
        let mut res: Vec<(usize, usize)> = vec![];
        let (mut prev, mut cnt) = (self.min_facor[n], 0);
        while self.min_facor[n] != 0 {
            let fact = self.min_facor[n];
            n /= fact;
            if prev == fact {
                cnt += 1;
            } else {
                res.push((prev, cnt));
                prev = fact;
                cnt = 1;
            }
        }
        if 0 < cnt {
            res.push((prev, cnt));
        }
        res
    }
}

pub fn pow_mod(mut x: u64, mut y: u64, modv: u64) -> u64 {
    let mut ret: u64 = 1;
    while y > 0 {
        if (y & 1) != 0 {
            ret = ret * x % modv;
        }
        y >>= 1;
        x = x * x % modv;
    }
    ret
}


#[test]
fn test_is_prime() {
    assert_eq!(true, is_prime(2));
    assert_eq!(true, is_prime(3));
    assert_eq!(false, is_prime(4));
    assert_eq!(true, is_prime(1_000_000_007));
    assert_eq!(false, is_prime(1_000_000_007 * 100003));
}

#[test]
fn test_prime_factor() {
    assert_eq!(vec![(998244353, 1)], prime_factor(998244353));
    assert_eq!(
        vec![(2, 3), (1_000_000_007, 1)],
        prime_factor(2 * 2 * 2 * 1_000_000_007)
    );
}
