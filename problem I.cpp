#include <bits/stdc++.h>
using namespace std;

class Tlong
{
public:
    const static int nmax=10000;
    Tlong(int n=0)
    {
        sign='+';
        len=0;
        if (n<0)
        {
            sign='-';
            n=abs(n);
        }
        if (n==0) ++len;
        while(n)
        {
            number[nmax-++len]=n%10;
            n/=10;
        }
    }
    bool is_zero() const;
    int get_length() const;
    int cmp(const Tlong &) const;
    int cmp_with_zero() const;
    bool operator<(const Tlong &) const;
    bool operator==(const Tlong &) const;
    bool operator>(const Tlong &) const;
    bool operator<=(const Tlong &) const;
    bool operator>=(const Tlong &) const;
    bool operator!=(const Tlong &) const;
    int operator[](int) const;
    Tlong operator-() const;
    Tlong operator+(const Tlong &) const;
    Tlong operator-(const Tlong &) const;
    Tlong& operator++();
    Tlong& operator--();
    Tlong operator*(int) const;
    Tlong operator*(const Tlong &) const;
    Tlong operator/(int) const;
    Tlong operator/(const Tlong &) const;
    int operator%(int) const;
    Tlong operator%(const Tlong &) const;
    Tlong& operator*=(const Tlong &);
    Tlong& operator+=(const Tlong &);
    Tlong& operator<<=(int);
    Tlong& operator>>=(int);
    operator bool() const;

    friend istream& operator>>(istream &in, Tlong &num);
    friend ostream& operator<<(ostream &out, const Tlong &num);

    friend void write_non_periodic_and_periodic_part_right_fraction(Tlong, Tlong);

private:
    char sign='+';
    int number[nmax]={};
    int len=1;
    void zeroing(int);
    int find_length(int) const;
    Tlong add_abs(const Tlong &) const;
    int cmp_abs(const Tlong &) const;
    Tlong sub_abs(const Tlong &) const;
    Tlong half_divide_abs(int) const;
    int half_divide_ost_abs(int) const;
    Tlong multiply(const Tlong &) const;
    Tlong divide_abs(const Tlong &) const;
    Tlong remaind_abs(const Tlong &denominator) const;
};

int Tlong::cmp(const Tlong &num) const
{
    if (sign!=num.sign)
    {
        if (sign == '+') return 1;
        return -1;
    }
    if (sign=='+') return cmp_abs(num);
    return -cmp_abs(num);
}

int Tlong::cmp_with_zero() const
{
    if (len == 1 && number[nmax-1] == 0) return 0;
    if (sign=='-') return -1;
    return 1;
}

bool Tlong::is_zero()const
{
    return (len == 1 && number[nmax-1] == 0);
}

int Tlong::get_length() const
{
    return len;
}

bool Tlong::operator<(const Tlong &num) const
{
    return cmp(num)==-1;
}

bool Tlong::operator==(const Tlong &num) const
{
    return cmp(num)==0;
}

bool Tlong::operator>(const Tlong &num) const
{
    return cmp(num)==1;
}

bool Tlong::operator<=(const Tlong &num) const
{
    return cmp(num)!=1;
}

bool Tlong::operator>=(const Tlong &num) const
{
    return cmp(num)!=-1;
}

bool Tlong::operator!=(const Tlong &num) const
{
    return cmp(num)!=0;
}

int Tlong::operator[](int number_dig_from_right_edge) const
{
    return number[nmax-number_dig_from_right_edge];
}

Tlong Tlong::operator-() const
{
    Tlong inverse_num=*this;
    if (is_zero() || sign == '-') inverse_num.sign='+';
    else inverse_num.sign='-';
    return inverse_num;
}
Tlong Tlong::operator+(const Tlong &num) const
{
    Tlong res;
    if (sign == num.sign)
    {
        res=add_abs(num);
        res.sign=sign;
        return res;
    }
    int compare_abs=cmp_abs(num), compare=cmp(num);
    res=sub_abs(num);
    if (compare*compare_abs == -1) res.sign='-';
    else res.sign='+';
    return res;
}

Tlong Tlong::operator-(const Tlong &num) const
{
    return *this+(-num);
}

Tlong& Tlong::operator++()
{
    int i=1;
    if (sign=='+')
    {
        while(number[nmax-i]==9)
        {
            number[nmax-i]=0;
            ++i;
        }
        ++number[nmax-i];
        if (i>len) ++len;
    }
    else
    {
        while (number[nmax-i]==0)
        {
            number[nmax-i]=9;
            ++i;
        }
        --number[nmax-i];
        if(is_zero()) sign='+';
        len=find_length(len);
    }
    return *this;
}

Tlong& Tlong::operator--()
{
    *this=-(++(-*this));
    return *this;
}

Tlong Tlong::operator*(int num) const
{
    Tlong res;
    if (is_zero() || num==0) return res;
    int S, p=0;
    if (sign=='+' && num>0 || sign=='-' && num<0) res.sign='+';
    else res.sign='-';
    for(int i=1; i<=len+11; ++i)
    {
        S=num*number[nmax-i]+p;
        p=S/10;
        res.number[nmax-i]=S%10;
    }
    res.len=res.find_length(len+11);
    return res;
}

Tlong Tlong::operator*(const Tlong &num) const
{
    if(num.len<len) return multiply(num);
    return num.multiply(*this);
}

Tlong Tlong::operator/(int denominator) const
{
    Tlong res(half_divide_abs(denominator));
    if(res.is_zero() || sign=='+' && denominator>0 || sign=='-' && denominator<0) return res;
    res.sign='-';
    return res;
}

Tlong Tlong::operator/(const Tlong &denominator) const
{
    Tlong res(divide_abs(denominator));
    if(res.is_zero() || sign==denominator.sign) return res;
    res.sign='-';
    return res;
}

int Tlong::operator%(int denominator) const
{
    int ost=half_divide_ost_abs(denominator);
    if (sign=='-') return -ost;
    return ost;
}

Tlong Tlong::operator%(const Tlong &denominator) const
{
    Tlong res(remaind_abs(denominator));
    if(res.is_zero() || sign=='+') return res;
    res.sign='-';
    return res;
}

Tlong& Tlong::operator*=(const Tlong &num)
{
    *this=*this*num;
    return *this;
}

Tlong& Tlong::operator+=(const Tlong &num)
{
    *this=*this+num;
    return *this;
}

Tlong& Tlong::operator<<=(int n_shifts)
{
    if(is_zero()) return *this;
    for(int i=nmax-len; i<nmax; ++i)
        number[i-n_shifts]=number[i];
    for(int i=nmax-n_shifts; i<nmax; ++i) number[i]=0;
    len+=n_shifts;
    return *this;
}

Tlong& Tlong::operator>>=(int n_shifts)
{
    for(int i=nmax-1-n_shifts; i>=nmax-len; --i)
        number[i+n_shifts]=number[i];
    for(int i=nmax-len; i<min(nmax-len+n_shifts, nmax-1); ++i) number[i]=0;
    len=max(1, len-n_shifts);
    return *this;
}

Tlong::operator bool() const
{
    return !(len == 1 && number[nmax-1] == 0);
}

void Tlong::zeroing(int n_end=nmax)
{
    sign='+';
    for (int i=nmax-len; i<n_end; ++i) number[i]=0;
    len=1;
}

int Tlong::find_length(int indent_from_right_edge=nmax) const
{
    int ind_start=nmax-indent_from_right_edge;
    while (ind_start<nmax && number[ind_start]==0) ++ind_start;
    if (ind_start==nmax) return 1;
    return nmax-ind_start;
}

Tlong Tlong::add_abs(const Tlong &num) const
{
    Tlong res;
    int length=max(len,num.len), S, p=0;
    for (int i=nmax-1; i>=nmax-length-1; --i)
    {
        S=number[i]+num.number[i]+p;
        res.number[i]=S%10;
        p=S/10;
    }
    if(res.number[nmax-length-1]) ++length;
    res.len=length;
    return res;
}

int Tlong::cmp_abs(const Tlong &num) const
{
    if (len > num.len) return 1;
    if (len < num.len) return -1;
    for (int i=nmax-len; i<nmax; ++i)
    {
        if (number[i]>num.number[i]) return 1;
        if (number[i]<num.number[i]) return -1;
    }
    return 0;
}

Tlong Tlong::sub_abs(const Tlong &num) const
{
    Tlong res;
    int length=max(len,num.len), S, p=0, length_res=1;
    if(cmp_abs(num)>=0)
    {
        for (int i=nmax-1; i>=nmax-length; --i)
        {
            S=number[i]-num.number[i]-p;
            res.number[i]=(S+10)%10;
            p=1-(S+10)/10;
            if (res.number[i]) length_res=nmax-i;
        }
    }
    else
    {
        for (int i=nmax-1; i>=nmax-length; --i)
        {
            S=num.number[i]-number[i]-p;
            res.number[i]=(S+10)%10;
            p=1-(S+10)/10;
            if (res.number[i]) length_res=nmax-i;
        }
    }
    res.len=length_res;
    return res;
}

Tlong Tlong::half_divide_abs(int denominator) const
{
    Tlong res;
    denominator=abs(denominator);
    int remaind=0;
    for (int i=nmax-len; i<nmax; ++i)
    {
        remaind*=10;
        remaind+=number[i];
        res.number[i]=remaind/denominator;
        remaind%=denominator;
    }
    res.len=res.find_length(len);
    return res;
}

int Tlong::half_divide_ost_abs(int denominator) const
{
    denominator=abs(denominator);
    int remaind=0;
    for (int i=nmax-len; i<nmax; ++i)
    {
        remaind=remaind*10+number[i];
        remaind%=denominator;
    }
    return remaind;
}

Tlong Tlong::multiply(const Tlong &num) const
{
    Tlong res;
    if (is_zero() || num.is_zero()) return res;
    int S, p=0;
    if (sign==num.sign) res.sign='+';
    else res.sign='-';
    for(int i=1; i<=num.len; ++i)
        for (int j=1; j<=len+1; ++j)
        {
            S=num.number[nmax-i]*number[nmax-j]+p+res.number[nmax+1-i-j];
            p=S/10;
            res.number[nmax+1-i-j]=S%10;
        }
    int length=len+num.len-1;
    if (res.number[nmax-length-1]) ++length;
    res.len=length;
    return res;
}

Tlong Tlong::divide_abs(const Tlong &denominator) const
{
    Tlong remaind;
    Tlong res;
    int dig=0;
    for(int i=len; i>0; --i)
    {
        remaind<<=1;
        remaind.number[nmax-1]=number[nmax-i];
        while(remaind.cmp_abs(denominator)!=-1)
        {
            remaind=remaind.sub_abs(denominator);
            ++dig;
        }
        res.number[nmax-i]=dig;
        dig=0;
    }
    if(len<=denominator.len)
    {
        res.len=1;
        return res;
    }
    res.len=len-denominator.len;
    if (res.number[nmax-res.len-1]) ++res.len;
    return res;
}

Tlong Tlong::remaind_abs(const Tlong &denominator) const
{
    Tlong remaind;
    for(int i=len; i>0; --i)
    {
        remaind<<=1;
        remaind.number[nmax-1]=number[nmax-i];
        while(remaind.cmp_abs(denominator)!=-1)
            remaind=remaind.sub_abs(denominator);
    }
    return remaind;
}

istream& operator>>(istream &in, Tlong &num)
{
    string S;
    in>>S;
    int length=S.size();
    num.zeroing(Tlong::nmax-length);
    if (S[0]=='+' || S[0]=='-')
    {
        num.sign=S[0];
        S.erase(0,1);
        --length;
    }
    num.len=length;
    for (int i=0; i<num.len; ++i)
        num.number[Tlong::nmax-num.len+i]=S[i]-48;
    return in;
}

ostream& operator<<(ostream &out, const Tlong &num)
{
    if(num.sign=='-') out<<num.sign;
    for (int i=Tlong::nmax-num.len; i<Tlong::nmax; ++i)
        out<<num.number[i];
    return out;
}

int len_n(int n, int base=10)
{
    if(n==0) return 1;
    int cnt=0;
    while(n)
    {
        n/=base;
        ++cnt;
    }
    return cnt;
}

Tlong pow(int base, int exp)
{
    int exp_len_in_bits=len_n(exp, 2);
    Tlong res(1);
    for(int i=exp_len_in_bits-1; i>=0; --i)
    {
        res=res*res;
        if(exp>>i&1) res=res*base;
    }
    return res;
}

Tlong nsd(Tlong a, Tlong b)
{
    while(a && b)
    {
        if(a<b) swap(a,b);
        a=a%b;
    }
    return a+b;

}

Tlong nsk(const Tlong &a, const Tlong &b)
{
    return a*b/nsd(a,b);
}

void write_non_periodic_and_periodic_part_right_fraction(Tlong a, Tlong b)
{
    int length_non_periodic_part=0;
    Tlong nsd_a_b=nsd(a,b);
    a=a/nsd_a_b;
    b=b/nsd_a_b;
    Tlong copy_b=b;
    while(copy_b[1]==0)
    {
        copy_b>>=1;
        ++length_non_periodic_part;
    }
    while(copy_b%2==0)
    {
        copy_b=copy_b/2;
        ++length_non_periodic_part;
    }
    while(copy_b%5==0)
    {
        copy_b=copy_b/5;
        ++length_non_periodic_part;
    }
    Tlong res(0);
    int dig=0;
    for(int i=length_non_periodic_part; i>0; --i)
    {
        a<<=1;
        while(a.cmp_abs(b)!=-1)
        {
            a=a.sub_abs(b);
            ++dig;
        }
        res.number[Tlong::nmax-i]=dig;
        dig=0;
    }
    res.len=length_non_periodic_part;
    cout<<res;                           //incorrect object res: res.len!=res.find_length()
    res=0;                               //correct object res
    if(a.cmp_with_zero()==0)  return;
    Tlong copy_a=a;
    int length_periodic_part=0;
    do
    {
        a<<=1;
        while(a.cmp_abs(b)!=-1)
        {
            a=a.sub_abs(b);
            ++dig;
        }
        res.number[length_periodic_part++]=dig;
        dig=0;
    } while(a!=copy_a);
    res.len=Tlong::nmax;
    res>>=Tlong::nmax-length_periodic_part;
    res.len=length_periodic_part;
    cout<<'('<<res<<')';
}

/*void inp_2nums(Tlong &num1, Tlong &num2)
{
    string S;
    cin>>S;
    num1.zeroing();
    num2.zeroing();
    int n1_len=0, n2_len=0;
    num1.sign='+';
    if (S[0]=='+' || S[0]=='-')
    {
        num1.sign=S[0];
        S.erase(0,1);
    }
    int i=1;
    int len_S=S.size();
    while (S[len_S-i]!='+' && S[len_S-i]!='-')
    {
        ++n2_len;
        num2.number[nmax-n2_len]=S[len_S-i]-48;
        ++i;
    }
    num2.sign=S[len_S-i];
    ++i;
    while (i<=len_S)
    {
        ++n1_len;
        num1.number[nmax-n1_len]=S[len_S-i]-48;
        ++i;
    }
    num1.len=n1_len;
    num2.len=n2_len;
}*/

Tlong factorial(int n)
{
    Tlong res(1);
    for (int i=2; i<=n; ++i)
    {
        res=res*i;
    }
    return res;
}

Tlong Fibonacci(int n)
{
    Tlong F_n1(1), F_n2(1), temp;
    if (n == 0) return Tlong(0);
    for (int i=2; i<n; ++i)
    {
        temp=F_n2;
        F_n2+=F_n1;
        F_n1=temp;
    }
    return F_n2;
}

int main()
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    cout.tie(0);
    Tlong a,b;
    cin>>a>>b;
    cout<<a/b;
    a=a%b;
    if(a.cmp_with_zero())
    {
        cout<<'.';
        write_non_periodic_and_periodic_part_right_fraction(a,b);
    }
    return 0;
}
