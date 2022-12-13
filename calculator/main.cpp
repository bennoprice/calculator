#define CATCH_CONFIG_MAIN

#include <iostream>
#include "calc.hpp"
#include "catch2/catch.hpp"

using namespace calc;

// Test all functionality of the calculator, which is:
//  - lit(i) to create literal expressions
//  - foo->add(bar), foo->sub(bar), foo->mul(bar) to perform operations on two expressions
//  - foo->toString() and foo->simplify() to display and evaluate calculator expressions

TEST_CASE( "Testing function - SumNumbers" ) {
	auto eq = [ ]( auto e, std::string before, std::string after ) {
		// Ensure the string representation matches expected value
		REQUIRE( e->to_string( ) == before );
		// Ensure the string represtation after simplication matches expected value
		REQUIRE( e->simplify( )->to_string( ) == after );
		// simplify() should NOT change the original value
		REQUIRE( e->to_string( ) == before );
	};

	SECTION( "testliterals" ) {
		eq( lit( 2 ), "2", "2" );
		eq( lit( -1 ), "-1", "-1" );
	}

	SECTION( "testadding" ) {
		eq( lit( 2 )->add( lit( 3 ) ), "(2 + 3)", "5" );
		eq( lit( 1 )->add( lit( 2 ) )->add( lit( 3 ) ), "((1 + 2) + 3)", "6" );
	}

	SECTION( "testsubtracting" ) {
		eq( lit( 1 )->sub( lit( 2 ) )->sub( lit( 3 ) ), "((1 - 2) - 3)", "-4" );
		eq( lit( 1 )->sub( lit( 2 )->sub( lit( 3 ) ) ), "(1 - (2 - 3))", "2" );
	}

	SECTION( "testmultiplying" ) {
		eq( lit( 1 )->mul( lit( 2 ) )->mul( lit( 3 ) ), "((1 * 2) * 3)", "6" );
	}

	SECTION( "testGraph" ) {
		auto x = lit( 4 )->add( lit( 5 ) );
		eq( x->mul( x ), "((4 + 5) * (4 + 5))", "81" );
	}

	SECTION( "testUseResults" ) {
		auto x = lit( 4 )->add( lit( 5 ) )->simplify( );
		eq( x->mul( x ), "(9 * 9)", "81" );
		auto y = x->mul( x )->simplify( );
		eq( y->add( x ), "(81 + 9)", "90" );
	}
}