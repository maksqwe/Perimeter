/*=============================================================================
    Copyright (c) 1999-2003 Jaakko J�rvi
    Copyright (c) 2001-2003 Joel de Guzman

    Use, modification and distribution is subject to the Boost Software
    License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
    http://www.boost.org/LICENSE_1_0.txt)
==============================================================================*/
#if !defined(FUSION_SEQUENCE_EQUAL_TO_HPP)
#define FUSION_SEQUENCE_EQUAL_TO_HPP

#include <boost/spirit/fusion/sequence/begin.hpp>
#include <boost/spirit/fusion/sequence/end.hpp>
#include <boost/spirit/fusion/sequence/detail/sequence_equal_to.hpp>

#ifdef FUSION_COMFORMING_COMPILER
#include <boost/utility/enable_if.hpp>
#include <boost/spirit/fusion/sequence/is_sequence.hpp>
#endif

namespace boost { namespace fusion
{
    template <typename Seq1, typename Seq2>
    inline bool
    operator==(sequence_base<Seq1> const& a, sequence_base<Seq2> const& b)
    {
        return detail::sequence_equal_to<Seq1 const, Seq2 const>::
            apply(
                boost::fusion::begin(a.cast())
              , boost::fusion::begin(b.cast())
            );
    }

#ifdef FUSION_COMFORMING_COMPILER

    template <typename Sequence>
    struct type_sequence;

    template <typename Seq1, typename Seq2>
    inline typename disable_if<fusion::is_sequence<Seq2>, bool>::type
    operator==(sequence_base<Seq1> const& a, Seq2)
    {
        return a == type_sequence<Seq2>();
    }

    template <typename Seq1, typename Seq2>
    inline typename disable_if<fusion::is_sequence<Seq1>, bool>::type
    operator==(Seq1, sequence_base<Seq2> const& b)
    {
        return type_sequence<Seq1>() == b;
    }

#endif
}}

#endif
