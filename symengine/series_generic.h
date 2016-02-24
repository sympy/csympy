/**
 *  \file series_generic.h
 *  Class for univariate series.
 *
 **/
#ifndef SYMENGINE_SERIES_GENERIC_H
#define SYMENGINE_SERIES_GENERIC_H

#include <symengine/polynomial.h>
#include <symengine/basic.h>
#include <symengine/expression.h>
#include <symengine/rational.h>
#include <symengine/series.h>

namespace SymEngine {
//! UnivariateSeries Class
class UnivariateSeries : public SeriesBase<UnivariateExprPolynomial, Expression, UnivariateSeries> {
    // UnivariateSeries 1 + 2*x + x**2 + O(x**5) has dict_ = {{0, 1}, {1, 2}, {2, 1}} with var_ = "x" and prec_ = 5
    unsigned int prec_;
public:
    IMPLEMENT_TYPEID(UNIVARIATESERIES)
    UnivariateSeries(const UnivariateExprPolynomial sp, const std::string varname, const unsigned degree) : SeriesBase(std::move(sp), varname, degree) {}
    static RCP<const UnivariateSeries> series(const RCP<const Basic> &t, const std::string &x, unsigned int prec);
    virtual std::size_t __hash__() const;
    virtual int compare(const Basic &o) const;
    virtual RCP<const Basic> as_basic() const;
    virtual umap_int_basic as_dict() const;
    virtual RCP<const Basic> get_coeff(int) const;
    static UnivariateExprPolynomial var(const std::string &s);
      
    static Expression convert(const Integer &x);
    static Expression convert(const mpq_class &x);
    static Expression convert(const Rational &x);
    static Expression convert(const Number &x);

    static unsigned ldegree(const UnivariateExprPolynomial &s);
    static UnivariateExprPolynomial mul(const UnivariateExprPolynomial &s, const UnivariateExprPolynomial &r, unsigned prec);
    static UnivariateExprPolynomial pow(const UnivariateExprPolynomial &s, int n, unsigned prec);
    static Expression find_cf(const UnivariateExprPolynomial &s, const UnivariateExprPolynomial &var, unsigned deg);
    static Expression root(Expression &c, unsigned n);
    static UnivariateExprPolynomial diff(const UnivariateExprPolynomial &s, const UnivariateExprPolynomial &var);
    static UnivariateExprPolynomial integrate(const UnivariateExprPolynomial &s, const UnivariateExprPolynomial &var);
    static UnivariateExprPolynomial subs(const UnivariateExprPolynomial &s, const UnivariateExprPolynomial &var, const UnivariateExprPolynomial &r, unsigned prec);
   
    static Expression sin(const Expression &c); 
    static Expression cos(const Expression &c); 
    static Expression tan(const Expression &c); 
    static Expression asin(const Expression &c); 
    static Expression acos(const Expression &c); 
    static Expression atan(const Expression &c); 
    static Expression sinh(const Expression &c); 
    static Expression cosh(const Expression &c); 
    static Expression tanh(const Expression &c); 
    static Expression asinh(const Expression &c); 
    static Expression acosh(const Expression &c);
    static Expression atanh(const Expression &c); 
    static Expression exp(const Expression &c); 
    static Expression log(const Expression &c);  
    
    bool is_canonical(const UnivariatePolynomial&, const unsigned int &) const;

};

inline RCP<const UnivariateSeries> univariate_series(RCP<const Symbol> i, unsigned int prec, const map_uint_mpz& dict)
{
    return make_rcp<const UnivariateSeries>(i, prec, dict);
}

RCP<const UnivariateSeries> add_uni_series (const UnivariateSeries& a, const UnivariateSeries &b);    
RCP<const UnivariateSeries> neg_uni_series (const UnivariateSeries& a);    
RCP<const UnivariateSeries> sub_uni_series (const UnivariateSeries& a, const UnivariateSeries &b);   
RCP<const UnivariateSeries> mul_uni_series (const UnivariateSeries& a, const UnivariateSeries &b);
}  //SymEngine
#endif //SYMENGINE_SERIES_GENERIC_H
