pub mod suffix_array {
    fn counting_sort(
        sa: &Vec<usize>,
        rank: &Vec<usize>,
        count: &mut Vec<usize>,
        result: &mut Vec<usize>,
        offset: usize,
    ) {
        for i in 0..count.len() {
            count[i] = 0;
        }
        for i in 0..rank.len() {
            let r = if i + offset < rank.len() {
                rank[i + offset] + 1
            } else {
                0
            };
            count[r + 1] += 1;
        }
        for i in 1..count.len() {
            count[i] += count[i - 1];
        }
        for i in 0..sa.len() {
            let r = if sa[i] + offset < sa.len() {
                rank[sa[i] + offset] + 1
            } else {
                0
            };
            result[count[r]] = sa[i];
            count[r] += 1;
        }
    }

    pub fn suffix_array(s: &[u8]) -> Vec<usize> {
        let len = s.len();
        let count_len = std::cmp::max(u8::max_value() as usize + 1, len) + 2;
        let mut count = vec![0; count_len];
        let mut rank = vec![0; len];
        let mut sa = vec![0; len];
        let mut tmp = (0..len).map(|i| i).collect::<Vec<usize>>();
        for i in 0..len {
            rank[i] = s[i] as usize;
        }
        counting_sort(&tmp, &rank, &mut count, &mut sa, 0);
        let mut offset = 1;
        while offset < len {
            counting_sort(&sa, &rank, &mut count, &mut tmp, offset);
            counting_sort(&tmp, &rank, &mut count, &mut sa, 0);
            tmp[sa[0]] = 0;
            for i in 1..len {
                tmp[sa[i]] = tmp[sa[i - 1]];
                if rank[sa[i - 1]] != rank[sa[i]] {
                    tmp[sa[i]] += 1;
                } else {
                    let a = sa[i - 1] + offset;
                    let b = sa[i] + offset;
                    let ra = if a < len { rank[a] + 1 } else { 0 };
                    let rb = if b < len { rank[b] + 1 } else { 0 };
                    if ra != rb {
                        tmp[sa[i]] += 1;
                    }
                }
            }
            std::mem::swap(&mut tmp, &mut rank);
            offset *= 2;
            if rank[*sa.last().unwrap()] == len - 1 {
                break;
            }
        }
        sa
    }

    pub fn lcp_array(s: &[u8], sa: &Vec<usize>) -> Vec<usize> {
        assert_eq!(s.len(), sa.len());
        let len = s.len();
        let mut rank = vec![0; len];
        for i in 0..len {
            rank[sa[i]] = i;
        }
        let mut h = 0;
        let mut lcp = vec![0; len - 1];
        for i in 0..len {
            if rank[i] == 0 {
                continue;
            }
            let j = sa[rank[i] - 1];
            if h > 0 {
                h -= 1;
            }
            while j + h < len && i + h < len {
                if s[j + h] != s[i + h] {
                    break;
                }
                h += 1;
            }
            lcp[rank[i] - 1] = h;
        }
        lcp
    }
}

#[cfg(test)]
pub fn test_lcp_array(s: &[u8], sa: &Vec<usize>) -> Vec<usize> {
    let mut lcp = Vec::with_capacity(s.len() - 1);
    for i in 1..s.len() {
        let mut c = 0;
        for j in 0..(s.len() - std::cmp::max(sa[i - 1], sa[i])) {
            if s[sa[i - 1] + j] != s[sa[i] + j] {
                break;
            }
            c += 1;
        }
        lcp.push(c);
    }
    lcp
}

#[test]
fn test_sa() {
    let s: Vec<u8> = b"abracadabra".to_vec();
    let mut sa: Vec<usize> = (0..s.len()).map(|i| i).collect::<Vec<usize>>();
    sa.sort_by(|i, j| s[*i..].cmp(&s[*j..]));

    assert_eq!(sa, suffix_array::suffix_array(&s));

    assert_eq!(test_lcp_array(&s, &sa), suffix_array::lcp_array(&s, &sa));
}
