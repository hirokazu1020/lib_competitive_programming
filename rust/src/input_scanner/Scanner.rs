pub struct Scanner {
    bytes: Vec<u8>,
    begin: usize,
    end: usize,
}

impl Scanner {
    pub fn new() -> Self {
        use std::io::Read;
        let mut buffer: Vec<u8> = Vec::new();
        std::io::stdin().read_to_end(&mut buffer).unwrap();
        let end = (0..buffer.len())
            .find(|i| buffer[*i].is_ascii_whitespace())
            .unwrap_or(buffer.len());
        Scanner {
            bytes: buffer,
            begin: 0,
            end: end,
        }
    }
    pub fn next<T: std::str::FromStr>(&mut self) -> T {
        let s: &str = unsafe { std::str::from_utf8_unchecked(&self.bytes[self.begin..self.end]) };
        let ret: T = s.parse::<T>().ok().expect("Parse error.");
        self.skip();
        ret
    }
    pub fn next_vec<T: std::str::FromStr>(&mut self, n: usize) -> Vec<T> {
        (0..n).map(|_| self.next()).collect()
    }
    pub fn next_word(&mut self) -> Vec<u8> {
        let r = Vec::from(&self.bytes[self.begin..self.end]);
        self.skip();
        r
    }
    fn skip(&mut self) {
        self.begin = (self.end..self.bytes.len())
            .find(|i| !self.bytes[*i].is_ascii_whitespace())
            .unwrap_or(self.bytes.len());
        self.end = (self.begin..self.bytes.len())
            .find(|i| self.bytes[*i].is_ascii_whitespace())
            .unwrap_or(self.bytes.len());
    }
}
