#![allow(unused_imports)]
#![allow(dead_code)]

pub fn foreach_subset<F: FnMut(usize)>(set: usize, mut f: F) {
    let mut i = set;
    loop {
        f(i);
        if i == 0 {
            break;
        }
        i = (i - 1) & set;
    }
}

pub fn foreach_superset<F: FnMut(usize)>(set: usize, superset: usize, mut f: F) {
    let mut i = set;
    while i <= superset {
        f(i);
        i = (i + 1) | set;
    }
}

#[test]
fn test_subset() {
    let mut v = vec![0; 8];
    foreach_subset(0b0101, |i| v[i] = 1);
    assert_eq!(vec![1, 1, 0, 0, 1, 1, 0, 0], v);
}

#[test]
fn test_superset() {
    let mut v = vec![0; 8];
    foreach_superset(0b0101, 0b0111, |i| v[i] = 1);
    assert_eq!(vec![0, 0, 0, 0, 0, 1, 0, 1], v);
}
