#![allow(unused_imports)]
#![allow(dead_code)]

fn foreach_subset<F: FnMut(usize)>(s: usize, mut f: F) {
    let mut i = s;
    loop {
        f(i);
        if i == 0 {
            break;
        }
        i = (i - 1) & s;
    }
}

fn foreach_superset<F: FnMut(usize)>(s: usize, u: usize, mut f: F) {
    let mut i = s;
    while i <= u {
        f(i);
        i = (i + 1) | s;
    }
}


#[test]
fn test_subset() {
    let mut v = vec![0; 8];
    foreach_subset(5, |i| v[i] = 1);
    assert_eq!(vec![1, 1, 0, 0, 1, 1, 0, 0], v);
}

#[test]
fn test_superset() {
    let mut v = vec![0; 8];
    foreach_superset(5, 7, |i| v[i] = 1);
    assert_eq!(vec![0, 0, 0, 0, 0, 1, 0, 1], v);
}
