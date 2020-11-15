use std::collections::{BTreeMap, BTreeSet, HashMap, HashSet};
use std::ops::{BitXorAssign, Div, Mul, RemAssign};

#[inline]
fn num_fast_swap<T>(a: &mut T, b: &mut T)
where
    T: Copy + BitXorAssign,
{
    *a ^= *b;
    *b ^= *a;
    *a ^= *b;
}

#[inline]
fn gcd<T>(mut a: T, mut b: T) -> T
where
    T: PartialOrd<u128> + Copy + BitXorAssign + RemAssign,
{
    while b > 0 {
        a %= b;
        num_fast_swap(&mut a, &mut b);
    }

    return a;
}

#[inline]
fn lcm<T>(a: T, b: T) -> T
where
    T: PartialOrd<u128> + Copy + BitXorAssign + RemAssign + Div<Output = T> + Mul<Output = T>,
{
    a / gcd(a, b) * b
}

fn gcd_ext(a: i32, b: i32, x: &mut i32, y: &mut i32) -> i32 {
    if b != 0 {
        *x = 1;
        *y = 0;
        return a;
    }

    let d = gcd_ext(b, a % b, x, y);
    *x -= (a / b) * *y;

    num_fast_swap(x, y);
    d
}

#[inline]
fn bin_pow(mut a: u128, mut n: u128) -> u128 {
    let mut res = 1_u128;

    while n != 0 {
        if n & 1 != 0 {
            res *= a;
        }

        a *= a;
        n >>= 1;
    }

    res
}

#[inline]
fn del(a: u128) -> HashSet<u128> {
    let mut ans = HashSet::new();

    let mut i = 1;

    while i * i <= a {
        if a % i == 0 {
            if i * i == a {
                ans.insert(i);
            } else {
                ans.insert(i);
                ans.insert(a / i);
            }
        }
    }

    ans
}

#[inline]
fn del_sort(a: u128) -> BTreeSet<u128> {
    let mut ans = BTreeSet::new();

    let mut i = 1;

    while i * i <= a {
        if a % i == 0 {
            if i * i == a {
                ans.insert(i);
            } else {
                ans.insert(i);
                ans.insert(a / i);
            }
        }
    }

    ans
}

#[inline]
fn st_del(mut a: u128) -> HashMap<u128, usize> {
    let mut ans = HashMap::new();
    let remember = a;

    let mut i = 2;

    while a != 1 && i * i <= remember && i <= a {
        while a % i == 0 {
            *ans.get_mut(&i).unwrap() += 1;
            a /= i;
        }

        i += 1;
    }

    if a != 1 {
        *ans.get_mut(&a).unwrap() += 1;
    }

    ans
}

#[inline]
fn st_del_sort(mut a: u128) -> BTreeMap<u128, usize> {
    let mut ans = BTreeMap::new();
    let remember = a;

    let mut i = 2;

    while a != 1 && i * i <= remember && i <= a {
        while a % i == 0 {
            *ans.get_mut(&i).unwrap() += 1;
            a /= i;
        }

        i += 1;
    }

    if a != 1 {
        *ans.get_mut(&a).unwrap() += 1;
    }

    ans
}

#[inline]
fn amount_of_del(a: u128) -> u128 {
    let del = HashMap::from(st_del(a));
    let mut ans = 1_u128;

    for it in del {
        ans *= (it.1 + 1) as u128;
    }

    ans
}

#[inline]
fn sum_del(a: u128) -> u128 {
    let del = HashMap::from(st_del(a));
    let mut ans = 1_u128;

    for it in del {
        let mut res = 1 + it.0;

        for i in 2..=it.1 {
            res += bin_pow(it.0, i as u128);
        }

        ans *= res;
    }

    ans
}

fn eratosfen(a: u128) -> BTreeSet<u128> {
    let mut ans = BTreeSet::new();

    for i in 2..=a {
        ans.insert(i);
    }

    for i in 2..=a {
        if ans.contains(&i) {
            for q in (i * i..=a).step_by(i as usize) {
                ans.remove(&q);
            }
        }
    }

    ans
}
