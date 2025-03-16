pub fn z_algorithm<T: std::cmp::Eq>(s: &[T]) -> Vec<usize> {
    let mut a: Vec<usize> = vec![0; s.len()];
    let (mut i, mut j) = (1, 0);
    a[0] = s.len();
    while i < s.len() {
        while i + j < s.len() && s[j] == s[i + j] {
            j += 1;
        }
        a[i] = j;
        if j == 0 {
            i += 1;
            continue;
        }
        let mut k = 1;
        while i + k < s.len() && k + a[k] < j {
            a[i + k] = a[k];
            k += 1;
        }
        i += k;
        j -= k;
    }
    a
}


#[test]
fn test() {
    let s = b"aaabaaaab".to_vec();
    let expected: Vec<usize> = vec![9, 2, 1, 0, 3, 4, 2, 1, 0];
    assert_eq!(expected, z_algorithm(&s));
}

//http://codeforces.com/problemset/problem/432/D
