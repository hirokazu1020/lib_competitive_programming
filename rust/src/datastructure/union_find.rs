pub struct UnionFind {
    par: Vec<usize>,
    size: Vec<usize>,
}

impl UnionFind {
    pub fn new(n: usize) -> Self {
        let mut uf = UnionFind {
            par: Vec::with_capacity(n),
            size: vec![1; n],
        };
        for i in 0..n {
            uf.par.push(i);
        }
        return uf;
    }
    pub fn find(&mut self, u: usize) -> usize {
        if self.par[u] != u {
            let p = self.par[u];
            self.par[u] = self.find(p);
            return self.par[u];
        }
        return u;
    }
    pub fn size(&mut self, u: usize) -> usize {
        let u = self.find(u);
        return self.size[u];
    }
    pub fn same(&mut self, u: usize, v: usize) -> bool {
        return self.find(u) == self.find(v);
    }
    pub fn unite(&mut self, u: usize, v: usize) {
        let u = self.find(u);
        let v = self.find(v);
        if u == v {
            return;
        }
        if self.size[u] < self.size[v] {
            self.par[u] = v;
            self.size[v] += self.size[u];
        } else {
            self.par[v] = u;
            self.size[u] += self.size[v];
        }
    }
}
