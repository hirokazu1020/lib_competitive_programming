#![allow(unused_imports)]
#![allow(dead_code)]

use std::collections::BTreeSet;
use std::ops::Bound;

pub fn unique<T: std::cmp::Eq>(v: &mut Vec<T>) {
    if v.len() == 0 {
        return;
    }
    let mut size = 1;
    for i in 1..v.len() {
        if v[size - 1] != v[i] {
            v.swap(size, i);
            size += 1;
        }
    }
    for _ in 0..(v.len() - size) {
        v.pop();
    }
}

pub fn lower_bound<T: std::cmp::Ord>(v: &[T], val: &T) -> usize {
    let mut lb: isize = -1;
    let mut ub: isize = v.len() as isize;
    while ub - lb > 1 {
        let mid = (lb + ub) / 2;
        if *val <= v[mid as usize] {
            ub = mid;
        } else {
            lb = mid;
        }
    }
    return ub as usize;
}

pub fn upper_bound<T: std::cmp::Ord>(v: &[T], val: &T) -> usize {
    let mut lb: isize = -1;
    let mut ub: isize = v.len() as isize;
    while ub - lb > 1 {
        let mid = (lb + ub) / 2;
        if *val < v[mid as usize] {
            ub = mid;
        } else {
            lb = mid;
        }
    }
    return ub as usize;
}

pub fn set_lower_bound<'a, T: std::cmp::Ord>(set: &'a BTreeSet<T>, val: &T) -> Option<&'a T> {
    return set.range((Bound::Included(val), Bound::Unbounded)).next();
}


#[test]
fn test_bound() {
    let v: Vec<i32> = vec![1, 2, 3, 4, 5];
    assert_eq!(2, lower_bound(&v, &3));
    assert_eq!(1, lower_bound(&v[1..], &3));
    assert_eq!(2, lower_bound(&v[0..2], &3));

    assert_eq!(3, upper_bound(&v, &3));
}

#[test]
fn test_set_bound() {
    let mut s: BTreeSet<i32> = BTreeSet::new();
    s.insert(1);
    s.insert(3);
    s.insert(5);
    assert_eq!(Some(&3), set_lower_bound(&s, &3));
    assert_eq!(None, set_lower_bound(&s, &6));
}

#[test]
fn test_set_bound_string() {
    let mut s: BTreeSet<String> = BTreeSet::new();
    s.insert("abc".to_string());
    s.insert("asv".to_string());
    s.insert("xxe".to_string());
    assert_eq!(
        Some(&"asv".to_string()),
        set_lower_bound(&s, &"asa".to_string())
    );
    assert_eq!(None, set_lower_bound(&s, &"zasdaf".to_string()));
}
