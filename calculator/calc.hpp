#include <string>
#include <memory>

namespace calc::detail
{
	class comp;
	class expr;
	class lit;

	using comp_t = std::shared_ptr<const comp>;
	using expr_t = std::shared_ptr<const expr>;
	using lit_t = std::shared_ptr<const lit>;

	enum class op {
		add,
		sub,
		mul
	};

	class arith {
	public:
		arith( op op )
			: op( op )
		{ }

		std::string to_string( ) const {
			switch ( op ) {
				case op::add:
					return "+";
				case op::sub:
					return "-";
				case op::mul:
					return "*";
			}
		}

		int eval( int lhs, int rhs ) const {
			switch ( op ) {
				case op::add:
					return lhs + rhs;
				case op::sub:
					return lhs - rhs;
				case op::mul:
					return lhs * rhs;
			}
		}
	private:
		op op;
	};

	class comp : public std::enable_shared_from_this<comp> {
	private:
		expr_t apply( const comp_t other, op op ) const {
			return std::make_shared<const expr>( shared_from_this( ), other, op );
		}
	public:
		virtual std::string to_string( ) const = 0;
		virtual lit_t simplify( ) const = 0;
		virtual int eval( ) const = 0;

		expr_t add( const comp_t other ) const {
			return apply( other, op::add );
		}

		expr_t sub( const comp_t other ) const {
			return apply( other, op::sub );
		}

		expr_t mul( const comp_t other ) const {
			return apply( other, op::mul );
		}
	};

	class expr : public comp {
	public:
		explicit expr( const comp_t lhs, const comp_t rhs, const arith op )
			: lhs( lhs )
			, rhs( rhs )
			, op( op )
		{ }

		std::string to_string( ) const override {
			return "(" + lhs->to_string( ) + " " + op.to_string( ) + " " + rhs->to_string( ) + ")";
		}

		int eval( ) const override {
			return op.eval( lhs->eval( ), rhs->eval( ) );
		}

		lit_t simplify( ) const override {
			return std::make_shared<const lit>( eval( ) );
		}
	private:
		const comp_t lhs;
		const comp_t rhs;
		const arith op;
	};

	class lit : public comp {
	public:
		explicit lit( int val )
			: val( val )
		{ }

		std::string to_string( ) const override {
			return std::to_string( val );
		}

		lit_t simplify( ) const override {
			return std::dynamic_pointer_cast<const lit>(shared_from_this( ));
		}
		
		int eval( ) const override {
			return val;
		}
	private:
		int val;
	};
}

namespace calc
{
	detail::lit_t lit( int val ) {
		return std::make_shared<const detail::lit>( val );
	}
}