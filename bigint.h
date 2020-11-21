#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <vector>

constexpr int maxn = 1e2 + 14, lg = 15;
constexpr int base = 1000000000;
constexpr int base_digits = 9;
using vll = std::vector<long long>;

class bigint
{
	
	std::vector<int> a_;
	int sign_{};

	friend std::pair<bigint, bigint> divmod(const bigint& a1, const bigint& b1)
	{
		const int norm = base / (b1.a_.back() + 1);

		const bigint a = a1.abs() * norm;
		const bigint b = b1.abs() * norm;
		bigint q, r;

		q.a_.resize(a.a_.size());

		for (int i = a.a_.size() - 1; i >= 0; i--)
		{
			r *= base;
			r += a.a_[i];

			const int s1 = r.a_.size() <= b.a_.size() ? 0 : r.a_[b.a_.size()];
			const int s2 = r.a_.size() <= b.a_.size() - 1 ? 0 : r.a_[b.a_.size() - 1];
			auto d = (1LL * base * s1 + s2) / b.a_.back();

			r -= b * d;

			while (r < 0)
				r += b, --d;

			q.a_[i] = d;
		}

		q.sign_ = a1.sign_ * b1.sign_;
		r.sign_ = a1.sign_;

		q.trim(); r.trim();
		return std::make_pair(q, r / norm);
	}

	static vll karatsuba_multiply(const vll& a, const vll& b)
	{
		const size_t n = a.size();
		vll res(n + n);

		if (n <= 32)
		{
			for (size_t i = 0; i < n; i++)
				for (size_t j = 0; j < n; j++)
					res[i + j] += a[i] * b[j];

			return res;
		}

		const size_t k = n >> 1;

		const vll a1(a.begin(), a.begin() + k);
		const vll b1(b.begin(), b.begin() + k);
		
		vll a2(a.begin() + k, a.end());
		vll b2(b.begin() + k, b.end());

		const vll a1_b1 = karatsuba_multiply(a1, b1);
		const vll a2_b2 = karatsuba_multiply(a2, b2);

		for (size_t i = 0; i < k; i++)
			a2[i] += a1[i];

		for (size_t i = 0; i < k; i++)
			b2[i] += b1[i];

		vll r = karatsuba_multiply(a2, b2);

		for (size_t i = 0; i < a1_b1.size(); i++)
			r[i] -= a1_b1[i];

		for (size_t i = 0; i < a2_b2.size(); i++)
			r[i] -= a2_b2[i];

		for (size_t i = 0; i < r.size(); i++)
			res[i + k] += r[i];

		for (size_t i = 0; i < a1_b1.size(); i++)
			res[i] += a1_b1[i];

		for (size_t i = 0; i < a2_b2.size(); i++)
			res[i + n] += a2_b2[i];

		return res;
	}

	static std::vector<int> convert_base(const std::vector<int>& a, const int old_digits, const int new_digits)
	{
		std::vector<long long> p(std::max(old_digits, new_digits) + 1);

		p[0] = 1;

		for (size_t i = 1; i < p.size(); i++)
			p[i] = p[i - 1] * 10;

		std::vector<int> res;

		long long cur = 0;
		int cur_digits = 0;

		for (int i : a)
		{
			cur += i * p[cur_digits];
			cur_digits += old_digits;

			while (cur_digits >= new_digits)
			{
				res.push_back(cur % p[new_digits]);
				cur /= p[new_digits];
				cur_digits -= new_digits;
			}
		}

		res.push_back(cur);

		while (!res.empty() && res.back() == 0)
			res.pop_back();

		return res;
	}

	void read(const std::string& s)
	{
		sign_ = 1;
		a_.clear();
		size_t pos = 0;

		while (pos < s.size() && (s[pos] == '-' || s[pos] == '+'))
		{
			if (s[pos] == '-')
				sign_ = -sign_;

			++pos;
		}

		for (size_t i = s.size() - 1; i >= pos; i -= base_digits)
		{
			int x = 0;

			for (size_t j = std::max(pos, i - base_digits + 1); j <= i; j++)
				x = x * 10 + s[j] - '0';

			a_.push_back(x);
		}

		trim();
	}

public:
	constexpr size_t size()
	{
		if (a_.empty())
			return 0;
		
		size_t ans = (a_.size() - 1) * base_digits;
		int ca = a_.back();
		
		while (ca != 0)
			ans++, ca /= 10;
		
		return ans;
	}
	
	bigint operator^(const bigint& v) const
	{
		bigint ans = 1;
		bigint a = *this;
		bigint b = v;

		while (!b.is_zero()) 
		{
			if (b % 2)
				ans *= a;
			
			a *= a;
			b /= 2;
		}
		
		return ans;
	}

	[[nodiscard]] std::string to_string() const
	{
		std::stringstream ss;
		ss << *this;
		std::string s;
		ss >> s;
		return s;
	}

	[[nodiscard]] int sumof() const
	{
		std::string s = to_string();
		int ans = 0;
		
		for (auto c : s) 
			ans += c - '0';
		
		return ans;
	}


	bigint() : sign_(1) {}

	bigint(const long long v) {
		*this = v;
	}

	bigint(const std::string& s) {
		read(s);
	}

	bigint& operator=(const bigint& v)
	{
		sign_ = v.sign_;
		a_ = v.a_;
		return *this;
	}

	bigint& operator=(long long v)
	{
		sign_ = 1;
		a_.clear();
		
		if (v < 0)
			sign_ = -1, v = -v;
		
		for (; v > 0; v = v / base)
			a_.push_back(v % base);

		return *this;
	}

	bigint operator+(const bigint& v) const
	{
		if (sign_ == v.sign_)
		{
			bigint res = v;

			for (size_t i = 0, carry = 0; i < std::max(a_.size(), v.a_.size()) || carry != 0; i++)
			{
				if (i == res.a_.size())
					res.a_.push_back(0);
				
				res.a_[i] += carry + (i < a_.size() ? a_[i] : 0);
				
				carry = res.a_[i] >= base;
				
				if (carry != 0)
					res.a_[i] -= base;
			}
			
			return res;
		}
		
		return *this - -v;
	}

	bigint operator-(const bigint& v) const
	{
		if (sign_ == v.sign_)
		{
			if (abs() >= v.abs())
			{
				bigint res = *this;
				
				for (size_t i = 0, carry = 0; i < v.a_.size() || carry != 0; i++)
				{
					res.a_[i] -= carry + (i < v.a_.size() ? v.a_[i] : 0);
					
					carry = res.a_[i] < 0;
					
					if (carry != 0)
						res.a_[i] += base;
				}
				
				res.trim();
				return res;
			}
			
			return -(v - *this);
		}
		
		return *this + -v;
	}

	void operator*=(int v)
	{
		if (v < 0)
			sign_ = -sign_, v = -v;

		for (size_t i = 0, carry = 0; i < a_.size() || carry != 0; i++)
		{
			if (i == a_.size())
				a_.push_back(0);

			const auto cur = 1LL * a_[i] * v + carry;
			carry = cur / base;
			a_[i] = cur % base;
		}
		
		trim();
	}

	bigint operator*(const int v) const
	{
		bigint res = *this;
		res *= v;
		return res;
	}

	void operator*=(long long v)
	{
		if (v < 0)
			sign_ = -sign_, v = -v;
		
		if (v > base)
		{
			*this = *this * (v / base) * base + *this * (v % base);
			return;
		}
		
		for (size_t i = 0, carry = 0; i < a_.size() || carry != 0; i++)
		{
			if (i == a_.size())
				a_.push_back(0);

			const auto cur = 1LL * a_[i] * v + carry;
			
			carry = cur / base;
			a_[i] = cur % base;
		}
		
		trim();
	}

	bigint operator*(const long long v) const
	{
		bigint res = *this;
		res *= v;
		return res;
	}

	bigint operator/(const bigint& v) const {
		return divmod(*this, v).first;
	}

	bigint operator%(const bigint& v) const {
		return divmod(*this, v).second;
	}

	void operator/=(int v)
	{
		if (v < 0)
			sign_ = -sign_, v = -v;
		
		for (auto i = 1LL * a_.size() - 1, rem = 0LL; i >= 0; i--)
		{
			const auto cur = 1LL * a_[i] + rem * base;
			a_[i] = cur / v;
			rem = cur % v;
		}
		
		trim();
	}

	bigint operator/(const int v) const
	{
		bigint res = *this;
		res /= v;
		return res;
	}

	constexpr long long operator%(int v) const
	{
		if (v < 0)
			v = -v;
		
		long long m = 0;
		
		for (auto i = 1LL * a_.size() - 1; i >= 0; i--)
			m = (a_[i] + m * 1LL * base) % (v * 1LL);
		
		return m * sign_;
	}

	void operator+=(const bigint& v) {
		*this = *this + v;
	}
	
	void operator-=(const bigint& v) {
		*this = *this - v;
	}
	
	void operator*=(const bigint& v) {
		*this = *this * v;
	}
	
	void operator/=(const bigint& v) {
		*this = *this / v;
	}

	constexpr bool operator<(const bigint& v) const
	{
		if (sign_ != v.sign_)
			return sign_ < v.sign_;
		
		if (a_.size() != v.a_.size())
			return a_.size() * sign_ < v.a_.size() * v.sign_;
		
		for (auto i = 1LL * a_.size() - 1; i >= 0; i--)
			if (a_[i] != v.a_[i])
				return a_[i] * sign_ < v.a_[i] * sign_;
		
		return false;
	}

	bool operator>(const bigint& v) const {
		return v < *this;
	}
	
	bool operator<=(const bigint& v) const {
		return !(v < *this);
	}
	
	bool operator>=(const bigint& v) const {
		return !(*this < v);
	}
	
	bool operator==(const bigint& v) const {
		return !(*this < v) && !(v < *this);
	}
	
	bool operator!=(const bigint& v) const {
		return *this < v || v < *this;
	}

	void trim()
	{
		while (!a_.empty() && !a_.back())
			a_.pop_back();
		
		if (a_.empty())
			sign_ = 1;
	}

	[[nodiscard]] constexpr bool is_zero() const {
		return a_.empty() || a_.size() == 1 && !a_[0];
	}

	bigint operator-() const
	{
		bigint res = *this;
		res.sign_ = -sign_;
		return res;
	}

	[[nodiscard]] bigint abs() const
	{
		bigint res = *this;
		res.sign_ *= res.sign_;
		return res;
	}

	[[nodiscard]] constexpr long long long_value() const
	{
		long long res = 0;
		
		for (auto i = 1LL * a_.size() - 1; i >= 0; i--)
			res = res * base + a_[i];
		
		return res * sign_;
	}

	friend bigint gcd(const bigint& a, const bigint& b) {
		return b.is_zero() ? a : gcd(b, a % b);
	}
	
	friend bigint lcm(const bigint& a, const bigint& b) {
		return a / gcd(a, b) * b;
	}

	friend std::istream& operator>>(std::istream& stream, bigint& v)
	{
		std::string s;
		stream >> s;
		v.read(s);
		return stream;
	}

	friend std::ostream& operator<<(std::ostream& stream, const bigint& v)
	{
		if (v.sign_ == -1)
			stream << '-';
		
		stream << (v.a_.empty() ? 0 : v.a_.back());
		
		for (auto i = 1LL * v.a_.size() - 2; i >= 0; i--)
			stream << std::setw(base_digits) << std::setfill('0') << v.a_[i];
		
		return stream;
	}

	bigint operator*(const bigint& v) const
	{
		const std::vector<int> a6 = convert_base(this->a_, base_digits, 6);
		const std::vector<int> b6 = convert_base(v.a_, base_digits, 6);
		
		vll a(a6.begin(), a6.end());
		vll b(b6.begin(), b6.end());
		
		while (a.size() < b.size())
			a.push_back(0);
		
		while (b.size() < a.size())
			b.push_back(0);
		
		while (a.size() & a.size() - 1)
			a.push_back(0), b.push_back(0);
		
		vll c = karatsuba_multiply(a, b);
		
		bigint res;
		res.sign_ = sign_ * v.sign_;
		
		for (size_t i = 0, carry = 0; i < c.size(); i++)
		{
			const auto cur = c[i] + carry;
			res.a_.push_back(cur % 1000000);
			carry = cur / 1000000;
		}
		
		res.a_ = convert_base(res.a_, 6, base_digits);
		res.trim();
		return res;
	}
};
