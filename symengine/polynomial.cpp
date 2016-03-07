#include <symengine/polynomial.h>
#include <symengine/add.h>
#include <symengine/mul.h>
#include <symengine/pow.h>
#include <symengine/constants.h>

namespace SymEngine {

UnivariateIntPolynomial::UnivariateIntPolynomial(const RCP<const Symbol> &var, const unsigned int &degree, map_uint_mpz&& dict) :
     degree_{degree}, var_{var}, dict_{std::move(dict)} {
    SYMENGINE_ASSERT(is_canonical(degree_, dict_))
}

UnivariateIntPolynomial::UnivariateIntPolynomial(const RCP<const Symbol> &var, const std::vector<integer_class> &v) : var_{var} {
    for (unsigned int i = 0; i < v.size(); i++)
        if (v[i] != 0)
            dict_add_term(dict_, v[i], i);
    degree_ = v.size() - 1;
    SYMENGINE_ASSERT(is_canonical(degree_, dict_))
}

bool UnivariateIntPolynomial::is_canonical(const unsigned int &degree_, const map_uint_mpz& dict) const {
    // map_uint_mpz ordered(dict.begin(), dict.end());
    // unsigned int prev_degree = (--ordered.end())->first;
    // return prev_degree == degree_;
    map_uint_mpz ordered(dict.begin(), dict.end());
    unsigned int prev_degree = (--ordered.end())->first;
    if (prev_degree != degree_)
        return false;

    return true;
}

std::size_t UnivariateIntPolynomial::__hash__() const {
    std::hash<std::string> hash_string;
    std::size_t seed = UNIVARIATEINTPOLYNOMIAL;
    seed += hash_string(this->var_->get_name());
    for (const auto &it : this->dict_) {
        std::size_t temp = UNIVARIATEINTPOLYNOMIAL;
        hash_combine<unsigned int>(temp, it.first);
        hash_combine<long long int>(temp, mp_get_si(it.second));
        seed += temp;
    }
    return seed;
}

bool UnivariateIntPolynomial::__eq__(const Basic &o) const {
    return eq(*var_, *(static_cast<const UnivariateIntPolynomial &>(o).var_)) and
        map_uint_mpz_eq(dict_, static_cast<const UnivariateIntPolynomial &>(o).dict_);
}

int UnivariateIntPolynomial::compare(const Basic &o) const {
    const UnivariateIntPolynomial &s = static_cast<const UnivariateIntPolynomial &>(o);

    if (dict_.size() != s.dict_.size())
        return (dict_.size() < s.dict_.size()) ? -1 : 1;

    int cmp = var_->compare(*s.var_);
    if (cmp != 0)
        return cmp;

    return map_uint_mpz_compare(dict_, s.dict_);
}

RCP<const Basic> UnivariateIntPolynomial::from_dict(const RCP<const Symbol> &var, map_uint_mpz &&d) {
    return make_rcp<const UnivariateIntPolynomial>(var, (--(d.end()))->first, std::move(d));
}

void UnivariateIntPolynomial::dict_add_term(map_uint_mpz &d, const integer_class &coef, const unsigned int &n) {
    auto it = d.find(n);
    if (it == d.end())
        d[n] = coef;
}

vec_basic UnivariateIntPolynomial::get_args() const {
    vec_basic args;
    map_uint_mpz d;
    for (const auto &p: dict_) {
        // args.push_back(UnivariateIntPolynomial::from_dict(var_, {{p.first, p.second}}));
        d = {{p.first, p.second}};
        if (d.begin()->first == 0)
            args.push_back( integer(d.begin()->second));
        else if (d.begin()->first == 1) {
            if (d.begin()->second == 1)
                args.push_back(var_);
            else
                args.push_back(Mul::from_dict(integer(d.begin()->second), {{var_, one}}));
        } else {
            if (d.begin()->second == 1)
              args.push_back( pow(var_, integer(d.begin()->first)));
            else
              args.push_back( Mul::from_dict(integer(d.begin()->second),{{var_, integer(d.begin()->first)}}));
        }
    }
    return args;
}

integer_class UnivariateIntPolynomial::max_coef() const {
    integer_class curr = dict_.begin()->second;
    for (const auto &it : dict_)
        if (it.second > curr)
            curr = it.second;
    return curr;
}

integer_class UnivariateIntPolynomial::eval(const integer_class &x) const {
    //TODO: Use Horner's Scheme
    integer_class ans(0), temp;
    for (const auto &p : dict_) {
        mp_pow_ui(temp, x, p.first);
        ans += p.second * temp;
    }
    return ans;
}

integer_class UnivariateIntPolynomial::eval_bit(const int &x) const {
    //TODO: Use Horner's Scheme
    integer_class ans(0);
    for (const auto &p : dict_) {
        integer_class t = integer_class(1) << x * p.first;
        ans += p.second * t;
    }
    return ans;
}

bool UnivariateIntPolynomial::is_zero() const {
    return (dict_.size() == 1 and dict_.begin()->second == 0);
}

bool UnivariateIntPolynomial::is_one() const {
    return dict_.size() == 1 and dict_.begin()->second == 1 and dict_.begin()->first == 0;
}

bool UnivariateIntPolynomial::is_minus_one() const {
    return dict_.size() == 1 and dict_.begin()->second == -1 and dict_.begin()->first == 0;
}

bool UnivariateIntPolynomial::is_integer() const {
    return dict_.size() == 1 and dict_.begin()->first == 0;
}

bool UnivariateIntPolynomial::is_symbol() const {
    return dict_.size() == 1 and dict_.begin()->first == 1 and dict_.begin()->second == 1;
}

bool UnivariateIntPolynomial::is_mul() const {
    return dict_.size() == 1 and dict_.begin()->first != 0 and dict_.begin()->second != 1 and dict_.begin()->second != 0;
}

bool UnivariateIntPolynomial::is_pow() const {
    return dict_.size() == 1 and dict_.begin()->second == 1 and dict_.begin()->first != 1 and dict_.begin()->first != 0;
}   
    
RCP<const UnivariateIntPolynomial> add_poly(const UnivariateIntPolynomial &a, const UnivariateIntPolynomial &b) {
    map_uint_mpz dict;
    for (const auto &it : a.get_dict())
        dict[it.first] = it.second;
    for (const auto &it : b.get_dict())
        dict[it.first] += it.second;
    return univariate_int_polynomial(a.get_var(), (--(dict.end()))->first, std::move(dict));
}

RCP<const UnivariateIntPolynomial> neg_poly(const UnivariateIntPolynomial &a) {
    map_uint_mpz dict;
    for (const auto &it : a.get_dict())
        dict[it.first] = -1 * it.second;
    return univariate_int_polynomial(a.get_var(), (--(dict.end()))->first, std::move(dict));
}

RCP<const UnivariateIntPolynomial> sub_poly(const UnivariateIntPolynomial &a, const UnivariateIntPolynomial &b) {
    map_uint_mpz dict;
    for (const auto &it : a.get_dict())
        dict[it.first] = it.second;
    for (const auto &it : b.get_dict())
        dict[it.first] -= it.second;

    return univariate_int_polynomial(a.get_var(), (--(dict.end()))->first, std::move(dict));
}

//Calculates bit length of number, used in mul_poly() only
template <typename T>
unsigned int bit_length(T t) {
    unsigned int count = 0;
    while (t > 0) {
        count++;
        t = t >> 1;
    }
    return count;
}

RCP<const UnivariateIntPolynomial> mul_poly(RCP<const UnivariateIntPolynomial> a, RCP<const UnivariateIntPolynomial> b) {
    //TODO: Use `const RCP<const UnivariateIntPolynomial> &a` for input arguments,
    //      even better is use `const UnivariateIntPolynomial &a`
    unsigned int da = a->get_degree();
    unsigned int db = b->get_degree();

    int sign = 1;
    if ((--(a->get_dict().end()))->second < 0) {
        a = neg_poly(*a);
        sign = -1 * sign;
    }
    if ((--(b->get_dict().end()))->second < 0) {
        b = neg_poly(*b);
        sign = -1 * sign;
    }

    integer_class p = std::max(a->max_coef(), b->max_coef());

    unsigned int N = bit_length(std::min(da + 1, db + 1)) + bit_length(p) + 1;

    integer_class a1(1);
    a1 <<= N;
    integer_class a2 = a1 / 2;
    integer_class mask = a1 - 1;
    integer_class sa = a->eval_bit(N);
    integer_class sb = b->eval_bit(N);
    integer_class r = sa*sb;

    std::vector<integer_class> v;
    integer_class carry(0);

    while (r != 0 or carry != 0) {
        integer_class b;
        //TODO:fix this
        mp_and(b, r, mask);
        if (b < a2) {
            v.push_back(b + carry);
            carry = 0;
        } else {
            v.push_back(b - a1 + carry);
            carry = 1;
        }
        r >>= N;
    }

    if (sign == -1)
        return neg_poly(*make_rcp<const UnivariateIntPolynomial>(a->get_var(), v));
    else
        return make_rcp<const UnivariateIntPolynomial>(a->get_var(), v);
}


UnivariatePolynomial::UnivariatePolynomial(const RCP<const Symbol> &var, const unsigned int &degree, const map_int_Expr&& dict) :
     degree_{degree}, var_{var}, dict_{std::move(dict)} {
    SYMENGINE_ASSERT(is_canonical(degree_, dict_))
}

UnivariatePolynomial::UnivariatePolynomial(const RCP<const Symbol> &var, const std::vector<Expression> &v) : var_{var} {
    for (unsigned int i = 0; i < v.size(); i++)
        if (v[i] != 0)
            dict_add_term(dict_, v[i], i);
    degree_ = v.size() - 1;
    SYMENGINE_ASSERT(is_canonical(degree_, dict_))
}

bool UnivariatePolynomial::is_canonical(const unsigned int &degree_, const map_int_Expr& dict) const {
    map_int_Expr ordered(dict.begin(), dict.end());
    unsigned int prev_degree = (--ordered.end())->first;
    if (prev_degree != degree_)
        return false;

    return true;
}

std::size_t UnivariatePolynomial::__hash__() const
{
    std::hash<std::string> hash_string;
    std::size_t seed = UNIVARIATEPOLYNOMIAL;

    seed += hash_string(this->var_->get_name());
    for (const auto &it : this->dict_)
    {
        std::size_t temp = UNIVARIATEPOLYNOMIAL;
        hash_combine<unsigned int>(temp, it.first);
        hash_combine<Basic>(temp, *(it.second.get_basic()));
        seed += temp;
    }
    return seed;
}

bool UnivariatePolynomial::__eq__(const Basic &o) const
{
    return eq(*var_, *(static_cast<const UnivariatePolynomial &>(o).var_)) and
        map_int_Expr_eq(dict_, static_cast<const UnivariatePolynomial &>(o).dict_);
}

int UnivariatePolynomial::compare(const Basic &o) const
{
    const UnivariatePolynomial &s = static_cast<const UnivariatePolynomial &>(o);

    if (dict_.size() != s.dict_.size())
        return (dict_.size() < s.dict_.size()) ? -1 : 1;

    int cmp = var_->compare(*s.var_);
    if (cmp != 0)
        return cmp;

    return map_int_Expr_compare(dict_, s.dict_);
}

RCP<const Basic> UnivariatePolynomial::from_dict(const RCP<const Symbol> &var, map_int_Expr &&d)
{
    if (d.size() == 1) {
        if (d.begin()->first == 0)
            return d.begin()->second.get_basic();
        else if (d.begin()->first == 1) {
            if (d.begin()->second == 0)
                return zero;
            else if (d.begin()->second == 1)
                return var;
            else if (is_a<Integer>(*d.begin()->second.get_basic()))
                return Mul::from_dict(rcp_static_cast<const Integer>(d.begin()->second.get_basic()), 
                    {{var, one}});
            else
                return mul(d.begin()->second.get_basic(), var);
        } else {
            if (d.begin()->second == 0)
                return zero;
            else if (d.begin()->second == 1)
                return pow(var, integer(d.begin()->first));
            else if (is_a<Integer>(*d.begin()->second.get_basic()))
                return Mul::from_dict(rcp_static_cast<const Integer>(d.begin()->second.get_basic()),
                    {{var, integer(d.begin()->first)}});
            else
                return pow(mul(d.begin()->second.get_basic(), var), integer(d.begin()->first));
        }
    } else
        return make_rcp<const UnivariatePolynomial>(var, (--(d.end()))->first, std::move(d));
}

void UnivariatePolynomial::dict_add_term(map_int_Expr &d, const Expression &coef, const unsigned int &n)
{
    auto it = d.find(n);
    if (it == d.end())
       d[n] = coef;
}

vec_basic UnivariatePolynomial::get_args() const {
    vec_basic args;
    for (const auto &p: dict_)
        args.push_back(UnivariatePolynomial::from_dict(var_, {{p.first, p.second}}));
    return args;
}

Expression UnivariatePolynomial::max_coef() const {
    Expression curr = dict_.begin()->second;
    for (const auto &it : dict_)
        if (curr.get_basic()->__cmp__(*it.second.get_basic()))
            curr = it.second;
    return curr;
}

Expression UnivariatePolynomial::eval(const Expression &x) const {
    //TODO: Use Horner's Scheme
    Expression ans = 0;
    for (const auto &p : dict_) {
        Expression temp;
        temp = pow_ex(x, Expression(p.first));
        ans += p.second * temp;
    }
    return ans;
}

bool UnivariatePolynomial::is_zero() const {
    return dict_.size() == 1 and dict_.begin()->second == 0;
}

bool UnivariatePolynomial::is_one() const {
    return dict_.size() == 1 and dict_.begin()->second == 1 and dict_.begin()->first == 0;
}

bool UnivariatePolynomial::is_minus_one() const {
    return dict_.size() == 1 and dict_.begin()->second == -1 and dict_.begin()->first == 0;
}

bool UnivariatePolynomial::is_integer() const {
    return dict_.size() == 1 and dict_.begin()->first == 0;
}

bool UnivariatePolynomial::is_symbol() const {
    return dict_.size() == 1 and dict_.begin()->first == 1 and dict_.begin()->second == 1;
}

bool UnivariatePolynomial::is_mul() const {
    return dict_.size() == 1 and dict_.begin()->first != 0 and
            dict_.begin()->second != 1 and dict_.begin()->second != 0;
}

bool UnivariatePolynomial::is_pow() const {
    return dict_.size() == 1 and dict_.begin()->second == 1 and
            dict_.begin()->first != 1 and dict_.begin()->first != 0;
}

RCP<const UnivariatePolynomial> add_uni_poly(const UnivariatePolynomial &a, const UnivariatePolynomial &b) {
    map_int_Expr dict;
    for (const auto &it : a.get_dict())
        dict[it.first] = it.second;
    for (const auto &it : b.get_dict())
        dict[it.first] += it.second;
    return univariate_polynomial(a.get_var(), (--(dict.end()))->first, std::move(dict));
}

RCP<const UnivariatePolynomial> neg_uni_poly(const UnivariatePolynomial &a) {
    map_int_Expr dict;
    for (const auto &it : a.get_dict())
        dict[it.first] = -1 * it.second;
    return univariate_polynomial(a.get_var(), (--(dict.end()))->first, std::move(dict));
}

RCP<const UnivariatePolynomial> sub_uni_poly(const UnivariatePolynomial &a, const UnivariatePolynomial &b) {
    map_int_Expr dict;
    for (const auto &it : a.get_dict())
        dict[it.first] = it.second;
    for (const auto &it : b.get_dict())
        dict[it.first] -= it.second;
    return univariate_polynomial(a.get_var(), (--(dict.end()))->first, std::move(dict));
}

RCP<const UnivariatePolynomial> mul_uni_poly(RCP<const UnivariatePolynomial> a, RCP<const UnivariatePolynomial> b) {
    map_int_Expr dict;
    for (const auto &i1 : a->get_dict())
        for (const auto &i2 : b->get_dict())
            dict[i1.first + i2.first] += i1.second * i2.second;
    return univariate_polynomial(a->get_var(), (--(dict.end()))->first, std::move(dict));
}

} // SymEngine
