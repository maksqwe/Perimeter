//
// Copyright Thomas Witt 2003. Permission to copy, use,
// modify, sell and distribute this software is granted provided this
// copyright notice appears in all copies. This software is provided
// "as is" without express or implied warranty, and with no claim as
// to its suitability for any purpose.
//
#include <boost/iterator/iterator_archetypes.hpp>
#include <boost/iterator/iterator_categories.hpp>
#include <boost/iterator/iterator_concepts.hpp>
#include <boost/concept_check.hpp>
#include <boost/cstdlib.hpp>

int main()
{
  {
    typedef boost::iterator_archetype<
        int
      , boost::iterator_archetypes::readable_iterator_t
      , boost::random_access_traversal_tag
    > iter;

    boost::function_requires< boost_concepts::ReadableIteratorConcept<iter> >();
    boost::function_requires< boost_concepts::RandomAccessTraversalConcept<iter> >();
  } 
  {
    typedef boost::iterator_archetype<
        int
      , boost::iterator_archetypes::readable_writable_iterator_t
      , boost::random_access_traversal_tag
    > iter;

    boost::function_requires< boost_concepts::ReadableIteratorConcept<iter> >();
    boost::function_requires< boost_concepts::WritableIteratorConcept<iter> >();
    boost::function_requires< boost_concepts::RandomAccessTraversalConcept<iter> >();
  } 
  {
    typedef boost::iterator_archetype<
        const int // I don't like adding const to Value. It is redundant. -JGS
      , boost::iterator_archetypes::readable_lvalue_iterator_t
      , boost::random_access_traversal_tag
    > iter;

    boost::function_requires< boost_concepts::ReadableIteratorConcept<iter> >();
    boost::function_requires< boost_concepts::LvalueIteratorConcept<iter> >();
    boost::function_requires< boost_concepts::RandomAccessTraversalConcept<iter> >();
  } 
  {
    typedef boost::iterator_archetype<
        int
      , boost::iterator_archetypes::writable_lvalue_iterator_t
      , boost::random_access_traversal_tag
    > iter;

    boost::function_requires< boost_concepts::WritableIteratorConcept<iter> >();
    boost::function_requires< boost_concepts::LvalueIteratorConcept<iter> >();
    boost::function_requires< boost_concepts::RandomAccessTraversalConcept<iter> >();
  } 

  return boost::exit_success;
}

