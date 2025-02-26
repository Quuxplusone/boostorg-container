// Boost.Container static_vector
// Unit Test

// Copyright (c) 2012-2013 Adam Wulkiewicz, Lodz, Poland.
// Copyright (c) 2012-2013 Andrew Hundt.

// Use, modification and distribution is subject to the Boost Software License,
// Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
#include <boost/core/lightweight_test.hpp>
#include <boost/container/vector.hpp>
#include <boost/container/list.hpp>
#include <boost/container/detail/iterator.hpp>
#include "../../intrusive/test/iterator_test.hpp"
#include <boost/container/detail/config_begin.hpp>

#include "static_vector_test.hpp"


template <typename T, size_t N>
void test_ctor_ndc()
{
   static_vector<T, N> s;
   BOOST_STATIC_ASSERT((static_vector<T, N>::static_capacity) == N);
   BOOST_TEST_EQ(s.size() , 0u);
   BOOST_TEST(s.capacity() == N);
   BOOST_TEST(s.max_size() == N);
   BOOST_TEST_THROWS( (void)s.at(0u), out_of_range_t);
}

template <typename T, size_t N>
void test_ctor_nc(size_t n)
{
   static_vector<T, N> s(n);
   BOOST_STATIC_ASSERT((static_vector<T, N>::static_capacity) == N);
   BOOST_TEST(s.size() == n);
   BOOST_TEST(s.capacity() == N);
   BOOST_TEST(s.max_size() == N);
   BOOST_TEST_THROWS( (void)s.at(n), out_of_range_t);
   if ( 1 < n )
   {
      s[0] = 10;
      BOOST_TEST(T(10) == s[0]);
      BOOST_TEST(T(10) == s.at(0));
      s.at(1) = 20;
      BOOST_TEST(T(20) == s[1]);
      BOOST_TEST(T(20) == s.at(1));
   }
}

template <typename T, size_t N>
void test_ctor_nd(size_t n, T const& v)
{
   static_vector<T, N> s(n, v);
   BOOST_STATIC_ASSERT((static_vector<T, N>::static_capacity) == N);
   BOOST_TEST(s.size() == n);
   BOOST_TEST(s.capacity() == N);
   BOOST_TEST_THROWS( (void)s.at(n), out_of_range_t);
   if ( 1 < n )
   {
      BOOST_TEST(v == s[0]);
      BOOST_TEST(v == s.at(0));
      BOOST_TEST(v == s[1]);
      BOOST_TEST(v == s.at(1));
      s[0] = T(10);
      BOOST_TEST(T(10) == s[0]);
      BOOST_TEST(T(10) == s.at(0));
      s.at(1) = T(20);
      BOOST_TEST(T(20) == s[1]);
      BOOST_TEST(T(20) == s.at(1));
   }
}

void test_support_for_initializer_list()
{
#if !defined(BOOST_NO_CXX11_HDR_INITIALIZER_LIST)
   {
      typedef static_vector<int, 2> sv_cap_2;
      sv_cap_2 sv = {10, 8};
      BOOST_TEST(10 == sv[0]);
      BOOST_TEST(8 == sv[1]);

      BOOST_TEST_THROWS(sv_cap_2({1, 1, 1}), bad_alloc_t);
   }

   {
      static_vector<int, 2> sv;
      sv.assign({1, 2});
      BOOST_TEST(1 == sv[0]);
      BOOST_TEST(2 == sv[1]);

      BOOST_TEST_THROWS(sv.assign({1, 2, 3}), bad_alloc_t);

      static_vector<int, 3> greaterThanSv = {1, 2, 3};
      BOOST_TEST_THROWS(sv = greaterThanSv, bad_alloc_t);
   }

   {
      static_vector<int, 2> sv;
      sv.insert(sv.begin(), {99, 95});
      BOOST_TEST(99 == sv[0]);
      BOOST_TEST(95 == sv[1]);

      BOOST_TEST_THROWS(sv.insert(sv.begin(), {101, 102, 103}), bad_alloc_t);
   }
#endif
}

template <typename T, size_t N>
void test_resize_nc(size_t n)
{
   static_vector<T, N> s;

   s.resize(n);
   BOOST_TEST(s.size() == n);
   BOOST_TEST(s.capacity() == N);
   BOOST_TEST_THROWS( (void)s.at(n), out_of_range_t);
   if ( 1 < n )
   {
      s[0] = 10;
      BOOST_TEST(T(10) == s[0]);
      BOOST_TEST(T(10) == s.at(0));
      s.at(1) = 20;
      BOOST_TEST(T(20) == s[1]);
      BOOST_TEST(T(20) == s.at(1));
   }
}

template <typename T, size_t N>
void test_resize_nd(size_t n, T const& v)
{
   static_vector<T, N> s;

   s.resize(n, v);
   BOOST_TEST(s.size() == n);
   BOOST_TEST(s.capacity() == N);
   BOOST_TEST_THROWS( (void)s.at(n), out_of_range_t);
   if ( 1 < n )
   {
      BOOST_TEST(v == s[0]);
      BOOST_TEST(v == s.at(0));
      BOOST_TEST(v == s[1]);
      BOOST_TEST(v == s.at(1));
      s[0] = T(10);
      BOOST_TEST(T(10) == s[0]);
      BOOST_TEST(T(10) == s.at(0));
      s.at(1) = T(20);
      BOOST_TEST(T(20) == s[1]);
      BOOST_TEST(T(20) == s.at(1));
   }
}

template <typename T, size_t N>
void test_push_back_nd()
{
   static_vector<T, N> s;

   BOOST_TEST(s.size() == 0);
   BOOST_TEST_THROWS( (void)s.at(0), out_of_range_t);

   for ( size_t i = 0 ; i < N ; ++i )
   {
      T t(static_cast<int>(i));
      s.push_back(t);
      BOOST_TEST(s.size() == i + 1);
      BOOST_TEST_THROWS( (void)s.at(i + 1), out_of_range_t);
      BOOST_TEST(T((int)i) == s.at(i));
      BOOST_TEST(T((int)i) == s[i]);
      BOOST_TEST(T((int)i) == s.back());
      BOOST_TEST(T(0) == s.front());
      BOOST_TEST(T((int)i) == *(s.data() + i));
   }
}

template <typename T, size_t N>
void test_pop_back_nd()
{
   static_vector<T, N> s;

   for ( size_t i = 0 ; i < N ; ++i )
   {
      T t((int)i);
      s.push_back(t);
   }

   for ( size_t i = N ; i > 1 ; --i )
   {
      s.pop_back();
      BOOST_TEST(s.size() == i - 1);
      BOOST_TEST_THROWS( (void)s.at(i - 1), out_of_range_t);
      BOOST_TEST(T((int)i - 2) == s.at(i - 2));
      BOOST_TEST(T((int)i - 2) == s[i - 2]);
      BOOST_TEST(T((int)i - 2) == s.back());
      BOOST_TEST(T(0) == s.front());
   }
}

template <typename It1, typename It2>
void test_compare_ranges(It1 first1, It1 last1, It2 first2, It2 last2)
{
   BOOST_TEST(boost::container::iterator_udistance(first1, last1) == boost::container::iterator_udistance(first2, last2));
   for ( ; first1 != last1 && first2 != last2 ; ++first1, ++first2 )
      BOOST_TEST(*first1 == *first2);
}

template <typename T, size_t N, typename C>
void test_copy_and_assign(C const& c)
{
   {
      static_vector<T, N> s(c.begin(), c.end());
      BOOST_TEST(s.size() == c.size());
      test_compare_ranges(s.begin(), s.end(), c.begin(), c.end());
   }
   {
      static_vector<T, N> s;
      BOOST_TEST(0 == s.size());
      s.assign(c.begin(), c.end());
      BOOST_TEST(s.size() == c.size());
      test_compare_ranges(s.begin(), s.end(), c.begin(), c.end());
   }
}

template <typename T, size_t N>
void test_copy_and_assign_nd(T const& val)
{
   static_vector<T, N> s;
   vector<T> v;
   list<T> l;

   for ( size_t i = 0 ; i < N ; ++i )
   {
      T t((int)i);
      s.push_back(t);
      v.push_back(t);
      l.push_back(t);
   }
   // copy ctor
   {
      static_vector<T, N> s1(s);
      BOOST_TEST(s.size() == s1.size());
      test_compare_ranges(s.begin(), s.end(), s1.begin(), s1.end());
   }
   // copy assignment
   {
      static_vector<T, N> s1;
      BOOST_TEST(0 == s1.size());
      s1 = s;
      BOOST_TEST(s.size() == s1.size());
      test_compare_ranges(s.begin(), s.end(), s1.begin(), s1.end());
   }

   // ctor(Iter, Iter) and assign(Iter, Iter)
   test_copy_and_assign<T, N>(s);
   test_copy_and_assign<T, N>(v);
   test_copy_and_assign<T, N>(l);

   // assign(N, V)
   {
      static_vector<T, N> s1(s);
      test_compare_ranges(s.begin(), s.end(), s1.begin(), s1.end());
      vector<T> a(N, val);
      s1.assign(N, val);
      test_compare_ranges(a.begin(), a.end(), s1.begin(), s1.end());
   }
}

template <typename T, size_t N>
void test_iterators_nd()
{
   static_vector<T, N> s;
   vector<T> v;

   for ( size_t i = 0 ; i < N ; ++i )
   {
      s.push_back(T((int)i));
      v.push_back(T((int)i));
   }

   test_compare_ranges(s.begin(), s.end(), v.begin(), v.end());
   test_compare_ranges(s.rbegin(), s.rend(), v.rbegin(), v.rend());

   s.assign(v.rbegin(), v.rend());

   test_compare_ranges(s.begin(), s.end(), v.rbegin(), v.rend());
   test_compare_ranges(s.rbegin(), s.rend(), v.begin(), v.end());
}

template <typename T, size_t N>
void test_erase_nd()
{
   static_vector<T, N> s;
   typedef typename static_vector<T, N>::iterator It;
   typedef typename static_vector<T, N>::difference_type dif_t;

   for ( size_t i = 0 ; i < N ; ++i )
      s.push_back(T((int)i));

   // erase(pos)
   {
      for ( size_t i = 0 ; i < N ; ++i )
      {
          static_vector<T, N> s1(s);
          It it = s1.erase(s1.begin() + dif_t(i));
          BOOST_TEST(s1.begin() + dif_t(i) == it);
          BOOST_TEST(s1.size() == N - 1);
          for ( size_t j = 0 ; j < i ; ++j )
              BOOST_TEST(s1[j] == T((int)j));
          for ( size_t j = i+1 ; j < N ; ++j )
              BOOST_TEST(s1[j-1] == T((int)j));
      }
   }
   // erase(first, last)
   {
      size_t n = N/3;
      for ( size_t i = 0 ; i <= N ; ++i )
      {
          static_vector<T, N> s1(s);
          size_t removed = i + n < N ? n : N - i;
          It it = s1.erase(s1.begin() + dif_t(i), s1.begin() + dif_t(i + removed));
          BOOST_TEST(s1.begin() + dif_t(i) == it);
          BOOST_TEST(s1.size() == N - removed);
          for ( size_t j = 0 ; j < i ; ++j )
              BOOST_TEST(s1[j] == T((int)j));
          for ( size_t j = i+n ; j < N ; ++j )
              BOOST_TEST(s1[j-n] == T((int)j));
      }
   }
}

template <typename T, size_t N, typename SV, typename C>
void test_insert(SV const& s, C const& c)
{
   size_t h = N/2;
   size_t n = size_t(float(h)/1.5f);
   typedef typename static_vector<T, N>::difference_type dif_t;

   for ( size_t i = 0 ; i <= h ; ++i )
   {
      static_vector<T, N> s1(s);

      typename C::const_iterator it = c.begin();
      boost::container::iterator_uadvance(it, n);
      typename static_vector<T, N>::iterator
          it1 = s1.insert(s1.begin() + dif_t(i), c.begin(), it);

      BOOST_TEST(s1.begin() + dif_t(i) == it1);
      BOOST_TEST(s1.size() == h+n);
      for ( size_t j = 0 ; j < i ; ++j )
          BOOST_TEST(s1[j] == T((int)j));
      for ( size_t j = 0 ; j < n ; ++j )
          BOOST_TEST(s1[j+i] == T(100 + (int)j));
      for ( size_t j = 0 ; j < h-i ; ++j )
          BOOST_TEST(s1[j+i+n] == T((int)j+(int)i));
   }
}

template <typename T, size_t N>
void test_insert_nd(T const& val)
{
   size_t h = N/2;

   static_vector<T, N> s, ss;
   vector<T> v;
   list<T> l;

   typedef typename static_vector<T, N>::iterator It;
   typedef typename static_vector<T, N>::difference_type dif_t;

   for ( size_t i = 0 ; i < h ; ++i )
   {
      s.push_back(T((int)i));
      ss.push_back(T(100 + (int)i));
      v.push_back(T(100 + (int)i));
      l.push_back(T(100 + (int)i));
   }

   // insert(pos, val)
   {
      for ( size_t i = 0 ; i <= h ; ++i )
      {
          static_vector<T, N> s1(s);
          It it = s1.insert(s1.begin() + dif_t(i), val);
          BOOST_TEST(s1.begin() + dif_t(i) == it);
          BOOST_TEST(s1.size() == h+1);
          for ( size_t j = 0 ; j < i ; ++j )
              BOOST_TEST(s1[j] == T((int)j));
          BOOST_TEST(s1[i] == val);
          for ( size_t j = 0 ; j < h-i ; ++j )
              BOOST_TEST(s1[j+i+1] == T((int)j+(int)i));
      }
   }
   // insert(pos, n, val)
   {
      size_t n = size_t(float(h)/1.5f);
      for ( size_t i = 0 ; i <= h ; ++i )
      {
          static_vector<T, N> s1(s);
          It it = s1.insert(s1.begin() + dif_t(i), n, val);
          BOOST_TEST(s1.begin() + dif_t(i) == it);
          BOOST_TEST(s1.size() == h+n);
          for ( size_t j = 0 ; j < i ; ++j )
              BOOST_TEST(s1[j] == T((int)j));
          for ( size_t j = 0 ; j < n ; ++j )
              BOOST_TEST(s1[j+i] == val);
          for ( size_t j = 0 ; j < h-i ; ++j )
              BOOST_TEST(s1[j+i+n] == T((int)j+(int)i));
      }
   }
   // insert(pos, first, last)
   test_insert<T, N>(s, ss);
   test_insert<T, N>(s, v);
   test_insert<T, N>(s, l);
}

template <typename T>
void test_capacity_0_nd()
{
   static_vector<T, 10> v(5u, T(0));

   typedef static_vector<T, 0> static_vector_0_t;
   static_vector_0_t s;
   BOOST_TEST(s.size() == 0);
   BOOST_TEST(s.capacity() == 0);
   BOOST_TEST_THROWS((void)s.at(0), out_of_range_t);
   BOOST_TEST_THROWS(s.resize(5u, T(0)), bad_alloc_t);
   BOOST_TEST_THROWS(s.push_back(T(0)), bad_alloc_t);
   BOOST_TEST_THROWS(s.insert(s.end(), T(0)), bad_alloc_t);
   BOOST_TEST_THROWS(s.insert(s.end(), v.begin(), v.end()), bad_alloc_t);
   BOOST_TEST_THROWS(s.assign(v.begin(), v.end()), bad_alloc_t);
   BOOST_TEST_THROWS(s.assign(5u, T(0)), bad_alloc_t);
   BOOST_TEST_THROWS(s.assign(5u, T(0)), bad_alloc_t);
   BOOST_TEST_THROWS(static_vector_0_t s2(v.begin(), v.end()), bad_alloc_t);
   BOOST_TEST_THROWS(static_vector_0_t s1(5u, T(0)), bad_alloc_t);
}

template <typename T, size_t N>
void test_exceptions_nd()
{
   static_vector<T, N> v(N, T(0));
   typedef static_vector<T, N/2> static_vector_n_half_t;
   static_vector_n_half_t s(N/2, T(0));

   BOOST_TEST_THROWS(s.resize(N, T(0)), bad_alloc_t);
   BOOST_TEST_THROWS(s.push_back(T(0)), bad_alloc_t);
   BOOST_TEST_THROWS(s.insert(s.end(), T(0)), bad_alloc_t);
   BOOST_TEST_THROWS(s.insert(s.end(), 1, T(0)), bad_alloc_t);
   BOOST_TEST_THROWS(s.insert(s.end(), v.begin(), v.end()), bad_alloc_t);
   BOOST_TEST_THROWS(s.assign(v.begin(), v.end()), bad_alloc_t);
   BOOST_TEST_THROWS(s.assign(N, T(0)), bad_alloc_t);
   BOOST_TEST_THROWS(static_vector_n_half_t s2(v.begin(), v.end()), bad_alloc_t);
   BOOST_TEST_THROWS(static_vector_n_half_t s1(N/2+1, T(0)), bad_alloc_t);
}

template <typename T, size_t N>
void test_swap_and_move_nd()
{
   {
      static_vector<T, N> v1, v2, v3, v4;
      static_vector<T, N> s1, s2;
      static_vector<T, N> s4;

      for (size_t i = 0 ; i < N ; ++i )
      {
          v1.push_back(T((int)i));
          v2.push_back(T((int)i));
          v3.push_back(T((int)i));
          v4.push_back(T((int)i));
      }
      for (size_t i = 0 ; i < N/2 ; ++i )
      {
          s1.push_back(T(100 + (int)i));
          s2.push_back(T(100 + (int)i));
          s4.push_back(T(100 + (int)i));
      }

      s1.swap(v1);
      s2 = boost::move(v2);
      static_vector<T, N> s3(boost::move(v3));
      s4.swap(v4);

      BOOST_TEST(v1.size() == N/2);
      BOOST_TEST(s1.size() == N);
      BOOST_TEST(v2.size() == 0);
      BOOST_TEST(s2.size() == N);
      BOOST_TEST(v3.size() == 0);
      BOOST_TEST(s3.size() == N);
      BOOST_TEST(v4.size() == N/2);
      BOOST_TEST(s4.size() == N);
      for (size_t i = 0 ; i < N/2 ; ++i )
      {
          BOOST_TEST(v1[i] == T(100 + (int)i));
          BOOST_TEST(v4[i] == T(100 + (int)i));
      }
      for (size_t i = 0 ; i < N ; ++i )
      {
          BOOST_TEST(s1[i] == T((int)i));
          BOOST_TEST(s2[i] == T((int)i));
          BOOST_TEST(s3[i] == T((int)i));
          BOOST_TEST(s4[i] == T((int)i));
      }
   }
   {
      static_vector<T, N> v1, v2, v3;
      static_vector<T, N/2> s1, s2, s3;

      for (size_t i = 0 ; i < N/2 ; ++i )
      {
          v1.push_back(T((int)i));
          v2.push_back(T((int)i));
          v3.push_back(T((int)i));
      }
      for (size_t i = 0 ; i < N/3 ; ++i )
      {
          s1.push_back(T(100 + (int)i));
          s2.push_back(T(100 + (int)i));
      }

      s1.swap(v1);
      s3 = v2;
      s2 = boost::move(v2);
      static_vector<T, N/2> s4(boost::move(v3));

      BOOST_TEST(v1.size() == N/3);
      BOOST_TEST(s1.size() == N/2);
      //iG moving does not imply emptying source
      //BOOST_TEST(v2.size() == 0);
      BOOST_TEST(s2.size() == N/2);
      BOOST_TEST(s3.size() == N/2);
      //iG moving does not imply emptying source
      //BOOST_TEST(v3.size() == 0);
      BOOST_TEST(s4.size() == N/2);
      for (size_t i = 0 ; i < N/3 ; ++i )
          BOOST_TEST(v1[i] == T(100 + (int)i));
      for (size_t i = 0 ; i < N/2 ; ++i )
      {
          BOOST_TEST(s1[i] == T((int)i));
          BOOST_TEST(s2[i] == T((int)i));
          BOOST_TEST(s3[i] == T((int)i));
          BOOST_TEST(s4[i] == T((int)i));
      }
   }
   {
      typedef static_vector<T, N/2> small_vector_t;
      static_vector<T, N> v(N, T(0));
      small_vector_t s(N/2, T(1));
      BOOST_TEST_THROWS(s.swap(v), bad_alloc_t);
      v.resize(N, T(0));
      BOOST_TEST_THROWS(s = boost::move(v), bad_alloc_t);
      BOOST_TEST_THROWS(s = v, bad_alloc_t);
      v.resize(N, T(0));
      BOOST_TEST_THROWS(small_vector_t s2(boost::move(v)), bad_alloc_t);
   }
}

template <typename T, size_t N>
void test_emplace_0p()
{
   //emplace_back()
   {
      static_vector<T, N> v;

      for (int i = 0 ; i < int(N) ; ++i )
          v.emplace_back();
      BOOST_TEST(v.size() == N);
      BOOST_TEST_THROWS(v.emplace_back(), bad_alloc_t);
   }
}

template <typename T, size_t N>
void test_emplace_2p()
{
   //emplace_back(pos, int, int)
   {
      static_vector<T, N> v;

      for (int i = 0 ; i < (int)N ; ++i )
          v.emplace_back(i, 100 + i);
      BOOST_TEST(v.size() == N);
      BOOST_TEST_THROWS(v.emplace_back((int)N, 100 + (int)N), bad_alloc_t);
      BOOST_TEST(v.size() == N);
      for (int i = 0 ; i < (int)N ; ++i )
          BOOST_TEST(v[std::size_t(i)] == T(i, 100 + i));
   }

   // emplace(pos, int, int)
   {
      typedef typename static_vector<T, N>::iterator It;
      typedef typename static_vector<T, N>::difference_type dif_t;

      int h = N / 2;

      static_vector<T, N> v;
      for ( int i = 0 ; i < h ; ++i )
          v.emplace_back(i, 100 + i);

      for ( int i = 0 ; i <= h ; ++i )
      {
          static_vector<T, N> vv(v);
          It it = vv.emplace(vv.begin() + dif_t(i), i+100, i+200);
          BOOST_TEST(vv.begin() + dif_t(i) == it);
          BOOST_TEST(vv.size() == size_t(h+1));
          for ( int j = 0 ; j < i ; ++j )
              BOOST_TEST(vv[std::size_t(j)] == T(j, j+100));
          BOOST_TEST(vv[std::size_t(i)] == T(i+100, i+200));
          for ( int j = 0 ; j < h-i ; ++j )
              BOOST_TEST(vv[std::size_t(j+i+1)] == T(j+i, j+i+100));
      }
   }
}

template <typename T, size_t N>
void test_sv_elem(T const& t)
{
   typedef static_vector<T, N> V;

   static_vector<V, N> v;

   v.push_back(V(N/2, t));
   V vvv(N/2, t);
   v.push_back(boost::move(vvv));
   v.insert(v.begin(), V(N/2, t));
   v.insert(v.end(), V(N/2, t));
   v.emplace_back(N/2, t);
}

bool default_init_test()//Test for default initialization
{
   const std::size_t Capacity = 100;

   typedef static_vector<int, Capacity> di_vector_t;

   {
      di_vector_t v(Capacity, default_init);
   }
   {
      typename dtl::aligned_storage<sizeof(di_vector_t)>::type as;
      di_vector_t& v = *::new(as.data) di_vector_t;
      int *p = v.data();

      for(std::size_t i = 0; i != Capacity; ++i, ++p){
         *p = static_cast<int>(i);
      }

      di_vector_t &rv = *::new(&v)di_vector_t(Capacity, default_init);
      di_vector_t::iterator it = rv.begin();

      for(std::size_t i = 0; i != Capacity; ++i, ++it){
         if(*it != static_cast<int>(i))
            return false;
      }

      v.~di_vector_t();
   }
   {
      di_vector_t v;

      int *p = v.data();
      for(std::size_t i = 0; i != Capacity; ++i, ++p){
         *p = static_cast<int>(i+100);
      }

      v.resize(Capacity, default_init);

      di_vector_t::iterator it = v.begin();
      for(std::size_t i = 0; i != Capacity; ++i, ++it){
         if(*it != static_cast<int>(i+100))
            return false;
      }
   }

   return true;
}


int main(int, char* [])
{
   using boost::container::test::movable_and_copyable_int;
   using boost::container::test::produce_movable_and_copyable_int;
   BOOST_TEST(counting_value::count() == 0);

   test_ctor_ndc<int, 10>();
   test_ctor_ndc<value_ndc, 10>();
   test_ctor_ndc<counting_value, 10>();
   BOOST_TEST(counting_value::count() == 0);
   test_ctor_ndc<movable_and_copyable_int, 10>();

   test_ctor_nc<int, 10>(5);
   test_ctor_nc<value_nc, 10>(5);
   test_ctor_nc<counting_value, 10>(5);
   BOOST_TEST(counting_value::count() == 0);
   test_ctor_nc<movable_and_copyable_int, 10>(5);

   test_ctor_nd<int, 10>(5, 1);
   test_ctor_nd<value_nd, 10>(5, value_nd(1));
   test_ctor_nd<counting_value, 10>(5, counting_value(1));
   BOOST_TEST(counting_value::count() == 0);
   test_ctor_nd<movable_and_copyable_int, 10>(5, produce_movable_and_copyable_int());

   test_resize_nc<int, 10>(5);
   test_resize_nc<value_nc, 10>(5);
   test_resize_nc<counting_value, 10>(5);
   BOOST_TEST(counting_value::count() == 0);
   test_resize_nc<movable_and_copyable_int, 10>(5);

   test_resize_nd<int, 10>(5, 1);
   test_resize_nd<value_nd, 10>(5, value_nd(1));
   test_resize_nd<counting_value, 10>(5, counting_value(1));
   BOOST_TEST(counting_value::count() == 0);
   test_resize_nd<movable_and_copyable_int, 10>(5, produce_movable_and_copyable_int());

   test_push_back_nd<int, 10>();
   test_push_back_nd<value_nd, 10>();
   test_push_back_nd<counting_value, 10>();
   BOOST_TEST(counting_value::count() == 0);
   test_push_back_nd<movable_and_copyable_int, 10>();

   test_pop_back_nd<int, 10>();
   test_pop_back_nd<value_nd, 10>();
   test_pop_back_nd<counting_value, 10>();
   BOOST_TEST(counting_value::count() == 0);
   test_pop_back_nd<movable_and_copyable_int, 10>();

   test_copy_and_assign_nd<int, 10>(1);
   test_copy_and_assign_nd<value_nd, 10>(value_nd(1));
   test_copy_and_assign_nd<counting_value, 10>(counting_value(1));
   BOOST_TEST(counting_value::count() == 0);
   test_copy_and_assign_nd<movable_and_copyable_int, 10>(produce_movable_and_copyable_int());

   test_iterators_nd<int, 10>();
   test_iterators_nd<value_nd, 10>();
   test_iterators_nd<counting_value, 10>();
   BOOST_TEST(counting_value::count() == 0);
   test_iterators_nd<movable_and_copyable_int, 10>();

   test_erase_nd<int, 10>();
   test_erase_nd<value_nd, 10>();
   test_erase_nd<counting_value, 10>();
   BOOST_TEST(counting_value::count() == 0);
   test_erase_nd<movable_and_copyable_int, 10>();

   test_insert_nd<int, 10>(50);
   test_insert_nd<value_nd, 10>(value_nd(50));
   test_insert_nd<counting_value, 10>(counting_value(50));
   BOOST_TEST(counting_value::count() == 0);
   test_insert_nd<movable_and_copyable_int, 10>(produce_movable_and_copyable_int());

   test_capacity_0_nd<int>();
   test_capacity_0_nd<value_nd>();
   test_capacity_0_nd<counting_value>();
   BOOST_TEST(counting_value::count() == 0);
   test_capacity_0_nd<movable_and_copyable_int>();

   test_exceptions_nd<int, 10>();
   test_exceptions_nd<value_nd, 10>();
   test_exceptions_nd<counting_value, 10>();
   BOOST_TEST(counting_value::count() == 0);
   test_exceptions_nd<movable_and_copyable_int, 10>();

   test_swap_and_move_nd<int, 10>();
   test_swap_and_move_nd<value_nd, 10>();
   test_swap_and_move_nd<counting_value, 10>();
   BOOST_TEST(counting_value::count() == 0);
   test_swap_and_move_nd<movable_and_copyable_int, 10>();

   test_emplace_0p<counting_value, 10>();
   BOOST_TEST(counting_value::count() == 0);

   test_emplace_2p<counting_value, 10>();
   BOOST_TEST(counting_value::count() == 0);

   test_sv_elem<int, 10>(50);
   test_sv_elem<value_nd, 10>(value_nd(50));
   test_sv_elem<counting_value, 10>(counting_value(50));
   BOOST_TEST(counting_value::count() == 0);
   test_sv_elem<movable_and_copyable_int, 10>(movable_and_copyable_int(50));

   BOOST_TEST(default_init_test() == true);

   test_support_for_initializer_list();

   ////////////////////////////////////
   //    Iterator testing
   ////////////////////////////////////
   {
      typedef boost::container::static_vector<int, 3> cont_int;
      cont_int a; a.push_back(0); a.push_back(1); a.push_back(2);
      boost::intrusive::test::test_iterator_random< cont_int >(a);
   }

   return boost::report_errors();
}

#include <boost/container/detail/config_end.hpp>

