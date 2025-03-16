pub struct KMP<T> {
    pattern: Vec<T>,
    fail: Vec<isize>,
}

impl<T: Copy + std::cmp::Eq> KMP<T> {
    pub fn new(pattern: &[T]) -> Self {
        let mut fail: Vec<isize> = vec![0; pattern.len() + 1];
        fail[0] = -1;
        for i in 1..fail.len() {
            fail[i] = KMP::next_node(&pattern, &fail, fail[i - 1], pattern[i - 1]);
        }
        KMP {
            pattern: Vec::from(pattern),
            fail: fail,
        }
    }

    pub fn next(&self, t: isize, ch: T) -> isize {
        KMP::next_node(&self.pattern, &self.fail, t, ch)
    }
    pub fn search(&self, text: &[T]) -> Vec<usize> {
        let mut ret: Vec<usize> = Vec::new();
        let mut t: isize = 0;
        for i in 0..text.len() {
            t = self.next(t, text[i]);
            if t == self.pattern.len() as isize {
                ret.push(i + 1 - self.pattern.len());
            }
        }
        ret
    }
    fn next_node(p: &[T], fail: &Vec<isize>, t: isize, ch: T) -> isize {
        let mut u = t;
        if u == p.len() as isize {
            u = fail[p.len()];
        }
        while u >= 0 && p[u as usize] != ch {
            u = fail[u as usize];
        }
        u + 1
    }
}

#[test]
fn test() {
    let p: Vec<u8> = Vec::from("abra");
    let kmp = KMP::new(&p);
    let s: Vec<u8> = Vec::from("abracadabra");
    assert_eq!(vec![0, 7], kmp.search(&s));
}
