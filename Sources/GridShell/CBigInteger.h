#ifndef _BIG_INT_
#define _BIG_INT_
//https://github.com/Wholanz/Encrypt-BigInteger/blob/master/%E6%BA%90%E4%BB%A3%E7%A0%81/cpp%E6%96%87%E4%BB%B6/main.cpp
//Stripped from iostream for memory friendly purposes
#include <string>
#include <stdexcept>
#include <algorithm>

class CBigIntegerError : public std::runtime_error {
  public:
    CBigIntegerError(const std::string& msg = " ") : runtime_error(msg) {
    }
};



class CBigInteger {

  private:
    std::string str_int;
    std::string legalDigit;



  public:
    static int CHAR_TO_DIGIT(char x);
    static char DIGIT_TO_CHAR(int x);

    CBigInteger(const std::string& str = "0");
    CBigInteger(const char* pstr);
    CBigInteger(const CBigInteger& b);

  public:

    const std::string& GetInteger()const {
      return str_int;
    }

  public:

    bool isLegal();

    bool isZero();

    void simplifyZero();

  private:

    int cmp(const CBigInteger& b) const;

    CBigInteger addPos(const CBigInteger& b) const;

    CBigInteger subPos(const CBigInteger& b) const;

    CBigInteger mulPos(const CBigInteger& b) const;

    CBigInteger divPos(const CBigInteger& b) const;

    CBigInteger modPos(const CBigInteger& b) const;

    CBigInteger pow(const CBigInteger& b) const;

  public:

    CBigInteger powMod(const CBigInteger& b, const CBigInteger& m) const;

  public:

    CBigInteger operator = (const CBigInteger& b);

    CBigInteger operator = (const std::string& str);

    CBigInteger operator - ()const;


};

#endif
