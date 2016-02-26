/**
 *  \file polynomial.h
 *  Class for sparse Polynomial: UnivariateIntPolynomial and Polynomial
 **/
#ifndef SYMENGINE_POLYNOMIALS_H
#define SYMENGINE_POLYNOMIALS_H

#include <symengine/monomials.h>
#include <symengine/dict.h>
#include <symengine/basic.h>
#include <symengine/integer.h>
#include <symengine/symbol.h>
#include <symengine/expression.h>
#include <map>

namespace SymEngine {

class UnivariateIntPolynomial : public Basic{
public:
    //! `degree` : Degree of UnivariateIntPolynomial
    //! `var_` : Variable of the uni-variate UnivariateIntPolynomial
    //! `dict_` : holds the UnivariateIntPolynomial
    // UnivariateIntPolynomial x**2 + 2*x + 1 has dict_ = {{0, 1}, {1, 2}, {2, 1}} with var_ = "x"
    unsigned int degree_;
    RCP<const Symbol> var_;
    map_uint_mpz dict_;
public:
    IMPLEMENT_TYPEID(UNIVARIATEINTPOLYNOMIAL)
    //! Constructor of UnivariateIntPolynomial class
    UnivariateIntPolynomial(const RCP<const Symbol> &var, const unsigned int &degree, map_uint_mpz&& dict);
    //! Constructor using a dense vector of mpz_class coefficients
    UnivariateIntPolynomial(const RCP<const Symbol> &var, const std::vector<mpz_class> &v);

    static RCP<const UnivariateIntPolynomial> create(const RCP<const Symbol> &var,
            const std::vector<mpz_class> &v) {
        return make_rcp<const UnivariateIntPolynomial>(var, v);
    }

    //! \return true if canonical
    bool is_canonical(const unsigned int &degree, const map_uint_mpz& dict) const;
    //! \return size of the hash
    std::size_t __hash__() const;
    /*! Equality comparator
     * \param o - Object to be compared with
     * \return whether the 2 objects are equal
     * */
    bool __eq__(const Basic &o) const;
    int compare(const Basic &o) const;
    std::string __str__() const;
    
    /*! Creates appropriate instance (i.e Symbol, Integer,
    * Mul, Pow, UnivariateIntPolynomial) depending on the size of dictionary `d`.
    */
    static RCP<const Basic> from_dict(const RCP<const Symbol> &var, map_uint_mpz &&d);
    /*!
    * Adds coef*var_**n to the dict_
    */
    static void dict_add_term(map_uint_mpz &d,
            const mpz_class &coef, const unsigned int &n);
    mpz_class max_coef() const;
    //! Evaluates the UnivariateIntPolynomial at value x
    mpz_class eval(const mpz_class &x) const;
    //! Evaluates the UnivariateIntPolynomial at value 2**x
    mpz_class eval_bit(const int &x) const;

    //! \return `true` if `0`
    bool is_zero() const;
    //! \return `true` if `1`
    bool is_one() const;
    //! \return `true` if `-1`
    bool is_minus_one() const;
    //! \return `true` if integer
    bool is_integer() const;
    //! \return `true` if symbol
    bool is_symbol() const;
    //! \return `true` if mul
    bool is_mul() const;
    //! \return `true` if pow
    bool is_pow() const;

    virtual vec_basic get_args() const;

    inline RCP<const Symbol> get_var() const {
        return var_;
    }
    inline const map_uint_mpz& get_dict() const {
        return dict_;
    };
}; //UnivariateIntPolynomial

//! Adding two UnivariateIntPolynomial a and b
RCP<const UnivariateIntPolynomial> add_poly(const UnivariateIntPolynomial &a, const UnivariateIntPolynomial &b);
//! Negative of a UnivariateIntPolynomial
RCP<const UnivariateIntPolynomial> neg_poly(const UnivariateIntPolynomial &a);
//! Subtracting two UnivariateIntPolynomial a and b
RCP<const UnivariateIntPolynomial> sub_poly(const UnivariateIntPolynomial &a, const UnivariateIntPolynomial &b);
//! Multiplying two UnivariateIntPolynomial a and b
RCP<const UnivariateIntPolynomial> mul_poly(RCP<const UnivariateIntPolynomial> a, RCP<const UnivariateIntPolynomial> b);

inline RCP<const UnivariateIntPolynomial> pow_exp(RCP<const UnivariateIntPolynomial> a, RCP<const UnivariateIntPolynomial> b);

inline RCP<const UnivariateIntPolynomial> int_coeff_polynomial(RCP<const Symbol> i, unsigned int deg, map_uint_mpz&& dict)
{
    return make_rcp<const UnivariateIntPolynomial>(i, deg, std::move(dict));
}


class UnivariatePolynomial : public Basic{
public:
    unsigned int degree_;
    RCP<const Symbol> var_;
    map_uint_Expr dict_;
public:
    IMPLEMENT_TYPEID(UNIVARIATEPOLYNOMIAL)
    //! Constructor of UnivariatePolynomial class
    UnivariatePolynomial(const RCP<const Symbol> &var, const unsigned int &degree, map_uint_Expr&& dict);
    //! Constructor using a dense vector of Expression
    UnivariatePolynomial(const RCP<const Symbol> &var, const std::vector<Expression> &v);

    static RCP<const UnivariatePolynomial> create(const RCP<const Symbol> &var, const std::vector<Expression> &v) {
        return make_rcp<const UnivariatePolynomial>(var, v);
    }

    bool is_canonical(const unsigned int &degree, const map_uint_Expr& dict) const;
    std::size_t __hash__() const;
    bool __eq__(const Basic &o) const;
    int compare(const Basic &o) const;
    std::string __str__() const;
    
    /*! Creates appropriate instance (i.e Symbol, Integer,
    * Mul, Pow, UnivariatePolynomial) depending on the size of dictionary `d`.
    */
    static RCP<const Basic> from_dict(const RCP<const Symbol> &var, map_uint_Expr &&d);
    /*!
    * Adds coef*var_**n to the dict_
    */
    static void dict_add_term(map_uint_Expr &d, const Expression &coef, const unsigned int &n);
    Expression max_coef() const;
    //! Evaluates the UnivariatePolynomial at value x
    Expression eval(const Expression &x) const;

    //! \return `true` if `0`
    bool is_zero() const;
    //! \return `true` if `1`
    bool is_one() const;
    //! \return `true` if `-1`
    bool is_minus_one() const;
    //! \return `true` if integer
    bool is_integer() const;
    //! \return `true` if symbol
    bool is_symbol() const;
    //! \return `true` if mul
    bool is_mul() const;
    //! \return `true` if pow
    bool is_pow() const;

    virtual vec_basic get_args() const;

    inline RCP<const Symbol> get_var() const {
        return var_;
    }
    inline const map_uint_Expr& get_dict() const {
        return dict_;
    };
}; //UnivariatePolynomial

//! Adding two UnivariatePolynomial a and b
RCP<const UnivariatePolynomial> add_uni_poly(const UnivariatePolynomial &a, const UnivariatePolynomial &b);
//! Negative of a UnivariatePolynomial
RCP<const UnivariatePolynomial> neg_uni_poly(const UnivariatePolynomial &a);
//! Subtracting two UnivariatePolynomial a and b
RCP<const UnivariatePolynomial> sub_uni_poly(const UnivariatePolynomial &a, const UnivariatePolynomial &b);
//! Multiplying two UnivariatePolynomial a and b
RCP<const UnivariatePolynomial> mul_uni_poly(RCP<const UnivariatePolynomial> a, RCP<const UnivariatePolynomial> b);

inline RCP<const UnivariatePolynomial> pow_exp(RCP<const UnivariatePolynomial> a, RCP<const UnivariatePolynomial> b);

inline RCP<const UnivariatePolynomial> univariate_polynomial(RCP<const Symbol> i, unsigned int deg, map_uint_Expr&& dict)
{
    return make_rcp<const UnivariatePolynomial>(i, deg, std::move(dict));
}

class UnivariateExprPolynomial {
private:
    RCP<const UnivariatePolynomial> poly_;
public:
    //! Construct UnivariateExprPolynomial from UnivariatePolynomial
#if defined(HAVE_SYMENGINE_IS_CONSTRUCTIBLE)
    template <typename T, typename = typename std::enable_if<std::is_constructible<RCP<const UnivariatePolynomial>, T &&>::value>::type>
#else
    template <typename T>
#endif
    UnivariateExprPolynomial(T &&o) : poly_(std::forward<T>(o)) {}
    UnivariateExprPolynomial() {}
    ~UnivariateExprPolynomial() SYMENGINE_NOEXCEPT {}
    UnivariateExprPolynomial(const UnivariateExprPolynomial &) = default;
    UnivariateExprPolynomial(UnivariateExprPolynomial &&other) SYMENGINE_NOEXCEPT : poly_(std::move(other.poly_)) {}
    UnivariateExprPolynomial(int i);    
    UnivariateExprPolynomial(Expression expr);
    
    UnivariateExprPolynomial &operator=(const UnivariateExprPolynomial &) = default;
    UnivariateExprPolynomial &operator=(UnivariateExprPolynomial &&other) SYMENGINE_NOEXCEPT 
    {
        if (this != &other)
            this->poly_.ptr() = std::move(other.poly_.ptr());
        return *this;
    }
     
    friend std::ostream &operator<<(std::ostream &os, const UnivariateExprPolynomial &expr)
    {
        os << expr.poly_.ptr()->__str__();
        return os;
    }
    
    friend UnivariateExprPolynomial operator+(const UnivariateExprPolynomial &a, const UnivariateExprPolynomial &b)
    {
        return UnivariateExprPolynomial(add_uni_poly(*(a.poly_.ptr()), *(b.poly_.ptr())));
    }
    
    UnivariateExprPolynomial &operator+=(const UnivariateExprPolynomial &other)
    {
        poly_ = add_uni_poly(*(poly_.ptr()), *(other.poly_.ptr()));
        return *this;
    }
    
    friend UnivariateExprPolynomial operator-(const UnivariateExprPolynomial &a, const UnivariateExprPolynomial &b)
    {
        return UnivariateExprPolynomial(sub_uni_poly(*(a.poly_.ptr()), *(b.poly_.ptr())));
    }
    
    UnivariateExprPolynomial operator-() const
    {
        UnivariateExprPolynomial retval(*this);
        neg_uni_poly(*(retval.poly_.ptr()));
        return retval;
    }
    
    UnivariateExprPolynomial &operator-=(const UnivariateExprPolynomial &other)
    {
        poly_ = sub_uni_poly(*(poly_.ptr()), *(other.poly_.ptr()));
        return *this;
    }
    
    friend UnivariateExprPolynomial operator*(const UnivariateExprPolynomial &a, const UnivariateExprPolynomial &b)
    {   
        return UnivariateExprPolynomial(mul_uni_poly(a.poly_, b.poly_));
    }   
    
    friend UnivariateExprPolynomial operator/(const UnivariateExprPolynomial &a, const Expression &b);
        
    UnivariateExprPolynomial &operator*=(const UnivariateExprPolynomial &other)
    {   
        poly_ = mul_uni_poly(poly_, other.poly_);
        return *this;
    }   
    
    bool operator==(const UnivariateExprPolynomial &other) const
    {   
        return eq(*poly_, *other.poly_);
    }   
    
    bool operator==(int i) const
    {
      return 1;//eq(*poly_, 
    }
     
    bool operator!=(const UnivariateExprPolynomial &other) const
    {   
        return not (*this == other);
    }   
    //! Method to get UnivariatePolynomial from UnivariateExprPolynomial
    const RCP<const UnivariatePolynomial>& get_univariate_poly() const
    {   
        return poly_;
    }   
    
    /*const RCP<const UnivariatePolynomial> get_basic() const
    {
        return poly_;
        //return make_rcp<const Symbol>("x");//make_rcp<const UnivariatePolynomial>(poly_->var_, poly_->get_args());
    }*/

    int compare(const UnivariateExprPolynomial &other)
    {
       return eq(*poly_, *other.poly_); 
    }
}; //UnivariateExprPolynomial


class sym_hash{
public:
  size_t operator()(const Symbol &s) const{
    return s.__hash__();
  }
};

class sym_compare{
  public:
  size_t operator()(const Symbol &a, const Symbol &b){
    return a.compare(b);
  }
};

class sym_eq{
 public:
  bool operator()(const Symbol &a, const Symbol &b){
    return a.__eq__(b);
  }
};

int umap_vec_mpz_compare(umap_vec_mpz &a, umap_vec_mpz &b){
  if(a.size() < b.size())
    return (a.size() < b.size()) ? -1 : 1;
  return 0;
};



}  //SymEngine

#endif
