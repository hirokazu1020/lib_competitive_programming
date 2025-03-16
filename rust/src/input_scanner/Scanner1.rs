pub struct Scanner {
    line: Vec<String>,
    idx: usize,
}

impl Scanner {
    pub fn new() -> Self {
        let mut buffer: Vec<u8> = Vec::new();
        std::io::Read::read_to_end(&mut std::io::stdin(), &mut buffer).unwrap();
        let b: &str = unsafe { std::str::from_utf8_unchecked(&mut buffer) };
        Scanner {
            line: b
                .trim()
                .split_ascii_whitespace()
                .collect::<Vec<&str>>()
                .iter()
                .map(|&s| String::from(s))
                .collect(),
            idx: 0,
        }
    }
    pub fn next<T>(&mut self) -> T
    where
        T: std::str::FromStr,
        T::Err: std::fmt::Debug,
    {
        let ret: T = self.line[self.idx].parse::<T>().unwrap();
        self.idx += 1;
        ret
    }
    pub fn next_vec<T>(&mut self, n: usize) -> Vec<T>
    where
        T: std::str::FromStr,
        T::Err: std::fmt::Debug,
    {
        let mut v: Vec<T> = Vec::with_capacity(n);
        for _i in 0..n {
            v.push(self.next());
        }
        v
    }
    pub fn next_chars(&mut self) -> Vec<char> {
        let s: String = self.next();
        s.chars().collect::<Vec<char>>()
    }
}
