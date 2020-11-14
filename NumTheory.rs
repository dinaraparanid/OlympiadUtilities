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
    return d;
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
